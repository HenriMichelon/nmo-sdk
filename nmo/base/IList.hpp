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
#ifndef _NMO_ILIST_H_
#define _NMO_ILIST_H_

namespace NMO {

/*!
\brief		Simple double headed list

			Simple but efficient list, for use with ListIterator			
		
			+ All indexes are 1 based
			+ List is automatically cleared

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: IList.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------


template <class Type>
class List: public NMOObject
{
public:
	List();
	virtual ~List();

	/*! Add an element at the current position of the list.
		Node information is NOT deleted on a Drop() or on a Clear()
		\return index of the node, 1 based, 0 on error
	*/	
#if (__GNUC__ == 2 && __GNUC_MINOR__ < 92)
	_DWORD Add(Type &);
#else
	virtual _DWORD Add(Type &);
#endif

	/*! Add a node at the current position of the list.
		Node information IS deleted on a Drop() or on a Clear()
		\return index of the node, 1 based, 0 on error
	*/	
#if (__GNUC__ == 2 && __GNUC_MINOR__ < 92)
	_DWORD Add(Type *);
#else
	virtual _DWORD Add(Type *);
#endif

	/* Return the total number of element */
	virtual _DWORD Count() const;

	/*! Destroy a specific element from the list.
		\param _DWORD index, 1 based
	*/	
	void Drop(_DWORD);

	/*! Replace the data associated with the element
		\param _DWORD index, 1 based
		\param Type* data */
	void Replace (_DWORD, Type*);

	/* Clear the whole list */
#if (__GNUC__ == 2 && __GNUC_MINOR__ < 92)
	void Clear();
#else
	virtual void Clear();
#endif

	/*! Sort the list in ascendant order.
		Write an '<' friend operator for custom sorting
	*/
	void Sort();

	/*! Copy the list elements without creating new elements */
	void Clone(List<Type>&);

	/*! Copy the list elements by creating new elements.
		The "Type" class must have a copy constructor.
	*/
	void Copy(List<Type>&);

	class SNode
	{
	public:
		Type *Info;
		SNode *Next;
		SNode *Prev;
		_BOOL deleteInfo;
	};

	SNode* First() const;
	SNode* Last() const;

protected:
	SNode	H, T;
	SNode	*head;
	SNode	*tail;
	_DWORD	nbElem;
};

#include "nmo/base/IList.inl"

}

#endif
