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

#if defined(__MQTT__)

//
// Benötigt: paho.mqtt.cpp
//
// https://github.com/eclipse/paho.mqtt.cpp
//
#include <stdio.h>
#include <string.h>

#include "NUtils.h"
#include "KElsterTable.h"
#include "KCanMqtt.h"

//#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "WP"
//#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     100L

KCanMqtt::KCanMqtt()
{
  InitOk = false;
  CanRecvBuffer = new KRingBuffer<KCanFrame>;
  CanSendBuffer = new KRingBuffer<KCanFrame>;
}

bool KCanMqtt::Init(const char * Address)
{
  if (!InitOk)
  {
    char addr[32] = "localhost";
    unsigned port = 1883;
    if (Address != NULL && !*Address)
    {
      int pos = 0;
      while (pos < 20 && *Address && *Address != ':')
      {
        addr[pos++] = *Address++;
      }
      addr[pos] = 0;
      if (*Address == ':')
      {
        NUtils::GetUnsigned(Address, port);
      }
    }

    MQTTClient_connectOptions conn_opts;
    memset(&conn_opts, 1, sizeof(conn_opts));
    conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    char ADDRESS[64];
    sprintf(ADDRESS, "tcp://%s:%d", addr, port);
    rc = MQTTClient_create(&MQTTclient, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTCLIENT_SUCCESS)
    {
      printf("MQTT failed to create client, return code %d\n", rc);
      return false;
    }
    printf("MQTT initialized: %s\n\n", ADDRESS);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    try
    {
      if ((rc = MQTTClient_connect(MQTTclient, &conn_opts)) != MQTTCLIENT_SUCCESS)
      {
        printf("MQTT failed to connect, return code %d\n", rc);
        return false;
      }
      InitOk = true;
    } catch(...)
    {
      printf("MQTT failed to connect\n");
    }

  }
  return InitOk;
}

KCanMqtt::~KCanMqtt()
{
  delete CanRecvBuffer;
  delete CanSendBuffer;
  if (InitOk)
  {
    MQTTClient_disconnect(MQTTclient, 10000);
    MQTTClient_destroy(&MQTTclient);
  }
}

void KCanMqtt::Execute()
{
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token;

  while (!Terminated() && InitOk)
  {
    if (LastSendToMqtt.Len > 5 || CanRecvBuffer->Pop(LastSendToMqtt))
    {
      int elster_idx = LastSendToMqtt.GetElsterIdx();
      int Value = LastSendToMqtt.GetValue();
      unsigned d = LastSendToMqtt.Data[0] & 0xf;
      if ((d == 2 || (d == 0 && LastSendToMqtt.Data[1] != 0x79)) && elster_idx >=0 && Value >= 0)
      {
        const ElsterIndex * elst_ind = GetElsterIndex(elster_idx);
        if (elst_ind != NULL)
        {
          char value_msg[64] = {0};
          char time_str[32];
          char topic[64];
          unsigned id = d == 2 ? LastSendToMqtt.Id : LastSendToMqtt.GetReceiverId();

          sprintf(topic, "dev_%x/%s", id, elst_ind->Name);

          SetValueType(value_msg, elst_ind->Type, Value);

          pubmsg.payload = (void *) value_msg;
          pubmsg.payloadlen = strlen(value_msg);
          pubmsg.qos = QOS;
          pubmsg.retained = 0;
          MQTTClient_publishMessage(MQTTclient, topic, &pubmsg, &token);
          MQTTClient_waitForCompletion(MQTTclient, token, TIMEOUT);
          NUtils::PrintTime(LastSendToMqtt.TimeStampDay, LastSendToMqtt.TimeStampMs, time_str, false);
          printf("MQTT message %s: %s:%s: %s\n", time_str, CLIENTID, topic, value_msg);
        }
      }
      LastSendToMqtt.Len = 0;
    }
    if (LastSendToCan.Len > 5 || CanSendBuffer->Pop(LastSendToCan))
    {

      LastSendToCan.Len = 0;
    }
    NUtils::SleepMs(1);
  }
}

#endif
