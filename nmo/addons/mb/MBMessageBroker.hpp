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

				Messages broker shared object super class

				(c) 2000-2002 Henri Michelon

$Id: MBMessageBroker.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_MB_MB_H_
#define _NMO_MB_MB_H_

namespace NMO {

class MBMessageBroker: public NMOObject {
public:

	MBMessageBroker();

	// Register a new module
	virtual _BOOL Register(const Stringz&) = 0;

	// Open a communication channel
	//	Stringz:	connection string
	//				usually "module:computer:user:passwd"
	//	return FALSE on error
	virtual _BOOL OpenCommunication(const Stringz&) = 0;

	// Close the currently opened communication channel
	virtual void CloseCommunication() = 0;

	// Open an output comunication port
	// return NULL on error
   	virtual MBMessagePort* OpenPort() = 0;

	// Open an input communication queue
	// return NULL on error
	virtual MBMessageQueue* OpenQueue() = 0;

	// return TRUE if the communicatino channel is open
	_BOOL IsOpen();

private:
	_BOOL open;
};

inline MBMessageBroker::MBMessageBroker(): open(FALSE) {};
inline MBMessageBroker::IsOpen() { return open; };

}

#endif
