/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_COLUMN_H_
#define _NMO_DB_COLUMN_H_

namespace NMO {

/*!
\brief		A column of a table: name & data type

  			This class describe a column of a table.
		   	Each column have a name and type.
			Some (like DBType::STRING or DBType::DATA columns)
			have a length in bytes.

\author		(c) 1999-2001 Henri Michelon
\version 	$Id: DBColumn.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBColumn: public NMOObject
{
public:
	//! Name of the column (unique for the table)
	Stringz	name;
	//! Type of data for this column (cf DBType)
	_LONG	type;
	//! Length of the data, in bytes
	_DWORD	len;

	/*! Create a new column
	  \param String		Name
	  \param _LONG		Type (cf DBType)
	  \param _DWORD		Length
	*/  
	DBColumn(Stringz N="", _LONG T=DBType::NONE, _DWORD S=0):
		name(N), type(T), len(S) {};
		
	inline virtual ~DBColumn() {};

	/*! Compare two columns by their name */
	friend _BOOL operator == (const DBColumn&, const DBColumn&);
};

inline _BOOL operator == (const DBColumn&A, const DBColumn&B)
{ return (A.name == B.name); };

}

#endif
