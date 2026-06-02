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

			Alarms are implemented using timers
			and not the alarm() call to avoid
			signal perturbation.
						
 
	(c) 1998-2001 Henri Michelon 

$Id: AlarmBSD.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <signal.h>
#include <sys/time.h>

#ifdef HAVE_PTHREAD
# include <pthread.h>
#endif

// alarm resolution in ms
#define ALARM_RES 30

class AbstractAlarmUnix
{
public:
	static _BOOL	locked;
    _DWORD  totTime;
    _DWORD  remTime;
	inline AbstractAlarmUnix(): totTime(0), remTime(0) {};
}; 

#define __m(o, x) ((AbstractAlarmUnix*)o->mAlarmAbstract)->x

NMO::List<Alarm> Alarm::timers;
_BOOL Alarm::installed = FALSE;
_BOOL AbstractAlarmUnix::locked = FALSE;

static void* nmo_alarm_start_routine(void*ALARM)
{
	ASSERT(ALARM);
	Alarm *alarm = (Alarm*)ALARM;
	ASSERT(alarm->event);
	ASSERT(alarm->object);
	(alarm->object->*(alarm->event))(++alarm->nbCalls);
	return NULL;
}

//--------------------------------------------
void alarm_handler(int)
{
	if (AbstractAlarmUnix::locked) { return; }
	AbstractAlarmUnix::locked = TRUE;
	ListIterator<Alarm> list(Alarm::timers);
	while (!list.End()) {
		Alarm *alarm = &list.Next();
		if (__m(alarm, remTime)) {
			__m(alarm, remTime)--;
		}
		else {
#if defined(HAVE_PTHREAD) && ! defined(_LINUX)
			pthread_t threadid;
			pthread_create(&threadid, NULL, nmo_alarm_start_routine, alarm);
#else
			nmo_alarm_start_routine(alarm);
#endif
			__m(alarm, remTime) = __m(alarm, totTime);
		}
	}
	AbstractAlarmUnix::locked = FALSE;
}


//--------------------------------------------
Alarm :: Alarm(_PTR O, const Alarm::Handler E): 
									started(FALSE), 
									nbCalls(0), 
									event(E)
{
	object = (NMOObject*)O;
	mAlarmAbstract = new AbstractAlarmUnix;
}


//--------------------------------------------
Alarm::~Alarm()
{
	Stop();
	delete (AbstractAlarmUnix*)mAlarmAbstract;
	Debug(timers.Clear());
}


//--------------------------------------------
void Alarm :: Start(const _DWORD ms)
{
	if (!started) {
		if (!installed) {
#ifdef _IRIX
			signal(SIGALRM, (void (*)())alarm_handler);
#else
			signal(SIGALRM, alarm_handler);
#endif
			itimerval it;
			Memory::Fill(&it, 0, sizeof(it));
            it.it_interval.tv_sec = 0;
		    it.it_interval.tv_usec = 1000 * ALARM_RES;
			it.it_value.tv_sec = 0;
			it.it_value.tv_usec = 1000 * ALARM_RES;
			setitimer(ITIMER_REAL, &it, 0);
			installed = TRUE;
			//dprintf("Alarm::Start: global alarm handler started\n");
		}
		AbstractAlarmUnix::locked = TRUE;
		nbCalls = 0;
		__m(this, totTime) = ms / ALARM_RES;
		__m(this, remTime) = __m(this, totTime);
		timers.Add(*this);
		started = TRUE;
		AbstractAlarmUnix::locked = FALSE;
	}
}


//--------------------------------------------
void Alarm :: Stop()
{
	if (started) {
		AbstractAlarmUnix::locked = TRUE;
		ListIterator<Alarm> list(Alarm::timers);
		while (!list.End()) {
			Alarm &timer = list.Next();
			if (&timer == this) {
				timers.Drop(list.Index()-1);
				break;
			}
		}
		started = FALSE;
		if (!timers.Count()) {
			signal(SIGALRM, NULL);
			itimerval it;
			Memory::Fill(&it, 0, sizeof(it));
			setitimer(ITIMER_REAL, &it, 0);
			installed = FALSE;
		}
		AbstractAlarmUnix::locked = FALSE;
	}
}
