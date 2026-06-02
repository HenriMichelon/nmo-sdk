/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_TYPE_H_
#define _NMO_DB_TYPE_H_

namespace NMO {

/*!

\brief		Types returned by queries



			Not all SQL92 types are represented

			to match a maximum number of RDBMS types.



\author		(c) 1999-2000 Henri Michelon

\version	$Id: DBType.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/

//--------------------------------------------------------------------------



class DBType: public NMOObject
{
public:
	enum
	{
		NONE,
		//! 1 bit boolean
		BOOL,
		//! 8 bits unsigned
		BYTE,
		//! 8 bits signed
		SHORT,
		//! 16 bits signed
		INT,
		//! 32 bits signed
		LONG,
		//! 256 bits max signed (not implemented)
		DECIMAL,
		//! 4 bytes float
		FLOAT,
		//! 8 bytes float
		DOUBLE,
		//! zero terminated string
		STRING,
		//! Date and time
		DATETIME,
		//! date only
		DATE,
		//! time only
		TIME,
		//! array of byte
		DATA
	} Type;


	/*!
		Return a string representation of the type
		('BYTE', 'BOOL', ...)
	*/
	static Stringz Format(_LONG, _DWORD = 0);

	/*! 
		Return the approxymative maximum number of character
		returned by DBValue::Format()
	*/
	static _DWORD DisplayLen(_LONG, _DWORD = 0);
};

}

#endif
