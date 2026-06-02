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

		(c) 2000-2002 Henri Michelon

$Id: ListIterator.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/

//-----------------------------------------------
template <class Type>
ListIterator<Type>::ListIterator()
{
	list = NULL;
	current = NULL;
	indexCurrent = 0;
}


//-----------------------------------------------
template <class Type>
inline ListIterator<Type>::ListIterator(List<Type>&lstLIST)
{
	list = &lstLIST;
	if (list->Count() > 0) {
		current = list->First();
	}
	else {
		current = NULL;
	}
	indexCurrent = 0;
}


//-----------------------------------------------
template <class Type>
ListIterator<Type>& ListIterator<Type>::operator = (List<Type>&lstLIST)
{
	list = &lstLIST;
	current = list->First();
	indexCurrent = 0;
	return *this;
}


//-----------------------------------------------
template <class Type>
ListIterator<Type>::ListIterator(ListIterator<Type>&iteLIST)
{
	list = iteLIST.list;
	current = iteLIST.current;
	indexCurrent = iteLIST.indexCurrent;
}


//-----------------------------------------------
template <class Type>
ListIterator<Type>& ListIterator<Type>::operator = (ListIterator<Type>&iteLIST)
{
	list = iteLIST.list;
	current = iteLIST.current;
	indexCurrent = iteLIST.indexCurrent;
	return *this;
}


//-----------------------------------------------
template <class Type>
Type &ListIterator<Type> :: operator [](_DWORD p)
{
	Debug(
		if (!list) throw NMOException("ListIterator::[] : no list attached");
		if (!p) throw NMOException("ListIterator::[] : list indexes are 1-based");
		if (p>list->Count()) throw NMOException("ListIterator::[] : index out of range");
	)

	List<Type>::SNode	*Tmp;
	_DWORD	i;
	_DWORD	end = p-1;

	Tmp = current;
	if (end>indexCurrent)
	{
		for (i = indexCurrent; (i < end) && Tmp->Next; i++) {
			Tmp = Tmp->Next;
		}
		indexCurrent = end;
		current = Tmp;
	}
	else if (end<indexCurrent)
	{
		for (i = indexCurrent; (i > end) && Tmp->Prev; i--) {
			Tmp = Tmp->Prev;
		}
		indexCurrent = end;
		current = Tmp;
	}
	else if (list->Count() == 1) {
		Tmp = list->First();
		indexCurrent = 0;
		current = Tmp;
	}
	return *Tmp->Info;
}


//-----------------------------------------------
template <class Type>
void ListIterator<Type> :: Reset(_BOOL FROM)
{
	Debug(if (!list) throw NMOException("ListIterator::Reset : no list attached");)
	if (FROM) {
		current = list->First();
		indexCurrent = 0;
	}
	else {
		current = list->Last();
		indexCurrent = list->Count();
	}
}


//-----------------------------------------------
template <class Type>
inline Type &ListIterator<Type> :: Prev(void)
{
	Debug(
		if (!list) throw NMOException("ListIterator::Prev : no list attached");
		if (Start()) throw NMOException("ListIterator::Next : beginning of list");
	)

	List<Type>::SNode *C = current;
	current = current->Prev;
	--indexCurrent;
	return *C->Info;
}



//-----------------------------------------------
template <class Type>
inline Type &ListIterator<Type> :: Next(void)
{
	Debug(
		if (!list) throw NMOException("ListIterator::Next : no list attached");
		if (End()) throw NMOException("ListIterator::Next : end of list");
	)

	List<Type>::SNode *C = current;
	current = current->Next;
	++indexCurrent;
	return *C->Info;
}


//-----------------------------------------------
template <class Type>
inline _BOOL ListIterator<Type> :: Start(void) const
{
	Debug(if (!list) throw NMOException("ListIterator::Start : no list attached");)
	return (!indexCurrent);
}



//-----------------------------------------------
template <class Type>
inline _BOOL ListIterator<Type> :: End(void) const
{
	Debug(if (!list) throw NMOException("ListIterator::End : no list attached");)
	return (indexCurrent >= list->Count());
}


//-----------------------------------------------
template <class Type>
inline Type& ListIterator<Type> :: Current(void) const
{
	Debug(
		if (!list) throw NMOException("ListIterator::Current : no list attached");
		if (!list->Count()) throw NMOException("ListIterator::Current : empty list");
	)

	return *current->Info;
}


//-----------------------------------------------
template <class Type>
_DWORD ListIterator<Type> :: Index(void) const
{
	Debug(
		if (!list) throw NMOException("ListIterator::Index : no list attached");
		if (!list->Count()) throw NMOException("ListIterator::Index : empty list");
	)

	return indexCurrent+1;
}


//-----------------------------------------------
template <class Type>
inline _DWORD ListIterator<Type> :: Count() const
{
	Debug(
		if (!list) throw NMOException("ListIterator::Index : no list attached");
	)
	return list->Count();
}


//-----------------------------------------------
template <class Type>
_DWORD ListIterator<Type> :: IndexOf(const Type &T) const
{
	_DWORD i;
	List<Type>::SNode *Tmp = list->First();

	for (i = 0; i < list->Count(); i++)
	{
		if (Tmp->Info == &T)
			break;
		Tmp = Tmp->Next;
	}
	if (Tmp->Info == &T) 
		return i + 1;
	else
		return 0;
}


//-----------------------------------------------
template <class Type>
_DWORD ListIterator<Type> :: SearchFor(const Type& T)
{
	_DWORD i;
	current = list->First();
	indexCurrent = 0;

	for (i = 0; i < list->Count(); i++)
	{
		if (*(current->Info) == T)
			break;
		current = current->Next;
		indexCurrent++;
	}
	List<Type>::SNode *tmp = current;
	current = current->Next;
	indexCurrent++;
	if (!tmp->Info) return 0;
	if (*(tmp->Info) == T) 
		return i + 1;
	else
		return 0;
}

//-----------------------------------------------
template <class Type>
List<Type>& ListIterator<Type> :: GetList()
{
	return *list;
}
