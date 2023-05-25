/*
 *
 *  Copyright (C) 2020 Jürg Müller, CH-5524
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

//
// Benötigt: paho.mqtt.cpp
//
// https://github.com/eclipse/paho.mqtt.cpp
//
#ifndef KCanMqtt_H
  #define KCanMqtt_H

  #include "NCanUtils.h"
  #include "MQTTClient.h"
  #include "KThread.h"
  #include "KCriticalSection.h"

  class KCanMqtt : public KThread
  {
    private:
      bool InitOk;
      MQTTClient MQTTclient;
      KCanFrame LastSendToMqtt;
      KCanFrame LastSendToCan;
    public:
      KRingBuffer<KCanFrame> * CanRecvBuffer; // send to mqtt
      KRingBuffer<KCanFrame> * CanSendBuffer; // received from mqtt

      KCanMqtt();
      virtual ~KCanMqtt();

      bool Init(const char * AddressAndPort);
      void Execute();

  };




#endif
