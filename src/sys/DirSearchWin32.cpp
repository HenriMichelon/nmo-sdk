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

$Id: DirSearchWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;


typedef struct
{
	WIN32_FIND_DATA win32data;
	HANDLE			win32handle;
} DirSearchAbstract;

#define DATA(D, x) ((DirSearchAbstract*)D.abstract)->x


//------------------------------------------------
DirSearch::FindData :: FindData()
{
	abstract = (_PTR) new DirSearchAbstract;
}

DirSearch::FindData :: ~FindData()
{
	delete (DirSearchAbstract*)abstract;
}



//------------------------------------------------------------------------
_BOOL DirSearch::FindFirstDir(const Stringz &P, FindData&D) const
{
	Stringz mypath = P;
	if (P[P.Len()-1] == PATHSEP)
	{
		mypath += (char*)ALLFILES;
	}
	else if ((mypath.InStr('*') == -1) && (mypath.InStr('?') == -1))
	{
		mypath += PATHSEP;
		mypath += (char*)ALLFILES;
	}
	DATA(D, win32handle) = ::FindFirstFile((char*)mypath, 
											&DATA(D, win32data));
	if (DATA(D, win32handle) == INVALID_HANDLE_VALUE)
		return FALSE;
	do
	{
		if ((Stringz::Strcmp(DATA(D, win32data).cFileName,".")) && 
			(Stringz::Strcmp(DATA(D, win32data).cFileName, "..")) &&
			(DATA(D, win32data).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
 			D.fileName = DATA(D, win32data).cFileName;
			return TRUE;
		}
	} while (::FindNextFile(DATA(D, win32handle), &DATA(D, win32data)));
	return FALSE;
}


//------------------------------------------------------------------------
_BOOL DirSearch::FindNextDir(FindData&D) const
{
	while (::FindNextFile(DATA(D, win32handle), &DATA(D, win32data))) 
	{
		if ((Stringz::Strcmp(DATA(D, win32data).cFileName,".")) && 
			(Stringz::Strcmp(DATA(D, win32data).cFileName, "..")) &&
			(DATA(D, win32data).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			D.fileName = DATA(D, win32data).cFileName;
			return TRUE;
		}
	} 
	return FALSE;
}


//------------------------------------------------------------------------
void DirSearch::FindCloseDir(FindData &D) const
{
	FindClose(DATA(D, win32handle));
}


//------------------------------------------------------------------------
_BOOL DirSearch::FindFirstFile(const Stringz& P, FindData&D) const
{
	DATA(D, win32handle) = ::FindFirstFile(P, &DATA(D, win32data));
	if (DATA(D, win32handle) == INVALID_HANDLE_VALUE)
		return FALSE;
	do
	{
		if (!(DATA(D, win32data).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			D.fileName = DATA(D, win32data).cFileName;
			return TRUE;
		}
	} while (::FindNextFile(DATA(D, win32handle), &DATA(D, win32data)));
	return FALSE;
}


//------------------------------------------------------------------------
_BOOL DirSearch::FindNextFile(FindData&D) const
{
	while (::FindNextFile(DATA(D, win32handle), &DATA(D, win32data)))
	{
		if (!(DATA(D, win32data).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			D.fileName = DATA(D, win32data).cFileName;
			return TRUE;
		}
	} 
	return FALSE;
}


//------------------------------------------------------------------------
void DirSearch::FindCloseFile(FindData &D) const
{
	FindClose(DATA(D, win32handle));
}
#endif
