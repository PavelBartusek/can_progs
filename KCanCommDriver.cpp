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

//
// Alle CAN-Bus-Treiber, welche auf einer seriellen Schnittstelle basieren.
//

#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

#include "NTypes.h"

#include "NUtils.h"
#include "KCanCommDriver.h"

#if defined(VCL)
KCanCommDriver::KCanCommDriver(TComponent * Owner) : KCommVCL(Owner)
#else
KCanCommDriver::KCanCommDriver()
#endif
{
  tel_len = 0;
  time_out = 50; // 50 msec
  time_stamp = 0;
}

KCanCommDriver::~KCanCommDriver()
{
  Close();
}

int KCanCommDriver::ReadFromCOM()
{
  int res = KComm::ReadFromCOM();

  if (res > NULL_CHAR)
  {
    tel_len = 0;
    return res;
  }
  if (res == NULL_CHAR && tel_len == 0)
  {
    NUtils::SleepMs(1);
    return res;
  }

  timeb tb;
  ftime(&tb);
  unsigned now = 1000*(tb.time % 1000000) + tb.millitm;

  int div = now - time_stamp;
  if (div < 0)    // time overflow
    div += 1000000000;
  if (div > (int) time_out)
    tel_len = 0;  // clear buffer after timeout

  if (res == NULL_CHAR)
    return NULL_CHAR;

  if (tel_len+1 >= High(tel))
  {
    tel_len = 0;
    return 0x2000;
  }

  tel[tel_len++] = (unsigned char) res;
  tel[tel_len] = 0;
  time_stamp = now;

  return res;
}

bool KCanCommDriver::WriteBufferToCOM(const char * str, int len)
{
  bool Ok = KComm::WriteBufferToCOM(str, len);
  NUtils::SleepMs(len);
  return Ok;
}

bool KCanCommDriver::Connect()
{
  return COM_INITIALISIERT();
}

void KCanCommDriver::Close()
{
  ExitCOM();
}


////////////////////////////////////////////////////////////////////////////////

// Code:
//
// V<cr>   Version with Answer  V1013<cr>   hardware / software version in bcd
// USBtin: uses v and V
// N<cr>   Get serial Number    NA123<cr>
// C<cr>   Close the can channel
// S1<cr>  Setup 20kbit
// X1<cr>  Turn ON the Auto Poll/Send feature   Answer is <cr> or <bell>
// r0<cr> don't use 
#if defined(__TEST_MICROCHIP__)
  // S4: 125 kbit/s
  #define INIT_STR  "\rC\rX1\rV\rS4\rO\r"
#elseif defined(__UVR__)
  // S1: 50kbit/s
  #define INIT_STR  "\rC\rX1\rV\rS2\rO\r"
#else
  // S1: 20kbit/s
  #define INIT_STR  "\rC\rX1\rV\rS1\rO\r"
#endif

/*
  SocketCan Treiber aktivieren:

  su
  ifconfig can0 down
  slcan_attach -f -s1 -o -n can0 /dev/ttyUSB0
  slcand ttyUSB0 can0
  ifconfig can0 up
  exit
*/

bool KCan232::Init(const char * CanDev)
{
  tel_len = 0;
  WaitSentFrame = false;
  SetBaudrate(115200);
  SetStopBits(1);
  SetParity(0);
  SetDTR(true);
  SetRTS(true);
  SetRtsFlowControl(false);
  SetComDev(CanDev);
  KCanDriver::Init(CanDev);
  
#if defined(__WINDOWS__)
  if (!strncmp("com", KCanDriver::dev, 3))
  {
    const char * ptr = KCanDriver::dev + 3;
    TInt64 n;
    if (NUtils::GetInt(ptr, n))
      SetComPortNr((int) n);
  }
#endif
  
  if (InitCOM())
  {
    if (AsSimulation)
      return true;
      
    const char * init = INIT_STR;
    while (init)
    {
      const char * p = strstr(init, "\r");
      if (!p)
        break;
      WriteBufferToCOM(init, (int)(p - init) + 1);
      NUtils::SleepMs(100);
      init += p - init + 1;
    }
    return true;
  }
  return false;
}

bool KCan232::ReceiveData(KCanFrame & Frame)
{
  int res = ReadFromCOM();
  while (res != NULL_CHAR)
  {
    if (res < NULL_CHAR)
    {
      switch (char(res))
      {
        case 7: // bell means error
          WaitSentFrame = false;
          break;

        case 't':
        case 'T':
        case 'r':
        case 'R':
          tel_len = 1; // synchronize
          tel[0] = (unsigned char) res;
          break;

        case 'V':
          if (AsSimulation)
          {
            tel_len = 0;
            WriteBufferToCOM("V1324\r", 6);
            NUtils::SleepMs(10);
          }
          break;

        case '\r':
          if (tel_len == 2)
          {
            // USBtin respose for "t..." is z<CR>
            // CAN232: 'Z' for switching time stamp
            if (tel[0] == 'Z' || tel[0] == 'z')
            {
              if (WaitSentFrame)
              {
                tel_len = 0;
                WaitSentFrame = false;
                Frame = SentFrame;
                return true;
              }
            }
          } else
          if (tel_len > 1)
          {
            tel[tel_len-1] = 0;
            KCan232Frame can232((char *)tel);
            if (can232.SetCanFrame(Frame))
            {
              NUtils::Time(Frame.TimeStampDay, Frame.TimeStampMs);
              tel_len = 0;

              if (Trace && AsSimulation)
              {
                char str[64];
                Frame.PrintTime(str, false);
                printf("recv: %s %s\n", str, tel);
              }
              return true;
            }
          }
          tel_len = 0;
          break;

        default:
          break;
      }
    }
    res = ReadFromCOM();
  }
  NUtils::SleepMs(1);
  return false;
}

bool KCan232::SendData(const KCanFrame & Frame)
{
  WaitSentFrame = false;
  KCan232Frame can232;
  if (can232.GetCanFrame(Frame) &&
      WriteBufferToCOM(can232.msg, (int) strlen(can232.msg)))
  {
    tel_len = 0;
    WaitSentFrame = true;
    if (WriteBufferToCOM("\r", 1))
    {
      SentFrame = Frame;
      NUtils::Time(SentFrame.TimeStampDay, SentFrame.TimeStampMs);
      if (Trace && AsSimulation)
      {
        char str[64];
        SentFrame.PrintTime(str, false);
        printf("send: %s %s\n", str, can232.msg);
      }
      return true;
    } else
      WaitSentFrame = false;
  }
  return false;
}


////////////////////////////////////////////////////////////////////////////////

// optisches Interface: 2400 Bit/s, even, 2 Stopp-Bits, RTS
// jede Message ist 12 Byte lang (vgl. KComfortFrame)

bool KCanCS::Init(const char * CanDev)
{
  tel_len = 0;
  WaitSentFrame = false;
  SenderIsCS = false;
  Delay = 0;
  // ComfortSoft standard setting
  SetBaudrate(2400);
  SetStopBits(2);
  SetParity(2); // even
  SetDTR(false); // optisches Original-Interface ist false
  SetRTS(true);
  SetRtsFlowControl(false);
  SetComDev(CanDev);
  KCanDriver::Init(CanDev);
#if defined(__WINDOWS__)
  if (!strncmp("com", KCanDriver::dev, 3))
  {
    const char * ptr = KCanDriver::dev + 3;
    TInt64 n;
    if (NUtils::GetInt(ptr, n))
      SetComPortNr((int) n);
  }
#endif

  return InitCOM();
}

bool KCanCS::ReceiveData(KComfortFrame & Frame)
{
  int res = ReadFromCOM();
  while (res != NULL_CHAR)
  {
    if (res < NULL_CHAR)
    {
      if (SenderIsCS && tel_len == 1 && tel[0] != 0x0d)
      {
        tel_len = 0;
      } else
      if (tel_len >= 12)
      {
        for (int i = 0; i < 12; i++)
          Frame.Data[i] = tel[i];

        tel_len = 0;
        if (Frame.CheckSum())
        {
#if defined(__SIMULATION__)
          if (Frame.Data[0] < 0x0d)
#endif
            return true;
        }
      }
    } else
      tel_len = 0;

    res = ReadFromCOM();
  }
  Delay++;
  NUtils::SleepMs(1);
  if (Delay > 200)
    tel_len = 0;
  if (tel_len == 0)
    Delay = 0;
  return false;
}

bool KCanCS::ReceiveData(KCanFrame & Frame)
{
  KComfortFrame cf;

  if (!ReceiveData(cf))
    return false;

  if (cf.CheckSum() &&
      cf.SetCanFrame(Frame))
  {
    NUtils::Time(Frame.TimeStampDay, Frame.TimeStampMs);
  } else
    Frame.Len = 0;

  return true;
}

bool KCanCS::SendData(const KCanFrame & Frame)
{
  KComfortFrame CF;

  if (!CF.GetCanFrame(Frame))
    return false;

  return SendData(CF);
}

bool KCanCS::SendData(const KComfortFrame & Frame)
{
  bool Ok = WriteBufferToCOM((char *)Frame.Data, 12);

  tel_len = 0;

  return Ok;
}


////////////////////////////////////////////////////////////////////////////////


bool KCanELM327::Init(const char * CanDev)
{
  tel_len = 0;
  WaitSentFrame = false;
  got_ok = go_none;
  SetBaudrate(38400);
  SetStopBits(1);
  SetParity(0);
  //SetDTR(true);
  //SetRTS(true);
  SetRtsFlowControl(false);
  SetComDev(CanDev);
  KCanDriver::Init(CanDev);
#if defined(__WINDOWS__)
  if (!strncmp("com", KCanDriver::dev, 3))
  {
    const char * ptr = KCanDriver::dev + 3;
    TInt64 n;
    if (NUtils::GetInt(ptr, n))
      SetComPortNr((int) n);
  }
#endif

  if (!InitCOM())
    return false;

  if (AsSimulation)
    return true;

  if (!SendCmd("AT D") || // set all to defaults
      !SendCmd("ATE0")) // echo off
    return false;
  // 11-Bit und 20 kbit/s
  if (//!SendCmd("AT PP 2F SV 19") ||
      !SendCmd("AT PP 2F ON"))
    return false;
  // Das geänderte Protokoll auswählen
  if (!SendCmd("AT SP C"))
    return false;
  // Header einschalten
  //if (!SendCmd("AT H1"))
  //  return false;
  // Kommunikation mitlesen (monitor all)
  return SendCmd("AT MA");
}

bool KCanELM327::SendCmd(const char * cmd)
{
  got_ok = go_none;
  if (!WriteBufferToCOM(cmd, (int) strlen(cmd)) ||
      !WriteBufferToCOM("\r", 2))
    return false;

  for (int i = 0; i < 200 && got_ok == go_none; i++)
  {
    KCanFrame Frame;
    
    ReceiveData(Frame);
    NUtils::SleepMs(1);
  }
  if (Trace)
  {
  #if defined(__CONSOLE__) 
    if (got_ok == go_none)
      printf("OK missing: %s\n", cmd);
    if (got_ok == go_ko)
      printf("got KO: %s\n", cmd);
  #endif
  }
  return got_ok == go_ok;
}

bool KCanELM327::ReceiveData(KCanFrame & Frame)
{
  int res = ReadFromCOM();
  while (res != NULL_CHAR)
  {
    //printf("%x  %c\n", res, (char) res);
    if (res < NULL_CHAR)
    {
      /*
      if (tel_len + 20 < sizeof(tel))
      {
        tel[tel_len++] = (char) res;
        tel[tel_len] = 0;
      }
      */
      switch (char(res))
      {
        case '>':
          tel_len = 0;
          return false;
          
        case 'O':
        case 'K':
          if (tel_len < 2)
            return false;
          
          got_ok = go_none;
          if (!strcmp((char *)tel + tel_len - 2, "OK"))
            got_ok = go_ok;
          if (!strcmp((char *)tel + tel_len - 2, "KO"))
            got_ok = go_ko;
          tel_len = 0;
          return false;

        case '\r':
        case '\n':
        {
          tel_len = 0;
         
          Frame.Init();
          
          const char * pos = (const char *) tel;
          unsigned char id;
          int id2;

          if (!NUtils::GetHexByte(pos, id))
            break;
          id2 = NUtils::GetHexDigit(*pos++);
          if (id2 < 0)
            break;

          Frame.Id = ((int)id << 4) + id2;
          while (*pos == ' ')
            pos++;

          while (*pos)
          {
            if (Frame.Len >= 8 ||
                !NUtils::GetHexByte(pos, Frame.Data[Frame.Len++]))
            {
              break;
            }

            while (*pos == ' ')
              pos++;
            if (!*pos)
            {
              NUtils::Time(Frame.TimeStampDay, Frame.TimeStampMs);
              return true;
            }
          }
          return false;
        }
        default:
          if ((unsigned char) res > 0x7f)
            tel_len = 0;
          break;
      }
    }
    res = ReadFromCOM();
  }
  NUtils::SleepMs(1);
  return false;
}

bool KCanELM327::SendData(const KCanFrame & Frame)
{
  char msg[128];

  WaitSentFrame = false;
  sprintf(msg, "ATSH%3.3X", Frame.Id);
  for (int i = 0; i < 7 && i < Frame.Len; i++)
    sprintf(msg + strlen(msg), " %2.2x", Frame.Data[i]);

  if (SendCmd(msg))
  {
    tel_len = 0;
    WaitSentFrame = true;
    SentFrame = Frame;
    NUtils::Time(SentFrame.TimeStampDay, SentFrame.TimeStampMs);
    if (Trace && AsSimulation)
    {
      char str[64];
      SentFrame.PrintTime(str, false);
      printf("send: %s %s\n", str, msg);
    }
    return true;
  } else
    WaitSentFrame = false;
  return false;
}
