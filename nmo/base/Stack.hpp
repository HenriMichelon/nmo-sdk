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
#ifndef _NMO_STACK_H_
#define _NMO_STACK_H_

namespace NMO {

/*!
\brief		Super class for all stack based collections

			This class is used by all "stack" collections.
			All stacks are filled from the top.

\author		(c) 1999-2001 Henri Michelon 
\version	$Id: Stack.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

template <class Type>
class Stack: public List<Type>
{
public:
	/*! Add a element on the top of the stack. */
	virtual void Push(Type &);

	/*! Get an element from the stack
		and remove it 
	*/	
	virtual Type &Pop() = 0;

	/*! Get an element from the stack
		without removing it 
	*/	
	virtual Type &Get() = 0;

	/*! Return TRUE if the stack is empty */
	virtual _BOOL Empty();
};

#include "nmo/base/Stack.inl"

}

#endif
