/*                                ---------
                                  [NMO-SDK]
                                  ---------

*/
#ifndef _NMO_SYSTEM_H_
#define _NMO_SYSTEM_H_

namespace NMO {

/*!
\brief		OS Specific operations

\author		(c) 1999-2002 Henri Michelon
\version	$Id: System.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------
class System
{
public:
	/*! List of arguments for Execute() */
	typedef List<Stringz> Args;

	/*! Modes of application execution */
	typedef enum
	{
		/*! Overlays calling process with 
			new process, destroying the 
			calling process
		*/
		EXEC_OVERLAY,
		/*! Suspends calling process until 
			execution of new process is complete
		*/
		EXEC_NORMAL,
		/*! Continues to execute calling 
			process concurrently with new process
		*/
		EXEC_BACKGROUND
	} ExecMode;

	/*! Shutdown() mode */
	typedef enum
	{
		/*! Halt all the processes and the operating system */
		SHUTDOWN_HALT,
		/*! Halt then try to power off the computer */
		SHUTDOWN_POWEROFF,
		/*! Halt then reboot the computer */
		SHUTDOWN_REBOOT
	} ShutdownMode;


	/*! Execute an application
		\param Stringz	: name (& path) of the application
		\param Args&	: arguments
		\param ExecMode: execution mode
		\param _PID*	: if !=NULL & ExecMode==BACKGROUND,
						receive the pid of the new process
		\return FALSE if error
	*/
	static _BOOL Execute(const Stringz&, Args&, ExecMode = EXEC_NORMAL,
						 _PID* = NULL);

	/*! Return a pseudo random number */
	static _LONG Rand();

	/*! Initialize pseudo random number generator */
	static void InitRand();


	/*! Try to shutdown the computer. */
	static _BOOL Shutdown(ShutdownMode = SHUTDOWN_POWEROFF);

};

}

#endif
