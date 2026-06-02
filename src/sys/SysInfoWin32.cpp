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

$Id: SysInfoWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
	Stringz r;
	if (!Env::Read("HOME", r))
		if (!Env::Read("WINDIR", r))
			r = "";
	return Stringz(r);
}


//----------------------------------------------------
Stringz SysInfo::Username()
{
	char name[1024];
	DWORD size = 1024;
	if (GetUserName(name, &size))
		return Stringz(name);
	else
		return Stringz("");
}


//----------------------------------------------------
Stringz SysInfo::Hostname()
{
	char name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(name, &size))
		return Stringz(name);
	else
		return Stringz("");
}


//----------------------------------------------------
Stringz SysInfo::Hardware()
{
	SYSTEM_INFO systemInfo;

	GetSystemInfo(&systemInfo);
	switch (systemInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return "x86";
	case PROCESSOR_ARCHITECTURE_MIPS:
		return "MIPS";
	case PROCESSOR_ARCHITECTURE_ALPHA:
		return "Alpha";
	case PROCESSOR_ARCHITECTURE_PPC:
		return "PPC";
	default:
		return "??";
	}
}



//----------------------------------------------------
Stringz SysInfo::OSName()
{
	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(osversion);
	if (GetVersionEx(&osversion))
	{
		switch(osversion.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_WINDOWS:
			return "Windows 4.x";
		case VER_PLATFORM_WIN32_NT:
			return "Windows NT";
		default:
			return "Windows ??";
		}
	}
	else
		return ("");
}



//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(osversion);
	if (GetVersionEx(&osversion))
	{
		Stringz v;
		v.Format("%d.%d", osversion.dwMajorVersion, osversion.dwMinorVersion);
		return Stringz(v);
	}
	else
		return ("");
}


//----------------------------------------------------
void SysInfo::OSVersion(_DWORD&VER, _DWORD&REL, _DWORD&BUILD)
{
	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(osversion);
	if (GetVersionEx(&osversion)) {
		VER = osversion.dwMajorVersion;
		REL = osversion.dwMinorVersion;
		if (osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) {
			BUILD = osversion.dwBuildNumber;
		}
		else {
			BUILD = LOWORD(osversion.dwBuildNumber);
		}
	}
}



//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	OSVERSIONINFO osversion;
	osversion.dwOSVersionInfoSize = sizeof(osversion);
	if (GetVersionEx(&osversion))
	{
		_DWORD build;
		if (osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) {
			build = osversion.dwBuildNumber;
		}
		else {
			build = LOWORD(osversion.dwBuildNumber);
		}
		Stringz rel;
		rel.Format("%d.%d.%d", osversion.dwMajorVersion, osversion.dwMinorVersion, build);
		return Stringz(rel);
	}
	else
		return ("");
}
#endif
