/*                                ---------
                                  [NMO-SDK]
                                  ---------
		ODBC connection helper

		Warning: 
		+ The SQLTables function is not fully
		supported by all ODBC drivers.  In this case,
		the Tables method can return nothing or only
		the table names, without types.
		+ Commits and rollbacks are not fully supported
		by all ODBC drivers.

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.


		(c) 1999-2000 Henri Michelon

$Id: DBDatabaseODBC.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------------*/

#ifndef _NMO_WIN32_ODBC_H_
#define _NMO_WIN32_ODBC_H_

#include <sql.h>
#include "modules/ModODBC.hpp"

namespace NMO {

static const char ODBCDRIVERNAME[] = "ODBC";

class DBDatabaseODBC: public DBDatabase
{
public:
	virtual ~DBDatabaseODBC();
	virtual _BOOL Connect(const Stringz&, const Stringz&, const Stringz& = "", const Stringz& = "");
	virtual void Disconnect();
	virtual _BOOL Query(const Stringz&);
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&);
	virtual _BOOL Query(const Stringz&, Array<DBColumn>&, List<DBRecord>&);
	virtual _BOOL Query(const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&, Array<DBColumn>&, List<DBRecord>&);
	virtual Stringz LastError();
	virtual _BOOL Columns(const Stringz&, List<DBColumn>&);
	virtual _BOOL Tables(List<Stringz>&, List<Stringz>&);
	virtual _BOOL DriverInfos(List<Stringz>&, List<Stringz>&);
	virtual Stringz SQLType(_LONG, _DWORD) const;

private:
	static ModODBC	*odbc;
	static _DWORD	instance;
	static HENV		henv;
	HDBC			hdbc;
	Stringz			error;

	void Error(HSTMT);
	_LONG SQL2DB(SWORD) const;
	_BOOL _Query(HSTMT, const Stringz&);
	_BOOL _Query(HSTMT, const Stringz&, Iterator<_LONG>&, Iterator<DBValue>&);
	_BOOL FetchResults(HSTMT, Array<DBColumn>&, List<DBRecord>&);
	void Bind(HSTMT hstmt, _WORD N, _LONG TYPE, _PTR&, _DWORD LEN, List<SDWORD>&OUTLEN);
};

}

#endif
