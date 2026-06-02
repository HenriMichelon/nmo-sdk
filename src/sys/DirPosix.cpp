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

$Id: DirPosix.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <sys/param.h>
#include <sys/stat.h>
using namespace NMO;


//---------------------------------------------
_BOOL Dir::IsWriteable(const Stringz& NAME)
{
	// XXX: maybe we have to use something more secure than access()
	return access((_CHAR*)NAME, R_OK | W_OK | X_OK) == 0;
}


//---------------------------------------------
_BOOL Dir::Exists(const Stringz& NAME)
{
	struct stat fb;
	if (!stat((_CHAR*)NAME, &fb))
		return (fb.st_mode & S_IFDIR);
	else
		return FALSE;
}


//---------------------------------------------
Stringz Dir::Current()
{
	char buffer[MAXPATHLEN];
	if( getcwd(buffer, MAXPATHLEN) == NULL )
		return Stringz("");
	else
		return Stringz(buffer);
}


//---------------------------------------------
_BOOL Dir::Create(const Stringz& NAME)
{
	Stringz dir(NAME);
	Expand(dir);
	return (mkdir((char*)dir, 0777) == 0);
}
