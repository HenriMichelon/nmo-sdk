/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_NET_STREAM_H_
#define _NMO_NET_STREAM_H_

namespace NMO {

/*!
\brief		Message based network communication procotol
			
			 Protocol independant communication protocol for
			 connected, stream based, communications

			Only TCP/IP is implemented, but you are welcome
			to implement your prefered protocol.

\author		(c) 1999-2000 Henri Michelon
\version	$Id: NetStream.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class NetStream: public NetProtocol
{
public:
	/*! Create a network Stream object
		\return NULL if error
	*/
	static NetStream* Create(NetID*);

	/*! Connect to another host
		\param NetID	: (in) ID of the host server
		\return FALSE on error
	*/
	virtual _BOOL Connect(const NetID*) = 0;

	/*! Wait for a Connect() from any host.
		returned value must be deleted after use.
		\return NULL on error
	*/		
	virtual NetID* WaitConnect() = 0;

};

}

#endif
