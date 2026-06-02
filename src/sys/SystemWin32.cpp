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

	(c) 1999-2001 Henri Michelon

$Id: SystemWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
---------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <process.h>
# include <io.h>
#endif


//------------------------------------------------------
_BOOL System::Execute(const Stringz &PATH, Args&ARGS, ExecMode MODE, _PID*)
{
	int mode;
	_LONG i=0;
	char** argv = new char*[ARGS.Count()+1];
	ListIterator<Stringz> args(ARGS);
	args.Reset();
	while (!args.End()) {
		argv[i++] = (char*)args.Next();
	}
	argv[i] = NULL;
	switch (MODE)
	{
	case EXEC_OVERLAY:
		mode = _P_OVERLAY;
		break;
	case EXEC_NORMAL:
		mode = _P_WAIT;
		break;
	case EXEC_BACKGROUND:
		mode = _P_NOWAIT;
		break;
	}
#ifdef _MSC_VER
	int r = (int) _spawnvp(mode, (char*)PATH, (const char*const*)argv);
#else
	int r = spawnvp(mode, (char*)PATH, (char*const*)argv);
#endif
	delete []argv;
	return (r != -1);
}


//------------------------------------------------------
_LONG System::Rand()
{
	return rand();
}

//------------------------------------------------------
void System::InitRand()
{
	srand( (unsigned)time( NULL ) );
}

typedef BOOL (* TOpenProcessToken)(HANDLE, DWORD, PHANDLE);
typedef BOOL (* TLookupPrivilegeValue)(LPCTSTR, LPCTSTR, PLUID);
typedef BOOL (* TAdjustTokenPrivileges)(HANDLE, BOOL, PTOKEN_PRIVILEGES, DWORD, PTOKEN_PRIVILEGES, PDWORD);

//------------------------------------------------------
_BOOL System :: Shutdown (System::ShutdownMode MODE)
{
	UINT uFlags;

	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(osversion);
	GetVersionEx(&osversion);
	if (osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		/*HMODULE hlib = GetModuleHandle("Advapi32");
		if (!hlib) {
			dprintf("System::Shutdown: LoadLibrary failed\n");
			return FALSE;
		}

		TOpenProcessToken POpenProcessToken = (TOpenProcessToken)GetProcAddress(hlib, "OpenProcessToken");
		TLookupPrivilegeValue PLookupPrivilegeValue = (TLookupPrivilegeValue)GetProcAddress(hlib, "LookupPrivilegeValueW");
		TAdjustTokenPrivileges PAdjustTokenPrivileges = (TAdjustTokenPrivileges)GetProcAddress(hlib, "AdjustTokenPrivileges");

		if ((!POpenProcessToken) || (!PLookupPrivilegeValue) || (!PAdjustTokenPrivileges)) {
			dprintf("System::Shutdown: GetProcAddress failed\n");
			return FALSE;
		}*/

		HANDLE hToken;              // handle to process token 
		TOKEN_PRIVILEGES tkp;       // pointer to token structure 

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  {
			dprintf("System::Shutdown: OpenProcessToken failed.");
			return FALSE;
		}

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
				&tkp.Privileges[0].Luid); 
 
		tkp.PrivilegeCount = 1;  // one privilege to set   
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
			(PTOKEN_PRIVILEGES)NULL, 0); 

#if 0
		if (!InitiateSystemShutdown(
					    NULL,    /* shut down local computer */
					    NULL,			/* msg. to user */
					    5,              /* time-out period */
					    FALSE,			/* prompt user to close apps. */
					    (MODE == SHUTDOWN_REBOOT) ||
						(MODE == SHUTDOWN_POWEROFF))) {          /* reboot after shutdown  */
		    dprintf("System::Shutdown: InitiateSystemShutdown failed.\n");
			return FALSE;
		}

		/* Disable shutdown privilege. */
		tkp.Privileges[0].Attributes = 0;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
							(PTOKEN_PRIVILEGES) NULL, 0);
		return TRUE;
#endif
	}

	switch (MODE) {
	case SHUTDOWN_HALT:
		uFlags = EWX_SHUTDOWN;
		break;
	case SHUTDOWN_POWEROFF:
		uFlags = EWX_POWEROFF;
		break;
	case SHUTDOWN_REBOOT:
		uFlags = EWX_REBOOT;
		break;
	default:
		return FALSE;
	}
	return ExitWindowsEx(uFlags, 0);
}
#endif
