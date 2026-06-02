/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2002 Henri Michelon

$Id: Application.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#if ( defined(_AMIGA) || defined(amigaos) ) && ! defined(AUTO_LIB)
	#include <exec/types.h>
	struct IntuitionBase *IntuitionBase = NULL;
#endif
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;

Stringz Application::path;
Args Application::args;
Application *Application::app = NULL;


//-------------------------------------------------
void nmo_main_init(int argc, char*argv[], const char*dgo)
{
	Debug(dopen(dgo);)
	Application::Argv().Set(argc, &argv);
	Application::SetPath(argv[0]);
}


//---------------------------------------------
int nmo_main_done()
{
#if defined(_AMIGA) && !defined(AUTO_LIB)
	if (IntuitionBase) CloseLibrary(&IntuitionBase->LibNode);
#endif
	Debug(  
		NMOException::SetTitle((UChar*)NULL);
		Application::SetPath(NULL);
		Application::Argv().Clear();
		ThCriticalSection::cs.Clear();
		dclose();
	)
#ifdef _MACOS
	ExitToShell();
#endif
	return 0;
}


//---------------------------------------------
Application::Application(const UStringz &N): appName(N) 
{ 
	app = this;
	NMOException::SetTitle(N);
}


//---------------------------------------------
const Stringz& Application::Path()
{
	return path;
}


//---------------------------------------------
void Application::SetPath(const char* PATH)
{
	path = PATH;
}


//---------------------------------------------
void Application::SetName(const UStringz&NAME)
{
	appName = NAME;
	NMOException::SetTitle(NAME);
}

//---------------------------------------------
const UStringz& Application::Name() const 
{ return (const UStringz&)appName; };


//---------------------------------------------
Args& Application::Argv()
{ return args; }

