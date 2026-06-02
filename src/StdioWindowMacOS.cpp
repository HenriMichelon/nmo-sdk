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

$Id: StdioWindowMacOS.cpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/Base.hpp>
#include "StdioWindowMacOS.hpp"
using namespace NMO;

#ifdef __MWERKS__ // Metrowerks Code Warrior console support

extern "C" {
static StdioWindowMacOS *__nmoconswnd = NULL;

short InstallConsole(short s) {
#pragma unused(s)
	return 0;
}

void RemoveConsole(void) { }

long WriteCharsToConsole(char *buffer, long n) {
	if (__nmoconswnd) {
		for (long i=0; i<n; i++) {
			__nmoconswnd->AddToBuffer(buffer[i], FALSE);
		}
	}
	return n;
}

long ReadCharsFromConsole(char *buffer, long n) {
	if (__nmoconswnd) {
		for (long i=0; i<n; i++) {
			buffer[i] = __nmoconswnd->GetKey();
		}
	}
	return n;
}

#if TARGET_API_MAC_CARBON == 1
int GetToolTrapAddress(int i) {
#pragma unused(i)
	return 0;
}
#endif

}

#endif // __MWERKS__


//-------------------------------------------------
StdioWindowMacOS::StdioWindowMacOS()
{
	ncols = DEFAULTNCOLS;
	nrows = DEFAULTNROWS;
	__nmoconswnd = this;
}


//-------------------------------------------------
StdioWindowMacOS::~StdioWindowMacOS()
{
	if (window) { DisposeWindow(window); }
}


//-------------------------------------------------
_BOOL StdioWindowMacOS::Init(Str255 name)
{
	Rect    wRect;

	InitCursor();

    exit = FALSE;	
	SetRect(&wRect,50,50,60,60);
	window = NewWindow(nil, 
						&wRect, 
						name, 
						true,
						zoomNoGrow, 
						(WindowPtr) -1, 
						true, 
						0);
	
	if (window == NULL) {
		dprintf("NewWindow failed");
		return FALSE;
	}
	
	FontInfo 	fi;
	GrafPtr		curPort;
	
	GetPort(&curPort);
	SetPort(GetWindowPort(window));
	TextFont(FMGetFontFamilyFromName("\pCourier"));
	TextSize(12);
	TextFace(normal);
	GetFontInfo(&fi);
	fontHeight = fi.ascent + fi.descent + fi.leading;
	SizeWindow(window, 
				ncols * CharWidth('A'),
				nrows * fontHeight,
				TRUE);
	ShowWindow(window);
	SetPort(curPort);
	
	mnuFile = NewMenu(MENU_FILE, "\pFile");
	AppendMenu(mnuFile, "\p(Quit/Q");
	InsertMenu(mnuFile, 0);
	EnableMenuItem(mnuFile, ITEM_QUIT);
	
	buffer.Add(new Stringz);
	return TRUE;
}


//-------------------------------------------------
void StdioWindowMacOS::DrawWindow()
{
	Rect		rect;
	GrafPtr		curPort;
	Str255		pstr;
	
	GetPort(&curPort);
	SetPort(GetWindowPort(window));
	BeginUpdate(window);
	EraseRect(GetWindowPortBounds(window, &rect));
	
	_DWORD y = 10;
	ListIterator<Stringz> buffers(buffer);
	buffers.Reset();	
	if (buffers.Count() > nrows) {
		_DWORD first = buffers.Count() - nrows;
		while (first--) {
			buffers.Next();
		}
	}
	
	while (!buffers.End()) {
		Stringz &line = buffers.Next();
		if (line.Len()) {
			CopyCStringToPascal(line, pstr);
			MoveTo(0, y);
			DrawString(pstr);
		}
		y += fontHeight;
	}
	
	EndUpdate(window);
	SetPort(curPort);
}


//-------------------------------------------------
void StdioWindowMacOS::AddToBuffer(const Stringz &STR)
{
	for (_DWORD i=0; i<STR.Len(); i++) {
		AddToBuffer(STR[i], FALSE);
	}
	Rect rect;
	InvalWindowRect(window, GetWindowPortBounds(window, &rect));
	DrawWindow();
}


//-------------------------------------------------
void StdioWindowMacOS::AddToBuffer(_CHAR ch, _BOOL refresh)
{
	ListIterator<Stringz> buffers(buffer);
	switch (ch) {
		case 0x08: {
				Stringz &line = buffers[buffer.Count()];
				if (line.Len()) {
					line = line.Left(line.Len()-1);
				}
			}
			break;
		case '\n':
		case '\r':
			buffer.Add(new Stringz);
			break;
		default:
			buffers[buffer.Count()] += ch;
			break;
	}
	
	if (refresh) {
		Rect rect;
		InvalWindowRect(window, GetWindowPortBounds(window, &rect));
	}
}


//-------------------------------------------------
_CHAR StdioWindowMacOS::GetKey(_BOOL display)
{
	EventRecord event;
	while(!EventHandler(event)) {
		if ((int)event.what == (int)keyDown) {
            _CHAR ch = event.message & charCodeMask;
            if (display) { AddToBuffer(ch); }
            return ch;
		}
	}
	return 0;
}


//-------------------------------------------------
_BOOL StdioWindowMacOS::EventHandler(EventRecord &event)
{
	if (!WaitNextEvent(everyEvent, &event, 
					   GetCaretTime()/2, NULL)) {
	    return FALSE;
	}

	
	//char car;			
	switch (event.what) {
		case mouseDown:
		    return MouseEventHandler(event);
			break;
		case keyDown:
		case autoKey:			    
			break;
		case keyUp:
    		/*car = (event.message & charCodeMask);
    		switch (car) {
    		    case 0x1a: // CTRL-Z
    		    case 0x04: // CTRL-D
    		    case 0x03: // CTRL-C
        		    exit = TRUE;
		            return TRUE;
		            break;
    		}*/
		    break;
		case activateEvt:
		case updateEvt:
			if (window == (WindowPtr)event.message) {
				DrawWindow();
			}
			break;
		case osEvt:
			break;
		default:
			break;
	}
	return FALSE;
}


//-------------------------------------------------
_BOOL StdioWindowMacOS::MouseEventHandler(EventRecord &event)
{
    Rect rect;
    GetWindowPortBounds(window, &rect);
    switch (FindWindow(event.where, &window)) {
        case inMenuBar:
            return MenuEventHandler(event);
            break;
        case inDrag:
            DragWindow(window, event.where, &rect);
        case inContent:
		case inGrow:
            if (window != FrontWindow()) {
			    SelectWindow(window);
			}
        default:
            break;
    }
    return FALSE;
}



//-------------------------------------------------
_BOOL StdioWindowMacOS::MenuEventHandler(EventRecord &event)
{
    _LONG   menuselect = MenuSelect(event.where);
    _SHORT	item = LoWord(menuselect);
  
    switch (HiWord(menuselect)) {
        case MENU_FILE:
        	switch(item) {
        		case ITEM_QUIT:
        			exit = TRUE;
        			return TRUE;
        		default:
        			break;
        	}
        default:
            break;
    }
    return FALSE;
}
#endif