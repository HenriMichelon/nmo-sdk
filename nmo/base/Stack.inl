/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.sourceforge.net/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

				(c) 1999-2001 Henri Michelon 
  
$Id: Stack.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/


//---------------------------------
template <class Type>
inline void Stack<Type>::Push(Type &T)
{
	Add(T);
}


//---------------------------------
template <class Type>
inline _BOOL Stack<Type>::Empty()
{
	return (Count() == 0);
}
