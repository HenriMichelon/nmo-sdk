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

		Threads under MacOS using the Thread Manager

$Id: ThThreadMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------------*/
#ifdef macintosh
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;

#define ID ((ThreadID*)(this->mThreadAbstract))

_DWORD ThThread :: gNumberOfThreads = 0;

//..............................................................................
voidPtr nmo_thmgr_callback (void *TH)
{
	++ThThread::gNumberOfThreads;
	ThThread *th = (ThThread*)TH;
	th->State() = ThThread::RUNNING;
	th->OnStart();
	th->Done();
	--ThThread::gNumberOfThreads;
	return NULL;
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
_BOOL ThThread::Start (ThreadPriority )
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");
	mThreadAbstract = new ThreadID;
	OSErr err = NewThread(kCooperativeThread,
					(OpaqueThreadEntryProcPtr*)nmo_thmgr_callback,
					this,
					0,
					kCreateIfNeeded,
					NULL,
					ID);
	Debug(dprintf("create task: %d\n", err);)
	return (err == noErr);
}


//..............................................................................
void ThThread::Done()
{
	if (mThreadAbstract) {
		DisposeThread(*ID, NULL, FALSE);
		delete ID;
		mThreadAbstract = NULL;
	}
	state = FINISHED;
}


//..............................................................................
void ThThread::Stop()
{
	ASSERT(mThreadAbstract);
	if (state == RUNNING) {
		SetThreadState(*ID, kStoppedThreadState, kNoThreadID);
	}
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	ASSERT(mThreadAbstract);
	if (state == RUNNING) {
		::ThreadState state;
		do {
			Yield_();
			GetThreadState(*ID, &state);
		}
		while (state != (::ThreadState)kStoppedThreadState);
	} 
}


//..............................................................................
void ThThread::Yield_()
{
	YieldToAnyThread ();
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
