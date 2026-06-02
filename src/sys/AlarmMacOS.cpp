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

	(c) 1998-2001 Henri Michelon

$Id: AlarmMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;

typedef struct 
{ 
	EventLoopTimerRef	timer;
	EventLoopTimerUPP	upp;
	_BOOL				locked;
} AlarmMacOSAbstract;

#define _m(x) ((AlarmMacOSAbstract*)(this->mAlarmAbstract))->x
#define __m(x) ((AlarmMacOSAbstract*)(alarm->mAlarmAbstract))->x

//--------------------------------------------
void nmo_alarm_handler(EventLoopTimerRef, void*DATA)
{
	Alarm *alarm = (Alarm*)DATA;
	if (!__m(locked)) {
		__m(locked) = TRUE;
		if (alarm->event) {
			(alarm->object->*alarm->event)(++alarm->nbCalls);
		}
		__m(locked) = FALSE;
	}
}

//--------------------------------------------
Alarm :: Alarm(_PTR O, const Alarm::Handler E): 
	started(FALSE), nbCalls(0), event(E)
{
	mAlarmAbstract = new AlarmMacOSAbstract;
	_m(locked) = FALSE;
	object = (NMOObject*)O;
}


//--------------------------------------------
Alarm::~Alarm()
{
	Stop();
	delete (AlarmMacOSAbstract*)mAlarmAbstract;
}


//--------------------------------------------
void Alarm :: Start(const _DWORD ms)
{
	if (!started) {
		nbCalls = 0;
		_m(upp) = NewEventLoopTimerUPP((EventLoopTimerProcPtr)nmo_alarm_handler);
		OSErr err = InstallEventLoopTimer(GetCurrentEventLoop(),
										ms * kEventDurationMillisecond,
										ms * kEventDurationMillisecond,
										_m(upp),
										(void*)this,
										&_m(timer));
		started = (err == noErr);
	}
}


//--------------------------------------------
void Alarm :: Stop()
{
	if (started) {
		started = FALSE;
		RemoveEventLoopTimer(_m(timer));
		DisposeEventLoopTimerUPP(_m(upp));
	}
}

#endif
