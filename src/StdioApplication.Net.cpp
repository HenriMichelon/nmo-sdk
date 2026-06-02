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

	(c) 2000 Henri Michelon

$Id: StdioApplication.Net.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(WIN32) || defined(_WIN32)
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
#include <stdio.h>
using namespace NMO;

//-------------------------------------------------
void StdioApplication::Run()
{
	OnCreate();
	OnDestroy();
}

//-------------------------------------------------
StdioApplication::StdioApplication(const UStringz&N):
	Application(N) {}

//-------------------------------------------------
StdioApplication::~StdioApplication()
{}


//-------------------------------------------------
void StdioApplication::Terminate()
{
	throw NMOException("Terminate");
}


//-------------------------------------------------
// static stub used because varargs functions
// aren't currently implemented in Managed C++ (??!!)
// HM, 05 Dec 2000
static void NmoConsoleWrite(const char*tmp) { Console::Write(tmp); }

_BOOL StdioApplication::Write(const char* FMT, ...)
{
	ASSERT(FMT);
	_DWORD l = Stringz::Strlen(FMT)*2 + 4000;
	_CHAR *tmp = new _CHAR[l];
	va_list arg;
	va_start(arg, FMT);
	_vsnprintf(tmp, l, FMT, arg);
	va_end(arg);
	NmoConsoleWrite(tmp);
	delete []tmp;
	return TRUE;
}


//-------------------------------------------------
_CHAR StdioApplication::GetChar()
{
	char c = Console::Read();
	Console::Read();
	Console::Read();
	return c;
}


//-------------------------------------------------
Stringz StdioApplication::Readln()
{
	return Console::ReadLine();
}


//-------------------------------------------------
void StdioApplication::Flush()
{
	Console::Out->Flush();
	Console::Error->Flush();
	Console::In->ReadToEnd();
}


//-------------------------------------------------
void StdioApplication::PutChar(_CHAR C)
{
	Console::Write(__wchar_t(C));
}
#endif
