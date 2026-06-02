/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_RECORD_H_
#define _NMO_DB_RECORD_H_

namespace NMO {

/*!
\brief		A row in a data set

			This class describe a row in a database set.

\author		(c) 1999 Henri Michelon
\version	$Id: DBRecord.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBRecord: public Array<DBValue>
{
public:
	/*! Create a new, empty, row, 
		and allocate suficient space for columns.
		\param	_DWORD	: (input) number of columns
	*/
	DBRecord(_DWORD L): Array<DBValue>(L) {};

	/*! Create a new, empty, row. */
	DBRecord() {};
};

}
#endif
