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
#ifndef _NMO_ITERATOR_H_
#define _NMO_ITERATOR_H_

namespace NMO {

/*!
\brief		  Super class for iterators

			This class describe an iterator. 
			Usually used as an integrated iterator in NMO.
			Iterated objects can be browsed from start to end and
			from end to start.

\author		(c) 1999-2001 Henri Michelon
\version	$Id: Iterator.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

template <class Type>
class Iterator: public NMOObject
{
public:
	/* Reset the iterator .
	   \param	_BOOL	: TRUE = current element is the first.
	   					FALSE = current element is the last
	*/		
	virtual void Reset(_BOOL = TRUE) = 0;

	/*! Return TRUE if the current element is the last */
	virtual _BOOL End() const = 0;

	/*! Return TRUE if the current element is the first */
	virtual _BOOL Start() const = 0;

	/*! Return the current node, then step to the previous node */
	virtual Type& Prev() = 0;

	/*! Return the current node, then step to the next node */
	virtual Type& Next() = 0;

	/*! Return the current node */
	virtual Type& Current() const = 0;

};

}

#endif
