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

  	(c) 1998-2001 Henri Michelon

$Id: ThThreadWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Thread.hpp>
# include <windows.h>
# include <winbase.h>
#endif

#define ID (*(HANDLE*)(this->mThreadAbstract))

//..............................................................................
unsigned long __stdcall nmo_mtthread_start_routine(ThThread *TH)
{
	ASSERT(TH);
	ThThread *th = (ThThread*)TH;
	th->State() = ThThread::RUNNING;
	th->OnStart();
	th->Done();
	return 0;
}


//..............................................................................
void ThThread::Yield_() {}

//..............................................................................
ThThread::ThThread()
{
	mThreadAbstract = NULL;
}

//..............................................................................
ThThread::~ThThread() { Stop(); }

//..............................................................................
_BOOL ThThread::Start (ThreadPriority P)
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");
	DWORD id;
	mThreadAbstract = new HANDLE;
	ID = CreateThread(NULL, 0, 
					(LPTHREAD_START_ROUTINE)nmo_mtthread_start_routine, 
					this, 0, &id);
	if (ID) {
		switch(P) {
		case LOW:
			SetThreadPriority(ID, THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case HIGH:
		case REAL_TIME: // avoid real real time thread under Windows,
						// or you will probably regret that !
			SetThreadPriority(ID, THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		case NORMAL:
			break;
		}
	};
	return (ID != NULL);
}


//..............................................................................
void ThThread::Done()
{
	if (mThreadAbstract != NULL) {
		delete (HANDLE*)mThreadAbstract;
		mThreadAbstract = NULL;
	}
	state = FINISHED;
}


//..............................................................................
void ThThread::Stop()
{
	if (state == RUNNING) { TerminateThread(ID, 0); }
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	if (state == RUNNING) {
		WaitForSingleObject(ID, INFINITE);
	}
}


//..............................................................................
ThThread::ThreadState& ThThread::State()
{
	return state;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#undef ID
#define ID (*(HANDLE*)(this->mCsAbstract))

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection::~ThCriticalSection()
{
	if (mCsAbstract) {
		Unlock();
		CloseHandle(ID);
		delete (HANDLE*)mCsAbstract;
		mCsAbstract = NULL;
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Lock()
{
	ASSERT(mCsAbstract);
	WaitForSingleObject(ID, INFINITE);
}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Unlock()
{
	ASSERT(mCsAbstract);
	ReleaseMutex(ID);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
_BOOL ThCriticalSection::Init()
{
	mCsAbstract = new HANDLE;
	return ((ID = CreateMutex(NULL, FALSE, (_CHAR*)name)) != NULL);
}

#endif
