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

*/
#ifndef _NMO_CONAPP_H_
#define _NMO_CONAPP_H_

#include <nmo/StdioApplication.hpp>

namespace NMO {

/*!
\brief		Console based application skeleton for
			use with text terminals

  			This class is used to write console based application.\n
			Minimal support is provided for screen based operations:
			page per page display, moving the cursor back.

			Keyboard support include support for non character keys
			(arrows, pageup/down, ...).


\author		(c) 1998-2001 Henri Michelon 
\version	$Id: ConsoleApplication.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ConsoleApplication : public StdioApplication
{
public:
	/*!
		Constants for non characters keys\n
		key codes are returned by ConsoleApplication::GetChar()
	*/			
	typedef enum
	{
		//! dummy key, normaly never returned by ConsoleApplication::GetChar()
		NONE,
		//! Escape
		ESC,
		//! Tabulation
		TAB,
		//! Delete
		DEL,
		//! Enter (both big and keypad)
		ENTER,
		//! Control-C combination
		CTRLC,
		//! Control-D combination
		CTRLD,
		QUIT,
		//! Backspae
		BACKSPACE,
		//! Home (start of page)
		HOME,
		//! End (end of page)
		END,
		//! Insert 
		INSERT,
		//! Page up (scroll up)
		PAGE_UP,
		//! Page down (scroll down)
		PAGE_DOWN,
		//! Arrow up
		ARROW_UP,
		//! Arrow down
		ARROW_DOWN,
		//! Arrow left
		ARROW_LEFT,
		//! Arrow right
		ARROW_RIGHT
	} KeyCode;

	/*! Create an application with the given application name (aka title) */
	ConsoleApplication(const UStringz&N);

	virtual ~ConsoleApplication();

	virtual void Run();
	virtual void Terminate();

	/*! write a formated string into the console
		On a page per page display, return TRUE 
	 	if CTRL-C pressed in page stop
	*/	
	_BOOL Write(const char*, ...);

	//! Start a page per page display for Write()
	void StartPage();

	// !Stop the page per page display for Write()
	void StopPage();

	/*! Get a char (without echo) from the console\n
		char&	: (output) result char\n
		return TRUE if char is a KeyCode\n
	*/	
	_BOOL GetChar(char&);

	/*! Get a char (without echo) from the console
		in a non blocking manner\n
		char&	: (output) result char\n
		_BOOL	: (output) TRUE is char is a keycode\n
		return TRUE if there is a char returned\n
	*/	
	_BOOL GetChar(_BOOL&, char&);

	//! put a char into the console
	void PutChar(char);

	//! put a char in the keyboard buffer 
	void UngetChar(char);

	//! force GetChar() to return a CTRLC
	void SendBreak();

	//! Beep the speaker or flash the screen
	void Beep();

	/* Read a string from the console\n
		String	: (input/output) string to complete\n
		return TRUE on CTRL-C\n
	*/	
	_BOOL Readln(Stringz&);

	//! Move cursor backward
	void MoveCursorBack(_DWORD);

	//! Return the number of rows of the display
	_DWORD Rows();

	//! Return the number of columns of the display
	_DWORD Cols();

	//! Change the application name
	void SetName(const UStringz&);

private:
	_LONG nrow;

	// Management page per page display
	//	char*	: last line displayed
	//	return TRUE on CTRL-C
	_BOOL PagePerPage(const Stringz&);

	void Writeln(const char*);
};


}

#endif
