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

#include "NTypes.h"

  #include <sys/types.h>
  #include <sys/time.h>
  #include <unistd.h> 

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "NUtils.h"

namespace NUtils
{
/*
double TimeSecs()
// Anzahl Sekunden seit dem 31.12.1999 0:00
{
  int Days, Ms;

  Time(Days, Ms);
  return Days*86400.0 + ((double) Ms) / 1000.0;
}
*/
void Time(const struct timeval & Time, int & aDays, int & aMs)
{
  aMs = 1000*(Time.tv_sec % 86400) + Time.tv_usec/1000;
  aDays = (int) (Time.tv_sec / 86400);
  // 72 76 80 84 88 92 96 : 7 Schaltjahre
  aDays -= (2000-1970)*365 + (2000-1970)/4;
  if (aMs < 0)
  {
    aMs += 1000*86400;
    aDays--;
  }
}

const unsigned cMonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
// jan feb mar  apr  mai  jun  jul  aug  sep  okt  nov  dez
//  31  28  31   30   31   30   31   31   30   31   30   31

#if defined(__WINDOWS__)
void Time(int & aDays, int & aMs)
{
  SYSTEMTIME SysTime;

  GetLocalTime(&SysTime);

  // Tage und ms seit 31. Dezember 1999 00:00
  // aDays == 1 ist der 1.1.2000
  aDays = SysTime.wYear;
  aDays = aDays*365 +
          (aDays + 3) / 4 + // Zusatztag für die Schaltjahre
          cMonthDays[SysTime.wMonth-1] +
          SysTime.wDay - 1;

  // im Schaltjahr nach dem 1. März einen Tag addieren
  if (!(SysTime.wYear & 3) && SysTime.wMonth >= 3)
    aDays++;

  aDays -= 2000*365 + (2000 + 3) / 4;
  aMs = 3600 * ((int) SysTime.wHour) +
        60 * SysTime.wMinute +
        SysTime.wSecond;
  aMs = 1000 * aMs + SysTime.wMilliseconds;
}
#else

void Time(int & aDays, int & aMs)
{
  struct timeval _Time;
  struct timezone _tz;

  if (!gettimeofday(&_Time, &_tz))  // seit 1. Jan. 1970
  {
    _Time.tv_sec -= 60*_tz.tz_minuteswest;
    Time(_Time, aDays, aMs);
  } else {
    aMs = 0;
    aDays = 0;
  }
}

#endif

void GetDateFromDays(int aDays, int & aDay, int & aMonth, int & aYear)
// aDays == 0 entspricht dem 1.1.2000 (aDay = 1, aMonth = 1, aYear = 2000)
{
  aMonth = 1;
  aDay = aDays;

  // immer 4 Jahre abzählen: 4*365 Tage + 1 Tag vom Schaltjahr
  aYear = aDay / (4*365 + 1);
  aDay -= aYear*(4*365 + 1);
  aYear = 4*aYear + 2000;
  if (aDay <= 0)
  {
    aDay += 4*365 + 1;
    aYear -= 4;
  }
  // das 1. Jahr (z.B. 2012) ist ein Schaltjahr, danach bis zu 3 normale Jahre
  if (aDay >= 366)
  {
    aYear++;
    aDay -= 366;

    while (aDay >= 365)
    {
      aYear++;
      aDay -= 365;
    }
  }
  aDay++; // der erste Tag im Monat ist der 1.
  while ((int) cMonthDays[aMonth] < aDay)
  {
    if (aMonth == 2 && !(aYear & 3))
    {
      if ((int) cMonthDays[2]+1 >= aDay)  // 28. Feb. in einem Schaltjahr
        break;
      aDay--;    // für den 29. Feb. (ist nicht in cMonthDays enthalten)
    }
    aMonth++;
  }
  aDay -= cMonthDays[aMonth-1];
}

void GetDateAndTime(double aTime, int & aDay, int & aMonth, int & aYear,
                    int & aHour, int & aMin, int & aSec, int & aMs)
{
  double Days = aTime / 86400.0;

  GetDateFromDays(int (Days), aDay, aMonth, aYear);

  int Ms = int (1000.0*(aTime - 86400.0*int(Days)));
  aHour = Ms / 3600000;
  aMin = (Ms / 60000) % 60;
  aSec = (Ms / 1000) % 60;
  aMs = Ms % 1000;
}

bool GetDays(int Year, int Month, int Day, int & Days)
{
  if (Year < 2000 || Year > 2100 ||
      Month < 1 || Month > 12 ||
      Day < 1 || Day > 31)
    return false;

  if (Month == 2)
  {
    if ((Year % 4) != 0 && Day >= 29)
      return false;

    if (Day > 29)
      return false;
  } else
  if ((unsigned) Day > cMonthDays[Month] - cMonthDays[Month-1])
    return false;

  Year -= 2000;
  Days = 365*Year + (Year - 1) / 4 + cMonthDays[Month-1] + Day;
  if ((Year % 4) == 0 && Month > 2)
    Days++;

  return true;
}

void PrintDayTime(int time_ms, char * OutStr)
{
  int Sec = time_ms / 1000;
  int Min = Sec / 60;
  int Hour = Min / 60;

  sprintf(OutStr, "%2.2d:%2.2d:%2.2d.%3.3d",
          Hour, Min % 60, Sec % 60, time_ms % 1000);
}

void PrintTime(int time_days, int time_ms, char * OutStr, bool csv)
{
  int Day, Month, Year;

  GetDateFromDays(time_days, Day, Month, Year);
  if (csv)
  {
    sprintf(OutStr, "'%4.4d-%2.2d-%2.2d',%d", Year, Month, Day, time_ms);
  } else {
    char day_str[32];

    PrintDayTime(time_ms, day_str);
    sprintf(OutStr, "%d.%d.%d %s",
            Day, Month, Year, day_str);
  }
}

void PrintMs(int days, int ms, char * stream)
{
  int time_stamp_1 = days;
  int time_stamp_2;

  time_stamp_1 += 30*365 + 30 / 4; // Tage zwischen 1970 und 2000
  time_stamp_1 *= 24*36; // 86'4 * 10'957 = 9'466'848
  time_stamp_1 += ms / 100000;
  if (ms < 0)
  {
    time_stamp_2 = 100000 + ms % 100000;
    time_stamp_1--;
  } else
    time_stamp_2 = ms % 100000;

  // GMT time_stamp_1 -= 2*36;

  // ./ms2time.sh 1'399'346'245'967  ==> 2014-05-06 05:17:25
  // date -d "2014-05-06 05:17:25" ==>  1'399'346'245  (03:17:25 einsetzen GMT!)
  sprintf(stream, "%d%5.5d", time_stamp_1, time_stamp_2);
}

void PrintDateTime(int days, int ms, char * stream)
{
  // format: YYYY-MM-DD HH:MM:SS.mmm
  int Day, Month, Year;
  char day_str[32];

  GetDateFromDays(days, Day, Month, Year);
  PrintDayTime(ms, day_str);

  sprintf(stream, "%4.4d-%2.2d-%2.2d %s", Year, Month, Day, day_str);
}

bool IsDigit(char Digit)
{
  return ('0' <= Digit && Digit <= '9');
}

int GetDigit(char Digit)
{
  if ('0' <= Digit && Digit <= '9')
    return Digit - '0';

  return -1;
}

bool IsHexDigit(char aDigit)
{
  if (('0' <= aDigit && aDigit <= '9') ||
      ('a' <= aDigit && aDigit <= 'f') ||
			('A' <= aDigit && aDigit <= 'F'))
    return true;

  return false;
}

int GetHexDigit(char aDigit)
{
  if ('0' <= aDigit && aDigit <= '9')
    return (int) aDigit - '0';

  if ('A' <= aDigit && aDigit <= 'F')
    return (int) aDigit - ('A' - 10);

  if ('a' <= aDigit && aDigit <= 'f')
    return (int) aDigit - ('a' - 10);

  return -1;
}

unsigned GetHex(const char * & Line)
{
  if (!Line)
    return 0;

  unsigned Hex = 0;

  while (IsHexDigit(*Line))
  {
    Hex = 16*Hex + GetHexDigit(*Line++);
  }
  return Hex;
}

bool GetHexByte(const char * & Line, unsigned char & byte)
{
  if (!IsHexDigit(Line[0]) ||
      !IsHexDigit(Line[1]))
    return false;

  byte = 16*GetHexDigit(Line[0]) +
            GetHexDigit(Line[1]);
  Line += 2;

  return true;
}

bool GetUnsigned(const char * & Line, unsigned & res)
{
  TInt64 r;
  bool Ok = GetInt(Line, r);
  if (Ok)
    Ok = (r >= 0 && r <= (TInt64) 0xffffffff);
  if (Ok)
    res = (unsigned) r;
  
  return Ok;
}
  
unsigned GetInt(TInt64 & res, const char * Line)
{
  if (!Line)
    return 0;

  const char * ptr = Line;
  bool Ok = GetInt(ptr, res);
  if (!Ok)
    return 0;

  return (unsigned)(ptr - Line);
}

bool GetInt(const char * & Line, TInt64 & res)
{
  if (!Line)
    return false;

  bool IsHex = false;
  if (Line[0] == '0' && Line[1] == 'x')
  {
    Line += 2;
    IsHex = true;
  } else
  if (Line[0] == '$')
  {
    Line++;
    IsHex = true;
  }
  if (IsHex)
  {
    if (!IsHexDigit(Line[0]))
      return false;

    unsigned u = GetHex(Line);
    res = (int) u;
    return true;
  }

  bool neg = *Line == '-';
  if (neg)
    Line++;

  bool Ok = ('0' <= *Line && *Line <= '9');

  res = 0;
  while ('0' <= *Line && *Line <= '9')
  {
    res = 10*res + (unsigned char)(*Line) - '0';
    Line++;
  }
  if (neg)
    res = -res;

  return Ok;
}

bool GetDouble(const char * & Ptr, double & res)
{
  bool neg = *Ptr == '-';
  if (neg)
    Ptr++;

  res = 0;
  if (*Ptr < '0' || *Ptr > '9')
    return false;

  while ('0' <= *Ptr && *Ptr <= '9')
  {
    res = 10.0*res + (unsigned char)(*Ptr - '0');
    Ptr++;
  }
  if (*Ptr == '.')
  {
    double Res2 = 0;
    double quot = 1.0;

    Ptr++;
    if (*Ptr < '0' || *Ptr > '9')
      return false;

    while ('0' <= *Ptr && *Ptr <= '9')
    {
      Res2 = 10.0*Res2 + (unsigned char)(*Ptr - '0');
      quot *= 10.0;
      Ptr++;
    }
    Res2 /= quot;
    res += Res2;
  }
  if (neg)
    res = -res;

  return true;
}

bool IsNumber(const char * aPtr)
{
  TInt64 res;

  if (!GetInt(aPtr, res))
    return false;

  return *aPtr == 0;
}
  
bool SkipBlanks(const char * & Line)
{
  bool Ok = *Line == ' ' || *Line == 9; // horizontal tab
  
  while (*Line == ' ' || *Line == 9)
    Line++;
  
  return Ok;
}

char LowerCase(char c)
{
  if ('A' <= c && c <= 'Z')
    c += (char)('a' - 'A');

  return c;
}

char UpperCase(char c)
{
  if ('a' <= c && c <= 'z')
    c -= (char)('a' - 'A');

  return c;
}
  
void ToLowerCase(char * Str)
{
  if (!Str)
    return;

  while (*Str)
  {
    if ('A' <= *Str && *Str <= 'Z')
      *Str = (char)(*Str + ('a' -'A'));
    Str++;
  }
}
  
const char * ExtractFilename(const char * FilePath)
{
  if (!FilePath)
    return "";

  for (long Pos = strlen(FilePath)-1; Pos >= 0; Pos--)
  {
    char c = FilePath[Pos];
    if (c == '/' || c == '\\')
      return FilePath + Pos + 1;
  }
  return FilePath;
}




bool DecodeUnixTime(const char * aDate, tm & aTm)
{
  bool Ok = true;

  memset(&aTm, 0, sizeof(aTm));
  // Valid range for 32 bit Unix time_t:  1970 through 2038
  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_year = 10*aTm.tm_year + *aDate++ - '0';
  if (*aDate == '/')
    aDate++;
  aTm.tm_year -= 1900; // calendar year minus 1900

  aTm.tm_wday = aTm.tm_year + ((aTm.tm_year+3) / 4); // Weekday (0..6; Sunday = 0)

  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_mon = 10*aTm.tm_mon + *aDate++ - '0';
  if (*aDate == '/')
    aDate++;
  if (aTm.tm_mon == 0 || aTm.tm_mon > 12) // 0..11
  {
    aTm.tm_mon = 1;
    Ok = false;
  }
  aTm.tm_mon--;

  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_mday = 10*aTm.tm_mday + *aDate++ - '0';
  while (*aDate == ' ')
    aDate++;
  if (aTm.tm_mday == 0 || aTm.tm_mday > 31) // 1..31
  {
    aTm.tm_mday = 1;
    Ok = false;
  }

  aTm.tm_yday = aTm.tm_mday + cMonthDays[aTm.tm_mon]; // day of year 0..365
  if (aTm.tm_mon <= 1 || (aTm.tm_year & 3))
    aTm.tm_yday--;
  aTm.tm_wday = (aTm.tm_wday + aTm.tm_yday) % 7;

  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_hour = 10*aTm.tm_hour + *aDate++ - '0';
  if (*aDate == ':')
    aDate++;
  if (aTm.tm_hour >= 24)          // 0..23
  {
    aTm.tm_hour = 0;
    Ok = false;
  }

  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_min = 10*aTm.tm_min + *aDate++ - '0';
  if (*aDate == ':')
    aDate++;
  if (aTm.tm_min >= 60)           // 0..59
  {
    aTm.tm_min = 0;
    Ok = false;
  }

  while ('0' <= *aDate && *aDate <= '9')
    aTm.tm_sec = 10*aTm.tm_sec + *aDate++ - '0';
  if (aTm.tm_sec >= 60)           // 0..59
  {
    aTm.tm_sec = 0;
    Ok = false;
  }

  aTm.tm_isdst = -1;  // Information über Sommerzeit steht nicht zur Verfügung.

  return Ok;
}

static bool quitting = false;

bool term_signal_is_set()
{
  return quitting;
}

}
