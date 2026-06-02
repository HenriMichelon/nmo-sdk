/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_DYNASET_H_
#define _NMO_DB_DYNASET_H_

namespace NMO {

/*!
\brief		Dynamic data set

			A dynamic data set is a partial view of a table
			which can be modified (edit/remove rows).

\author		(c) 1999 Henri Michelon
\version	$Id: DBDynaset.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBDynaset: public DBSet
{
public:
	/*! Create a new set.
		\param	DBDatabase	: (input) source database
		\param	Stringz		: (input) table name
	*/
	DBDynaset(DBDatabase&, const Stringz&);

	/*! Open a part of a table
		\param DBDatabase	parent database
		\param Stringz		name of the table
		\param Collection<DBColumn> columns for columns selection
		\param Collection<DBValue> values for rows selection
	*/	
	static DBDynaset *Create(DBDatabase&, const Stringz&,
							 Collection<DBColumn>&, Collection<DBValue>&);
	
	/*!
	  Refresh the content of the set
	  The current content of the set is replaced.
	*/  
	virtual void Refresh();

	/*!
	  Delete current record
	  There is no way to restore it.
	*/  
	virtual void Delete();

	/*!
	  Enter edit mode for current record
	  You must call this method before doing an Update()
	*/  
	virtual void Edit();

	/*! 
	  Commit modifications mades after Edit()
	  to the current row
	*/ 
	virtual void Update();

#ifdef __DEBUG
	virtual void Reset();
	virtual DBRecord& Next();
#endif

private:
	_BOOL			edit;
	DBRecord		edited;

};

}
#endif
