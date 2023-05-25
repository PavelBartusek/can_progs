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

#if !defined(KCanServer_H)

  #define KCanServer_H

  #include "NCanUtils.h"
  #include "KCanDriver.h"
  #include "KThread.h"
  #include "KCriticalSection.h"
#if defined(__MQTT__)
  #include "KCanMqtt.h"
#endif

  // In diesem Thread wird der Empfangs-Puffer (CanRecvRingBuffer) aufgefüllt.
  // GetFrame liest ein Telegramm vom RingBuffer aus. 
  class KCanServer : public KThread
  {
    private:
      unsigned FrameCount;
      bool IsInitalized;
      KCanDriver * CanDriver;
      KCanDriver * CanSendDriver; // immer KSocketCanDriver (oder NULL)
      volatile enum {sfIdle, sfSend, sfOk} sfState;
      KCanFrame SendFrame;
      KRingBuffer<KCanFrame> * CanRecvRingBuffer; // received from CAN-Bus
      KRingBuffer<KCanFrame> * CanSendRingBuffer; // send to CAN-Bus
    protected:
      class KCanMqtt * CanMqtt;
    public:
      bool Trace;
      bool AsSimulation;
      enum NCanUtils::driver_type DriverType;

      //KCanServer(class KRingBuffer<KCanFrame> * MqttRing = NULL);
      KCanServer(bool UseMqtt = false);
      virtual ~KCanServer();

      bool Init(const char * CanDev);
      void Halt();
      bool GetFrame(KCanFrame & Frame);
      const char * GetDev() const { return CanDriver ? CanDriver->dev : ""; }
      unsigned GetPort() const { return CanDriver ? CanDriver->GetPort() : 0; }
      bool SendData(const KCanFrame & Frame);
      void SetTrace(bool trace);
      bool InitMqtt(const char * AddressAndPort);

      virtual void Resume();
      virtual void Execute(); // Füllt den CanRecvRingBuffer.
  };

#endif

