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

		Threads under MacOS using the Multiprocessing Services

$Id: ThThreadMacOSMP.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------------*/
#ifdef macintosh
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;

typedef struct
{
	MPTaskID	task;
	MPQueueID	queue;
} ThreadMacOSAbstract;
	

#define __m(x) ((ThreadMacOSAbstract*)(this->mThreadAbstract))->x

//..............................................................................
OSStatus nmo_mp_callback (void *TH)
{
	ThThread *th = (ThThread*)TH;
	th->State() = ThThread::RUNNING;
	th->OnStart();
	th->Done();
	return noErr;
}

ThThread::ThreadState& ThThread::State() { return state; };


//..............................................................................
ThThread::ThThread(): state(WAITING)
{
	mThreadAbstract = NULL;
}


//..............................................................................
ThThread::~ThThread() { Stop(); }

//..............................................................................
void ThThread::Yield_() 
{
	MPYield();
}


//..............................................................................
_BOOL ThThread::Start (ThreadPriority )
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");
	mThreadAbstract = new ThreadMacOSAbstract;
	OSStatus err = MPCreateQueue(&__m(queue));
	dprintf("create queue: %d\n", err);
	err = MPCreateTask(nmo_mp_callback,
						 this,
						 0,
						 __m(queue),
						 (_PTR)1,
						 (_PTR)2,
						 kMPCreateTaskValidOptionsMask,
						 &__m(task));
	Debug(dprintf("create task: %d\n", err);)
	return (err == noErr);
}


//..............................................................................
void ThThread::Done()
{
	if (mThreadAbstract) {
		MPDeleteQueue(__m(queue));
		delete (ThreadMacOSAbstract*)mThreadAbstract;
		mThreadAbstract = NULL;
	}
	state = FINISHED;
}



//..............................................................................
void ThThread::Stop()
{
	if (mThreadAbstract && (state == RUNNING)) {
		MPTerminateTask(__m(task), noErr);
	}
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	ASSERTMSG(state != WAITING, "ThThread::Wait: thread never start");
	if (state == RUNNING) {
		//MPWaitOnQueue(__m(queue),NULL,NULL,NULL,kDurationForever);
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#undef ID
#define ID (MPCriticalRegionID*)(this->mCsAbstract)

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection::~ThCriticalSection()
{
	if (mCsAbstract) {
		Unlock();
		MPDeleteCriticalRegion(*ID);
		delete ID;
	}

}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Lock()
{
	MPEnterCriticalRegion(*ID, kDurationForever);
}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Unlock()
{
	MPExitCriticalRegion(*ID);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
_BOOL ThCriticalSection::Init()
{
	mCsAbstract = (_PTR) new MPCriticalRegionID;
	return (MPCreateCriticalRegion(ID) == noErr);
}

#endif
