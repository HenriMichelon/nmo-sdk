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

	(c) 1999-2002 Henri Michelon

$Id: DaemonApplicationWin32.cpp,v 1.2 2002/11/13 07:59:17 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#include <nmo/DaemonApplication.hpp>

class DaemonWin32Abstract
{
public:
	DWORD mainThread;
	DWORD childThread;
};

#define DATA(x) ((DaemonWin32Abstract*)(this->mDaemonApplicationAbstract))->x

/*void nmo_daemonwin32_service_start (DWORD, LPTSTR) {
	DaemonApplication &app = (DaemonApplication&)Application::App();
	SERVICE_STATUS          MyServiceStatus; 
	SERVICE_STATUS_HANDLE   MyServiceStatusHandle; 

    DWORD status; 
    DWORD specificError; 
 
    MyServiceStatus.dwServiceType        = SERVICE_WIN32; 
    MyServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    MyServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | 
        SERVICE_ACCEPT_PAUSE_CONTINUE; 
    MyServiceStatus.dwWin32ExitCode      = 0; 
    MyServiceStatus.dwServiceSpecificExitCode = 0; 
    MyServiceStatus.dwCheckPoint         = 0; 
    MyServiceStatus.dwWaitHint           = 0; 
 
    MyServiceStatusHandle = RegisterServiceCtrlHandler( 
        "MyService", 
        MyServiceCtrlHandler); 
 
    if (MyServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
    { 
        SvcDebugOut(" [MY_SERVICE] RegisterServiceCtrlHandler 
            failed %d\n", GetLastError()); 
        return; 
    } 
 
    // Initialization code goes here. 
    status = MyServiceInitialization(argc,argv, &specificError); 
 
    // Handle error condition 
    if (status != NO_ERROR) 
    { 
        MyServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        MyServiceStatus.dwCheckPoint         = 0; 
        MyServiceStatus.dwWaitHint           = 0; 
        MyServiceStatus.dwWin32ExitCode      = status; 
        MyServiceStatus.dwServiceSpecificExitCode = specificError; 
 
        SetServiceStatus (MyServiceStatusHandle, &MyServiceStatus); 
        return; 
    } 
 
    // Initialization complete - report running status. 
    MyServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    MyServiceStatus.dwCheckPoint         = 0; 
    MyServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (MyServiceStatusHandle, &MyServiceStatus)) 
    { 
        status = GetLastError(); 
        SvcDebugOut(" [MY_SERVICE] SetServiceStatus error
            %ld\n",status); 
    } 
 
    
	app.OnCreate();
	app.OnDestroy();
}*/

//-------------------------------------------------
unsigned long __stdcall nmo_daemonwin32_start_routine(DaemonApplication *APP)
{
	ASSERT(APP);
	Timer::Sleep(10);
	APP->OnCreate();
	APP->Terminate();
	return 0;
}



//-------------------------------------------------
void dae_main_init(HINSTANCE hInst)
{
	Debug(dopen("debug.txt");)
	// Parse command line arguments
	// to create application arguments list
	Stringz arg;
	_CHAR* cline = GetCommandLine();
	for (_DWORD i=0; i<strlen(cline); i++)
	{
		if (cline[i] == ' ')
		{
			Application::Argv().Add(new Stringz(arg));
			arg = "";
		}
		else
			arg += cline[i];
	}
	Application::Argv().Add(new Stringz(arg));
	ListIterator<Stringz> list(Application::Argv());
	Application::SetPath(list[1]);
}


//------------------------------------------------------------------
DaemonApplication::DaemonApplication(const UStringz&N): Application(N)
{ }


//------------------------------------------------------------------
void DaemonApplication::SetName(const UStringz&)
{ }


//------------------------------------------------------------------
void DaemonApplication::Terminate()
{
	if (DATA(mainThread) != GetCurrentThreadId()) {
		PostThreadMessage(DATA(mainThread), WM_QUIT, 0, 0);
		ExitThread(0);
	}
	else {
		PostQuitMessage(0);
	}
}


//------------------------------------------------------------------
void DaemonApplication::Run()
{
/*	if (osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		SERVICE_STATUS          MyServiceStatus; 
		SERVICE_STATUS_HANDLE   MyServiceStatusHandle; 
 
				
	}
	else {*/
		mDaemonApplicationAbstract = (_PTR) new DaemonWin32Abstract;
		DATA(mainThread) = GetCurrentThreadId();
		CreateThread(NULL, 0,
					(LPTHREAD_START_ROUTINE)nmo_daemonwin32_start_routine,
					this, 0, &DATA(childThread));

		MSG msg;
		do {
			int res = GetMessage(&msg, NULL, 0, 0);
			if (res == 0) {
				break;
			}
			else if (res > 0) {
				if (msg.message == WM_QUIT) { 
					break; 
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		while(TRUE);

		OnDestroy();
		delete (DaemonWin32Abstract*)mDaemonApplicationAbstract;
	//}
}


//------------------------------------------------------------------
void DaemonApplication::Write(DaemonLogLevel, const _CHAR* MSG, ...)
{
Debug(
	_CHAR temp[1000];
	va_list arg;
	va_start(arg, MSG);
	_vsnprintf(temp, 1000, MSG, arg);
	va_end(arg);
	dprintf("%s\n", temp);
	)
}
#endif
