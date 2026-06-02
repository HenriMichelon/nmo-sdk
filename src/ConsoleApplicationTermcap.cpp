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

$Id: ConsoleApplicationTermcap.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#if !defined(sun) && !defined(sgi)
	#include <nmo/Base.hpp>
#else
	#include <nmo/config.h>
#endif
extern "C" {
#ifdef HAVE_NCURSES
	#ifdef sgi
		#define bool char
		#define _BOOL bool
	#endif
	#include <curses.h>
	#include <term.h>
	#ifdef sgi
		#undef _BOOL
	#endif
#elif HAVE_TERMCAP
	#include <termcap.h>
#endif	
#if defined(macintosh) && defined(__MACH__)
# include <curses.h>
# include <unistd.h>
#endif
#ifndef __ATHEOS__
# include <termios.h>
#endif   
}

#if defined(sun) || defined(sgi)
#include <nmo/Base.hpp>
#endif
#include <nmo/Sys.hpp>
#include <nmo/Application.hpp>
#include <nmo/ConsoleApplication.hpp>
using namespace NMO;
#include <stdio.h>


// class to store equivalences between termcap key codes and
// ConsoleApplication key codes
class __KeyCode
{
public:
	Stringz	cap;
	ConsoleApplication::KeyCode code;
	inline __KeyCode (char *STR, ConsoleApplication::KeyCode CODE): code(CODE)
	{
		cap = STR;
		for (_WORD i=0; i<cap.Len(); i++)
			if (STR[i] == 27) ((_CHAR*)cap)[i+1] = ' ';
	};
};
static List<__KeyCode> __keycodes;

// static buffer for termcap access
static char __bp_termcap[1024];


//-------------------------------------------------
ConsoleApplication::ConsoleApplication(const UStringz&N): 
	StdioApplication(N)
{
	// Open termcap database entry for current terminal
	Stringz term;
	if (!Env::Read("TERM", term))
		term = "vt100";
	int r = tgetent(__bp_termcap, (_CHAR*)term);
	if (r == -1) throw NMOException("Couldn't open termcap database");
	if (r == 0) throw NMOException("Couldn't open terminal capabilities");
	Debug(dprintf("opening terminal '%s'\n", (_CHAR*)term);)

	// Fill the list of key codes equiv from termcap database
	char buf[1024];
	char *x = buf;
	__keycodes.Add(new __KeyCode(tgetstr("kl", &x),
								 ConsoleApplication::ARROW_LEFT));
	__keycodes.Add(new __KeyCode(tgetstr("kr", &x),
								 ConsoleApplication::ARROW_RIGHT));
	__keycodes.Add(new __KeyCode(tgetstr("ku", &x),
								 ConsoleApplication::ARROW_UP));
	__keycodes.Add(new __KeyCode(tgetstr("kd", &x),
								 ConsoleApplication::ARROW_DOWN));
	__keycodes.Add(new __KeyCode(tgetstr("kI", &x),
								 ConsoleApplication::INSERT));
	__keycodes.Add(new __KeyCode(tgetstr("dc", &x),
								 ConsoleApplication::DEL));
	__keycodes.Add(new __KeyCode(tgetstr("kh", &x),
								 ConsoleApplication::HOME));
	__keycodes.Add(new __KeyCode(tgetstr("@7", &x),
								 ConsoleApplication::END));
	__keycodes.Add(new __KeyCode(tgetstr("kP", &x),
								 ConsoleApplication::PAGE_UP));
	__keycodes.Add(new __KeyCode(tgetstr("kN", &x),
								 ConsoleApplication::PAGE_DOWN));
	__keycodes.Add(new __KeyCode(tgetstr("kb", &x),
								 ConsoleApplication::BACKSPACE));
	__keycodes.Add(new __KeyCode(tgetstr("@8", &x),
								 ConsoleApplication::ENTER));
	__keycodes.Add(new __KeyCode(tgetstr("kT", &x),
								 ConsoleApplication::TAB));
	__keycodes.Add(new __KeyCode(tgetstr("@2", &x),
								 ConsoleApplication::ESC));
}


//-------------------------------------------------
ConsoleApplication::~ConsoleApplication()
{
#ifdef __DEBUG
	// Clear list before Garbage() for debug
	//	(prevent display of "Block not freed")
	__keycodes.Clear();
#endif
}


//-------------------------------------------------
_BOOL ConsoleApplication::GetChar(_CHAR&RESULT)
{
	const _WORD READBUFFERSIZE = 255;
	// Change terminal line discipline
	termios __tios, tsave;
	tcgetattr(STDIN_FILENO, &tsave);
	__tios = tsave;
	// unset "input canonical": do not wait for "ENTER" key for the
	//	read() function
	__tios.c_lflag &= ~ICANON;
	// unset character echoing while read()
	__tios.c_lflag &= ~ECHO;
	// ignore CTRLC (INTR) and QUIT signals
	__tios.c_lflag &= ~ISIG;
#ifdef sgi
	__tios.c_cc[VMIN] = 10;//READBUFFERSIZE;
	__tios.c_cc[VTIME] = 1;
#endif
	tcsetattr(STDIN_FILENO, TCSANOW, &__tios);	

	// default return value
	_BOOL keyCode = FALSE;

	// read user input
	_CHAR buf[READBUFFERSIZE];
	buf[read(STDIN_FILENO, &buf, READBUFFERSIZE)] = 0;
	if (buf[0] == 27) buf[1] = ' ';

	// search for a key with a complicated key code
	ListIterator<__KeyCode> list(__keycodes);
	while (!list.End())
	{
		__KeyCode &code = list.Next();
		if (code.cap == buf)
		{
			keyCode = TRUE;
			RESULT = code.code;
			break;
		}
	}
	if (!keyCode)
	{
		// normal key or keycode not found
		// use well known values for some keys
		switch (buf[0])
		{
		case 127:
			keyCode = TRUE;
			RESULT = ConsoleApplication::BACKSPACE;
			break;
		case 9:
			keyCode = TRUE;
			RESULT = ConsoleApplication::TAB;
			break;
		case 3:
			keyCode = TRUE;
			RESULT = ConsoleApplication::CTRLC;
			break;
		case 4:
			keyCode = TRUE;
			RESULT = ConsoleApplication::CTRLD;
			break;
		case 27:
			keyCode = TRUE;
			RESULT = ConsoleApplication::ESC;
			break;
		case 28:
			keyCode = TRUE;
			RESULT = ConsoleApplication::QUIT;
			break;
		case 13:
		case 10:
			keyCode = TRUE;
			RESULT = ConsoleApplication::ENTER;
			break;
		default:
			RESULT = buf[0];
			break;
		}
	}
	// restore terminal line discipline
	tcsetattr(STDIN_FILENO, TCSANOW, &tsave);	
	return keyCode;
}


//-------------------------------------------------
void ConsoleApplication:: PutChar(char C)
{
	putchar(C);
	fflush(stdout);
}


//-------------------------------------------------
void ConsoleApplication:: SendBreak()
{
	ungetc(3, stdin);
}


//-------------------------------------------------
void ConsoleApplication:: UngetChar(char C)
{
	ungetc(C, stdin);
}


//-------------------------------------------------
void ConsoleApplication:: Beep()
{
	char buf[256];
	char *x = buf;
	if (tgetstr("bl", &x)) printf("%s", buf);
}


//-------------------------------------------------
void ConsoleApplication:: SetName(const UStringz&) {}


//-------------------------------------------------
_DWORD ConsoleApplication:: Cols()
{
	return tgetnum("co");
}


//-------------------------------------------------
_DWORD ConsoleApplication:: Rows()
{
	return tgetnum("li");
}


//-------------------------------------------------
void ConsoleApplication:: Writeln(const char*STR) 
{
	printf((char*)STR);
	fflush(stdout);
}
