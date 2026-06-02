/*
$Id: DaemonApp.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DaemonApplication.hpp>
using namespace NMO;

class my_daemon: public DaemonApplication
{
public:
	my_daemon(UStringz N): DaemonApplication(N) {};

private:
	void OnCreate()
	{
		Write(NORMAL, "OnCreate");
		Write(NOTICE, "waiting for 5 seconds");
		dprintf("Waiting for 5 seconds\n");
		Timer::Sleep(5000);
		Write(WARNING, "a warning");
		Write(ERR, "an error");
		Write(PANIC, "a panic");
	}
	void OnDestroy()
	{
		Write(NORMAL, "OnDestroy");
	}
};

DebugOutput("daemon_debug.txt");
CreateApplication(my_daemon, "My Daemon");
