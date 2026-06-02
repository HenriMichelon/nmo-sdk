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

$Id: SystemBSD.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef _LINUX
    #include <signal.h>
    #include <sys/reboot.h>
    #define MY_HALT         RB_HALT_SYSTEM
    #define MY_REBOOT        RB_AUTOBOOT
    #define MY_POWEROFF        0x4321fedc  | MY_HALT
#elif _IRIX
    #include <signal.h>
    #include <sys/uadmin.h>
    #define MY_HALT            AD_HALT
    #define MY_REBOOT        AD_BOOT
    #ifdef AD_POWEROF
	    #define MY_POWEROFF        AD_POWEROF
    #else
	    #define MY_POWEROFF        AD_HALT
    #endif
#elif _ATHEOS
    #include <atheos/kernel.h>
#elif _QNX
	#include <sys/sysmgr.h>
#else
    #include <signal.h>
    #include <sys/reboot.h>
    #define MY_HALT         RB_HALT
    #define MY_REBOOT        RB_AUTOBOOT
    #ifdef __FreeBSD__
        #define MY_POWEROFF        RB_POWEROFF | MY_HALT
    #elif __OpenBSD__
        #define MY_POWEROFF        RB_POWERDOWN | MY_HALT
    #else
        #define MY_POWEROFF        MY_HALT
    #endif
#endif

//------------------------------------------------------
_LONG NMO::System::Rand()
{
    return random();
}


//------------------------------------------------------
void NMO::System::InitRand()
{
#ifdef __FreeBSD__
    srandomdev();
#else
    time_t t;
    time(&t);
    srandom(t);
#endif
}


//------------------------------------------------------
_BOOL NMO :: System :: Shutdown (System::ShutdownMode MODE)
{
#ifdef _ATHEOS
   return reboot ();
#elif _QNX
   return sysmgr_reboot ();
#else   
    int howto;
    switch (MODE) {
    case SHUTDOWN_HALT: howto = MY_HALT; break;
    case SHUTDOWN_POWEROFF: howto = MY_POWEROFF | MY_HALT; break;
    case SHUTDOWN_REBOOT: howto = MY_REBOOT; break;
    default:
      return FALSE;
    }
    /* do it cleanly: cf FreeBSD:src/sys/reboot/reboot.c */
    sync();
#ifdef _IRIX
    return (uadmin(A_SHUTDOWN, howto, 0) != -1);
#else
    kill(1, SIGTSTP);
    signal(SIGHUP, SIG_IGN);
    kill(-1, SIGTERM);
    sleep(2);
    sync();
    sleep(2);
    kill(-1, SIGKILL);
 #if defined(__NetBSD__) || defined(_SUNOS)
    return (reboot(howto, NULL) != -1);
 #else
    return (reboot(howto) != -1);
 #endif // __NetBSD__
#endif // _IRIX
#endif // _ATHEOS

}
