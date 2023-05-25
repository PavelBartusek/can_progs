/*
 *
 *  Copyright (C) 2014 Jürg Müller, CH-5524
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/ .
 */

#if defined(__PYTHON__)
  #include <Python.h>
#endif

#include <stdio.h>
#include <string.h>

#include "NTypes.h"

#include "NUtils.h"
#include "NCanUtils.h"
#include "KCriticalSection.h"
#include "KCanCommDriver.h"
#if !defined(__NO_REMOTE__)
  #include "KCanTcpDriver.h"
#endif
#if defined(__LINUX__) && !defined(__MAC__)
  #include "KSocketCanDriver.h"
#endif
#if defined(__WINDOWS__)
  #include "special/Kusb2can.h"
#endif

#include "KCanServer_.h"

KCanServer::KCanServer(bool UseMqtt)
{
  DriverType = NCanUtils::dt_can;
  Trace = false;
  AsSimulation = false;

  CanDriver = NULL;
  CanSendDriver = NULL;
  IsInitalized = false;
  FrameCount = 0;
  CanRecvRingBuffer = new KRingBuffer<KCanFrame>();
  CanSendRingBuffer = new KRingBuffer<KCanFrame>();
  CanMqtt = NULL;
#if defined(__MQTT__)
  if (UseMqtt)
    CanMqtt = new KCanMqtt;
#endif
}

KCanServer::~KCanServer()
{
  if (CanDriver)
  {
    CanDriver->Close();
    delete CanDriver;
  }
  if (CanSendDriver)
  {
    CanSendDriver->Close();
    delete CanSendDriver;
  }
  delete CanRecvRingBuffer;
  delete CanSendRingBuffer;
#if defined(__MQTT__)
  if (CanMqtt)
    delete CanMqtt;
#endif
}

void KCanServer::Resume()
{
  KThread::Resume();
#if defined (__MQTT__)
  if (CanMqtt)
  {
    CanMqtt->Resume();
  }
#endif
}

bool KCanServer::InitMqtt(const char * AddressAndPort)
{
  if (CanMqtt == NULL)
    return false;

#if defined(__MQTT__)
  return CanMqtt->Init(AddressAndPort);
#else
  return false;
#endif
}

bool KCanServer::Init(const char * CanDev)
{
  if (IsInitalized)
    return false;

  IsInitalized = true;
  if (!CanDriver)
  {
    enum NCanUtils::driver_type new_dt;
      
    if (DriverType == NCanUtils::dt_cs  || DriverType == NCanUtils::dt_elm327)
      new_dt = DriverType;
    else
      new_dt = NCanUtils::GetDriverType(CanDev);
 
    #if !defined(__NO_REMOTE__)
      if (new_dt == NCanUtils::dt_unknown && KIpSocket::IsHostAddr(CanDev))
        new_dt = NCanUtils::dt_can232_remote;
    #endif
      
    if (new_dt != NCanUtils::dt_unknown)
      DriverType = new_dt;
   
    switch (DriverType)
    {
    #if defined (__LINUX__) && !defined(__MAC__)
      case NCanUtils::dt_can:
        CanDriver = new KSocketCanDriver;
        #if !defined(__UVR__)
          CanSendDriver = new KSocketCanDriver;
        #endif
        break;
    #endif
    #if !defined(__NO_REMOTE__)
      case NCanUtils::dt_can232_remote:
        CanDriver = new KCan232Tcp;
        break;
    #endif
        
      case NCanUtils::dt_cs:
        CanDriver = new KCanCS;
        break;
        
      case NCanUtils::dt_can232:
        CanDriver = new KCan232;
        break;
    #if defined(__ELM327__)
      case NCanUtils::dt_elm327:
        CanDriver = new KCanELM327;
        break;
    #endif
    #if defined(__WINDOWS__)    
      case NCanUtils::dt_8dev:
        CanDriver = new Kusb2canDriver;
        break;
    #endif
        
      default:
        return false;
    }
  }
  
  CanDriver->AsSimulation = AsSimulation;
  if (!CanDriver->Init(CanDev))
    return false;
  
#if defined (__LINUX__) && !defined(__MAC__)
  if (CanSendDriver)
  {
    if (!CanSendDriver->Init(CanDev) ||
        !CanSendDriver->Connect())
      return false;
  }
#endif
  return true;
}

void KCanServer::Halt()
{
  Terminate();
  while (!Terminated())
    NUtils::SleepMs(1);
#if defined(__MQTT__)
  if (CanMqtt)
  {
    CanMqtt->Terminate();
    while (!CanMqtt->Terminated())
      NUtils::SleepMs(1);
  }
#endif
  NUtils::SleepMs(10);
}
/*
void KCanServer::PutFrame(const KCanFrame & Frame)
{

}
*/
bool KCanServer::GetFrame(KCanFrame & Frame)
{
  bool Ok = CanRecvRingBuffer->Pop(Frame);
#if defined(__MQTT__)
  if (Ok && CanMqtt && CanMqtt->CanRecvBuffer != NULL)
    CanMqtt->CanRecvBuffer->Push(Frame);
#endif
  return Ok;
}

bool KCanServer::SendData(const KCanFrame & Frame)
{
  if (!CanDriver)
    return false;

  SendFrame = Frame;
  sfState = sfSend;

  for (int delay = 0; delay < 50 && sfState == sfSend; delay++)
  {
    NUtils::SleepMs(1);
  }
  return sfState == sfOk;
}

void KCanServer::SetTrace(bool trace)
{
  Trace = trace;
  if (CanDriver)
    CanDriver->Trace = trace;
}

void KCanServer::Execute()
{
#if defined(__CONSOLE__) && defined(__DEBUG__)
  printf("start CanServer\n");
#endif
  Init(NULL);
  if (!CanDriver->Connect())
    Terminate();

  while (!Terminated())
  {
    if (CanDriver)
    {
      KCanFrame Frame;
      bool Ok;
      while (CanDriver->ReceiveData(Frame))
      {
        Frame.Counter = (int)FrameCount++;
        CanRecvRingBuffer->Push(Frame);
      }
      if (sfState == sfSend)
      {
        if (CanSendDriver)
          Ok = CanSendDriver->SendData(SendFrame);
        else 
          Ok = CanDriver->SendData(SendFrame);
        sfState = Ok ? sfOk : sfIdle;
      } else
      if (CanSendRingBuffer != NULL &&
          CanSendRingBuffer->Pop(Frame))
      {
        Ok = CanDriver->SendData(Frame);
      }
    }
    NUtils::SleepMs(1);
  }
#if defined(__CONSOLE__) && defined(__DEBUG__)
  printf("stop CanServer\n");
#endif
}

