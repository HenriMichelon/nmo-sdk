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

$Id: SysInfoMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;

#ifdef __MACH__
	#include <sys/utsname.h>
#endif

//----------------------------------------------------
Stringz SysInfo::OSName()
{
	return "MacOS";
}


//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	_LONG 	version;
	Stringz	result;
	if (Gestalt(gestaltSystemVersion, &version) == noErr) {
		result.Format("%hx", (version & 0xff00) >> 8);
	}
	return result;
}


//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	_LONG 	version;
	Stringz	result;
	if (Gestalt(gestaltSystemVersion, &version) == noErr) {
		result.Format("%hx.%hx.%hx",(version & 0xff00) >> 8,
								(version & 0xf0) >> 4,
								(version & 0x0f));
	}
	return result;
}


//----------------------------------------------------
void SysInfo::OSVersion(_DWORD&VER, _DWORD&REL, _DWORD&BUILD)
{
	_LONG 	version;
	if (Gestalt(gestaltSystemVersion, &version) == noErr) {
		Stringz	r;
		r.Format("%hx", (version & 0xff00) >> 8);
		VER = r;
		r.Format("%hx", (version & 0xf0) >> 4);
		REL = r;
		r.Format("%hx", (version & 0x0f));
		BUILD = r;
	}
	else {
		VER = 0;
		REL = 0;
		BUILD = 0;
	}
}


//----------------------------------------------------
Stringz SysInfo::Hardware()
{
	_LONG 	gestaltres;
	Str255	indstring;
	_CHAR	machine[255];
	Stringz	cpu;
	
	if (Gestalt(gestaltNativeCPUfamily, &gestaltres) == noErr) {
		switch (gestaltres) {
		case gestaltCPU601:
			cpu = "601";
			break;
		case gestaltCPU603:
			cpu = "603";
			break;
		case gestaltCPU604:
			cpu = "604";
			break;
		case gestaltCPU603e:
			cpu = "603e";
			break;
		case gestaltCPU603ev:
			cpu = "603ev";
			break;
		case gestaltCPU750:
			cpu = "750";
			break;
		case gestaltCPU604e:
			cpu = "604e";
			break;
		case gestaltCPU604ev:
			cpu = "604ev";
			break;
		case 268: //gestaltCPUG4:
			cpu = "G4";
			break;
		default:
			break;
		}
		if (gestaltres > 5) {
			cpu = "PowerPC " + cpu;
		}
	}
	
	machine[0] = 0;
	if (Gestalt(gestaltMachineType, &gestaltres) == noErr) {
		GetIndString(indstring, kMachineNameStrID, gestaltres);
		CopyPascalStringToC(indstring, machine);
	}
	
	Stringz smachine = machine;
	smachine.Trim();
	return cpu + ' ' + smachine;
}


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
/*#ifdef __MACH__
	Stringz r;
	if (!Env::Read("HOME", r)) { return ""; }
	return r;
#else*/
	short vrefnum;
	long dirID;
	OSErr err = FindFolder(kOnSystemDisk,
							kDesktopFolderType,
							kCreateFolder,
							&vrefnum,
							&dirID);
	if (err != noErr) {
		FindFolder(kOnSystemDisk,
					kUsersFolderType,
					kCreateFolder,
					&vrefnum,
					&dirID);
	}
	if (err != noErr) {
		FindFolder(kOnSystemDisk,
					kSystemFolderType,
					kCreateFolder,
					&vrefnum,
					&dirID);
	}
	return EnvPath::ExtractPath(vrefnum, dirID);
//#endif // __MACH__
}



//----------------------------------------------------
Stringz SysInfo::Hostname()
{
#ifdef __MACH__
	struct utsname name;
    if (!uname(&name))
		return name.nodename;
	else
#endif
	return "";
}

//----------------------------------------------------
Stringz SysInfo::Username()
{
#ifdef __MACH__
	Stringz r;
	if (!Env::Read("USER", r)) { return ""; }
	return r;
#else
	return "";
#endif
}

#endif
