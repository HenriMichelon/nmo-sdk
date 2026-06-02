/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_SNAP_H_
#define _NMO_DB_SNAP_H_

namespace NMO {

/*!
\brief		Snapshot: result of an SQL query represented in a set

\author		(c) 1999-2000 Henri Michelon
\version	$Id: DBSnapshot.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBSnapshot: public DBSet
{
public:
	/*! Query a database
		\param DBDatabase	: parent database
		\param String		: SQL Query (SQL92 ANSI prefered)
	*/
	static DBSnapshot* Open(DBDatabase&, const Stringz&);

	virtual ~DBSnapshot() {};
	virtual void Refresh();

private:
	DBSnapshot(DBDatabase&, const Stringz&);
};

}

#endif
