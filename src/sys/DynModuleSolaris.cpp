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

	(c) 2000-2002 Henri Michelon

$Id: DynModuleSolaris.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <dlfcn.h>
#include <errno.h>

#define handle (this->mDynModuleAbstract)


//-------------------------------------------------------------------
_BOOL DynModule::Load(const Stringz&NAME)
{
	VALIDATETHIS;
	Stringz name(NAME);
	if (File::FileExt(name) != "so") { name += ".so"; }
	_PTR desc = dlopen((_CHAR*)name, RTLD_LAZY);
	Debug(if (!desc) { dprintf("dlerror: %s\n", dlerror()); })
	if (!desc) {
		if ((!desc) && (name.InStr(PATHSEP) == -1)) {
			desc = dlopen((_CHAR*)("lib" + name), RTLD_LAZY);
			Debug(if (!desc) { dprintf("dlerror: %s\n", dlerror()); })
		}
		if ((!desc)&& (name.InStr(PATHSEP) == -1)) {
			desc = dlopen((_CHAR*)("./" + name), RTLD_LAZY);
			Debug(if (!desc) { dprintf("dlerror: %s\n", dlerror()); })
   		}
	}
	Debug(if (!desc) dprintf("dlerror: %s\n", dlerror());)
	handle = desc;
	return (handle != NULL);
}


//-------------------------------------------------------------------
_PTR DynModule::GetMethodAddress(const Stringz&NAME)
{
	VALIDATETHIS;
	if (handle) {
#if defined(__OpenBSD__)
		Stringz name('_' + NAME);
		return dlsym(handle, (_CHAR*)name);
#else
		return dlsym(handle, (_CHAR*)NAME);
#endif
	}
	return NULL;
}


//-------------------------------------------------------------------
DynModule::~DynModule()
{
	VALIDATETHIS;
	if (handle != NULL) { dlclose(handle); }
}
