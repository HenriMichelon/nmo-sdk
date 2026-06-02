/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

 	(c) 1998-2002 Henri Michelon

$Id: TimerUnix.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-----------------------------------------------------------------------*/
#include <nmo/Base.hpp>
#include <nmo/Sys.hpp>
using namespace NMO;
#include <sys/time.h>
#include <time.h>

typedef struct {
#ifdef HAVE_CLOCKGETTIME
	timespec startTime;
#elif HAVE_GETTIMEOFDAY
    timeval startTime;
#endif
} TimerUnixAbstract;

#define __m(x) ((TimerUnixAbstract*)mTimerAbstract)->x


//--------------------------------------------
Timer :: Timer()
{ 
	mTimerAbstract = NULL;
}


//--------------------------------------------
Timer :: ~Timer()
{
	if (mTimerAbstract != NULL) { 
		delete (TimerUnixAbstract*)mTimerAbstract;
	}
}


//--------------------------------------------
void Timer :: Start()
{
	if (mTimerAbstract == NULL) { mTimerAbstract = new TimerUnixAbstract; }
#ifdef HAVE_CLOCKGETTIME
	clock_gettime(CLOCK_REALTIME, &__m(startTime));
#elif HAVE_GETTIMEOFDAY
	gettimeofday(&__m(startTime), NULL);
#endif	
}


//--------------------------------------------
_DWORD Timer :: Stop()
{
	if (mTimerAbstract) {
		_DWORD res = 0;
#ifdef HAVE_CLOCKGETTIME
		timespec endTime;
		clock_gettime(CLOCK_REALTIME, &endTime);
		endTime.tv_sec -= __m(startTime).tv_sec;
		endTime.tv_nsec -= __m(startTime).tv_nsec;
		res = ((endTime.tv_sec * 1000) + (endTime.tv_nsec / 1000000));
#elif HAVE_GETTIMEOFDAY
        timeval endTime;
        gettimeofday(&endTime, NULL);
        endTime.tv_sec -= __m(startTime).tv_sec;
        endTime.tv_usec -= __m(startTime).tv_usec;
        res = ((endTime.tv_sec * 1000) + (endTime.tv_usec/1000));
#endif
		return res;
	}
	return 0;
}


//--------------------------------------------
_DWORD Timer :: Value()
{
	if (mTimerAbstract) {
#ifdef HAVE_CLOCKGETTIME
		timespec endTime;
		clock_gettime(CLOCK_REALTIME, &endTime);
		endTime.tv_sec -= __m(startTime).tv_sec;
		endTime.tv_nsec -= __m(startTime).tv_nsec;
		return ((endTime.tv_sec * 1000) + (endTime.tv_nsec / 1000000));
#elif HAVE_GETTIMEOFDAY
        timeval endTime;
        gettimeofday(&endTime, NULL);
        endTime.tv_sec -= __m(startTime).tv_sec;
        endTime.tv_usec -= __m(startTime).tv_usec;
        return ((endTime.tv_sec * 1000) + (endTime.tv_usec/1000));
#endif
	}
	return 0;
}


//--------------------------------------------
void Timer :: Sleep(const _DWORD mseconds)
{
#ifdef HAVE_CLOCKGETTIME
    timespec startTime, itTime;
    clock_gettime(CLOCK_REALTIME, &startTime);
    do {
		clock_gettime(CLOCK_REALTIME, &itTime);
	}
    while (((itTime.tv_sec - startTime.tv_sec) * 1000) +
          (((itTime.tv_nsec - startTime.tv_nsec)/ 1000000)) < (_LONG)mseconds);
#elif HAVE_GETTIMEOFDAY
	timeval startTime, itTime;
    gettimeofday(&startTime, NULL);
	do  {
    	gettimeofday(&itTime, NULL);
	}
    while (((itTime.tv_sec - startTime.tv_sec) * 1000) +
          (((itTime.tv_usec - startTime.tv_usec)/ 1000)) < (_LONG)mseconds);
#endif
}

