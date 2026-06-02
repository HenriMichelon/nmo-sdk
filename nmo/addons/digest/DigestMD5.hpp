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
#ifndef _NMO_ADDONS_DIGEST_MD5_H_
#define _NMO_ADDONS_DIGEST_MD5_H_

namespace NMO {

/*!
\brief		RSA MD5 message digest

\author		(c) 2000-2002 Henri Michelon
\version	$Id: DigestMD5.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DigestMD5: public Digest {
private:	
	_DWORD	state[4];	/* state (ABCD) */
   	_DWORD	count[2];	/* number of bits, modulo 2^64 (lsb first) */
    _BYTE	buffer[64];	/* input buffer */
	static 	_BYTE PADDING[64];
	
	void Init();
	void Update (const _PTR, _DWORD);
	void Final (_PTR);

	void Pad ();
	void Transform (_DWORD [4], const _BYTE[64]);

	void Encode (_BYTE*, _DWORD*, _DWORD);
	void Decode (_DWORD*, _BYTE*, _DWORD);
};

}

#endif
