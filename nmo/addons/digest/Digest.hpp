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
#ifndef _NMO_ADDONS_DIGEST_H_
#define _NMO_ADDONS_DIGEST_H_

namespace NMO {

/*!
\brief		Super class for all message digest algos

\author		(c) 2000-2002 Henri Michelon
\version	$Id: Digest.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Digest: public NMOObject {
public:

	/*! Compute a digest sum for an ASCIIZ string
		excluding terminal char \0
		\param Stringz		: (input) string to compute sum for
		\return an hex based string
	*/
	const Stringz String(const Stringz&);

	/*! Compute a digest sum for a block of data
		\param _PTR	: (input) address of data block
		\param _DWORD	: (input) size of data block in bytes
		\return an hex based string
	*/
	const Stringz Data(_PTR, _DWORD);

	/*! Compute a digest sum for an already opened file
		computation start at the file current position.
		\param File	: (input) file object. Must be open with READ mode
		\return an hex based string
	*/
	const Stringz File(NMO::File&);
	
private:
	virtual void Init() = 0;
	virtual void Update (const _PTR, _DWORD) = 0;
	virtual void Final (_PTR) = 0;
	const Stringz End();
};

}

#endif
