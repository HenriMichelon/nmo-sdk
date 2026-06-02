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

    (c) 2001 Henri Michelon

$Id: ThThreadAtheos.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------------*/
#if defined(WIN32) || defined(_WIN32)
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Thread.hpp>
#include <atheos/threads.h>
#include <atheos/semaphore.h>

#define ID ((thread_id*)(this->mThreadAbstract))

//..............................................................................
static int nmo_mtthread_start_routine(void*TH)
{
	ASSERT(TH);
	ThThread *th = (ThThread*)TH;
	th->State() = ThThread::RUNNING;
	th->OnStart();
	th->Done();
	return 0;
}

ThThread::ThreadState& ThThread::State() { return state; };


//..............................................................................
ThThread::ThThread()
{
	state = WAITING;
	mThreadAbstract = NULL;
}


//..............................................................................
ThThread::~ThThread() 
{ 
   Stop(); 
}


//..............................................................................
void ThThread::Yield_() 
{
}


//..............................................................................
_BOOL ThThread::Start (ThreadPriority PRI)
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");

    int pri;
	switch (PRI)
	{
	case LOW:
	   pri = LOW_PRIORITY;
      	   break;
	case HIGH:
	   pri = DISPLAY_PRIORITY;
           break;
	case REAL_TIME:
	   pri = REALTIME_PRIORITY;
	   break;
	default:
	   pri = NORMAL_PRIORITY;
	   break;
	}
	Debug(dprintf("ThThread: new thread with priority=%d\n", pri);)

	mThreadAbstract = new thread_id;
        *ID = spawn_thread ("", nmo_mtthread_start_routine, pri, 0, this);
        return TRUE;
}


//..............................................................................
void ThThread::Done()
{
	state = FINISHED;
	if (mThreadAbstract != NULL) {
		delete (thread_id*)mThreadAbstract;
		mThreadAbstract = NULL;
	}
}


//..............................................................................
void ThThread::Stop()
{
	if (state == RUNNING) {
	}
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	if (state == RUNNING) {
	       wait_for_thread((*ID));
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#undef ID
#define ID ((sem_id*)(this->mCsAbstract))

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection::~ThCriticalSection()
{
	if (mCsAbstract) {
		Unlock();
		delete_semaphore((*ID));
		delete ID;
	}

}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Lock()
{
	lock_mutex((*ID), TRUE);
}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Unlock()
{
	unlock_mutex((*ID));
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
_BOOL ThCriticalSection::Init()
{
	mCsAbstract = new sem_id;
	return (!create_semaphore("", 1, SEM_WARN_DBL_LOCK));
}

#endif