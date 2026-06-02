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

		Milisecond based timer.
		Use the "multimedia" timer functions of the
		MM API of Windows.
		Applications must be linked with winmm.lib.

	(c) 1998-2001 Henri Michelon

$Id: TimerWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <mmsystem.h>
#endif

typedef struct {
	_DWORD StartTime;
} AbstractTimerWin32;

#define __m(x) ((AbstractTimerWin32*)mTimerAbstract)->x


//--------------------------------------------
Timer :: Timer() 
{
	mTimerAbstract = NULL;
}


//--------------------------------------------
Timer :: ~Timer() 
{ 
	if (mTimerAbstract != NULL) {
		delete (AbstractTimerWin32*)mTimerAbstract;
	}
};


//--------------------------------------------
void Timer :: Start()
{
	if (mTimerAbstract == NULL) { mTimerAbstract = new AbstractTimerWin32; }
	__m(StartTime) = ::timeGetTime();
}


//--------------------------------------------
_DWORD Timer :: Stop()
{
	if (mTimerAbstract != NULL) {
		_DWORD res = ::timeGetTime() - __m(StartTime);
		return res;
	}
	return 0;
}


//--------------------------------------------
_DWORD Timer :: Value()
{
	if (mTimerAbstract != NULL) {
		return ::timeGetTime() - __m(StartTime);
	}
	return 0;
}


//--------------------------------------------
void Timer :: Sleep(const _DWORD time)
{
	::Sleep(time);
}
#endif
