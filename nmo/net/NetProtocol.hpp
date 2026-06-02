/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_NET_PROTOCOL_H_
#define _NMO_NET_PROTOCOL_H_

namespace NMO {

/*!
\brief		Network communication procotol

			Super class for all network communication procotols.

\author		(c) 1999-2001 Henri Michelon
\version	$Id: NetProtocol.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class NetProtocol: public NMOObject
{
public:
	/*! Send data to an host
		\param	NetID	: (in) ID of remote host
		\param _PTR	: (in) address of data block to send
		\param _DWORD	: (in) size (in bytes) of data block
		\return the number of bytes really sent
				or 0 on error
	*/
	virtual _DWORD Send(const NetID*, const _PTR, _DWORD) = 0;

	/*! Wait for a data block from any hosts
		\param _PTR		: (in) address of data block
		\param _DWORD		: (in) size of the data block, in bytes
		\param NetID		: (in/out) ID of source host
		\param _DWORD		: (in) timeout in milliseconds,
								or 0 for infinite
		\return the number of bytes received or 0 on error
	*/
	virtual _DWORD Receive(const _PTR, _DWORD, NetID*&, _DWORD=0) = 0;
};

}

#endif
