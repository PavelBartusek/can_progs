/*
 *
 *  Copyright (C) 2007-2014 Jürg Müller, CH-5524
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

#if !defined(__NO_MULTITHREADS__)
  #if defined(__WINDOWS__)
    #include <windows.h>
  #else
    #include <pthread.h>
    #include <sys/sem.h>
  #endif
#endif

#include "NTypes.h"

#include "KCriticalSection.h"

#include "NCanUtils.h"

// Ohne diesen Trick wird der Konstruktor nicht generiert!
template <class FrameType>
static bool init_proc()
{
  FrameType canframe;
  KRingBuffer <KCanFrame> frame;
  frame.Push(canframe);
  frame.Pop(canframe);
  frame.RingCount();
  frame.RingBufferFull();

  return true;
}

static bool init = init_proc<KCanFrame>();


KCriticalSection::KCriticalSection()
{
  mSection = 0;
  #if !defined(__NO_MULTITHREADS__)
    #if defined(__WINDOWS__)
      mSection = new CRITICAL_SECTION;
      InitializeCriticalSection((CRITICAL_SECTION *) mSection);
    #else
      bool AttrOk = false;
      pthread_mutexattr_t Attribute;

      mSection = new pthread_mutex_t;
      if (!pthread_mutexattr_init(&Attribute))
      {
        AttrOk = pthread_mutexattr_settype(&Attribute, PTHREAD_MUTEX_RECURSIVE /* 1 */) == 0;
        pthread_mutex_init((pthread_mutex_t *) mSection, AttrOk ? &Attribute : NULL);

        pthread_mutexattr_destroy(&Attribute);
      } else
        pthread_mutex_init((pthread_mutex_t *) mSection, NULL);
    #endif
  #endif
}

KCriticalSection::~KCriticalSection()
{
  #if !defined(__NO_MULTITHREADS__)
    #if defined(__WINDOWS__)
      DeleteCriticalSection((CRITICAL_SECTION *) mSection);
      delete (CRITICAL_SECTION *) mSection;
    #else
      pthread_mutex_destroy((pthread_mutex_t *) mSection);
      delete (pthread_mutex_t *) mSection;
    #endif
  #endif
  mSection = NULL;
}

bool KCriticalSection::Acquire()
{
  if (!mSection)
    return false;

  #if !defined(__NO_MULTITHREADS__)
    #if defined(__WINDOWS__)
      EnterCriticalSection((CRITICAL_SECTION *) mSection);
    #else
      pthread_mutex_lock((pthread_mutex_t *) mSection);
    #endif
  #endif
  return true;
}

bool KCriticalSection::TryEnter()
{
  if (mSection)
  {
  #if !defined(__NO_MULTITHREADS__)
    #if defined(__WINDOWS__)
      #if(_WIN32_WINNT >= 0x0400)
        // TryEnterCriticalSection ist unter Windows 95 nicht verfï¿½gbar
        // Bei einem erfolgreichen TryEnterCriticalSection muss danach die
        // CriticalSection mittels LeaveCriticalSection wieder freigegeben werden.
        return TryEnterCriticalSection((CRITICAL_SECTION *) mSection) != 0;
      #else
        Acquire();
        return true;
      #endif
    #else
      return pthread_mutex_trylock((pthread_mutex_t *) mSection) == 0;
    #endif
  #endif
  }
  return false;
}

void KCriticalSection::Release()
{
  if (!mSection)
    return;

  #if !defined(__NO_MULTITHREADS__)
    #if defined(__WINDOWS__)
      LeaveCriticalSection((CRITICAL_SECTION *) mSection);
    #else
      pthread_mutex_unlock((pthread_mutex_t *) mSection);
    #endif
  #endif
}



template <class FrameType>
KRingBuffer<FrameType>::KRingBuffer()
{
  FrameCount = 0;
  RingHead = RingTail = 0;
  RingBufferSize = 10240;
  RingBuffer = new FrameType[RingBufferSize];
  Semaphore = new KCriticalSection;
}

template <class FrameType>
KRingBuffer<FrameType>::~KRingBuffer()
{
  delete Semaphore;
  delete [] RingBuffer;
}

template <class FrameType>
int  KRingBuffer<FrameType>::RingCount() const
{
  return (RingHead - RingTail + RingBufferSize) % RingBufferSize;
}

template <class FrameType>
bool KRingBuffer<FrameType>::RingBufferFull() const
{
  return RingCount() > 3*(RingBufferSize / 4);
}

template <class FrameType>
void KRingBuffer<FrameType>::IncRingPtr(volatile int & RingPtr)
{
  RingPtr = (RingPtr + RingBufferSize + 1) % RingBufferSize;
}

template <class FrameType>
void KRingBuffer<FrameType>::DecRingPtr(volatile int & RingPtr)
{
  RingPtr = (RingPtr + RingBufferSize - 1) % RingBufferSize;
}

template <class FrameType>
bool KRingBuffer<FrameType>::Push(const FrameType & Frame)
{
  bool Ok = true;
  if (Semaphore->Acquire())
  {
    IncRingPtr(RingHead);
    if (RingTail == RingHead)
    {
      IncRingPtr(RingTail);
      Ok = false;
    }
    RingBuffer[RingHead] = Frame;

    Semaphore->Release();
  }
  return Ok;
}

template <class FrameType>
bool KRingBuffer<FrameType>::Pop(FrameType & Frame)
{
  bool Ok = false;
  if (RingHead == RingTail)
    return Ok;

  if (Semaphore->Acquire())
  {
    if (RingTail != RingHead)
    {
      Ok = true;
      IncRingPtr(RingTail);
      Frame = RingBuffer[RingTail];
    }
    Semaphore->Release();
  }
  return Ok;
}

template <class FrameType>
bool KRingBuffer<FrameType>::GetNext(FrameType & Frame)
{
  bool Ok = false;
  if (RingHead == RingTail)
    return Ok;

  if (Semaphore->Acquire())
  {
    if (RingTail != RingHead)
    {
      Ok = true;
      IncRingPtr(RingTail);
      Frame = RingBuffer[RingTail];
      DecRingPtr(RingTail);
    }
    Semaphore->Release();
  }
  return Ok;
}
