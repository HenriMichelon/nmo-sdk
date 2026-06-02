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

				Messages broker output port super class

				(c) 2000-2002 Henri Michelon

$Id: MBMessagePort.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_MB_MBPORT_H_
#define _NMO_MB_MBPORT_H_

namespace NMO {

class MBMessagePort: public NMOObject {
public:

	// Send a message into the message port
	//	_DWORD 	: message number
	//	_PTR	: output buffer address
	//	_DWORD	: size in bytes of the output buffer address
	//	return FALSE on error
	virtual _BOOL Send(_DWORD, _PTR = NULL, _DWORD = 0) = 0;
};

}

#endif
