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

$Id: DirWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <stdlib.h>
# include <direct.h>
#endif


//---------------------------------------------
_BOOL Dir::Exists(const Stringz& NAME)
{
	_DWORD attr = GetFileAttributes((_CHAR*)NAME);
	if (attr != 0xFFFFFFFF)
		return (attr & FILE_ATTRIBUTE_DIRECTORY);
	else
		return FALSE;
}


//---------------------------------------------
Stringz Dir::Current()
{
	char buffer[_MAX_PATH];
	if( getcwd(buffer, _MAX_PATH) == NULL )
		return Stringz("");
	else
		return Stringz(buffer);
}


//---------------------------------------------
_BOOL Dir::Create(const Stringz& NAME)
{
	return (!mkdir((char*)NAME));
}


//---------------------------------------------
_BOOL Dir::Remove(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	return (!rmdir((char*)dir));
}


//---------------------------------------------
_BOOL Dir::Change(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	return (!chdir((char*)dir));
}
#endif
