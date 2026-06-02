/*
$Id: File.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class MyApp: public StdioApplication 
{ 
	void OnCreate()
	{
		Write("CurDir: %s\n", (_CHAR*)Dir::Current());
		File *file = File::Open("File.cpp", File::READ);
		if (!file) {
			Write("Can't open file\n");
			return;
		}
		
		while (!file->Eof()) {
			_BYTE b;
			file->Read(&b, 1);
			Write("%c", b);
		}
		delete file;		
	}

public:
	MyApp(const UStringz &N): StdioApplication(N) {};
};

DebugOutput(NULL);
CreateApplication(MyApp, "MyApp");

