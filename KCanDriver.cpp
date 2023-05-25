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

#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

#include "NTypes.h"

#include "KCanDriver.h"

bool KCanDriver::Init(const char * CanDev)
{
  char d[High(dev)+5];
  struct stat Stat;
  
  dev[0] = 0;
  if (!CanDev || !*CanDev ||
      strlen(CanDev)+5 > High(dev))
    return false;

  strcpy(dev, CanDev);
  strcpy(d, dev);

#if defined(__LINUX__)
  
  #if !defined(__MAC__)
    // SocketCan driver
    // Raspi: /dev/can0 ist nicht definiert (aber unter ifconfig zu sehen)
    if (NCanUtils::GetDriverType(dev) == NCanUtils::dt_can)
    {
      return true;
    }
  #endif
  
  if (strncmp(dev, "/dev/", 5))
    strcpy(d, "/dev/");
  strcat(d, dev);
#else
  if (dev[0] == 'C')
    dev[0] = 'c';
  if (dev[1] == 'O')
    dev[1] = 'o';
  if (dev[2] == 'M')
    dev[2] = 'm';

  strcpy(d, dev);
#endif

  return !stat(d, &Stat) && (Stat.st_mode & S_IFMT);
}

bool KCanDriver::SendData(const KComfortFrame & Frame)
{
  KCanFrame CanFrame;

  if (!Frame.SetCanFrame(CanFrame))
    return false;

  return SendData(CanFrame);
}

