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


				(c) 2000 Henri Michelon 

$Id: DirAmiga.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/dos_protos.h>
#include <nmo/NMO.hpp>
using namespace NMO;


//---------------------------------------------
_BOOL Dir::Remove(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	return (!::DeleteFile((char*)dir));
}


//---------------------------------------------
_BOOL Dir::Change(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	BPTR lock = ::Lock((_CHAR*)dir, SHARED_LOCK);
	if (lock != NULL) {
		_CHAR name[4096];
		::NameFromLock(lock, name, 4096);
		::UnLock(::CurrentDir(lock));
		::SetCurrentDirName(name);
		
		return TRUE;
	}
	return FALSE;
}


//---------------------------------------------
_BOOL Dir::IsWriteable(const Stringz& NAME)
{
	_BOOL res = FALSE;
	BPTR lock = ::Lock((_CHAR*)NAME, ACCESS_WRITE);
	res = (lock != NULL);
	if (!res) {
		lock = ::Lock((_CHAR*)NAME, ACCESS_READ);
		if (lock != NULL) {
			FileInfoBlock *infoBlock = (FileInfoBlock*)::AllocDosObject(DOS_FIB, NULL);
			if (infoBlock != NULL) {
				if (::Examine(lock, infoBlock)) {
					res = (infoBlock->fib_Protection & FIBF_WRITE) ||
							(infoBlock->fib_Protection & FIBF_OTR_WRITE) ||
							(infoBlock->fib_Protection & FIBF_GRP_WRITE);
				}
				::FreeDosObject(DOS_FIB, infoBlock);
			}
		}
	}
	if (res) { ::UnLock(lock); }
	return res;
}


//---------------------------------------------
_BOOL Dir::Exists(const Stringz& NAME)
{
	BPTR lock = ::Lock((_CHAR*)NAME, 0);
	if (lock != NULL) {
		::UnLock(lock);
		return TRUE;
	}
	return FALSE;
}


//---------------------------------------------
Stringz Dir::Current()
{
	char buffer[4096];
	if (::GetCurrentDirName(buffer, 4096) == DOSFALSE)
		return Stringz("");
	else
		return Stringz(buffer);
}


//---------------------------------------------
_BOOL Dir::Create(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	BPTR lock = ::CreateDir((char*)dir);
	if (lock != NULL) {
		::UnLock(lock);
		return TRUE;
	}
	return FALSE;
}
#endif
