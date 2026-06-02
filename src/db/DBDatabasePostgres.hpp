/*                                ---------
                                  [NMO-SDK]
                                  ---------
 
				PostgreSQL ORDBMS connection helper

				Warning:
				Exotic data types (like geometric ones) aren't
				supported for SQL92 maximum compatibility.

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

				(c) 1999-2000 Henri Michelon
$Id: DBDatabasePostgres.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_POSTGRES_H_
#define _NMO_POSTGRES_H_

#include <libpq-fe.h>

namespace NMO {

static const char POSTGRESDRIVERNAME[] = "Postgres";

class DBDatabasePostgres: public DBDatabase
{
public:

	DBDatabasePostgres();

	virtual ~DBDatabasePostgres();

	// Connect to the database
	//	String	: server name
	//	String	: database name
	//	String	: user name
	//	String	: password
	//	return FALSE on connection error
	virtual _BOOL Connect(const Stringz&, const Stringz&, const Stringz& = "", const Stringz& = "");

	// Disconnect from the database
	virtual void Disconnect();

	// Execute a SQL Query
	//	String	: SQL Query (SQL92 ANSI prefered)
	//	return FALSE on execution error
	virtual _BOOL Query(const Stringz&);

	// Execute a SQL Query
	//	String	: SQL Query (SQL92 ANSI prefered)
	//	Iterator<_LONG> : parameters types
	//	Iterator<DBValue> : parameters values
	//	return FALSE on execution error
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&);

	// Execute a SQL Query & fetch results
	//	String	: SQL Query (SQL92 ANSI prefered)
	//	Array<DBColumn>	: (output) columns names & types
	//	List<DBRecord>	: (output) result of the query
	//	return FALSE on execution error
	virtual _BOOL Query(const Stringz&, Array<DBColumn>&, List<DBRecord>&);

	// Execute a SQL Query & fetch results
	//	String	: SQL Query (SQL92 ANSI prefered)
	//	Iterator<_LONG> : parameters types
	//	Iterator<DBValue> : parameters values
	//	Array<DBColumn>	: (output) columns names & types
	//	List<DBRecord>	: (output) result of the query
	//	return FALSE on execution error
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&, Array<DBColumn>&, List<DBRecord>&);

	// Get the message of the last error
	virtual Stringz LastError();
	
	// Convert a DBType in a SQL Type name
	//	_LONG	: DBType to convert
	//	_DWORD	: length of the type
	virtual Stringz SQLType(_LONG, _DWORD = 0) const;

	// Get columns names and types of a table
	//	String	: table name
	//	List	: (output) columns description
	virtual _BOOL Columns(const Stringz&, List<DBColumn>&);

	// Get the names of all the database tables
	//	List	: (output) tables names
	virtual _BOOL Tables(List<Stringz>&, List<Stringz>&);

	// Get information about the driver
	//	String	: (output) information name
	//	String	: (output) values
	//	note: values.Count >= names.Count()
	virtual _BOOL DriverInfos(List<Stringz>&, List<Stringz>&);

private:
	PGconn	*pgconn;

	_LONG PQType2DBType(Oid) const;
	void FormatQuery(Stringz&, Iterator<_LONG>&, Iterator<DBValue>&) const;
	void SetValue(DBValue&, const DBColumn&, const char*) const;
};

}

#endif
