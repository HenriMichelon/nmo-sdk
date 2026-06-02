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

$Id: TimerAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <devices/timer.h>
#include <clib/timer_protos.h>
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <nmo/NMO.hpp>
#include "AmigaDevices.hpp"
using namespace NMO;
#include <unistd.h>

class TimerAmiga 
{
public:
	static _DWORD nbInstance;
	static timerequest* TimeIO;
	struct timeval starttime;
};

struct Device *TimerBase;
_DWORD TimerAmiga :: nbInstance = 0;
timerequest* TimerAmiga::TimeIO = NULL;

#define __m(x) ((TimerAmiga*)this->mTimerAbstract)->x

//--------------------------------------------
Timer :: Timer()
{
	mTimerAbstract = new TimerAmiga;
	if (!TimerAmiga::nbInstance++) {
		AmigaDevices :: Open("timer.device", (ULONG) UNIT_VBLANK, 
									(struct IORequest **)&TimerAmiga::TimeIO, 0, 
									sizeof(struct timerequest));
		TimerBase = TimerAmiga::TimeIO->tr_node.io_Device;
	}
}


//--------------------------------------------
Timer :: ~Timer() 
{
	ASSERT(mTimerAbstract);
	delete (TimerAmiga*)mTimerAbstract;
	Stop();
	if (!--TimerAmiga::nbInstance) {
		AmigaDevices :: Close((struct IORequest *)TimerAmiga::TimeIO);
	}
}


//--------------------------------------------
void Timer :: Start()
{
	ASSERT(mTimerAbstract);
	AmigaDevices :: DoCommand((struct IORequest *)TimerAmiga::TimeIO, 
										(_WORD) TR_GETSYSTIME);
	__m(starttime) = TimerAmiga::TimeIO->tr_time;
}


//--------------------------------------------
_DWORD Timer :: Stop()
{
	ASSERT(mTimerAbstract);
	AmigaDevices :: DoCommand((struct IORequest *)TimerAmiga::TimeIO, 
									  TR_GETSYSTIME);
	::SubTime(&TimerAmiga::TimeIO->tr_time, &__m(starttime));
	return (TimerAmiga::TimeIO->tr_time.tv_secs * 1000 + 
			  TimerAmiga::TimeIO->tr_time.tv_micro / 1000);
}


//--------------------------------------------
_DWORD Timer :: Value()
{
	return Stop();
}


//--------------------------------------------
void Timer :: Sleep(const _DWORD mseconds)
{
	usleep(mseconds * 1000);
}

#endif
