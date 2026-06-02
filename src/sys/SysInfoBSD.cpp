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

	(c) 2001 Henri Michelon

$Id: SysInfoBSD.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <sys/param.h>
#include <sys/types.h>
#include <sys/sysctl.h>

//----------------------------------------------------
Stringz SysInfo::OSName()
{
	size_t	len = 0;
	int		mib[] = { CTL_KERN, KERN_OSTYPE };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);
	return result;
}


//----------------------------------------------------
Stringz SysInfo::OSRelease()
{
	size_t	len = 0;
	int		mib[] = { CTL_KERN, KERN_OSRELEASE };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);
	return result;
}


//----------------------------------------------------
Stringz SysInfo::OSVersion()
{
	size_t	len = 0;
	int		mib[] = { CTL_KERN, KERN_VERSION };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);
	_LONG pos = result.InStr('\n');
	if (pos > -1) {
		return result.Left(pos);
	}
	return result;
}


//----------------------------------------------------
void SysInfo::OSVersion(_DWORD&VER, _DWORD&REL, _DWORD&BUILD)
{
	VER = 0;
	REL = 0;
	BUILD = 0;

	size_t	len = 0;
	int		mib[] = { CTL_KERN, KERN_OSRELEASE };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);

	_LONG pos = result.InStr('.');
	if (pos > 0) {
		VER = result.Left(pos);
		REL = result.Right(result.Len() - pos - 1);
	}
}


//----------------------------------------------------
Stringz SysInfo::Hardware()
{
	size_t	len = 0;
	int		mib[] = { CTL_HW, HW_MACHINE };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);
	return result;
}


//----------------------------------------------------
Stringz SysInfo::Hostname()
{
	size_t	len = 0;
	int		mib[] = { CTL_KERN, KERN_HOSTNAME };
    sysctl(mib, 2, NULL, &len, NULL, 0);
	Stringz result(_DWORD(len + 1));
    sysctl(mib, 2, (_CHAR*)result, &len, NULL, 0);
	return result;
}


//----------------------------------------------------
Stringz SysInfo::Userhome()
{
	Stringz r;
	Env::Read("HOME", r);
	return r;
}


//----------------------------------------------------
Stringz SysInfo::Username()
{
	Stringz r;
	Env::Read("USER", r);
	return r;
}
