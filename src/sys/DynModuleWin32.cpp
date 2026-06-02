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

$Id: DynModuleWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;

#define handle (this->mDynModuleAbstract)

//--------------------------------------------------------------------
_BOOL DynModule::Load(const Stringz&NAME)
{
	VALIDATETHIS;
	Stringz name(NAME);
	Stringz ext = File::FileExt(name);
	ext.UCase();
	if (ext != Stringz("DLL"))
		name += ".DLL";
	handle = LoadLibrary((LPCTSTR)name);
	return (handle != NULL);
}


//-------------------------------------------------------------------
_PTR DynModule::GetMethodAddress(const Stringz&NAME)
{
	VALIDATETHIS;
	ASSERT(handle);
	return GetProcAddress((HINSTANCE)handle, (LPCSTR)NAME);
}


//-------------------------------------------------------------------
DynModule::~DynModule()
{
	VALIDATETHIS;
	if (handle) { FreeLibrary((HINSTANCE)handle); }
}
#endif
