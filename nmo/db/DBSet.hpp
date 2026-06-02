/*                                ---------
                                  [NMO-SDK]
                                  ---------

*/
#ifndef _NMO_DB_SET_H_
#define _NMO_DB_SET_H_

namespace NMO {

/*!

\brief		A set of data.

			Super class for all data sets (Dynaset, Table).
			Never used directly.


\author		(c) 1999 Henri Michelon
\version	$Id: DBSet.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/


class DBSet: public Iterator<DBRecord>
{
public:	
	/*! Initialize with a name or a query
		and a parent database

	*/
	DBSet(DBDatabase&, const Stringz&);
	virtual ~DBSet() {};

	/*! Return the name of the set or the query */
	const Stringz& Name() const;
	
	/*! Return the parent database */
	DBDatabase& Database() const;

	/*! Get a column index
		\param String	: column name
		\return 1 based index or 0 on error

	*/
	_DWORD ColumnIndex(const Stringz&);

	/*! Get a column
		\param String	: column name
		\param DBColumn: (output) column name & type
		\return 1 based index or 0 on error

	*/
	_DWORD Column(const Stringz&, DBColumn&);

	/*! Refresh the content of the set
		by requerying the database */
	virtual void Refresh() = 0;

	/*! Return the value of a column
		of the current record*/
	virtual DBValue& operator[] (_DWORD);

	/*! reset the iterator on the first element */
	virtual void Reset(_BOOL = TRUE);

	/*! return TRUE if the last element is reached */
	virtual _BOOL End() const;


	/*! return TRUE if the first element is reached */

	virtual _BOOL Start() const;

	/*! Return the current node, and step to the next node */
	virtual DBRecord& Next();


	/*! Return the current node, and step to the previous node */

	virtual DBRecord& Prev();

	/*! Return the current node */
	virtual DBRecord& Current() const;


	/*! Return the list of colums */
	Array<DBColumn>& Columns() const;


	/*! Return the list of rows */

	List<DBRecord>& Records() const;

protected:
	Stringz			name;
	DBDatabase		*database;
	ListIterator<DBRecord> lstRecords;
	List<DBRecord>	records;
	Array<DBColumn>	columns;
};


inline Array<DBColumn>& DBSet::Columns() const
{ return (Array<DBColumn>&)columns; };


inline List<DBRecord>& DBSet::Records() const
{ return (List<DBRecord>&)records; };


inline void DBSet::Reset(_BOOL F)
{ lstRecords.Reset(F); }

inline _BOOL DBSet::Start() const
{ return lstRecords.Start(); }

inline _BOOL DBSet::End() const
{ return lstRecords.End(); }

inline DBRecord& DBSet::Prev()
{ return lstRecords.Prev(); }

inline DBRecord& DBSet::Next()
{ return lstRecords.Next(); }

inline DBRecord& DBSet::Current() const
{ return lstRecords.Current(); }

}

#endif
