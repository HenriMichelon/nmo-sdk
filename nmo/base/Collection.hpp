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
#ifndef _NMO_COLLECTION_H_
#define _NMO_COLLECTION_H_

namespace NMO {

/*!
\brief		Collection super class

			A Collection can be accessed like an classic array

\author		(c) 1999-2001 Henri Michelon
\version	$Id: Collection.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

template <class Type>
class Collection: public NMOObject
{
public:
	/*! Return the total number of elements */
	virtual _DWORD Count() const = 0;

	/*! Get an element reference.
		\param _DWORD	index: 1 <= index <= Count()
	*/	
	virtual Type &operator [](_DWORD) = 0;

	/*! Return the index for an element by is reference
		\return	index of the element (1 based),
				0 if the element is not found
	*/		
	virtual _DWORD IndexOf(const Type&) const = 0;
};

}

#endif
