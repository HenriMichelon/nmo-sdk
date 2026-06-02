/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DB_TABLE_H_
#define _NMO_DB_TABLE_H_

namespace NMO {

/*!
\brief		a table in a database

			A table is a set of data you can modify
			(add, remove, edit).

\author		(c) 1999-2000 Henri Michelon
\version	$Id: DBTable.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DBTable: public DBSet
{
public:
	
	/*! On a existing table in a database
		\param DBDatabase	: parent database
		\param String		: name of the table
		\param String		: name of the field for the ORDER BY 
							(optional)
		\return NULL on error
	*/
	static DBTable *Open(DBDatabase&, const Stringz&, const Stringz& = "");

	/*! Create a new table in a database
		\param DBDatabase	: parent database
		\param String		: name of the table
		\param Collection<DBColumn> : columns description
		\param _DWORD		: primary key index (0 for no pk)
	*/
	static DBTable *Create(DBDatabase&, const Stringz&,
						   Collection<DBColumn>&, _DWORD=0);

	virtual ~DBTable() {};
	
	virtual void Refresh();

	/*! Add a new record in the table */
	virtual void Add();
	
	/*! Delete current record */
	virtual void Delete();

	/*! Enter edit mode for current record */
	virtual void Edit();

	/*! Confirm an Add(), or an Edit() */
	virtual void Update();

	/*! Add a column on the table.
		throw a DBException on error
		\param String	: column name
		\param DBType	: column data type
	*/
	void AddColumn(const Stringz&, DBType);

	/*! Drop table from the database */
	void Drop();

#ifdef __DEBUG
	virtual void Reset();
	virtual DBRecord& Next();
#endif

private:
	_BOOL			add;
	_BOOL			edit;
	DBRecord		edited;

	DBTable(DBDatabase&, const Stringz&);
};

}

#endif
