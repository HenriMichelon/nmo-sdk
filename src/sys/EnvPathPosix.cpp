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

	(c) 2000-2001 Henri Michelon

$Id: EnvPathPosix.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;


EnvPath::EnvPath(PathType TYPE)
{
	switch (TYPE) {
	case PATH_BIN:
		{
			Stringz path;
			Env::Read("PATH", path);
			if (path.Len() == 0) { path = "/"; }
			path.Split(*this, VARSEP);
		}
		break;	  
	case PATH_SHARE:
		{
			Stringz dir = "/usr/local/share";
			if (!Dir::IsWriteable(dir)) { dir = SysInfo::Userhome(); }
			Add(new Stringz(dir));
		}
		break;
	case PATH_TEMP:
		if (Dir::Exists("/usr/tmp")) { Add(new Stringz("/usr/tmp")); }
		if (Dir::Exists("/var/tmp")) { Add(new Stringz("/var/tmp")); }
		if (Dir::Exists("/tmp")) { Add(new Stringz("/tmp")); }
		break;
	case PATH_CONF:
		{
			if (Dir::Exists("/usr/local/etc")) { Add(new Stringz("/usr/local/etc")); }
#ifdef __NetBSD__
			if (Dir::Exists("/usr/pkg/etc")) { Add(new Stringz("/usr/pkg/etc")); }
#endif
#ifdef _IRIX
			if (Dir::Exists("/usr/freeware/etc")) { Add(new Stringz("/usr/freeware/etc")); }
#endif
			if (Dir::Exists("/etc")) { Add(new Stringz("/etc")); }
			Add(new Stringz(SysInfo::Userhome()));
		}
		break;
	case PATH_LIB:
		{
			Stringz path;
			Env::Read("LD_LIBRARY_PATH", path);
			if (path.Len() == 0) { path = "/usr/local/lib"; }
			path.Split(*this, VARSEP);
		}
		break;
	case PATH_APPS:
		{
			Stringz dir = "/usr/local";
			if (!Dir::IsWriteable(dir)) { dir = SysInfo::Userhome(); }
			Add(new Stringz(dir));
		}
		break;
	default:
		break;
	}
}
