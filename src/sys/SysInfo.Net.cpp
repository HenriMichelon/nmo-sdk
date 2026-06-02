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

	(c) 2000-2001 Henri Michelon

$Id: SysInfo.Net.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#ifdef _MANAGED
#include <nmo/NMO.hpp>
using namespace NMO;


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
	return "";
}


//----------------------------------------------------
Stringz SysInfo::Username()
{
	return "";
}


//----------------------------------------------------
Stringz SysInfo::Hostname()
{
	return Stringz("");
}


//----------------------------------------------------
Stringz SysInfo::Hardware()
{
	return "??";
}



//----------------------------------------------------
Stringz SysInfo::OSName()
{
#pragma push_macro("new")
#undef new
//	OperatingSystem * os = new OperatingSystem;
#pragma pop_macro("new")
/*	switch (get_Platform()) {
	case PlatformID::Win32NT: return "Windows NT";
	case PlatformID::Win32S: return "Windows 32";
	case PlatformID::Win32Windows: return "Windows";
	default: return "??";
	}*/
}



//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	return ("");
}



//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	return ("");
}

#endif
