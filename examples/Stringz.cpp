/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: Stringz.cpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>

class test: public StdioApplication
{
public:
	test(UStringz N): StdioApplication(N) {};
	
private:
	void OnCreate()
	{
		Stringz str("A long, not so long string to managed");
		Write("String: %s\n", (_CHAR*)str);
		str.Sub('l', 'L');
		Write("sub('l', 'L'): %s\n", (_CHAR*)str);
		str.Sub("Long", "short");
		Write("sub('Long', 'short'): %s\n", (_CHAR*)str);
	}
};


DebugOutput(NULL);
CreateApplication(test, "Stringz class demo");
