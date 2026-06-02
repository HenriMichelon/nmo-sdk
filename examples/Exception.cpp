/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: Exception.cpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class test: public StdioApplication
{
public:
	test(UStringz N): StdioApplication(N) {};
	
private:
	void OnCreate()
	{
		Stringz name = Name();
		Write("%s\n", (_CHAR*)name);

		try {
			throw NMOException(name);
		}
		catch (NMOException e) {
			e.Display();
		}
	}
};


DebugOutput(NULL);
CreateApplication(test, "Exception raising");
