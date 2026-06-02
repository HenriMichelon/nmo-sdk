/*                                ---------
                                  [NMO-SDK]
                                  ---------
	
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1999-2001 Henri Michelon

$Id: Array.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/


//--------------------------------------------------------
template <class Type>
inline Array<Type>::Array(): size(0), array(NULL), current(1)
{}


//--------------------------------------------------------
template <class Type>
inline Array<Type>::Array(_DWORD S): size(0), array(NULL), current(1)
{
	Dim(S);
}


//--------------------------------------------------------
template <class Type>
Array<Type>::~Array()
{
	Clear();
}


//--------------------------------------------------------
template <class Type>
inline _DWORD Array<Type>::Count() const
{
	return size;
}


//--------------------------------------------------------
template <class Type>
inline Type& Array<Type>::operator [](_DWORD IDX)
{
	ASSERT(IDX);
	ASSERT(IDX <= size);
	return array[IDX];
}


//--------------------------------------------------------
template <class Type>
void Array<Type>::Dim(_DWORD SIZE)
{
	ASSERT(!size);
	ASSERT(SIZE);
	Type *tmp= new Type[SIZE+1];
	if (tmp)
	{
		array = tmp;
		size = SIZE;
	}
	else
		size = 0;
}


//--------------------------------------------------------
template <class Type>
void Array<Type>::ReDim(_DWORD SIZE)
{
	ASSERT(size);
	Type *tmp = new Type[SIZE+1];
	if (!tmp) return;
	Memory::Copy(tmp, array, sizeof(Type)*size);
	delete []array;
	size = SIZE;
}


//--------------------------------------------------------
template <class Type>
void Array<Type>::Clear()
{
	if (size)
	{
		delete []array;
		array = NULL;
		size = 0;
	}
}


//--------------------------------------------------------
template <class Type>
_DWORD Array<Type>::IndexOf(const Type&VAL) const
{
	for (_DWORD i=1; i<=size; i++)
	{
		if (&VAL == &array[i])
			return i;
	}
	return 0;
}

//--------------------------------------------------------
template <class Type>
void Array<Type>::Reset(_BOOL FROM)
{
	if (FROM)
		current =  1;
	else
		current = size;
}


//--------------------------------------------------------
template <class Type>
inline _BOOL Array<Type>::Start() const
{
	return (current == 1);
}


//--------------------------------------------------------
template <class Type>
inline _BOOL Array<Type>::End() const
{
	return (current > size);
}


//--------------------------------------------------------
template <class Type>
inline Type& Array<Type>::Prev()
{
	ASSERT(!Start());
	return array[current--];
}


//--------------------------------------------------------
template <class Type>
inline Type& Array<Type>::Next()
{
	ASSERT(!End());
	return array[current++];
}


//--------------------------------------------------------
template <class Type>
inline Type& Array<Type>::Current() const
{
	ASSERT(!End());
	return array[current];
}


//--------------------------------------------------------
template <class Type>
inline Type* Array<Type>::Address() const
{
	return array;
}


//--------------------------------------------------------
template <class Type>
inline _DWORD Array<Type>::Index() const
{
	return current;
}


//--------------------------------------------------------
template <class Type>
inline const Array<Type>& Array<Type>::operator = (const Array<Type>&A)
{
	Clear();
	if (A.size)
	{
		Dim(A.size);
		for (_DWORD i=1; i<=size; i++)
			array[i] = A.array[i];
	}
	return *this;
}

