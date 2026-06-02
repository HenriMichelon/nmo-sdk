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

*/
#ifndef _NMO_SCONAPP_H_
#define _NMO_SCONAPP_H_

namespace NMO {
/*!
\brief		Simple Console based application skeleton

  			This class is used to write simple console based application.\n
  			Interaction of this kind of application is limited to
  			output of strings and input of characters.

\author		(c) 2000-2001 Henri Michelon 
\version	$Id: StdioApplication.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class StdioApplication : public Application
{
public:
	// Create an application using the given name (aka title)
	StdioApplication(const UStringz&N);
	virtual ~StdioApplication();

	virtual void Run();
	virtual void Terminate();

	/*! write a formated string into the console
		\return always TRUE
	*/	
	virtual _BOOL Write(const _CHAR*, ...);

	/*! Get a char (without echo) from the console\n
		char&	: (output) result char\n
	*/	
	virtual _CHAR GetChar();

	//! put a char into the console
	virtual void PutChar(_CHAR);

	/*! Read a string from the console\n
	*/	
	virtual Stringz Readln();
	
	/*! Flush input & output buffers
	*/  
	virtual void Flush();

private:
#ifdef _MACOS
	_PTR mStdioApplicationAbstract;
#endif
};

}

#undef CreateApplication
#if defined(_WIN32) && ! defined(_MANAGED)
extern void cons_main_init();
#define CreateApplication(T, N) int main() { cons_main_init(); try { T app(N); app.Run(); } catch (NMOException e) { e.Display(); } return nmo_main_done(); }
#else
#define CreateApplication(T, N) NmoCreateApplication(T, N)
#endif

#endif

