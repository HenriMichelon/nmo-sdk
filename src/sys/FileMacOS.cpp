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

$Id: FileMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include "MoreFiles/FileCopy.h"
#include <nmo/NMO.hpp>
using namespace NMO;

#define HANDLE ((short)this->handle)

//------------------------------------------------
_PTR File::Mode(File::FileMode fmode)
{
	switch (fmode) {
	case READ:
		return (_PTR)fsRdPerm;
	case WRITE:
	case CREATE:
		return (_PTR)fsWrPerm;
	case APPEND:
		return (_PTR)fsRdWrPerm;
	}
	ASSERT(FALSE);
	return NULL;
}


//------------------------------------------------
File *File :: Open(const Stringz &NAME, const File::FileMode fmode)
{
	OSErr err;
	Str255 filename;
	CopyCStringToPascal((_CHAR*)NAME, filename);
	if (fmode == CREATE) {
		HFileParam pb;
		Memory::Fill(&pb, 0, sizeof(HFileParam));
		pb.ioNamePtr = filename;
		PBHDeleteSync((HParmBlkPtr) &pb);
		if ((err = ::PBHCreateSync((HParmBlkPtr) &pb)) != noErr) {
    		Debug(dprintf("PBHCreateSync: %d (%d long)\n", err, NAME.Len());)
			return NULL;
		}
	}
	short refnum;
	if ((err = ::HOpenDF(0, 0, filename, (char)Mode(fmode), &refnum)) == noErr) {
		File *file = new File((_PTR)refnum, NAME);
		file->mode = fmode;
		return file;
	}
	Debug(dprintf("HOpenDF: %d\n", err);)
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
		::FSClose(HANDLE);
		handle = NULL;
	}
}


typedef struct {
	ParamBlockRec pbrec;
	_BOOL	callPending;
} MyParamBlockRec;


/*typedef struct 
{ 
	ParamBlockRec pb; // standard paramblock 
	Boolean callPending; // our pending flag 
} FSReadAsyncRec; */


static pascal void MyReadCompletion(ParamBlockRec *pb)
{
    ((MyParamBlockRec*)pb)->callPending = false;
    TRACE;
}


//------------------------------------------------
_LONG File :: Read(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(T);
	ParamBlockRec pb;
    pb.ioParam.ioRefNum = HANDLE;
    pb.ioParam.ioBuffer = (Ptr)T;
    pb.ioParam.ioReqCount = N;
    pb.ioParam.ioPosMode = fsAtMark + pleaseCacheMask;
    pb.ioParam.ioPosOffset = 0;
    OSErr err = PBReadSync(&pb);
    if ((err == noErr) || (err == eofErr)) {
    	return pb.ioParam.ioActCount;
    }
    return -1;
}


//------------------------------------------------
_LONG File :: Write(const _PTR T, const _LONG N)
{
	VALIDATETHIS;
	ASSERT(handle);
	ASSERT(T);
	long count = N;
	if (::FSWrite(HANDLE, &count, (Ptr)T) == noErr) {
		return count;
	}
	return -1;
}


//------------------------------------------------
_BOOL File :: Eof() const
{
	VALIDATETHIS;
	ASSERT(handle);
	long eof;
	if (::GetEOF(HANDLE, &eof) == noErr) {
		long fpos;
		if (::GetFPos(HANDLE, &fpos) == noErr) {
			return (fpos == eof);
		}
	}
	return TRUE;
}


//------------------------------------------------
_BOOL File::Seek(const _LONG pos, const SeekMode MODE)
{
	VALIDATETHIS;
	short fmode;
	switch (MODE)
	{
	case START: fmode = fsFromStart; break;
	case END: fmode = fsFromLEOF; break;
	case CURRENT: fmode = fsFromMark; break;
	}
	return (::SetFPos(HANDLE, fmode, pos) == noErr);
}



//------------------------------------------------
_LONG File::Position() const
{
	long pos;
	if (::GetFPos(HANDLE, &pos) == noErr) {
		return pos;
	}
	return 0;
}


//------------------------------------------------
_BOOL File::SetDateTime(const Stringz &NAME, const DateTime &M)
{
	return FALSE;
}


//------------------------------------------------------
_BOOL File::Remove(const Stringz &NAME)
{
	Str255 name;
	CopyCStringToPascal((_CHAR*)NAME, name);
	OSErr err = ::HDelete(0, 0, name);
	Debug(if (err != noErr) { dprintf("File::Remove: %d\n", err); })
	return (err == noErr);
}


//------------------------------------------------
_BOOL File::CopyFile(const Stringz &src, const Stringz &dst)
{
	Stringz dstpath = File::FilePath(dst);
	Stringz dstfname = File::FileName(dst);
	Str255 srcname;
	Str255 dstname;
	Str255 dstpathname;
	CopyCStringToPascal(src, srcname);
	CopyCStringToPascal(dstpath, dstpathname);
	CopyCStringToPascal(dstfname, dstname);
	Remove(dst);
	return (noErr == FileCopy(0,
			 			 0,
						 srcname,
						 0,
						 0,
						 dstpathname,
						 dstname,
						 nil,
						 0,
						 true));
}


//------------------------------------------------------
_BOOL File::Exists(const Stringz &NAME)
{
	Str255	fileName;
	FInfo 	fndrInfo;
	CopyCStringToPascal(NAME, fileName);
	return (::HGetFInfo(0, 0, fileName, &fndrInfo) == noErr);
}


//------------------------------------------------
_BOOL File::Stats(const Stringz &NAME, FileStats& STATS)
{
	return FALSE;
}


#endif
