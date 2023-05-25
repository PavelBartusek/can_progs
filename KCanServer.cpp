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

#include "KCanServer.h"

KCanServer::KCanServer(bool UseMqtt)
{
  DriverType = NCanUtils::dt_can;
  Trace = false;
  AsSimulation = false;

  CanDriver = NULL;
  IsInitalized = false;
  FrameCount = 0;
#if defined (__LINUX__) && !defined(__MAC__)
  CanSendDriver = NULL;
#endif
#if defined(__MQTT__)
  CanRingBuffer = new KRingBuffer<KCanFrame>();
  MqttRingBuffer = MqttRing;
#else
  RingHead = RingTail = 0;
  RingBufferSize = 10240;
  RingBuffer = new KCanFrame[RingBufferSize];
  RingBufferSemaphore = new KCriticalSection;
#endif
}

KCanServer::~KCanServer()
{
  if (CanDriver)
  {
    CanDriver->Close();
    delete CanDriver;
  }
#if defined (__LINUX__) && !defined(__MAC__)
  if (CanSendDriver)
  {
    CanSendDriver->Close();
    delete CanSendDriver;
  }
#endif
#if defined(__MQTT__)
  delete CanRingBuffer;
#else
  delete RingBufferSemaphore;
  delete [] RingBuffer;
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
        
      case NCanUtils::dt_elm327:
        CanDriver = new KCanELM327;
        break;

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
    ;

  NUtils::SleepMs(10);
}
#if !defined(__MQTT__)
int  KCanServer::RingCount() const
{
  return (RingHead - RingTail + RingBufferSize) % RingBufferSize;
}

void KCanServer::IncRingPtr(volatile int & RingPtr)
{
  RingPtr = (RingPtr + RingBufferSize + 1) % RingBufferSize;
}
#endif

bool KCanServer::RingBufferFull() const
{
#if defined(__MQTT__)
  return CanRingBuffer->RingBufferFull();
#else
  return RingCount() > 3*(RingBufferSize / 4);
#endif
}

void KCanServer::PutFrame(const KCanFrame & Frame)
{
#if defined(__MQTT__)
  CanRingBuffer->Push(Frame);
  if (MqttRingBuffer != NULL)
    MqttRingBuffer->Push(Frame);
#else
  if (RingBufferSemaphore->Acquire())
  {
    IncRingPtr(RingHead);
    if (RingTail == RingHead)
      IncRingPtr(RingTail);
    RingBuffer[RingHead] = Frame;

    RingBufferSemaphore->Release();
  }
#endif
}

bool KCanServer::GetFrame(KCanFrame & Frame)
{
#if defined(__MQTT__)
  return CanRingBuffer->Pop(Frame);
#else
  bool Ok = false;
  if (RingHead == RingTail)
    return Ok;

  if (RingBufferSemaphore->Acquire())
  {
    if (RingTail != RingHead)
    {
      Ok = true;
      IncRingPtr(RingTail);
      Frame = RingBuffer[RingTail];
    }
    RingBufferSemaphore->Release();
  }
  return Ok;
#endif
}

bool KCanServer::SendData(const KCanFrame & Frame)
{
#if defined (__LINUX__) && !defined(__MAC__)
  if (CanSendDriver && (Frame.Data[0] & 0xf) == 2)
    return CanSendDriver->SendData(Frame);
#endif

  if (!CanDriver)
    return false;

  return CanDriver->SendData(Frame);
}

bool KCanServer::SendData(const KComfortFrame & Frame)
{
#if defined (__LINUX__) && !defined(__MAC__)
  if (CanSendDriver)
    return CanSendDriver->SendData(Frame);
#endif

  if (!CanDriver)
    return false;

  return CanDriver->SendData(Frame);
}

bool KCanServer::ReceiveData(KCanFrame & Frame)
{
  if (CanDriver)
  {
    return CanDriver->ReceiveData(Frame);
  }
  return false;
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
    KCanFrame Frame;

    if (CanDriver->ReceiveData(Frame) &&
        Frame.Data[0] == 0xd2) // Antwort für ID 680
    {
      Frame.Counter = (int)FrameCount++;
      PutFrame(Frame);
    }
    NUtils::SleepMs(1);
  }
#if defined(__CONSOLE__) && defined(__DEBUG__)
  printf("stop CanServer\n");
#endif
}

