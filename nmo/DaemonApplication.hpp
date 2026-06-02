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
#ifndef _NMO_DAEMONAPP_H_
#define _NMO_DAEMONAPP_H_

namespace NMO {

/*!
 \brief	Daemon application super class

		This classes is used to create 'daemon' (aka services, background) 
		applications, with logging facilities.

\author		(c) 1999-2002 Henri Michelon 
\version	$Id: DaemonApplication.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DaemonApplication : public Application
{
public:
	/*!
	  Levels for writing message to the system logs
	*/  
	typedef enum {
		NORMAL,		
		NOTICE,		
		WARNING,
		ERR,		//! This is ERROR message, called ERR because of name collisiont
		PANIC
	} DaemonLogLevel;

	DaemonApplication(const UStringz&);
	virtual ~DaemonApplication() {};

	virtual void Run();
	virtual void Terminate();
	virtual void SetName(const UStringz&);

	/*! Write a message to the system log
	  	Message can't be written, depending on the level
	 */
	void Write(DaemonLogLevel, const _CHAR*, ...);
private:
#ifdef _WIN32
	_PTR mDaemonApplicationAbstract;
#endif
};


}

#undef CreateApplication
#ifdef _WIN32
extern void dae_main_init(HINSTANCE);
#define CreateApplication(T, N) int PASCAL WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int) { dae_main_init(hInst); try { T app(N); app.Run(); } catch (NMOException e) { e.Display(); } return nmo_main_done(); }
#else
#define CreateApplication(T, N) NmoCreateApplication(T, N)
#endif

#endif
