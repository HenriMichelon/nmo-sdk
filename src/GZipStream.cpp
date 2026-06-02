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
			
$Id: GZipStream.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#include "datatypes/zlib/zlib.h"

#define GZH (*((gzFile*)(this->gzhandle)))

//-------------------------------------------------------------------
GZipStream::GZipStream(const Stringz&NAME, File::FileMode MODE,
					   _BYTE LEVEL, GzipStrategy STRATEGY):
	FileStream(NAME, MODE)
{
	Open(LEVEL, STRATEGY);
}


//-------------------------------------------------------------------
GZipStream::GZipStream(File*FILE, _BYTE LEVEL, GzipStrategy STRATEGY):
	FileStream(FILE)
{
	Open(LEVEL, STRATEGY);
}


//-------------------------------------------------------------------
GZipStream::~GZipStream()
{
	if (gzhandle !=  NULL) {
		gzclose(GZH);
		delete (gzFile*)gzhandle;
		gzhandle = NULL;
	}
}


//-------------------------------------------------------------------
void GZipStream::Open(_BYTE LEVEL, GzipStrategy STRATEGY)
{
	Stringz fmode;
	switch (FileObject()->Mode()) {
	case File::READ:
		fmode = "r"; break;
	case File::WRITE:
		fmode = "w"; break;
	case File::READWRITE:
		fmode = "r+"; break;
	case File::CREATE:
		fmode = "w+"; break;
	case File::APPEND:
		fmode = "a"; break;
	default:
		ASSERT(FALSE);
		break;
	}

	Stringz mode;
	if (FileObject()->Mode() != File::READ) {
		mode.Format("%s%01d", (_CHAR*)fmode, LEVEL);
		switch (STRATEGY) {
		case DEFAULT:
			  break;
		case FILTERED:
			  mode += "f"; break;
		case HUFFMANONLY:
			  mode += "h"; break;
		}
	}
	else {
		mode = fmode;
	}
	
	FileObject()->Close();
	Stringz name = FileObject()->FilePath();
	if (name.Len()) { name += PATHSEP; }
	gzFile gzfh = gzopen(name + FileObject()->FileName(), 
						  mode);
	if (gzfh== NULL) { 
		dprintf("GZipStream::Open: error gzopen()\n");
		throw ExcStreamCreate(); 
	}
	gzhandle = new gzFile;
	GZH = gzfh;
}


//-------------------------------------------------------------------
_DWORD GZipStream::Read(_PTR BUFF, const _DWORD SIZE)
{
	_DWORD r = gzread(GZH, BUFF, SIZE);
   	if (r != SIZE) {
		if (!(gzeof(GZH))) { throw ExcStreamRead(); }
	}
	return r;
}


//-------------------------------------------------------------------
_DWORD GZipStream::Write(const _PTR BUFF, const _DWORD SIZE)
{
	_DWORD r = gzwrite(GZH, (_PTR)BUFF, SIZE);
   	if (r != SIZE) {
		if (!(gzeof(GZH))) { throw ExcStreamWrite(); }
	}
	return r;
}


//-------------------------------------------------------------------
_DWORD GZipStream::Position() const
{
	return gztell(GZH);
}


//-------------------------------------------------------------------
void GZipStream::Seek(const _LONG POS, const SeekMode MODE)
{
	gzseek(GZH, POS, File::SeekMode(MODE));
}


//-------------------------------------------------------------------
_BOOL GZipStream::End() const
{
	return gzeof(GZH);
}


#ifdef __DEBUG
//-------------------------------------------------------------------
void GZipStream::Invariant() const 
{
	ASSERT(gzhandle);
}
#endif
