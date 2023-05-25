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

  // In diesem Thread wird der Empfangs-Puffer (RingBuffer) aufgefüllt.
  // GetFrame liest ein Telegramm vom RingBuffer aus. 
  class KCanServer : public KThread
  {
    private:
    #if defined(__MQTT__)
      KRingBuffer<KCanFrame> * CanRingBuffer;
      KRingBuffer<KCanFrame> * MqttRingBuffer;
    #else
      class KCriticalSection * RingBufferSemaphore;
      int RingBufferSize;
      volatile int RingHead;
      volatile int RingTail;
      KCanFrame * RingBuffer;
      void IncRingPtr(volatile int & RingPtr);
    #endif
      unsigned FrameCount;
      bool IsInitalized;
      KCanDriver * CanDriver;
    #if defined (__LINUX__) && !defined(__MAC__)
      KCanDriver * CanSendDriver; // immer KSocketCanDriver (oder NULL)
    #endif
      bool ReceiveData(KCanFrame & Frame); // füllt CanRingBuffer

    public:
      bool Trace;
      bool AsSimulation;
      enum NCanUtils::driver_type DriverType;

      KCanServer(bool UseMqtt = false);
      virtual ~KCanServer();

      bool Init(const char * CanDev);
      void Halt();
      void PutFrame(const KCanFrame & Frame);
      bool GetFrame(KCanFrame & Frame);
    #if !defined(__MQTT__)
      int  RingCount() const;
    #endif
      bool RingBufferFull() const;
      const char * GetDev() const { return CanDriver ? CanDriver->dev : ""; }
      unsigned GetPort() const { return CanDriver ? CanDriver->GetPort() : 0; }
      bool SendData(const KCanFrame & Frame);
      bool SendData(const KComfortFrame & Frame);
      void SetTrace(bool trace);

      virtual void Execute(); // Füllt den CanRingBuffer.
  };

#endif

