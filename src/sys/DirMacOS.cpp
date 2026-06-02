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

	(c) 1998-2001 Henri Michelon

$Id: DirMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;

//---------------------------------------------
_BOOL Dir::Remove(const Stringz& NAME)
{
	Str255 pstr;
	CopyCStringToPascal(NAME, pstr);
	return (::HDelete(0, 0, pstr) == noErr);
}


//---------------------------------------------
_BOOL Dir::Create(const Stringz& NAME)
{
	Str255		pstr;
	HFileParam	fpb;
	CopyCStringToPascal(NAME, pstr);
	fpb.ioNamePtr = pstr;
	fpb.ioVRefNum = 0;
	fpb.ioDirID = 0;
	return (PBDirCreateSync((HParmBlkPtr)&fpb) == noErr);
}

//---------------------------------------------
_BOOL Dir::Change(const Stringz& )
{
	return FALSE;
}


//---------------------------------------------
_BOOL Dir::IsWriteable(const Stringz& )
{
//	return access((_CHAR*)NAME, R_OK | W_OK | X_OK) == 0;
	return TRUE;
}


//---------------------------------------------
_BOOL Dir::Exists(const Stringz& NAME)
{
	FSSpec spec;
	Str255	fileName;
	CopyCStringToPascal(NAME, fileName);
	OSErr err = FSMakeFSSpec(0, 0L, fileName, &spec);
	return (err == noErr);
}


//---------------------------------------------
Stringz Dir::Current()
{
	long 		dirID;
	short 		vrefnum;
	if (::HGetVol(NULL, &vrefnum, &dirID) == noErr) {
		return EnvPath::ExtractPath(vrefnum, dirID);
	}
	else {
		return Stringz("");
	}
}


#endif
