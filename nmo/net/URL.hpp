/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_NET_URL_H_
#define _NMO_NET_URL_H_

namespace NMO {

/*!
\brief		URL manipulation routines

\author		(c) 2000-2001 Henri Michelon
\version	$Id: URL.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class URL: public NMOObject
{
public:
	/*! Split an URL in different parts.
	  	\param	Stringz	: (in) URL
		\param	Stringz	: (out) network procotol
		\param	Stringz	: (out) server DNS name or IP address
		\param	Stringz : (out) port number
		\param	Stringz	: (out) directory
		\param	Stringz	: (out) file name
		\param	Stringz	: (out) optionals parameters
		\return FALSE on incorrect URL
	*/	
	_BOOL Split(const Stringz&, Stringz&, Stringz&, Stringz&, Stringz&, 
				   Stringz&, Stringz&);
};

}

#endif
