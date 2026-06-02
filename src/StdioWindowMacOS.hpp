/*                                ---------
                                  [NMO-SDK]
                                  ---------

				Text window for stdio applications on MacOS.
				
				STDIO Support is VERY simplified. 
				Display IS NOT optimized, scrolling & refresh are VERY slow.				
				(Actually StdioApplications are mainly used for
				SDK porting under MacOS, not for production).

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2002 Henri Michelon 

$Id: StdioWindowMacOS.hpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _MACOS_STDIOWINDOW_H_
#define _MACOS_STDIOWINDOW_H_

namespace NMO {

// Starting window size in chars
const _DWORD DEFAULTNCOLS = 100;
const _DWORD DEFAULTNROWS = 40;

class StdioWindowMacOS : public NMOObject
{
public:
	StdioWindowMacOS();
	virtual ~StdioWindowMacOS();

	/* Create & initialize window, font & menu
		Str255	: application name
		return FALSE on error
	*/
	_BOOL Init(Str255);
	
	/* Wait for a key
		_BOOL	: echo on/off
	*/
    _CHAR GetKey(_BOOL = TRUE);
    
    /* Add a string to the display buffer */
	void AddToBuffer(const Stringz&);
	
	/* Add a char to the last line of the display buffer */
	void AddToBuffer(_CHAR, _BOOL = TRUE);
	
	/* Return TRUE if exit is requested by the user */
	_BOOL ExitRequested();
	
private:	
	/* Menu IDs */
	enum {
		MENU_APPLE	= 1000,
		MENU_FILE	= 2000
	};

	/* File menu item IDs */	
	enum {
		ITEM_QUIT	= 1
	};
	
    _BOOL			exit; 		// TRUE = exit requested
	_DWORD			ncols;		// number of cols, in chars
	_DWORD			nrows;		// number of rows, in chars
	_DWORD			fontHeight;	// a line height in pixels
	WindowPtr		window;		// The Window
	MenuHandle		mnuApple;	// the Apple menu
	MenuHandle		mnuFile;	// the File menu
	List<Stringz>	buffer;		// Text to be displayed
	
	void DrawWindow();
	_BOOL EventHandler(EventRecord&);
	_BOOL MouseEventHandler(EventRecord&);
	_BOOL MenuEventHandler(EventRecord&);
};

inline _BOOL StdioWindowMacOS::ExitRequested() { return exit; };

}

#endif
