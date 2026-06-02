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

$Id: DirScan.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

//------------------------------------------------
void DirScan::ScanTree(DirScanList &D, const Stringz& path) const
{
	FindData		mydata;
	Stringz			mypath(path);
	List<Stringz> 	subdirs;
	if (path.Len())
		if (path.Right(1) == Stringz(PATHSEP))
			mypath = path.Copy(0, path.Len()-1);
	D.Add(new Stringz(path));
	//dprintf("first: %s\n", (char*)mypath);
	if (!FindFirstDir(mypath, mydata))
	{
		FindCloseDir(mydata);
		return;
	}
	do
	{
		mypath = path;
		if (path[path.Len()-1] != PATHSEP)
			mypath += PATHSEP;
		mypath += (char*)mydata.fileName;
		subdirs.Add(new Stringz(mypath));
	}
	while (FindNextDir(mydata));
	FindCloseDir(mydata);
	ListIterator<Stringz> list(subdirs);
	while (!list.End()) {
		ScanTree(D, list.Next());
	}
}


//------------------------------------------------
void DirScan::GetFileNames(DirScanList&F, DirScanList&D, DirScanList&E) const
{
	FindData	mydata;
	Stringz		*fname;
	Stringz		path;

	ListIterator<Stringz> lstE;
	ListIterator<Stringz> list(D);
	while (!list.End())
	{
		Stringz &dir = list.Next();
		lstE = E;
		lstE.Reset();
		while (!lstE.End())
		{
			if (dir[dir.Len()-1] == PATHSEP)
				path = dir;
			else
				path = dir + PATHSEP;
			path += lstE.Next();
			//dprintf("search for %s\n", (char*)path);
			if (!FindFirstFile(path, mydata))
			{
				FindCloseFile(mydata);
				continue;
			}
			do
			{
				if ((Stringz::Strcmp(mydata.fileName, "..")))
				{
					fname = new Stringz(dir);
					if ((*fname)[fname->Len()-1] != PATHSEP)
						*fname += PATHSEP;
					*fname += mydata.fileName;
					F.Add(fname);
				}
			}
			while (FindNextFile(mydata));
			FindCloseFile(mydata);
		}
	}
}
