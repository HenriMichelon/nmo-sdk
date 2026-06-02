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

$Id: ConsoleApplicationMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/Base.hpp>
using namespace NMO;
#include <nmo/Application.hpp>
#include <nmo/ConsoleApplication.hpp>


//-------------------------------------------------
ConsoleApplication::ConsoleApplication(const UStringz&N): 
	StdioApplication(N)
{
}


//-------------------------------------------------
ConsoleApplication::~ConsoleApplication()
{
}
		

//-------------------------------------------------
void ConsoleApplication:: Writeln(const char *)
{
}


//-------------------------------------------------
_BOOL ConsoleApplication:: GetChar(char&)
{
	return FALSE;
}


//-------------------------------------------------
void ConsoleApplication:: PutChar(char )
{
}


//-------------------------------------------------
void ConsoleApplication:: SendBreak() 
{
}


//-------------------------------------------------
void ConsoleApplication:: Beep() 
{
}


//-------------------------------------------------
_DWORD ConsoleApplication::Rows() 
{
	return 0;
}


//-------------------------------------------------
_DWORD ConsoleApplication::Cols() 
{
	return 0;
}


//-------------------------------------------------
void ConsoleApplication::SetName(const UStringz&)
{
}

#endif