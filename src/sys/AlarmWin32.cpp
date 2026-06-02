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

$Id: AlarmWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <winreg.h>
# include <mmsystem.h>
#endif

List<Alarm> Alarm::timers;


//--------------------------------------------
void CALLBACK TimeProc(UINT uID, UINT, DWORD ms, DWORD, DWORD)
{
	ListIterator<Alarm> list(Alarm::timers);
	while (!list.End()) {
		Alarm &alarm = list.Next();
		if (alarm.ID == uID) {
			(alarm.object->*alarm.event)(++alarm.nbCalls);
			alarm.ID = ::timeSetEvent(ms, 5, TimeProc,
									 ms, TIME_ONESHOT);
			break;
		}
	}
}


//--------------------------------------------
Alarm :: Alarm(_PTR O, const Alarm::Handler E): 
	event(E), ID(0), nbCalls(0), started(FALSE) 
{
	object = (NMOObject*)O;	
}


//--------------------------------------------
Alarm::~Alarm()
{
	Stop();
}


//--------------------------------------------
void Alarm :: Start(const _DWORD ms)
{
	if (!started) {
		if ((ID = ::timeSetEvent(ms, 5, TimeProc, 
								 ms, TIME_ONESHOT)) != NULL) {
			nbCalls = 0;
			timers.Add(*this);
			started = TRUE;
		}
	}
}


//--------------------------------------------
void Alarm :: Stop()
{
	if (started) {
		timeKillEvent(ID);
		ListIterator<Alarm> list(timers);
		while (!list.End()) {
			Alarm &timer = list.Next();
			if (timer.ID == ID) {
				timers.Drop(list.Index()-1);
				break;
			}
		}
		started = FALSE;
	}
}


//--------------------------------------------
_BOOL operator == (const Alarm&A1, const Alarm&A2)
{
	return ((A1.event == A2.event) && (A1.ID == A2.ID));
}



//--------------------------------------------
_BOOL Alarm::Started() const { return started; }
#endif
