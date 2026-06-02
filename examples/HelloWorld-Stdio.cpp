// $Id: HelloWorld-Stdio.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
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
		Write("Hello World!\n");
	}
};

DebugOutput(NULL);
CreateApplication(ConsApp, "Hello World Stdio");
