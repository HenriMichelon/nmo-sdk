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

$Id: SysInfoPosix.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------*/
#include "nmo/NMO.hpp"
#include <sys/utsname.h>
using namespace NMO;

//----------------------------------------------------
Stringz SysInfo::OSName()
{
	struct utsname name;
	if (!uname(&name))
		return name.sysname;
	else
		return "";
}


//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	struct utsname name;
	if (!uname(&name))
		return name.release;
	else
		return "";
}


//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	struct utsname name;
	if (!uname(&name))
		return name.version;
	else
		return "";
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
	struct utsname name;
	if (!uname(&name))
		return name.machine;
	else
		return "";
}


//----------------------------------------------------
Stringz SysInfo::Hostname()
{
	struct utsname name;
	if (!uname(&name))
		return name.nodename;
	else
		return "";
}


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
	Stringz r;
	if (!Env::Read("HOME", r))
		r = "";
	return Stringz(r);
}


//----------------------------------------------------
Stringz SysInfo::Username()
{
	Stringz r;
	if (!Env::Read("USER", r))
		r = "";
	return Stringz(r);
}
