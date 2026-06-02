/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_VALUE_H_
#define _NMO_DB_VALUE_H_

namespace NMO {

/*!
\brief		A typed value in a table

\author		(c) 1999-2001 Henri Michelon
\version	$Id: DBValue.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBValue: public NMOObject
{
public:
	//! boolean
	_BOOL		vBool;
	//! 8 bits unsigned integer
	_BYTE		vByte;
	//! 8 bits signed integer
	_SHORT		vShort;
	//! 16 bits signed integer
	_INT		vInt;
	//! 32 bits signed integer
	_LONG		vLong;
	//! 156 bits max integer (not really implemented !)
	Decimal		vDecimal;
	//! "simple" precision float
	_FLOAT		vFloat;
	//! "double" precision float
	_DOUBLE		vDouble;
	//! This is not a currency, and this is not implemented
	_DOUBLE		vCurrency;
	//! asciiz string
	Stringz		vString;
	//! date & time
	DateTime	vDateTime;
	//! fixed size byte buffer
	Array<_BYTE> vData;

	/*! 
		Return a formatted string used to display
		the content of the value
	*/
	Stringz Format(_LONG, const Stringz& = "");
	inline virtual ~DBValue() {};
};

}

#endif
