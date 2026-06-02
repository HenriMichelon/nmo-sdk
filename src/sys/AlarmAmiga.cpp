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

$Id: AlarmAmiga.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <nmo/NMO.hpp>
using namespace NMO;

NMO::List<Alarm> Alarm::timers;
_BOOL Alarm::installed = FALSE;


//--------------------------------------------
Alarm :: Alarm(_PTR O, const Alarm::Handler E): 
									started(FALSE), nbCalls(0),
									event(E)
{
	object = (NMOObject*)O;
}


//--------------------------------------------
Alarm::~Alarm()
{
	Stop();
	Debug(timers.Clear());
}


//--------------------------------------------
void Alarm :: Start(const _DWORD ms)
{
	if (!started) {
		if (!installed) {
			installed = TRUE;
		}
		nbCalls = 0;
		timers.Add(*this);
		started = TRUE;
	}
}


//--------------------------------------------
void Alarm :: Stop()
{
	if (started) {
		ListIterator<Alarm> list(timers);
		while (!list.End()) {
			Alarm &timer = list.Next();
			if (&timer == this) {
				timers.Drop(list.Index() - 1);
				break;
			}
		}
		started = FALSE;
	}
}
#endif
