/*                                ---------
                                  [NMO-SDK]
                                  ---------
 
				MySQL RDBMS connection helper

				Warning:
				Table names are CASE SENSITIVES

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

				(c) 1999-2000 Henri Michelon

$Id: DBDatabaseMySQL.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_MYSQL_H_
#define _NMO_MYSQL_H_

#include <mysql/mysql.h>
#include "modules/ModMySQL.hpp"

namespace NMO {

static const char MYSQLDRIVERNAME[] = "MySQL";

class DBDatabaseMySQL: public DBDatabase
{
public:

	DBDatabaseMySQL();

	virtual ~DBDatabaseMySQL();

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
	//	IList<DBRecord>	: (output) result of the query
	//	return FALSE on execution error
	virtual _BOOL Query(const Stringz&, Array<DBColumn>&, List<DBRecord>&);

	// Execute a SQL Query & fetch results
	//	String	: SQL Query (SQL92 ANSI prefered)
	//	Iterator<_LONG> : parameters types
	//	Iterator<DBValue> : parameters values
	//	Array<DBColumn>	: (output) columns names & types
	//	IList<DBRecord>	: (output) result of the query
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
	//	IList	: (output) columns description
	virtual _BOOL Columns(const Stringz&, List<DBColumn>&);

	// Get the names of all the database tables
	//	IList	: (output) tables names
	virtual _BOOL Tables(List<Stringz>&, List<Stringz>&);

	// Get information about the driver
	//	String	: (output) information name
	//	String	: (output) values
	//	note: values.Count >= names.Count()
	virtual _BOOL DriverInfos(List<Stringz>&, List<Stringz>&);

private:
	MYSQL			mysql;
	static ModMySQL	*module;
	static _DWORD	nInst;

	_LONG MySQL2DBType(enum_field_types) const;
	_LONG MySQL2DBType(Stringz&) const;
	void Convert(enum_field_types, _BYTE *, _DWORD, DBValue&) const;
	void FormatQuery(Stringz&, Iterator<_LONG>&, Iterator<DBValue>&) const;
	DateTime::Date ConvertDate(const Stringz&) const;
	DateTime::Time ConvertTime(const Stringz&) const;
};

}

#endif
