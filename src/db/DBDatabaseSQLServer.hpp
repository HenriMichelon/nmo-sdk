/*                                ---------
                                  [NMO-SDK]
                                  ---------

				Microsoft SQL Server 7 connection helper

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.


				(c) 2000 Henri Michelon

$Id: DBDatabaseSQLServer.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
----------------------------------------------------------------------------*/

#ifndef _NMO_WIN32_DBSQLSERVER_H_
#define _NMO_WIN32_DBSQLSERVER_H_

#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>

namespace NMO {

static const char SQLSERVERDRIVERNAME[] = "SQLServer";

class DBDatabaseSQLServer: public DBDatabase
{
public:

	DBDatabaseSQLServer();

	virtual ~DBDatabaseSQLServer();
	virtual _BOOL Connect(const Stringz&, const Stringz&, const Stringz& = "", const Stringz& = "");
	virtual void Disconnect();
	virtual _BOOL Query(const Stringz&);
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&);
	virtual _BOOL Query(const Stringz&, Array<DBColumn>&, List<DBRecord>&);
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&, Array<DBColumn>&, List<DBRecord>&);
	virtual Stringz LastError();
	virtual Stringz SQLType(_LONG, _DWORD = 0) const;
	virtual _BOOL Columns(const Stringz&, List<DBColumn>&);
	virtual _BOOL Tables(List<Stringz>&, List<Stringz>&);
	virtual _BOOL DriverInfos(List<Stringz>&, List<Stringz>&);

private:
	static _DWORD	instance;
    PDBPROCESS		dbproc;    // The connection with SQL Server. 
	Stringz			error;

	int err_handler(PDBPROCESS, INT, INT, INT, LPCSTR, LPCSTR);

	_LONG SQL2DB(SHORT ) const;
	_BOOL _Query(const Stringz&);
	_BOOL _Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&);
	_BOOL FetchResults(Array<DBColumn>&, List<DBRecord>&);
};

}

#endif
