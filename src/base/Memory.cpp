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

	(c) 1998-2001 Henri Michelon 

$Id: Memory.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#if !defined(_MSC_VER) && ! defined(__DEBUG)

void Memory::CopyDWORD(_PTR dst, const _PTR src, _DWORD lng)
{
	ASSERT(dst && src);
	if (!lng) { return; };

	_DWORD *TmpSrc = (_DWORD*)src;
	_DWORD *TmpDst = (_DWORD*)dst;
	_DWORD nbDword = lng / 4;
	_DWORD reste = lng - (nbDword * 4);

	do {
		*TmpDst = *TmpSrc;
		TmpDst++;
		TmpSrc++;
	} while (--nbDword);

	if (reste) {
		_BYTE *tmpSrc = (_BYTE*)TmpSrc;
		_BYTE *tmpDst = (_BYTE*)TmpDst;
		do {
			*tmpDst = *tmpSrc;
			tmpDst++;
			tmpSrc++;
		}
		while (--reste);
	}
}



void Memory::CopyWORD(_PTR dst, const _PTR src, _DWORD lng)
{
	ASSERT(dst && src);
	if (!lng) { return; };

	_WORD *TmpSrc = (_WORD *)src;
	_WORD *TmpDst = (_WORD *)dst;
	_DWORD nbDword = lng / 2;
	_DWORD reste = lng - (nbDword * 2);

	do {
		*TmpDst = *TmpSrc;
		TmpDst++;
		TmpSrc++;
	} while (--nbDword);

	if (reste) {
		_BYTE *tmpSrc = (_BYTE*)TmpSrc;
		_BYTE *tmpDst = (_BYTE*)TmpDst;
		do {
			*tmpDst = *tmpSrc;
			tmpDst++;
			tmpSrc++;
		} while (--reste);
	}
}

#endif // _MSC_VER

#if defined(__DEBUG) && ! defined(_MSC_VER)
 
void Memory::CopyWORD(_PTR A, const _PTR B, _DWORD C)
{ Copy(A, B, C); }
 
void Memory::CopyDWORD(_PTR A, const _PTR B, _DWORD C)
{ Copy(A, B, C); }
 
#endif
