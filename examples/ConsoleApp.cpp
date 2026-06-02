// $Id: ConsoleApp.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
#include <nmo/NMO.hpp>
#include <nmo/ConsoleApplication.hpp>
using namespace NMO;

class ConsApp: public ConsoleApplication
{
public:
	ConsApp(UStringz N): ConsoleApplication(N) {};
	
private:
	_CHAR* KeycodeName(_CHAR C)
	{
		switch(C) {
		case NONE: return "none";
		case ESC: return "ESC";
		case TAB: return "TAB";
		case DEL: return "DEL";
		case ENTER: return "ENTER";
		case CTRLC: return "CTRL-C";
		case CTRLD: return "CTRL-D";
		case QUIT: return "QUIT";
		case BACKSPACE: return "BACKSPACE";
		case HOME: return "HOME";
		case END: return "END";
		case INSERT: return "INSERT";
		case PAGE_UP: return "PAGE UP";
		case PAGE_DOWN: return "PAGE DOWN";
		case ARROW_UP: return "ARROW UP";
		case ARROW_DOWN: return "ARROW DOWN";
		case ARROW_LEFT: return "ARROW LEFT";
		case ARROW_RIGHT: return "ARROW RIGHT";
		default:
			return "??";
		}
	}

	void OnCreate()
	{
		Beep();
		PutChar('C');
		PutChar('\n');
		Write("terminal size : %dx%d\n", Cols(), Rows());
		Write("Press a key (ENTER to quit)\n");

		_CHAR c;
		do {
		if (GetChar(c))
			Write("KEYCODE: %s\n", KeycodeName(c));
		else
			Write("char: %c (%d)\n", c, c);
		} while (c != ENTER);
	}
};

DebugOutput(NULL);
CreateApplication(ConsApp, "Console Application");

