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
#ifndef _NMO_SYS_ALARM_H_
#define _NMO_SYS_ALARM_H_

namespace NMO {

/*!
\brief		Timer triggered alarms

			An alarm is triggered by a millisecond based timer
			and is associated with a NMOObject inherited class
			method.
			
			Example:

			\code
			class MyClass: public NMOObject

			{
				Alarm *myalarm;
			
				void myalarmhandler(_DWORD COUNT) {
					// This method is called by the Alarm object
					Debug(dprintf("myalarmhandler call number %d", COUNT);)
				}
				
				MyClass () {
					myalarm = new Alarm(this, Alarm::Handler(myalarmhandler));
					myalarm->Start(1000); // Call the handler each second
				}
				
				~MyClass () {
					delete myalarm;
				}
			}

			\endcode
						
			\par
			WARNING: on some OS, alarms are triggered in a "special" state, 
			like at processor interupt time. PLEASE avoid doing low level
			OS things like running a thread, allocating memory, running
			another Alarm or raising signals in your OnAlarm() method.

\author		(c) 1998-2001 Henri Michelon
\version	$Id: Alarm.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Alarm: public NMOObject
{
public:
	/*! Alarm handler method. The _DWORD parameter represent
		the number of calls since the Start() call
	*/	
	typedef void (NMOObject::*Handler)(_DWORD);

	/*! Initialize an alarm and associate it with an object method
		\param NMOObject	: (input) object containing the method
		\param Handler		: (input) handler method
	*/
	Alarm(_PTR, const Handler);
	
	virtual ~Alarm();

	/*! Start the timer
		\param _DWORD	: (input) events interval (milliseconds)
	*/
	void Start(const _DWORD);

	/*! Return TRUE is the timer is running */
	_BOOL Started() const;

	/*! Stop the timer */
	void Stop();

	friend _BOOL operator == (const Alarm&, const Alarm&);

private:
	_BOOL	started;

public: // public for os specific callback functions
	_DWORD		nbCalls;
	NMOObject	*object;
	Handler		event;
	
	static _BOOL installed;
	static List<Alarm> timers;
	
#ifdef _WIN32
	_DWORD	ID;
#else
	_PTR	mAlarmAbstract;
#endif
};


}

#endif
