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

$Id: ThThreadAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Thread.hpp>
#include <dos/dostags.h>
#include <exec/semaphores.h>

class AbstractThreadAmiga
{
public:
	struct Task *task;
	struct Task *parent;
	ThThread::ThreadPriority pri;
};

#define _m(x) ((AbstractThreadAmiga*)(this->mThreadAbstract))->x
#define __m(t,x) ((AbstractThreadAmiga*)(t->mThreadAbstract))->x

//..............................................................................
int nmo_mtthread_start_routine()
{
	::Delay(10);
	struct Task *me = FindTask(0L);
	if (me != NULL) {
		ThThread *th = (ThThread*) me->tc_UserData;
		if (th != NULL) {
			th->State() = ThThread::RUNNING;
			switch (__m(th, pri)) {
			case ThThread::LOW:
				SetTaskPri(me, -32);
				break;
			case ThThread::HIGH:
				SetTaskPri(me, 32);
				break;
			case ThThread::REAL_TIME:
				SetTaskPri(me, 64);
				break;
			case ThThread::NORMAL:
				SetTaskPri(me, 0);
				break;
			}

			th->OnStart();
			SetTaskPri(me, 0);
			::Signal(__m(th,parent), SIGBREAKF_CTRL_F);
			th->Done();
			return 0;
		}
		Debug(dprintf("nmo_mtthread_start_routine: current task does not have a ThThread pointer\n");)
	}
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
ThThread::~ThThread() { Stop(); }


//..............................................................................
void ThThread::Yield_() { }


//..............................................................................
_BOOL ThThread::Start (ThreadPriority PRI)
{
	ASSERTMSG(state != RUNNING, "ThThread::Start: thread already running");
	Stringz name(Application::App().Name());
	name += " thread";
	mThreadAbstract = (_PTR) new AbstractThreadAmiga;
	_m(parent) = FindTask(0L);
	_m(pri) = PRI;
	_m(task) = (struct Task*)::CreateNewProcTags(NP_Entry, (ULONG) nmo_mtthread_start_routine,
												NP_Name, (ULONG) (_CHAR*)name,
												NP_StackSize, 16000,
												NP_Error, Output(),
												NP_CloseError, FALSE,
												NP_Output, Output(),
												NP_CloseOutput, FALSE);
	if (_m(task) == NULL) { 
		delete (AbstractThreadAmiga*)mThreadAbstract;
		mThreadAbstract = NULL;
		return FALSE; 
	}
	_m(task)->tc_UserData = (_PTR) this;
	return TRUE;
}


//..............................................................................
void ThThread::Done()
{
	state = FINISHED;
	if (mThreadAbstract != NULL) {
		::DeleteTask(_m(task));
		delete (AbstractThreadAmiga*)mThreadAbstract;
		mThreadAbstract = NULL;
	}
}


//..............................................................................
void ThThread::Stop()
{
	if (state == RUNNING) { 
		::Signal(_m(task), SIGBREAKF_CTRL_C);
	}
	Done();
}


//..............................................................................
void ThThread::Wait()
{
	if (state == RUNNING) {
        ::SetSignal(0L, SIGBREAKF_CTRL_F | SIGBREAKF_CTRL_C);
        ::Wait(SIGBREAKF_CTRL_F | SIGBREAKF_CTRL_C);
	}
}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class AbstractCSAmiga
{
public:
	struct	::SignalSemaphore sem;
	_BOOL	locked;
};

#undef _m
#define _m(x) ((AbstractCSAmiga*)(this->mCsAbstract))->x

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection::~ThCriticalSection()
{
	if (mCsAbstract != NULL) {
		Unlock();
		delete (AbstractCSAmiga*)mCsAbstract;
	}

}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Lock()
{
	ObtainSemaphore(&_m(sem));
	_m(locked) = TRUE;
}

	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void ThCriticalSection::Unlock()
{
	if (_m(locked)) {
		ReleaseSemaphore(&_m(sem)); 
		_m(locked) = FALSE;
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
_BOOL ThCriticalSection::Init()
{
	if (mCsAbstract == NULL) { mCsAbstract = (_PTR) new AbstractCSAmiga; }
	Memory::Fill(&_m(sem), 0, sizeof(struct ::SignalSemaphore));
	_m(locked) = FALSE;
	InitSemaphore(&_m(sem));
	return TRUE;
}

#endif
