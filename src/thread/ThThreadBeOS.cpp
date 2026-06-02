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

$Id: ThThreadBeOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------------*/
#ifdef __BEOS__
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;

#define ID (*(thread_id*)(this->mThreadAbstract))

//..............................................................................
static int32 nmo_mtthread_start_routine(void*TH)
{
	ASSERT(TH);
	ThThread *th = (ThThread*)TH;
	th->State() = ThThread::RUNNING;
	th->OnStart();
	th->Done();
	return 0;
}


//..............................................................................
ThThread::~ThThread()
{
	Stop();
};

//..............................................................................
void ThThread::Yield_() {}


//..............................................................................
_BOOL ThThread::Start (ThreadPriority PRI)
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");
	int32 pri;
	switch (PRI)
	{
	case LOW:
		pri = B_LOW_PRIORITY;
		break;
	case HIGH:
		pri = B_URGENT_PRIORITY;		
		break;
	case REAL_TIME:
		pri = B_REAL_TIME_PRIORITY;		
		break;
	default:
		pri = B_NORMAL_PRIORITY;		
		break;
	}
	mThreadAbstract = new thread_id;
	ID = spawn_thread(thread_func(nmo_mtthread_start_routine),
							NULL,
							pri,
							this);
	return (ID != 0);
}


//..............................................................................
void ThThread::Done()
{
	if (mThreadAbstract ) {
		delete ID;
		mThreadAbstract = NULL;
	}
	state = FINISHED;
}

//..............................................................................
void ThThread::Stop()
{
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	if (state == RUNNING) {
		wait_for_thread(ID, NULL);
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection::~ThCriticalSection()
{
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Lock()
{
}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Unlock()
{
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
_BOOL ThCriticalSection::Init()
{
	return FALSE;
}

#endif
