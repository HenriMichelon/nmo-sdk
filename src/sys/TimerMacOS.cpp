/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: TimerMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-----------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;


typedef struct {
	UnsignedWide ticks;
} TimerMacOSAbstract;

#define __m(x) ((TimerMacOSAbstract*)(this->mTimerAbstract))->x


//--------------------------------------------
Timer :: Timer()
{
	mTimerAbstract = new TimerMacOSAbstract;
}


//--------------------------------------------
Timer :: ~Timer()
{
	delete (TimerMacOSAbstract*)mTimerAbstract;
}


//--------------------------------------------
void Timer :: Start()
{
	ASSERT(mTimerAbstract);
	Microseconds(&__m(ticks));
}


//--------------------------------------------
_DWORD Timer :: Stop()
{
	ASSERT(mTimerAbstract);
	UnsignedWide stopticks;
	Microseconds(&stopticks);
	stopticks.lo = (stopticks.lo - __m(ticks).lo) / 1000;
	stopticks.hi = (stopticks.hi - __m(ticks).hi) / 1000;
	__m(ticks).lo = 0;
	__m(ticks).hi = 0;
	return (stopticks.lo + (stopticks.hi << 32));
}

//--------------------------------------------
_DWORD Timer :: Value()
{
	ASSERT(mTimerAbstract);
	UnsignedWide stopticks;
	Microseconds(&stopticks);
	stopticks.lo = (stopticks.lo - __m(ticks).lo) / 1000;
	stopticks.hi = (stopticks.hi - __m(ticks).hi) / 1000;
	return (stopticks.lo + (stopticks.hi << 32));
}

//--------------------------------------------
void Timer :: Sleep(const _DWORD MS)
{
	UnsignedWide startticks;
	UnsignedWide stopticks;
	Microseconds(&startticks);
	EventRecord evtrecord;
	do {
		WaitNextEvent(everyEvent,
					&evtrecord,
					durationMillisecond,
					NULL);
		Microseconds(&stopticks);
	} while ((((stopticks.lo - startticks.lo) / 1000) + 
 			 ((stopticks.hi - startticks.hi) / 1000)) < MS);
}
#endif
