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

$Id: ConsoleApplication.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Application.hpp>
#include <nmo/ConsoleApplication.hpp>
using namespace NMO;
#include "nmo/LC/LC.hpp"
#include <stdio.h>


//-------------------------------------------------
void ConsoleApplication::Run()
{
	StopPage();
	OnCreate();
	OnDestroy();
}


//-------------------------------------------------
void ConsoleApplication::Terminate()
{
	throw NMOException("Terminate");
}


//-------------------------------------------------
_BOOL ConsoleApplication::Write(const char* FMT, ...)
{
	if (FMT) {
		char *Temp = new char [Stringz::Strlen(FMT)*2 + 4096];
		va_list arg;
		va_start(arg, FMT);
#ifdef _BSD_VA_LIST_		
		vsprintf(Temp, FMT, (_BSD_VA_LIST_)arg);
#else
		vsprintf(Temp, FMT, arg);
#endif
		va_end(arg);
		Writeln(Temp);
		Stringz txt = Temp;
		delete []Temp;
		return PagePerPage(txt);
	}
	return FALSE;
}


//-------------------------------------------------
_BOOL ConsoleApplication::PagePerPage(const Stringz&TXT)
{
	_CHAR *STR = TXT;
	if (nrow != -1)
	{
		_DWORD i;

		for (i=0; i<Stringz::Strlen(STR); i++)
		{
			if (STR[i] == '\n')
 				nrow++;
		}
		i = Stringz::Strlen(STR)-1;
		if ((nrow >= (_LONG)Rows()) && (STR[i] == '\n'))
		{
			nrow = 1;
			Writeln(LC_NMO_NEXTPAGE);
			MoveCursorBack(Stringz::Strlen(LC_NMO_NEXTPAGE));
			_CHAR c;
			if (GetChar(c))
			{
				if (c == CTRLC)
				{
					Writeln("^C\n");
					return TRUE;
				}
			}
			for (i=0; i<sizeof(LC_NMO_NEXTPAGE); i++)
				PutChar(' ');
			MoveCursorBack(sizeof(LC_NMO_NEXTPAGE));
		}
	}
	return FALSE;
}


//-------------------------------------------------
void ConsoleApplication::StartPage()
{
	nrow = 1;
}


//-------------------------------------------------
void ConsoleApplication::StopPage()
{
	nrow = -1;
}


//------------------------------------------------------
void ConsoleApplication::MoveCursorBack(_DWORD x)
{
	for (_DWORD i=0; i<x; i++)
		PutChar('\010');
}
