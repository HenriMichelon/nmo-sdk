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

	(c) 1998-2002 Henri Michelon

$Id: IList.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/

//-----------------------------------------------
template <class Type>
_DWORD List<Type> :: Add(Type &P)
{
	SNode *Tmp;
	Tmp = new SNode;
	Tmp->Info = &P;
	Tmp->deleteInfo = FALSE;
	Tmp->Prev = tail->Prev;
	Tmp->Next = tail;
	Tmp->Prev->Next = Tmp;
	tail->Prev = Tmp;
	++nbElem;
	return nbElem;
}


//-----------------------------------------------
template <class Type>
_DWORD List<Type> :: Add(Type *P)
{
	if (P == NULL) { return 0; }
	SNode *Tmp;
	Tmp = new SNode;
	Tmp->Info = P;
	Tmp->deleteInfo = TRUE;
	Tmp->Prev = tail->Prev;
	Tmp->Next = tail;
	Tmp->Prev->Next = Tmp;
	tail->Prev = Tmp;
	++nbElem;
	return nbElem;
}


//-----------------------------------------------
template <class Type>
inline _DWORD List<Type> :: Count(void) const
{
	return nbElem;
}


//-----------------------------------------------
template <class Type>
inline List<Type> :: List()
{
	nbElem = 0;
	head = &H;
	tail = &T;
	if (head)
	{
		head->Info = NULL;
		head->Prev = NULL;
		head->Next = tail;
		head->deleteInfo  = FALSE;
	}
	if (tail)
	{
		tail->Info = NULL;
		tail->Next = NULL;
		tail->Prev = head;
		tail->deleteInfo = FALSE;
	}
}


//-----------------------------------------------
template <class Type>
List<Type> :: ~List()
{
	Clear();
}


//-----------------------------------------------
template <class Type>
void List<Type> :: Clear()
{
	register SNode *tmp;
	register SNode *mark = tail->Prev;

	while (mark != head)	{
		tmp = mark;
		mark = mark->Prev;
		if (tmp->deleteInfo) {
			delete tmp->Info;
		}
		
		delete tmp;
	}
	tail->Prev = head;
	head->Next = tail;
	nbElem = 0;
}


//-----------------------------------------------
template <class Type>
void List<Type> :: Drop(_DWORD dwIDX)
{
	Debug(
		if (!dwIDX) throw NMOException("List::Drop : list indexes are 1-based");
		if (dwIDX > nbElem) throw NMOException("List::Drop : index out of range");
	)

	_DWORD idx;	
	SNode *current = First();
	for (idx = 1; (idx < dwIDX) && (idx <= nbElem); idx ++) {
		current = current->Next;
	}
	if (idx == dwIDX) {
		SNode *P = current->Prev;
		SNode *N = current->Next;
		P->Next = N;
		N->Prev = P;
		nbElem--;
		if (current->deleteInfo) {
			delete current->Info;
		}
		delete current;
	}
}


//-----------------------------------------------
template <class Type>
void List<Type> :: Replace (_DWORD dwIDX, Type *DATA)
{
	Debug(
		if (!dwIDX) throw NMOException("List::Drop : list indexes are 1-based");
		if (dwIDX > nbElem) throw NMOException("List::Drop : index out of range");
	)

	_DWORD idx;	
	SNode *current = head->Next;
	for (idx = 1; (idx < dwIDX) && (idx <= nbElem); idx ++) {
		current = current->Next;
	}
	if (idx == dwIDX) {
		if (current->deleteInfo) {
			delete current->Info;
		}
		current->Info = DATA;
		current->deleteInfo = TRUE;
	}
}


//-----------------------------------------------
template <class Type>
void List<Type> :: Sort()
{
	if (nbElem < 2) return;

	SNode *sort;
	SNode *first = head->Next; 
	SNode *mark = first->Next;

	while (mark != tail)
	{
		sort = mark;
		mark = mark->Next;
		while ((*(sort->Info)) < (*(sort->Prev->Info)))
		{
			Type *tmpi = sort->Info;
			sort->Info = sort->Prev->Info;
			sort->Prev->Info = tmpi;

			_BOOL tmpd = sort->deleteInfo;
			sort->deleteInfo = sort->Prev->deleteInfo;
			sort->Prev->deleteInfo = tmpd;

			sort = sort->Prev;
			if (sort == first) break;
		}
	}
}


//-----------------------------------------------
template <class Type>
inline List<Type>::SNode * List<Type> :: First() const 
{ 
	return head->Next; 
}


//-----------------------------------------------
template <class Type>
inline List<Type>::SNode * List<Type> :: Last() const 
{
	return tail->Prev;
}


//------------------------------------------------------
template <class Type>
inline void List<Type> :: Clone(List<Type>&SRC)
{
	Clear();
	SNode *tmp = SRC.First();
	for (_DWORD idx = 0; idx < SRC.Count(); idx ++) {
		Add(*(tmp->Info));
		tmp = tmp->Next;
	}
}


//------------------------------------------------------
template <class Type>
inline void List<Type> :: Copy(List<Type>&SRC)
{
	Clear();
	SNode *tmp = SRC.First();
	for (_DWORD idx = 0; idx < SRC.Count(); idx ++) {
		Add(new Type(*(tmp->Info)));
		tmp = tmp->Next;
	}
}

