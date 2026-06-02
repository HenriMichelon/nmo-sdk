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

$Id: File.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <stdio.h>
#ifdef _POSIX
# include <stdarg.h>
# include <string.h>
#endif


//------------------------------------------------
File *File :: Open(const File *F, const Stringz &name)
{
	ASSERT(F);
	return new File(F->handle, name);
}


//------------------------------------------------
_BOOL File::Stats(FileStats& STATS)
{
	VALIDATETHIS;
	return Stats(name, STATS);
}


//------------------------------------------------
Stringz File::FileName() 
{
	VALIDATETHIS;
	return FileName(name);
}


//------------------------------------------------
Stringz File::FilePath()
{
	VALIDATETHIS;
	return FilePath(name);
}


//------------------------------------------------
_BOOL File::Readln(Stringz&S)
{
	_CHAR c = 0;
	S = "";
	while (c != 10)
	{
		if (Read(&c, 1) != 1)
			break;
		if (c == 13) continue;
		if (c == 10) continue;
		S += c;
	}
	S += '\00';
	return (c == 10);
}


//------------------------------------------------
void File::Writeln(char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	_DWORD l = Stringz::Strlen(fmt)*2 + 4096;
	_CHAR *Str = new _CHAR[l];
#if defined(HAVE_VSNPRINTF) || defined(__BCPLUSPLUS__)
	vsnprintf(Str, l, fmt, arg);
#elif defined(_WIN32) && defined(_MSC_VER)
	_vsnprintf(Str, l, fmt, arg);
#else
#ifdef _BSD_VA_LIST_ 	
	vsprintf(Str, fmt, (_BSD_VA_LIST_)arg);
#else
	vsprintf(Str, fmt, arg);
#endif
#endif
	Write(Str, Stringz::Strlen(Str));
	delete []Str;
	va_end(arg);
}


//------------------------------------------------
Stringz File::FileExt(const Stringz&P) 
{
	_LONG oldpos, pos = -1;
	do {
		oldpos = pos;
		pos = P.InStr('.', pos+1);
	} while (pos != -1);
	if (oldpos == -1) {
		return "";
	}
	else {
		return P.Copy(oldpos+1, P.Len()-oldpos-1);
	}
}


//------------------------------------------------
Stringz File::FileName(const Stringz&P) 
{
	_LONG oldpos, pos = -1;
	do {
		oldpos = pos;
		pos = P.InStr(PATHSEP, pos+1);
	} while (pos != -1);
	if (oldpos == -1) {
		return P;
	}
	else {
		return P.Copy(oldpos+1, P.Len()-oldpos-1);
	}
}


//------------------------------------------------
Stringz File::FilePath(const Stringz&P) 
{
	int oldpos, pos = -1;
	do {
		oldpos = pos;
		pos = P.InStr(PATHSEP, pos+1);
	} while (pos != -1);
	if (oldpos == -1) {
		return "";
	}
	else {
		return P.Copy(0, oldpos);
	}
}


//------------------------------------------------
File::FileMode File::Mode() const
{
	return mode;
}

