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
#ifndef _NMO_ARRAY_H_
#define _NMO_ARRAY_H_

namespace NMO {

/*!
\brief		Array based collection


			An Array is a collection with an intergrated interator.


\author		(c) 1999-2002 Henri Michelon
\version	$Id: Array.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------


template <class Type>
class Array: public NMO::Collection<Type>, public NMO::Iterator<Type>
{
public:
	/*! Create an array with a selected number of elements */
	Array(_DWORD);

	/*! Create an empty array */
	Array();

	virtual ~Array();

	/*! Create an array.
		On error, array have 0 elements.
		The array must be cleared (0 elements) before call
		\param _DWORD	(input) number of elements
	*/	
	virtual void Dim(_DWORD);

	/*! Change the size of the array.
		On error, the array remain unchanged.
		The array must have 1 or more elements
		allocated with Dim() or ReDim() before call
		\param _DWORD	(input) new number of elements
	*/	
	virtual void ReDim(_DWORD);

	/*! Destroy the array, 
	   clearing all elements 
	*/   
	virtual void Clear();

	/*! Return the address of the first element of the array */
	Type* Address() const;

	/* Return the index (1 based) of the current element */
	virtual _DWORD Index() const;

	/* Inherited from Collection */
	const Array<Type>& operator = (const Array<Type>&);
	virtual _DWORD Count() const;
	virtual Type &operator [](_DWORD);
	virtual _DWORD IndexOf(const Type&) const;

	/* Inherited from Iterator */
	virtual void Reset(_BOOL = TRUE);
	virtual _BOOL End() const;
	virtual _BOOL Start() const;
	virtual Type& Next();
	virtual Type& Prev();
	virtual Type& Current() const;


private:
	_DWORD	size;
	Type	*array;
	_DWORD	current;
};

#include "nmo/base/Array.inl"

}

#endif
