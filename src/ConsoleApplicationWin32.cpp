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

$Id: ConsoleApplicationWin32.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/ConsoleApplication.hpp>
# include <windows.h>
# include <conio.h>
#endif

#define COLOR BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

//-------------------------------------------------
void cons_main_init()
{
	Debug(dopen(NULL);)
	Stringz arg;
	_CHAR* cline = GetCommandLine();
	for (_DWORD i=0; i<strlen(cline); i++)
	{
		if (cline[i] == ' ')
		{
			Application::Argv().Add(new Stringz(arg));
			arg = "";
		}
		else
			arg += cline[i];
	}
	Application::Argv().Add(new Stringz(arg));
	ListIterator<Stringz> list(Application::Argv());
	Application::SetPath(list[1]);
}


//-------------------------------------------------
ConsoleApplication::ConsoleApplication(const UStringz&N): 
	StdioApplication(N)
{
	COORD coord;

	coord.X = 0;
	coord.Y = 0;

#ifndef _CONSOLE
	if (GetStdHandle(STD_OUTPUT_HANDLE) == INVALID_HANDLE_VALUE)
		AllocConsole();
#endif
	SetConsoleTitleA(Stringz(N));
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), 0);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), 
					ENABLE_PROCESSED_OUTPUT |
					ENABLE_WRAP_AT_EOL_OUTPUT);
	/*SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR);
	FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							COLOR, Rows() * Cols(), coord, &n);*/
}


//-------------------------------------------------
ConsoleApplication::~ConsoleApplication()
{
	FreeConsole();
}
		

//-------------------------------------------------
void ConsoleApplication:: Writeln(const char *STR)
{
	DWORD n;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
				STR, strlen(STR), &n, NULL);
}


//-------------------------------------------------
_BOOL ConsoleApplication:: GetChar(char&CHAR)
{
	DWORD nrecords;
	INPUT_RECORD input;

	while (TRUE)
	{
		if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),
						&input, 1, &nrecords))
		{
			CHAR = NONE;
			return FALSE;
		}
		if (input.EventType == KEY_EVENT)
		{
			if (input.Event.KeyEvent.bKeyDown == TRUE)
			{
				if ((!input.Event.KeyEvent.dwControlKeyState) ||
					(input.Event.KeyEvent.uChar.AsciiChar != 0))
					break;
				if ((input.Event.KeyEvent.dwControlKeyState & ENHANCED_KEY) &&
					(!(input.Event.KeyEvent.dwControlKeyState & RIGHT_ALT_PRESSED)))
					break;
				continue;
			}
		}
	}

	CHAR = input.Event.KeyEvent.uChar.AsciiChar;
	if (input.Event.KeyEvent.dwControlKeyState & ENHANCED_KEY)
	{
		switch (input.Event.KeyEvent.wVirtualScanCode)
		{
		case 75:
			CHAR = ARROW_LEFT;
			break;
		case 77:
			CHAR = ARROW_RIGHT;
			break;
		case 72:
			CHAR = ARROW_UP;
			break;
		case 80:
			CHAR = ARROW_DOWN;
			break;
		case 83:
			CHAR = DEL;
			break;
		case 71:
			CHAR = HOME;
			break;
		case 2:
			CHAR = END;
			break;
		case 82:
			CHAR = INSERT;
			break;
		case 73:
			CHAR = PAGE_UP;
			break;
		case 81:
			CHAR = PAGE_DOWN;
			break;
		default:
			return GetChar(CHAR);
		}
		return TRUE;
	}
	else if (((input.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) ||
			(input.Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED)) &&
			(!(input.Event.KeyEvent.dwControlKeyState & RIGHT_ALT_PRESSED)))
	{
		switch (CHAR)
		{
		case 3:
			CHAR = CTRLC;
			return TRUE;
		case 4:
			CHAR = CTRLD;
			return TRUE;
		}
		return TRUE;
	}
	else
	{
		switch(CHAR)
		{
		case -3:
			CHAR = '~';
			break;
		case 8:
			CHAR = BACKSPACE;
			return TRUE;
		case 9:
			CHAR = TAB;
			return TRUE;
		case 13:
			CHAR = ENTER;
			return TRUE;
		case 27:
			CHAR = ESC;
			return TRUE;
		}
		return FALSE;
	}
}


//-------------------------------------------------
void ConsoleApplication:: PutChar(char C)
{
	DWORD n;
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),
				&C, 1, &n, NULL);
}


//-------------------------------------------------
void ConsoleApplication:: SendBreak() 
{
	GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
}


//-------------------------------------------------
void ConsoleApplication:: Beep() 
{
	MessageBeep(MB_OK);
}


//-------------------------------------------------
_DWORD ConsoleApplication::Rows() 
{
	CONSOLE_SCREEN_BUFFER_INFO infos;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &infos))
		return infos.dwSize.Y;
	else
		return 25;
}


//-------------------------------------------------
_DWORD ConsoleApplication::Cols() 
{
	CONSOLE_SCREEN_BUFFER_INFO infos;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &infos))
		return infos.dwSize.X;
	else
		return 25;
}


//-------------------------------------------------
void ConsoleApplication::SetName(const UStringz&NAME)
{
	Stringz title(NAME);
	Application::SetName(NAME);
	SetConsoleTitleA(title);
}
#endif
