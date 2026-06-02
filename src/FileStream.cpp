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

$Id: FileStream.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/sys/File.hpp>
#include <nmo/Stream.hpp>
#include <nmo/FileStream.hpp>
using namespace NMO;

#define fhandle ((File*)(this->mFileStreamAbstract))

//-------------------------------------------------------------------
FileStream::FileStream(const Stringz&NAME, File::FileMode MODE)
{
	if (!(mFileStreamAbstract = (_PTR)File::Open(NAME, MODE))) {
		throw ExcStreamCreate();
	}
}


//-------------------------------------------------------------------
FileStream::FileStream(File*FILE)
{
	ASSERTMSG(FILE, "Can't assign a NULL File* to a FileStream");
	mFileStreamAbstract = (_PTR)FILE;
}


//-------------------------------------------------------------------
FileStream::~FileStream()
{
	ASSERT(fhandle);
	delete (File*)mFileStreamAbstract;
}


//-------------------------------------------------------------------
_DWORD FileStream::Read(_PTR BUFF, const _DWORD SIZE)
{
	ASSERT(fhandle);
	_DWORD r = fhandle->Read(BUFF, SIZE);
   	if (r != SIZE) {
		if (!(fhandle->Eof())) { 
			Debug(dprintf("FileStream: error reading %d bytes (%d readed) from %s\n", 
						  SIZE, r, (_CHAR*)fhandle->FileName());)
			throw ExcStreamRead(); 
		}
	}
	return r;
}


//-------------------------------------------------------------------
_DWORD FileStream::Write(const _PTR BUFF, const _DWORD SIZE)
{
	ASSERT(fhandle);
	_DWORD r = fhandle->Write(BUFF, SIZE);
   	if (r != SIZE) {
		if (!(fhandle->Eof())) { 
			Debug(dprintf("FileStream: error writing %d bytes (%d readed) from %s\n", 
						  SIZE, r, (_CHAR*)fhandle->FileName());)
			throw ExcStreamWrite(); 
		}
	}
	return r;
}


//-------------------------------------------------------------------
_DWORD FileStream::Position() const
{
	ASSERT(fhandle);
	return fhandle->Position();
}


//-------------------------------------------------------------------
void FileStream::Seek(const _LONG POS, const SeekMode MODE)
{
	ASSERT(fhandle);
	fhandle->Seek(POS, File::SeekMode(MODE));
}


//-------------------------------------------------------------------
_BOOL FileStream::End() const
{
	ASSERT(fhandle);
	return fhandle->Eof();
}


//-------------------------------------------------------------------
_DWORD FileStream::Size()
{
	_DWORD pos = Position();
	Seek(0, END);
	_DWORD r = Position();
	Seek(pos, START);
	return r;
}


//-------------------------------------------------------------------
File* FileStream::FileObject() const 
{
	ASSERT(fhandle);
	return fhandle;
}


Debug(
//-------------------------------------------------------------------
void FileStream::Invariant() const 
{
	ASSERTMSG(fhandle, "FileStream: NULL file object");
}
)
