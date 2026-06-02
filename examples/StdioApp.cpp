// $Id: StdioApp.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class ConsApp: public StdioApplication
{
public:
	ConsApp(UStringz N): StdioApplication(N) { };
	
private:
	void OnCreate()
	{
		PutChar('C');
		PutChar('\n');

		Write("Enter a char then enter: ");
		_CHAR c = GetChar();
		Write("char: %c\n", c);

		Stringz s;
		Write("Enter some lines of text (a blank line to quit).\n");
		do {
			Write("Type text: ");
			s = Readln();
			Write("text: %s\n", (_CHAR*)s);
		} while (s.Len());
	}
};


DebugOutput(NULL);
CreateApplication(ConsApp, "Stdio Application");

