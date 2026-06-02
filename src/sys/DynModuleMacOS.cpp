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

$Id: DynModuleMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
---------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;

#define handle (this->mDynModuleAbstract)

//--------------------------------------------------------------------
_BOOL DynModule::Load(const Stringz&NAME)
{
	VALIDATETHIS;
	Str255 pname;
	Str255 errmsg;
	Ptr addr;
	Stringz name(NAME);
	name.LCase();
	CopyCStringToPascal(name, pname);
	CFragConnectionID *cid = new CFragConnectionID;
	OSErr err = GetSharedLibrary(pname, 
								 kPowerPCCFragArch,
								 kLoadCFrag,
								 cid,
								 &addr,
								 errmsg);						 
	if (err != noErr) {
		name += ".dll";
		CopyCStringToPascal(name, pname);
		err = GetSharedLibrary(pname, 
								 kPowerPCCFragArch,
								 kLoadCFrag,
								 cid,
								 &addr,
								 errmsg);
	}
	if (err == noErr) {	
		handle = cid; 
	}
	else {
		delete cid;
		Debug(dprintf("Error loading shared library: %d\n", err);)
	}
	return (handle != NULL);
}


//-------------------------------------------------------------------
_PTR DynModule::GetMethodAddress(const Stringz&NAME)
{
	VALIDATETHIS;
	ASSERT(handle);
	Ptr addr;
	Str255 pname;
	CFragSymbolClass sclass;
	CopyCStringToPascal(NAME, pname);
	OSErr err = FindSymbol(*(CFragConnectionID*)handle,
							(ConstStr255Param) pname,
							&addr,
							&sclass);
	if (err == noErr) { return addr; }						
	Debug(else { dprintf("Error in finding symbol: %d\n", err); })
	return NULL;
}


//-------------------------------------------------------------------
DynModule::~DynModule()
{
	VALIDATETHIS;
	if (handle) { 
		CloseConnection((CFragConnectionID*)handle); 
		delete (CFragConnectionID*)handle;
	}
}

#endif
