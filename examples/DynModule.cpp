/* 
$Id: DynModule.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
#include "DynModule.hpp"
using namespace NMO;

DebugOutput(NULL);
CreateApplication(Dynload, "Dynload");

typedef int (*FGetMyData)();
typedef char* (*FMyMethod)(char*STR);

//---------------------------------------------------------
void Dynload::OnCreate()
{
	DynModule *mod = DynModule::Create("MyModule");
	if (!mod) {
		Write("Error loading dynamic module %s\n", "MyModule");
		return;
	}

	FGetMyData GetMyData = (FGetMyData) mod->GetMethodAddress("GetMyData");
	FMyMethod MyMethod = (FMyMethod) mod->GetMethodAddress("MyMethod");
	if ((!GetMyData) || (!MyMethod))
	{
		Write("Error linking dynamic module %s\n", "MyModule");
		return;
	}

	Stringz teststring("azerty");
	Write("MyData = %d\n", (*GetMyData)());
	Write("MyMethod =  %s\n", (*MyMethod)(teststring));
	Write("MyData = %d\n", (*GetMyData)());

	delete mod;
}
