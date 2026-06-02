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

	(c) 1998-2001 Henri Michelon

$Id: FileWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;

#ifdef _MSC_VER
	//#include <sys/utime.h>
#else
	#include <time.h>
	#include <sys/stat.h>
#endif

#ifdef __BCPLUSPLUS__
	#include <utime.h>
#endif	


//------------------------------------------------
_PTR File::Mode(File::FileMode fmode)
{
	switch(fmode)
	{
	case READ:
		return (_PTR)OF_READ;
	case WRITE:
		return (_PTR)OF_WRITE;
	case READWRITE:
		return (_PTR)OF_READWRITE;
	case CREATE:
		return (_PTR)OF_CREATE;
	case APPEND:
		return (_PTR)OF_READWRITE;
	default:
		ASSERTMSG(FALSE, "File:: Unknown file mode");
		return NULL;
	}
}


//------------------------------------------------
File *File :: Open(const Stringz &NAME, const File::FileMode fmode)
{
	OFSTRUCT of;
	HFILE handle = ::OpenFile((_CHAR*)NAME, &of, (UINT)Mode(fmode));
	if (handle > 0) {
		File *file = new File((_PTR)handle, NAME);
		file->mode = fmode;
		return file;
	}
	return NULL;
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
		::FlushFileBuffers((HANDLE)handle);
		::CloseHandle((HANDLE)handle);
		handle = NULL;
	}
}


//------------------------------------------------
_LONG File :: Read(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	DWORD res;
	if (::ReadFile((HANDLE)handle, T, N, &res, NULL)) {
		Debug(dprintf("File::Read: %d = %d\n", N, res);)
		return res;
	}
	return -1;
}


//------------------------------------------------
_LONG File :: Write(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	DWORD res;
	if (::WriteFile((HANDLE)handle, T, N, &res, NULL)) {
		return res;
	}
	return -1;
}


//------------------------------------------------
_BOOL File :: Eof() const
{
	VALIDATETHIS;
	ASSERT(handle);
	DWORD curpos = ::SetFilePointer((HANDLE)handle, 0, NULL, FILE_CURRENT);
	_DWORD res = (curpos == ::SetFilePointer((HANDLE)handle, 0, NULL, FILE_END));
	::SetFilePointer((HANDLE)handle, curpos, NULL, FILE_BEGIN);
	return res;
}


//------------------------------------------------
_BOOL File::Seek(const _LONG pos, const SeekMode MODE)
{
	VALIDATETHIS;
	DWORD fmode;
	switch (MODE)
	{
		case END: 
			fmode = FILE_END; break;
		case CURRENT: 
			fmode = FILE_CURRENT; break;
		default: 
			fmode = FILE_BEGIN; break;
	}
	return (SetFilePointer((HANDLE)handle, pos, NULL, fmode) != -1);
}



//------------------------------------------------
_BOOL File::SetDateTime(const Stringz &NAME, const DateTime &M)
{
	struct utimbuf	timbuf;
	struct tm		*tmtime;

	// use the current time for the tm_isdst field
	time_t			ttime;
	time(&ttime);
	tmtime = localtime(&ttime);

	tmtime->tm_sec = M.time.second;
	tmtime->tm_min = M.time.minute;
	tmtime->tm_hour = M.time.hour;
	tmtime->tm_mday = M.date.dayOfMonth;
	tmtime->tm_mon = M.date.month-1;
	tmtime->tm_year = M.date.year - 1900;
	tmtime->tm_wday = M.date.dayOfWeek;
	if (DateTime::Date::weekStartMonday) {
		if (tmtime->tm_wday == 6) {
			tmtime->tm_wday = 0;
		}
		else {
			tmtime->tm_wday++;
		}
	}
	timbuf.modtime = mktime(tmtime) - _timezone;
	timbuf.actime = timbuf.modtime;
	return utime((char*)NAME, &timbuf);
}


//------------------------------------------------------
_BOOL File::Remove(const Stringz &NAME)
{
	return ::DeleteFile((_CHAR*)NAME);
}


//------------------------------------------------
_BOOL File::CopyFile(const Stringz &src, const Stringz &dst)
{
	::CopyFile((_CHAR*)src, (_CHAR*)dst, FALSE);
	return TRUE;// XXX: CopyFile return an error code even
				// when copy succeed under some circumstances
				// bug or features ???
}


//------------------------------------------------
_LONG File::Position() const
{
	return ::SetFilePointer((HANDLE)handle, 0, NULL, FILE_CURRENT);
}


//------------------------------------------------------
_BOOL File::Exists(const Stringz &NAME)
{
	return (::GetFileAttributes((_CHAR*)NAME) != -1);
}


//------------------------------------------------
/*_BOOL File::Stats(const Stringz &NAME, FileStats& STATS)
{
	HANDLE handle = CreateFile((_CHAR*)NAME, 0, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if (handle != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION infos;
		if (GetFileInformationByHandle(handle, &infos))
		{
			SYSTEMTIME time;
			Date date;
			Time ttime;
			STATS.isDirectory = infos.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY;
			STATS.size = infos.nFileSizeHigh << 16 + infos.nFileSizeLow;
			FileTimeToSystemTime(&infos.ftCreationTime, &time);
			date = Date(time.wDay, time.wMonth, time.wYear, time.wDayOfWeek);
			ttime = Time(time.wHour, time.wMinute, time.wSecond);
			STATS.creatDate = DateTime(date, ttime);
			FileTimeToSystemTime(&infos.ftLastWriteTime, &time);
			date = Date(time.wDay, time.wMonth, time.wYear, time.wDayOfWeek);
			ttime = Time(time.wHour, time.wMinute, time.wSecond);
			STATS.modifDate = DateTime(date, ttime);
			if (Date::weekStartMonday)
			{
				STATS.creatDate.date.dayOfWeek++;
				STATS.modifDate.date.dayOfWeek++;
			}
			CloseHandle(handle);
			return TRUE;
		}
		CloseHandle(handle);
	}
	return FALSE;
}*/


//------------------------------------------------
_BOOL File::Stats(const Stringz &NAME, FileStats& STATS)
{
	struct stat	stats;
	tm		*time;
	DateTime::Date	date;

	if (stat((_CHAR*)NAME, &stats) == -1)
	{
		//Debug(dprintf("fstat: %s\n", sys_errlist[errno]);)
		return FALSE;
	}
	
	STATS.size = stats.st_size;
	
	time = localtime(&stats.st_mtime);
	date = DateTime::Date(time->tm_mday, time->tm_mon + 1, time->tm_year + 1900, time->tm_wday);
	if (DateTime::Date::weekStartMonday)
	{
		if (!date.dayOfWeek)
			date.dayOfWeek = 6;
		else
			date.dayOfWeek--;
	}
	STATS.modifDate = DateTime(date, DateTime::Time(time->tm_hour, time->tm_min, time->tm_sec));

	time = localtime(&stats.st_ctime);
	date = DateTime::Date(time->tm_mday, time->tm_mon + 1, time->tm_year + 1900, time->tm_wday);
	if (DateTime::Date::weekStartMonday)
	{
		if (!date.dayOfWeek)
			date.dayOfWeek = 6;
		else
			date.dayOfWeek--;
	}
	STATS.creatDate = DateTime(date, DateTime::Time(time->tm_hour, time->tm_min, time->tm_sec));

	_DWORD attr = GetFileAttributes((_CHAR*)NAME);
	STATS.isDirectory = FALSE;
	STATS.readAccess = FALSE;
	STATS.writeAccess = FALSE;
	STATS.execAccess = FALSE;
	if (attr != 0xFFFFFFFF)
	{
		STATS.isDirectory = attr & FILE_ATTRIBUTE_DIRECTORY;
		STATS.readAccess = TRUE;
		STATS.writeAccess = !(attr & FILE_ATTRIBUTE_READONLY);
		if (NAME.Len()>4)
		{
			Stringz ext = NAME.Copy(NAME.Len()-4, 4);
			ext.LCase();
			STATS.execAccess = (ext == (_CHAR*)".exe") || (ext == (_CHAR*)".com") || 
								(ext == (_CHAR*)".bat") || (ext == (_CHAR*)".cmd");
		}
	}
	return TRUE;
}
#endif
