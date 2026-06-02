/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: SysInfo.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;

class MyApp: public StdioApplication 
{ 
	void OnCreate()
	{
		Write("OS Name: %s\n", (char*)SysInfo::OSName());
		Write("OS Version: %s\n", (char*)SysInfo::OSVersion());
		Write("OS Release: %s\n", (char*)SysInfo::OSRelease());
		Write("Hardware platform: %s\n", (char*)SysInfo::Hardware());
		Write("Hostname: %s\n", (char*)SysInfo::Hostname());
		Write("Username: %s\n", (char*)SysInfo::Username());
		Write("Userhome: %s\n", (char*)SysInfo::Userhome());
	}

public:
	MyApp(const UStringz &N): StdioApplication(N) {};
};

DebugOutput(NULL);
CreateApplication(MyApp, "MyApp");
