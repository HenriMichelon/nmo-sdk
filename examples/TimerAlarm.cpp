/*
$Id: TimerAlarm.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;


class MyApp: public StdioApplication 
{ 
public:
	MyApp(UStringz N): StdioApplication(N) {};

private:
	Alarm	*alarm;
	void OnAlarm(const _DWORD ncalls)
	{
		Stringz msg;
		msg.Format("OnAlarm(): %d", ncalls);
		Write("%s\n", (char*)msg);
	}

	void OnCreate()
	{
		_DWORD	scantime;
		Timer	timer;
		timer.Start();
		alarm = new Alarm(this, Alarm::Handler(&MyApp::OnAlarm));
		alarm->Start(1000);

		Write("computing...\n");
		Stringz a;
		for (int i=0; i<1024*1024; i++) { 
			a = Name();
		};
		scantime = timer.Value();
		if (scantime>1000)
			Write("temp time: %d,%d secs.\n", scantime / 1000,
					(scantime - (scantime / 1000)*1000)/100);
		else
			Write("temp time: %d ms.\n", scantime);
		Write("sleeping 2 secs...\n");
		Timer::Sleep(2000);
		scantime = timer.Stop();
		if (scantime>1000)
			Write("Total processing time: %d,%d secs.\n", scantime / 1000,
					(scantime - (scantime / 1000)*1000)/100);
		else
			Write("Total processing time: %d ms.\n", scantime);
		delete alarm;
	}
};

//--------------------------------------------
DebugOutput(NULL);
CreateApplication(MyApp, "Timer & Alarm test");
