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

$Id: DirSearchBSD.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


typedef struct {
	DIR	*dir;
	Stringz	rootPath;
	Stringz	extension;
	Stringz	cPath;
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
	__m(D, cPath) = Dir::Current();
	if ((__m(D, dir) = opendir((_CHAR*)P)) == NULL) {
		return FALSE;
	}
	__m(D, rootPath) = P;
	//dprintf("FirstDir: %s opened\n", (_CHAR*)D.rootPath);
	Dir::Change(__m(D, cPath));
	return FindNextDir(D);
}


//------------------------------------------------
_BOOL DirSearch::FindNextDir(DirSearch::FindData&D) const
{
	struct dirent 	*dir;
	Stringz			tmp;
	struct stat		stats;
	__m(D, cPath) = Dir::Current();
	while ((dir = readdir(__m(D, dir))) != NULL) {
		tmp = __m(D, rootPath) + PATHSEP + Stringz(dir->d_name);
		stat((char*)tmp, &stats);
		if (S_ISDIR(stats.st_mode)) {
			if ((Stringz::Strcmp(dir->d_name,".")) && 
				(Stringz::Strcmp(dir->d_name, ".."))) {
				D.fileName = dir->d_name;
				//dprintf("NextDir: %s\n", dir->d_name);
				Dir::Change(__m(D, cPath));
				return TRUE;
			}
		}
	} 
	Dir::Change(__m(D, cPath));
	return FALSE;
}


//------------------------------------------------
void DirSearch::FindCloseDir(DirSearch::FindData &D) const
{
	if (__m(D, dir) != NULL) { closedir(__m(D, dir)); }
}


//------------------------------------------------
_BOOL DirSearch::FindFirstFile(const Stringz &P, DirSearch::FindData&D) const
{	
	__m(D, cPath) = Dir::Current();
	int oldpos, pos = -1;
	do	{
		oldpos = pos;
		pos = P.InStr(PATHSEP, pos+1);
	} while (pos != -1);
	Stringz path = P.Copy(0, oldpos);
	
	if ((__m(D, dir) = opendir((char*)path)) == NULL) {
		return FALSE;
	}
	__m(D, rootPath) = P;
	//dprintf("FindFirstFile: %s opened\n", (char*)path);
	return FindNextFile(D);
}


//------------------------------------------------
_BOOL DirSearch::FindNextFile(DirSearch::FindData&D) const
{
	_LONG oldpos, pos = -1;
	do	{
		oldpos = pos;
		pos = __m(D, rootPath).InStr('/', pos+1);
	} while (pos != -1);
	Stringz path = __m(D, rootPath).Copy(oldpos+1, 
														__m(D, rootPath).Len()-oldpos);
	Stringz realPath = __m(D, rootPath).Copy(0, 
														__m(D, rootPath).Len()-path.Len());
	//dprintf("%s\n", (char*)realPath);

	struct dirent 	*dir;
	Stringz			tmp;
	while ((dir = readdir(__m(D, dir))) != NULL) {
		if ((!Stringz::Strcmp(dir->d_name, ".")) || 
			 (!Stringz::Strcmp(dir->d_name, ".."))) {
			continue;
		}
		if (!chdir((char*)(realPath+(const _CHAR*)dir->d_name))) {
			continue;
		}
		D.fileName = dir->d_name;
		if (path == "*") { return TRUE; }
		pos = path.InStr('*');
		if (pos == -1) {
			if (!Stringz::Strcmp(dir->d_name, (char*)path)) {
				return TRUE;
			}
		}
		if (pos>0) {
			//dprintf("%s  - %s\n", dir->d_name, (char*)path);
			if (Stringz::Strncmp(dir->d_name, (char*)path, pos)) {
				continue;
			}
			if (pos == ((_LONG)path.Len()-1)) {
				return TRUE;
			}
		}
		Stringz after = path.Copy(pos+1, path.Len()-pos);
		if (!Stringz::Strncmp(&dir->d_name[Stringz::Strlen(dir->d_name)-after.Len()], 
			(char*)after, after.Len())) {
			return TRUE;
		}
	} 
	return FALSE;
}


//------------------------------------------------
void DirSearch::FindCloseFile(DirSearch::FindData &D) const
{
	//dprintf("FindCloseFile: %s\n", (char*)D.rootPath);
	if (__m(D, dir) != NULL) {
		closedir(__m(D, dir));
		Dir::Change(__m(D, cPath));
	}
}
