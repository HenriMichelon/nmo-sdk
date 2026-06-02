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

$Id: StdioApplicationMacOS.cpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/Base.hpp>
#include <nmo/Application.hpp>
#include <nmo/StdioApplication.hpp>
#include "StdioWindowMacOS.hpp"
using namespace NMO;


#define WINDOW(field) ((StdioWindowMacOS*)(this->mStdioApplicationAbstract))->field
	

//-------------------------------------------------
StdioApplication::StdioApplication(const UStringz&N):
	Application(N)
{
	mStdioApplicationAbstract = new StdioWindowMacOS;
}


//-------------------------------------------------
StdioApplication::~StdioApplication()
{
	delete (StdioWindowMacOS*)mStdioApplicationAbstract;
}



//-------------------------------------------------
void StdioApplication::Run()
{
	throw NMOException("StdioApplication support is currently broken under MacOS. Need Carbonization");
	Str255		name;
	Stringz		namez = Name();
	CopyCStringToPascal(namez, name);	
	if (WINDOW(Init(name))) {
		OnCreate();
		OnDestroy();
		Write("-- press any key to close this window --");
		WINDOW(GetKey());
	}
}


//-------------------------------------------------
void StdioApplication::Terminate()
{
	ExitToShell();
}


//-------------------------------------------------
_BOOL StdioApplication::Write(const char*FMT, ...)
{
	_DWORD len = Stringz::Strlen(FMT)*2 + 1024;
	_CHAR* str = new _CHAR[len];
	va_list arg;
	va_start(arg, FMT);
	vsnprintf(str, len, FMT, arg);
	va_end(arg);
	WINDOW(AddToBuffer(str));
	delete []str;
	return TRUE;
}


//-------------------------------------------------
_CHAR StdioApplication::GetChar()
{
    _CHAR ch = WINDOW(GetKey());
	if ((!ch) && (WINDOW(ExitRequested()))) {
	    Terminate();
	}
	_CHAR enter;
	do {
		enter = WINDOW(GetKey());
	} while (enter && (enter != 0x0d));
	return ch;
}


//-------------------------------------------------
Stringz StdioApplication::Readln()
{
	_CHAR 	ch;
    Stringz result;
    
    do {
        ch = WINDOW(GetKey)();
        switch (ch) {
        	case 0:
        		if (WINDOW(ExitRequested())) {
        			Terminate();
        		}
        		break;
        	case 0x0d:
        		break;
        	case 0x08d:
        		if (result.Len()) {
        			result = result.Left(result.Len()-1);
        		}
        		break;
        	default:
	            result += ch;
	            break;
        }
    } while (ch && (ch != 0x0d));
	return result;
}


//-------------------------------------------------
void StdioApplication::Flush()
{
	FlushEvents(keyDownMask, 0);
}


//-------------------------------------------------
void StdioApplication::PutChar(_CHAR ch)
{
	WINDOW(AddToBuffer(ch));
}
#endif