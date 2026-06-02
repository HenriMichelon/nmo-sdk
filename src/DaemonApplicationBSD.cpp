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

    (c) 1999-2002 Henri Michelon 

$Id: DaemonApplicationBSD.cpp,v 1.2 2002/11/08 10:24:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/Base.hpp>
#include <nmo/Application.hpp>
#include <nmo/DaemonApplication.hpp>
using namespace NMO;

#include <syslog.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#ifdef _BEOS
#include <stdio.h>
#endif

// used to convert from DaemonLogLevel to syslog priority
const int LEVEL2PRI[] = { 
	LOG_NOTICE,
	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERR,
	LOG_ALERT };


//------------------------------------------------------------------
DaemonApplication::DaemonApplication(const UStringz&N): Application(N)
{ }


//------------------------------------------------------------------
void DaemonApplication::SetName(const UStringz&NAME)
{ 
	Application::SetName(NAME);
	// Change application name in the syslog
	closelog();
	Stringz name = NAME;
	openlog((_CHAR*)name, LOG_CONS | LOG_PID , LOG_USER);
	setlogmask(LOG_UPTO(LOG_ALERT));
}


//------------------------------------------------------------------
void DaemonApplication::Run()
{
  	pid_t p = fork();
    if (p < 0) { throw NMOException("Can't run in background"); }
    if (p == 0) { 
    	close(0);
	    close(1);
    	close(2);
	    open ("/dev/null", O_RDWR); /* will always be fd 0 under POSIX*/
    	dup2(0, 1); /* set stdout */
	    dup2(0, 2); /* set stderr */

		// open syslog logging facility
		Stringz name = Name();
		openlog((_CHAR*)name, LOG_CONS | LOG_PID , LOG_USER);
		setlogmask(LOG_UPTO(LOG_NOTICE));
		OnCreate();
		OnDestroy();
		closelog();
	}
}


//------------------------------------------------------------------
void DaemonApplication::Terminate()
{
	OnDestroy();
	closelog();
#ifdef _POSIX
	_exit(0);
#else
	exit(0);
#endif
}


//------------------------------------------------------------------
void DaemonApplication::Write(DaemonApplication::DaemonLogLevel LEVEL, const _CHAR*MSG, ...)
{
#ifdef _BEOS
	_CHAR temp[1000];
	va_list arg;
	va_start(arg, MSG);
	vsprintf(temp, MSG, arg);
	va_end(arg);
	syslog(LEVEL2PRI[LEVEL], temp);
#else
	va_list arg;
	va_start(arg, MSG);
	vsyslog(LEVEL2PRI[LEVEL], MSG, arg);
	va_end(arg);
#endif
}

