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

	(c) 1998-2002 Henri Michelon 

$Id: FileAnsi.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <stdio.h>
#include <time.h>

#ifdef _POSIX
# include <utime.h>
# include <sys/param.h>
# include <sys/stat.h>
#endif

#ifdef _MACOS
# include <unix.mac.h>
#endif

#ifdef _WIN32
# ifdef __GNUC__
#  define CHMOD chmod
# else
#  define CHMOD _chmod
# endif
# ifdef _MSC_VER
#  include <sys/utime.h>
# endif
# ifdef __BCPLUSPLUS__
#  include <utime.h>
# endif
# include <io.h>
#else
# define CHMOD chmod
#endif

#include <errno.h>
#ifndef HAVE_SYSERRLIST
extern const char * const sys_errlist[];
#endif
   
#ifdef MAXBSIZE /* BSD & GG */
#define BLKSIZE MAXBSIZE
#endif

#ifdef _LINUX
#define BLKSIZE DEV_BSIZE
#endif

#ifndef BLKSIZE
#define BLKSIZE 16192
#endif



//------------------------------------------------
_PTR File::Mode(File::FileMode fmode)
{
	switch(fmode)
	{
	case READ:
		return (_PTR)"rb";
	case WRITE:
		return (_PTR)"wb";
	case READWRITE:
		return (_PTR)"r+b";
	case CREATE:
		return (_PTR)"w+b";
	case APPEND:
		return (_PTR)"ab";
	default:
		ASSERT(FALSE);
		return NULL;
	}
}


//------------------------------------------------
File *File :: Open(const Stringz &NAME, const File::FileMode fmode)
{
	FILE *H = fopen((char*)NAME, (_CHAR*)Mode(fmode));
	if (H)
	{
		File *file = new File(H, NAME);
		file->mode = fmode;
		Debug(dprintf("File: opening %s (%s): %x\n", (_CHAR*)NAME, (_CHAR*)Mode(fmode), H);)
		return file;
	}
	else {
#ifndef _MACOS
		Debug(dprintf("File: error opening %s (%s): %s\n", 
					  (_CHAR*)NAME,
					  (_CHAR*)Mode(fmode),
					  sys_errlist[errno]);)
#endif
		return NULL;
	}
}


//------------------------------------------------
File :: ~File()
{
	Close();
}


//------------------------------------------------
void File :: Close()
{
	VALIDATETHIS;
	if (handle != NULL) {
		Debug(dprintf("File: closing %s : %x\n", (_CHAR*)name, handle);)
		fclose((FILE*)handle);
		handle = NULL;
	}
}


//------------------------------------------------
_LONG File :: Read(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	int r = fread((void*)T, 1, N, (FILE*)handle);
	if ((!r) && (!feof((FILE*)handle))) {
		return -1;
	}
	return r;
}


//------------------------------------------------
_LONG File :: Write(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	int r = fwrite(T, 1, N, (FILE*)handle);
	if (r < N) {
		return -1;
	}
	return r;
}


//------------------------------------------------
_BOOL File :: Eof() const
{
	VALIDATETHIS;
	ASSERT(handle);
	return feof((FILE*)handle);
}


//------------------------------------------------
_BOOL File::Seek(const _LONG pos, const SeekMode MODE)
{
	VALIDATETHIS;
	int fmode;
	switch (MODE)
	{
	case END: fmode = SEEK_END; break;
	case CURRENT: fmode = SEEK_CUR; break;
	default: fmode = SEEK_SET; break;
	}
	return (fseek((FILE*)handle, pos, fmode) == 0);
}



//------------------------------------------------
_BOOL File::SetDateTime(const Stringz &NAME, const DateTime &M)
{
	struct utimbuf	timbuf;
	struct tm		time;
	time.tm_sec = M.time.second;
	time.tm_min = M.time.minute;
	time.tm_hour = M.time.hour;
	time.tm_mday = M.date.dayOfMonth;
	time.tm_mon = M.date.month-1;
	time.tm_year = M.date.year;
#ifdef _POSIX
	time.tm_year = M.date.year-1900;
#endif
	time.tm_wday = M.date.dayOfWeek;
	time.tm_isdst = 0;
	if (DateTime::Date::weekStartMonday) {
		if (time.tm_wday == 6) {
			time.tm_wday = 0;
		}
		else {
			time.tm_wday++;
		}
	}
#ifdef _WIN32
	timbuf.modtime = mktime(&time) - _timezone;
#endif // _WIN32

#ifdef _POSIX
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
	timbuf.modtime = timegm(&time);
#else
	timbuf.modtime = mktime(&time);
#endif // _BSD
#endif // _POSIX

	timbuf.actime = timbuf.modtime;
	return utime((char*)NAME, &timbuf);
}


//------------------------------------------------------
_BOOL File::Remove(const Stringz &NAME)
{
	return remove((char*)NAME) != -1;
}


#ifndef _MACOS
//------------------------------------------------
_BOOL File::CopyFile(const Stringz &src, const Stringz &dst)
{
	File *fd_src, *fd_dst;
	_BYTE _file_iobuf[BLKSIZE];
	
	struct stat osstat;
	stat((_CHAR*)src, &osstat);

	if ( (fd_src = File::Open((char*)src, File::READ)) == NULL)
		return FALSE;
	if ( (fd_dst = File::Open((char*)dst, File::CREATE)) == NULL) 
	{
		//Debug(dprintf("create %s\n", (char*)dst);)
		delete fd_src;
		return FALSE;
	}

	_BOOL error = FALSE;
	_DWORD nblocks = osstat.st_size / BLKSIZE;
	for (_DWORD i=0; i<nblocks; i++) {
		if (fd_src->Read(_file_iobuf, BLKSIZE) != BLKSIZE) 	{
			error =TRUE;
			break;
		}
		if (fd_dst->Write(_file_iobuf, BLKSIZE) != BLKSIZE) {
			error =TRUE;
			break;
		}
	}
	if (!error) {
		_DWORD remain = osstat.st_size - (nblocks * BLKSIZE);
		if (fd_src->Read(_file_iobuf, remain) != (_LONG) remain) {
			error = TRUE;
		}
		else if (fd_dst->Write(_file_iobuf, remain) != (_LONG) remain) {
			error = TRUE;
		}
	}
	delete fd_src;
	delete fd_dst;
	CHMOD((_CHAR*)dst, osstat.st_mode);
	return !error;
}
#endif


//------------------------------------------------
_LONG File::Position() const
{
	fpos_t pos;
	fgetpos((FILE*)handle, &pos);
#ifdef HAVE_FPOS_T
	return (_LONG)pos.__pos;
#else
	return (_LONG)pos;
#endif
}

