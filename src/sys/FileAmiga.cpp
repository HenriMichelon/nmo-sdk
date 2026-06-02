/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2000-2001 Henri Michelon 

$Id: FileAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <dos/dos.h>
#include <clib/dos_protos.h>
#include <nmo/NMO.hpp>
using namespace NMO;

#define BLKSIZE 8192
#define HANDLE ((BPTR)(this->handle))


//------------------------------------------------
_PTR File::Mode(File::FileMode fmode)
{
	switch(fmode)
	{
	case READ:
		return (_PTR)MODE_OLDFILE;
	case WRITE:
		return (_PTR)MODE_READWRITE;
	case READWRITE:
		return (_PTR)MODE_READWRITE;
	case CREATE:
		return (_PTR)MODE_NEWFILE;
	case APPEND:
		return (_PTR)MODE_READWRITE;
	default:
		ASSERT(FALSE);
		return NULL;
	}
}


//------------------------------------------------
File *File :: Open(const Stringz &NAME, const File::FileMode fmode)
{ 
	BPTR handle = ::Open((_CHAR*)NAME, LONG(Mode(fmode)));
	if (handle)
	{
		File *file = new File((_PTR)handle, NAME);
		file->mode = fmode;
		return file;
	}
	else
		return NULL;
}


//------------------------------------------------
File :: ~File()
{
	Close();
}


//------------------------------------------------
void File::Close()
{
	VALIDATETHIS;
	if (HANDLE) {
		::Flush(HANDLE);
		::Close(HANDLE);
		handle = NULL;
	}
}


//------------------------------------------------
_LONG File :: Read(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	return ::FRead(HANDLE, (void*)T, 1, N);
}


//------------------------------------------------
_LONG File :: Write(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	return (::FWrite(HANDLE, (void*)T, N, 1) * N);
}


//------------------------------------------------
_BOOL File :: Eof() const
{
	VALIDATETHIS;
	ASSERT(handle);
	LONG curpos = ::Seek(HANDLE, 0, OFFSET_END);
	_BOOL res = (curpos == ::Seek(HANDLE, 0, OFFSET_CURRENT));
	::Seek(HANDLE, curpos, OFFSET_BEGINNING);
	return res;
}


//------------------------------------------------
_BOOL File::Seek(const _LONG pos, const SeekMode MODE)
{
	VALIDATETHIS;
	LONG fmode;
	switch (MODE) {
		case END: 
			fmode = OFFSET_END; break;
		case CURRENT: 
			fmode = OFFSET_CURRENT; break;
		default: 
			fmode = OFFSET_BEGINNING; break;
	}
	return (::Seek(HANDLE, pos, fmode) == 0);
}



//------------------------------------------------
_LONG File::Position() const
{
	return ::Seek(HANDLE, 0, OFFSET_CURRENT);
}



//------------------------------------------------
_BOOL File::SetDateTime(const Stringz &NAME, const NMO::DateTime &M)
{
	Stringz	sdate;
	Stringz	stime;
	::DateTime	date;
	
	sdate.Format("%02d-%02d-%02d", 
						M.date.dayOfMonth,
						M.date.month,
						M.date.year);
	stime.Format("%02d:%02d:%02d",
						M.time.hour,
						M.time.minute,
						M.time.second);
	date.dat_Format = FORMAT_CDN;
	date.dat_Flags = 0;
	date.dat_StrDate = (UBYTE*)(_CHAR*)sdate;
	date.dat_StrTime = (UBYTE*)(_CHAR*)stime;
	::StrToDate(&date);
	return ::SetFileDate((_CHAR*)NAME, &date.dat_Stamp);
}


//------------------------------------------------------
_BOOL File::Remove(const Stringz &NAME)
{
	return ::DeleteFile((_CHAR*)NAME);
}


//------------------------------------------------
_BOOL File::CopyFile(const Stringz &src, const Stringz &dst)
{
	File *fd_src, *fd_dst;

	if ( (fd_src = File::Open((char*)src, File::READ)) == NULL)
		return FALSE;
	if ( (fd_dst = File::Open((char*)dst, File::CREATE)) == NULL) 
	{
		//Debug(dprintf("create %s\n", (char*)dst);)
		delete fd_src;
		return FALSE;
	}

	_DWORD nbytes;
	_BYTE *buff = new _BYTE[BLKSIZE];
	do {
		nbytes = fd_src->Read(buff, BLKSIZE);
		fd_dst->Write(buff, nbytes);
	} while (nbytes == BLKSIZE);
	delete []buff;

	delete fd_src;
	delete fd_dst;
	//CHMOD((_CHAR*)dst, osstat.st_mode);
	return TRUE;
}



//------------------------------------------------------
_BOOL File::Exists(const Stringz &NAME)
{
	BPTR lock = ::Lock((_CHAR*)NAME, 0L);
	if (lock != NULL) {
		::UnLock(lock);
		return TRUE;
	}
	return FALSE;
}



//------------------------------------------------
_BOOL File::Stats(const Stringz &NAME, FileStats& STATS)
{
	BPTR lock = ::Lock((_CHAR*)NAME, 0L);
	if (lock == NULL) { return FALSE; }
	
	_BOOL res = FALSE;
	FileInfoBlock *infoBlock = (FileInfoBlock*)::AllocDosObject(DOS_FIB, NULL);
	if (infoBlock == NULL) {	
		if (::Examine(lock, infoBlock)) {
			STATS.size = infoBlock->fib_Size;
			STATS.isDirectory = infoBlock->fib_DirEntryType > 0;
			STATS.readAccess = infoBlock->fib_Protection & FIBF_READ;
			STATS.readAccess = infoBlock->fib_Protection & FIBF_READ;
			STATS.writeAccess = infoBlock->fib_Protection & FIBF_WRITE;
			STATS.execAccess = infoBlock->fib_Protection & FIBF_EXECUTE;
			//STATS.modifDate.date.day = 
			res = TRUE;
		}
		::FreeDosObject(DOS_FIB, (APTR)infoBlock);
	}
	::UnLock(lock);
	return res;
}


#endif
