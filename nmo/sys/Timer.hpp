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
#ifndef _NMO_SYS_TIMER_H_
#define _NMO_SYS_TIMER_H_

namespace NMO {

/*!

\brief		millisecond based timer device

\author		(c) 1998-2002 Henri Michelon
\version	$Id: Timer.hpp,v 1.2 2002/11/08 10:23:10 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Timer: public NMOObject
{
public:
	Timer();
	virtual ~Timer();

	/*!	reset & start the timer */
	void Start();

	/*!	stop the timer
		\return: number of millisecond since Start()
	*/
	_DWORD Stop();

	/*! return the number of milliseconds since Start();
	*/
	_DWORD Value();

	/*!	wait for (_DWORD) milliseconds */
	static void Sleep(const _DWORD);

private:	
	_PTR mTimerAbstract;
};


}

#endif
