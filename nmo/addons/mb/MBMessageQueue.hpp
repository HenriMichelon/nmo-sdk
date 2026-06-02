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

				Messages broker input queue super class

				(c) 2000-2002 Henri Michelon

$Id: MBMessageQueue.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_MB_MBQUEUE_H_
#define _NMO_MB_MBQUEUE_H_

namespace NMO {

class MBMessageQueue: public NMOObject {
public:

	// Wait for a message to arrive into the message queue
	// return 0 on error or message number
	virtual _DWORD Wait() = 0;

	// Get last older message from the input queue
	//	return 0 if no message in the queue or error
	//	or message number
	virtual _DWORD Get() = 0;

	// Get data associated with the message read with Get() or Wait()
	//	_PTR	: address of data buffer
	//	_DWORD	: size in bytes of the buffer
	//	return the number of bytes read from from queue
	virtual _DWORD GetData(_PTR, _DWORD) = 0;
};

}

#endif
