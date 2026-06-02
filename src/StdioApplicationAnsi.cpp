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

$Id: StdioApplicationAnsi.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Application.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;
#include <stdio.h>

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
_BOOL StdioApplication::Write(const char* FMT, ...)
{
	ASSERT(FMT);
	va_list arg;
	va_start(arg, FMT);
#ifdef _BSD_VA_LIST_
	vprintf(FMT, (_BSD_VA_LIST_)arg);
#else
	vprintf(FMT, arg);
#endif
	va_end(arg);
	return TRUE;
}


//-------------------------------------------------
_CHAR StdioApplication::GetChar()
{
	fflush(stdin);
	_CHAR ch = fgetc(stdin);
	while (fgetc(stdin) > 0x20) {};
	return ch;
}


//-------------------------------------------------
Stringz StdioApplication::Readln()
{
	char temp[10*1024];
	fgets(temp, 10*1024, stdin);
	temp[Stringz::Strlen(temp)-1] = 0; // drop last enter char
	return Stringz(temp);
}


//-------------------------------------------------
void StdioApplication::Flush()
{
#ifdef __FreeBSD__
	fpurge(stdin);
#else
	fflush(stdin);
#endif
	fflush(stdout);
	fflush(stderr);
}


//-------------------------------------------------
void StdioApplication::PutChar(_CHAR C)
{
	fputc(C, stdout);
}
