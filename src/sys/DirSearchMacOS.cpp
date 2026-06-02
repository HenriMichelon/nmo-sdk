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

	(c) 1998-2000 Henri Michelon

$Id: DirSearchMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;

typedef struct {
	HParamBlockRec	inforec;
	Str255			name;
} DirSearchAbstract;

#define __m(D, x) ((DirSearchAbstract*)D.abstract)->x

//------------------------------------------------
DirSearch::FindData :: FindData()
{
	abstract = (_PTR) new DirSearchAbstract;
}

DirSearch::FindData :: ~FindData()
{
	delete (DirSearchAbstract*)abstract;
}


//------------------------------------------------
_BOOL DirSearch::FindFirstDir(const Stringz &P, DirSearch::FindData&D) const
{
	/*CopyCStringToPascal(P, __m(D, name));
	__m(D, inforec).ioNamePtr = &__m(D, name);
	__m(D, inforec).ioVRefNum = 0;
	__m(D, inforec).ioDrDirID = 0;
	__m(D, inforec).ioACUser = 0;
	__m(D, inforec).ioFDirIndex = 1;
	__m(D, inforec).ioCompletion = NULL;*/
	return FindNextDir(D);
}


//------------------------------------------------
_BOOL DirSearch::FindNextDir(DirSearch::FindData&D) const
{

	return FALSE;
}


//------------------------------------------------
void DirSearch::FindCloseDir(DirSearch::FindData &D) const
{
}


//------------------------------------------------
_BOOL DirSearch::FindFirstFile(const Stringz &P, DirSearch::FindData&D) const
{	
	return FindNextFile(D);
}


//------------------------------------------------
_BOOL DirSearch::FindNextFile(DirSearch::FindData&D) const
{
	return FALSE;
}


//------------------------------------------------
void DirSearch::FindCloseFile(DirSearch::FindData &D) const
{

}
#endif
