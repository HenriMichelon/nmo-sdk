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

$Id: SysInfoAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <exec/execbase.h>
#include <nmo/NMO.hpp>
using namespace NMO;

extern struct ExecBase *SysBase;

#ifndef AFF_68060
#define AFF_68060 (1L<<7)
#endif

//----------------------------------------------------
Stringz SysInfo::OSName()
{
#ifdef amigaos
	return "AmigaOS";
#endif
}


//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	Stringz res;
	res.Format("%d.%d", 
					SysBase->LibNode.lib_Version,
					SysBase->SoftVer);
	return res;
}


//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	  switch (SysBase->LibNode.lib_Version)
	  {
		case 34:
		case 35:
			return "1.3"; break;
		case 36:
			return "2.0"; break;
		case 37:
			return "2.04"; break;
		case 38:
			return "2.1"; break;
		case 39:
			return "3.0"; break;
		case 40:
			if (SysBase->SoftVer >= 68) {
				return "3.5"; break;
			}
			else {
				return "3.1"; break;
			}
		default:
			return "??"; break;
	  }
}


//----------------------------------------------------
void SysInfo::OSVersion(_DWORD&VER, _DWORD&REL, _DWORD&BUILD)
{
ASSERT(FALSE);
		VER = 0;
		REL = 0;
		BUILD = 0;
}


//----------------------------------------------------
Stringz SysInfo::Hardware()
{
	Stringz res;
	if (SysBase->AttnFlags & 255) {
		if (SysBase->AttnFlags & AFF_68060) {
			res = "m68060";
		}
		else if (SysBase->AttnFlags & AFF_68040) {
			res = "m68040";
		}
		else if (SysBase->AttnFlags & AFF_68030) {
			res = "m68030";
		}
		else if (SysBase->AttnFlags & AFF_68020) {
			res = "m68020";
		}
		else if (SysBase->AttnFlags & AFF_68010) {
			res = "m68010";
		}
		
		if (SysBase->AttnFlags & AFF_FPU40) {
			res += "+FPU";
		}
		else if (SysBase->AttnFlags & AFF_68882) {
			res += "+68882";
		}
		else if (SysBase->AttnFlags & AFF_68881) {
			res += "+68881";
		}
#ifdef _PPC
		res += " + PowerPC";
#endif
	}
	else {
#ifdef _PPC
		res = "PowerPC";
#endif
	}
	return res;
}


//----------------------------------------------------
Stringz SysInfo::Hostname()
{
	return "localhost";
}


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
	Stringz r;
	if (!Env::Read("HOME", r))
		r = "SYS:";
	return Stringz(r);
}


//----------------------------------------------------
Stringz SysInfo::Username()
{
	Stringz r;
	if (!Env::Read("USER", r))
		r = "amigaman";
	return Stringz(r);
}

#endif