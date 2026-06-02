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
#ifndef _NMO_FIFO_H_
#define _NMO_FIFO_H_

namespace NMO {
/*!
\brief		First in - first out stack

			The elements returned by Pop() and Get() are taken
			from the bottom of the stack.

\author		(c) 1999-2001 Henri Michelon 
\version	$Id: Fifo.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

template <class Type>
class Fifo: public Stack<Type>
{
public:
	/*! Get an element from the bottom of the stack
		and remove it 
	*/	
	virtual Type &Pop();

	/*! Get an element from the bottom of the stack
		without removing it 
	*/	
	virtual Type &Get();

};

#include "nmo/base/Fifo.inl"

}

#endif
