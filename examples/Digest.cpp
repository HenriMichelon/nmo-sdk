/*
	On FreeBSD, you can compare with:
		md5 -s toto
		cat Digest.cpp | md5

$Id: Digest.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
#include <nmo/addons/Digest.hpp>
using namespace NMO;


class digest: public StdioApplication
{
public:
	digest(UStringz N): StdioApplication(N) {};
	
private:
	void OnCreate()
	{
		// The main md5 object used multiple times
		DigestMD5 md5;

		// Compute a sample md5 sum for a ASCIIZ string
		Stringz string = "toto";
		Write("MD5(\"%s\") = %s\n", (_CHAR*)string, 
								(_CHAR*)md5.String(string));

		// Compute a sample md5 sum for a ASCIIZ string
		Write("MD5([%s\\0]) = %s\n", (_CHAR*)string, 
								(_CHAR*)md5.Data((_PTR)(_CHAR*)string,
												 string.Len()+1));

		// Compute a sample md5 sum for a text file
		Stringz filename = "Digest.cpp";
		File *file = File::Open(filename, File::READ);
		if (file) {
			Write("MD5(%s) = %s\n", (_CHAR*)filename,
									(_CHAR*)md5.File(*file));
			delete file;
		}
		else {
			Write("Can't open %s\n", (_CHAR*)filename);
		}
	}

};


DebugOutput(NULL);
CreateApplication(digest, "Digest");
