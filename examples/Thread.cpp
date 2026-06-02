/*
$Id: Thread.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class my_thread: public ThThread
{
public:
	StdioApplication *app;

	void OnStart()
	{
		app->Write("waiting for critical section...\n");
		app->Flush();
		ThCriticalSection *cs = ThCriticalSection::Create("MYCS");
		cs->Lock();
		app->Write("entering critical section...\n");
		app->Flush();
		for (_DWORD i=0; i<9000; i++)
			for (_DWORD j=0; j<5000; j++)
				{ ThThread::Yield_(); };
		app->Write("exiting critical section...\n");
		app->Flush();
		cs->Unlock();
	}
};

class mt: public StdioApplication
{
public:
	mt(const UStringz&NAME): StdioApplication(NAME) {};

private:
	void OnCreate()
	{
		my_thread t1, t2;
		t1.app = this;
		t2.app = this;
		Write("thread 1 started: %d\n", t1.Start(ThThread::LOW));
		_DWORD i;
		for (i=0; i<5000; i++)
			for (_DWORD j=0; j<3000; j++)
				{ ThThread::Yield_(); };
		Write("thread 2 started: %d\n", t2.Start(ThThread::LOW));
		for (i=0; i<5000; i++)
			for (_DWORD j=0; j<5000; j++)
				{ ThThread::Yield_(); };
		Write("Waiting for the death of thread 1\n");
		t1.Wait();
		Write("Waiting for the death of thread 2\n");
		t2.Wait();
		Write("end\n");
#ifdef __DEBUG
		ThCriticalSection::cs.Clear();
#endif
	}

};

DebugOutput(NULL);
CreateApplication(mt, "Simple MT app")
