/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_NET_ID_H_
#define _NMO_NET_ID_H_

namespace NMO {

/*!
\brief		Host network identifier.

			Protocol independant network indentifier.

\author		(c) 1999-2002 Henri Michelon
\version	$Id: NetID.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class NetID: public NMOObject
{
public:
	FileCfg	cfg;

	virtual ~NetID();

	/*! Create an ID for a host
		\param FileCfg	: (in) configurations file for the protocol and host
	*/
	static NetID* Create(FileCfg&);

	/*! Create an ID for a host
		\param Stringz	: (in) driver name
		\param FileCfg	: (in) configurations file for the protocol and host
	*/
	static NetID* Create(const Stringz&, FileCfg&);

	/*! Return the host name */
	virtual Stringz Name() const = 0;

	/*! Return the formatted host address */
	virtual Stringz Address() const  = 0;

	/*! Return the driver name */
	const Stringz& Driver();

	/*! Return the last error for
		the corresponding driver
	*/
	virtual Stringz LastError() const = 0;

	friend _BOOL operator == (NetID&, NetID&);
	friend _BOOL operator != (NetID&, NetID&);

private:
	_DWORD					driver;
	static _DWORD			nInst;
	static List<Stringz>	drivers;
};

inline _BOOL operator == (NetID&A, NetID&B) { return (A.Address() == B.Address()); }
inline _BOOL operator != (NetID&A, NetID&B) { return (!(A==B)); };


}

#endif
