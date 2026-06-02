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
#ifndef _NMO_LISTITERATOR_H_
#define _NMO_LISTITERATOR_H_

namespace NMO {

/*!
\brief		  Indepent iterator for List objects

\author		(c) 2000-2002 Henri Michelon
\version	$Id: ListIterator.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

template <class Type>
class ListIterator: public NMO::Collection<Type>, public NMO::Iterator<Type>
{
public:
	ListIterator();

	ListIterator(List<Type>&);
	ListIterator(ListIterator<Type>&);

	ListIterator<Type>& operator = (List<Type>&);
	ListIterator<Type>& operator = (ListIterator<Type>&);

	virtual void Reset(_BOOL = TRUE);
	virtual _BOOL End() const;
	virtual _BOOL Start() const;
	virtual Type& Prev();
	virtual Type& Next();
	virtual Type& Current() const;
	_DWORD Index() const;

	Type &operator [](_DWORD);
	_DWORD IndexOf(const Type&) const;
	_DWORD Count() const;

	_DWORD SearchFor(const Type&);
	 
	List<Type>& GetList();

private:
	List<Type>::SNode	*current;
	_DWORD				indexCurrent;
	List<Type>			*list;
};

#include "nmo/base/ListIterator.inl"

}

#endif
