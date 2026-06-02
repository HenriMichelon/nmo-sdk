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

	(c) 1998-2001 Henri Michelon

$Id: IRect.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/interface/IRect.hpp>


//----------------------------------------------------------------------------
void IRect :: Intersection(const IRect&A, const IRect&B)
{
	left = MAX(A.left, B.left);
	top = MAX(A.top, B.top);
	width = MIN(A.left + A.width, B.left + B.width) - left;
	height = MIN(A.top + A.height, B.top + B.height) - top;
}


//----------------------------------------------------------------------------
const IRect& IRect :: operator = (const IRect&R)
{
	left = R.left;
	top = R.top;
	width = R.width;
	height = R.height;
	return *this;
}


//----------------------------------------------------------------------------
_BOOL IRect :: operator == (const IRect&R) const
{
	return ((left == R.left) &&
			(top == R.top) &&
            (width == R.width) &&
            (height == R.height));
}


//----------------------------------------------------------------------------
_BOOL IRect :: Contains(_LONG X, _LONG Y) const
{
	return ((X >= left) && (X < _LONG((left + width))) &&
			(Y >= top) && (Y < _LONG((top + height))));
}


//----------------------------------------------------------------------------
_BOOL IRect :: Contains(const IRect&R) const
{
	return ((R.left >= left) && (R.top >= top) &&
			((R.left + R.width) <= (left + width)) &&
			((R.top + R.height) <= (top + height)));
}


