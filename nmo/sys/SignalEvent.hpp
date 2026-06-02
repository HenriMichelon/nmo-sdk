/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_SIGNAL_H_
#define _NMO_SYS_SIGNAL_H_

namespace NMO {

/*!
\brief		Signal handling.

			Only the most currents signal are supported
			for maximum portability.

			Example of use:

			\code
			class MyClass: public SignalEvent
			{
			public:
				MyClass()
				{
					Redirect(TERMINATE);
				}
				
				~MyClass()
				{
					Unredirect(TERMINATE);
				}
			private:
				void OnSignal(Signal)
				{
					dprintf("Received a TERM signal !\n");
					MyApplication().Terminate();
				}
			}
			\endcode

\author		(c) 1999-2002 Henri Michelon
\version	$Id: SignalEvent.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class SignalEvent: public NMOObject
{
public:
	/*! Supported signals */
	typedef enum
	{
		NONE,
		ABORT,
		INTERRUPT,
		SEGVIOLATION,
		ILLEGALINSTR,
		TERMINATE
	} Signal;

	/*! Redirect a signal.
		The handler remains installed after a
		signal has been delivered
	*/
	void Redirect(Signal);

	/*! Terminate a redirection */
	void Unredirect(Signal);

	/*! Tell the OS the process want to ignore the signal */
	void Ignore(Signal);

	/*! Signal handler */
	virtual void OnSignal(Signal) = 0;

	static SignalEvent *_sig_handler;
};


}

#endif
