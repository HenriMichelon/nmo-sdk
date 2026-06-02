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
#ifndef _NMO_EXC_H_
#define _NMO_EXC_H_

#include <stdarg.h>

namespace NMO {

/*!
\brief		Generic exception class for the SDK  

			All exceptions raised in hte NMO-SDK classes inherit from
			this class.
			If a NMOException is not catched, the Display() method is
			automatically called.

\author		(c) 1998-2001 Henri Michelon 		
\version	$Id: Exc.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class UStringz;

const int EXCMSGMAXLEN = 1024;

class NMOException
{
public:
	/*! Create a exception with a formatted message.
	  	cf C printf() functions for arguments
	*/	
   	NMOException (const char*, ...);

	/*! Display the exception message.
	  	The display method is architecture dependent.
	*/	
	void Display();

	const char* Message() const;

	static void SetTitle(const UStringz&);

protected:
	static UStringz sTitle;
	void SetMessage(const char*, ...);

private:
	char mMsg[EXCMSGMAXLEN];
};

}

#endif // _EXC_H_
