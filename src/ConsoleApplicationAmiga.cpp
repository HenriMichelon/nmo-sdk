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

$Id: ConsoleApplicationAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <nmo/Base.hpp>
#include <nmo/Application.hpp>
#include <nmo/ConsoleApplication.hpp>
using namespace NMO;

typedef struct consabstract {
	struct Window   *win;
	struct IOStdReq *writeReq;    // I/O request block pointer 
	struct MsgPort  *writePort;   // replyport for writes      
	struct IOStdReq *readReq;     // I/O request block pointer
	struct MsgPort  *readPort;    // replyport for reads
	UBYTE				ch;
	Stringz			name;
} AbstractCons;

#define __m(c) ((AbstractCons*)(this->abstract))->c


//-------------------------------------------------
ConsoleApplication::ConsoleApplication(const UStringz&N): 
	StdioApplication(N)
{
#if ! defined(AUTO_LIB)
	if (!IntuitionBase) {
		IntuitionBase = (struct IntuitionBase*) OpenLibrary(INTUITIONNAME, INTUITIONVER);
    }
    if (!IntuitionBase) {
        throw NMOException("Can't open \"%s\" version %ld or newer.", INTUITIONNAME, INTUITIONVER);
    }
#endif
	__m(name) = N;
	struct NewWindow nw = {
		50, 50, 							
    	620,200, 						
		~0,~0,                     
    	CLOSEWINDOW,               
    	WINDOWDEPTH|WINDOWSIZING|
    	WINDOWDRAG|WINDOWCLOSE|
    	SMART_REFRESH|ACTIVATE,    
    	NULL,                      
    	NULL,                      
    	(UBYTE*)(_CHAR*)__m(name),
    	NULL,                      
    	NULL,                      
    	50, 50,                    
    	640,480,                   
    	WBENCHSCREEN               
   };
   
	// Allocate abstract structure
	abstract = (_PTR) new AbstractCons;
	Memory::Fill(abstract, 0, sizeof(AbstractCons));

	// Open a window 
	if(!(__m(win) = OpenWindow(&nw))) {
		throw NMOException("Can't open window");
	}
         
	// Create reply port and io block for writing to console 
	if(!(__m(writePort) = CreatePort(NULL, 0))) {
		throw NMOException("Can't create write port");
	}
	if (!(__m(writeReq) = (struct IOStdReq *) CreateExtIO(__m(writePort), 
														  (LONG)sizeof(struct IOStdReq)))) {
		throw NMOException("Can't create write request");
	}

    // Create reply port and io block for reading from console 
	if (!(__m(readPort) = CreatePort(NULL, 0))) {
		throw NMOException("Can't create read port");
	}
	if (!(__m(readReq) = (struct IOStdReq *) CreateExtIO(__m(readPort), 
														 (LONG)sizeof(struct IOStdReq)))) {
    	throw NMOException("Can't create read request");
	}

	// Attach the new window to our I/O port
	__m(writeReq)->io_Data = (APTR) __m(win);
	__m(writeReq)->io_Length = sizeof(struct Window);

	// Open & attach console device
	if (OpenDevice("console.device", 0, (IORequest*)__m(writeReq), 0)) {
		throw NMOException("Can't open console.device");
	}
	__m(readReq)->io_Device = __m(writeReq)->io_Device; 
	__m(readReq)->io_Unit   = __m(writeReq)->io_Unit;

	__m(readReq)->io_Command = CMD_READ;
	__m(readReq)->io_Data = (APTR)&__m(ch);
	__m(readReq)->io_Length = 1;
	SendIO((IORequest*)__m(readReq));
}


//-------------------------------------------------
ConsoleApplication::~ConsoleApplication()
{
	// Stop any pending operation
	if(!(CheckIO((IORequest*)__m(readReq)))) {
		AbortIO((IORequest*)__m(readReq));
	}
	WaitIO((IORequest*)__m(readReq));

	CloseDevice((IORequest*)__m(writeReq));
	if(__m(win)) { CloseWindow(__m(win)); }
	if(__m(readReq)) { DeleteExtIO((IORequest*)__m(readReq)); }
	if(__m(readPort)) { DeletePort(__m(readPort)); }
	if(__m(writeReq)) { DeleteExtIO((IORequest*)__m(writeReq)); }
	if (__m(writePort)) { DeletePort(__m(writePort)); }
	if (abstract) { delete (AbstractCons*)abstract; }
}


//-------------------------------------------------
_BOOL ConsoleApplication::GetChar(_CHAR&RESULT)
{
	IOStdReq *readreq = NULL;
	BYTE 		InControl = 0;
	_BOOL		keyCode = FALSE;
	Stringz	seq;
	
	do {

	   // Wait for a message from the console
	   WaitPort(__m(readPort));
	   readreq = (IOStdReq *)GetMsg(__m(readPort));
	 	// Read next char
	   readreq->io_Command = CMD_READ;
		readreq->io_Data = (APTR)&__m(ch);
   	readreq->io_Length = 1;
	   SendIO((IORequest*)readreq);
   	
   	// 0x1B ESC not followed by '[', is not CSI seq 
      /*if (InControl==1) {
      	if(c == '[')
      		InControl = 2;
         else 
         	InControl = -2;
      }

      if ((c==0x9B)||(c==0x1B)) { // Control seq starting 
      	InControl = (c==0x1B) ? 1 : 2;
      }

		// Valid ESC sequence terminator ends an ESC seq
      if ((InControl==3)&&((c >= 0x40) && (c <= 0x7E))) {
      	InControl = -1;
      }
      if (InControl==2) {
      	InControl = 3;
      	keyCode = TRUE;
      }
      
      // ESC sequence finished (-1 if OK, -2 if bogus) 
      if (InControl < 0) {
			InControl = 0;
      }
      if (InControl) {
      	seq += c;
      }*/
	} while (InControl);
	              
	if (!keyCode)
	{
		// normal key or keycode not found
		// use well known values for some keys
		switch (__m(ch))
		{
		case 8:
			keyCode = TRUE;
			RESULT = ConsoleApplication::BACKSPACE;
			break;
		case 127:
			keyCode = TRUE;
			RESULT = ConsoleApplication::DEL;
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
			keyCode = TRUE;
			RESULT = ConsoleApplication::ENTER;
			break;
		default:
			RESULT = __m(ch);
			break;
		}
	}
	Debug(dprintf("keycode: %d, char: %d (%c)\n", keyCode, __m(ch), __m(ch));)
	return keyCode;
}


//-------------------------------------------------
void ConsoleApplication:: PutChar(char C)
{
	__m(writeReq)->io_Command = CMD_WRITE;
	__m(writeReq)->io_Data = (APTR)&C;
	__m(writeReq)->io_Length = 1;
	DoIO((IORequest*)__m(writeReq));
}


//-------------------------------------------------
void ConsoleApplication:: SendBreak()
{
	Writeln("\033[\03@");
}


//-------------------------------------------------
void ConsoleApplication:: UngetChar(char C)
{
	Stringz seq = "\033" + C + '@';
	Writeln((_CHAR*)seq);
}


//-------------------------------------------------
void ConsoleApplication:: Beep()
{
	PutChar(7);
}


//-------------------------------------------------
void ConsoleApplication:: SetName(const UStringz&STR) 
{
	__m(name) = STR;	
	SetWindowTitles(__m(win), (_CHAR*)__m(name), (_CHAR*)__m(name));
}


//-------------------------------------------------
_DWORD ConsoleApplication:: Cols()
{
	return ((__m(win)->Width - 
				__m(win)->BorderLeft -
				__m(win)->BorderRight) / 
				(__m(win)->IFont->tf_XSize));
}


//-------------------------------------------------
_DWORD ConsoleApplication:: Rows()
{
	return ((__m(win)->Height - 
				__m(win)->BorderTop -
				__m(win)->BorderBottom) / 
				(__m(win)->IFont->tf_YSize));
}


//-------------------------------------------------
void ConsoleApplication:: Writeln(const char*STR) 
{
    __m(writeReq)->io_Command = CMD_WRITE;
    __m(writeReq)->io_Data = (APTR)STR;
    __m(writeReq)->io_Length = ~0;
    DoIO((IORequest*)__m(writeReq));
}

#endif
