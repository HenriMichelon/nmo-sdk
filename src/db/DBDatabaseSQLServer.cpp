/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1999-2001 Henri Michelon

$Id: DBDatabaseSQLServer.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/config.h>
#if defined(WIN32) || defined(_WIN32)
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "DBDatabaseSQLServer.hpp"
using namespace NMO;

_DWORD DBDatabaseSQLServer::instance = 0;

//=========================================================
DBDatabaseSQLServer::DBDatabaseSQLServer(): DBDatabase()
{
	if (!instance)
	    dbinit ();
	instance++;
    //dberrhandle(DBERRHANDLE_PROC (err_handler));
}


//=========================================================
DBDatabaseSQLServer::~DBDatabaseSQLServer()
{
	Disconnect();
	instance--;
	if (!instance)
		dbexit();
}


//=========================================================
_BOOL DBDatabaseSQLServer::Connect(const Stringz&S, const Stringz&B, const Stringz&U, const Stringz&P)
{
	ASSERT(!connected);
    PLOGINREC		login;     // The login information. 

    // Get a LOGINREC.
	base = B;
	user = U;
	passwd = P;
	base.Trim();
	user.Trim();
	passwd.Trim();
    login = dblogin ();
	if (!login) return FALSE;
	if (user.Len())
		DBSETLUSER (login, user);
	if (passwd.Len())
		DBSETLPWD (login, passwd);
	DBSETLAPP (login, "NMO Rocks!");

    // Get a DBPROCESS structure for communication with SQL Server. 
	Stringz server = S;
	server.Trim();
	if (server.Len())
		dbproc = dbopen (login, server);
	else
		dbproc = dbopen (login, NULL);
	if (!dbproc) return FALSE;

	if (dbuse(dbproc, base) == FAIL) return FALSE;

	connected = TRUE;
	return TRUE;
}


//=========================================================
void DBDatabaseSQLServer::Disconnect()
{
	if (connected)
		dbclose(dbproc);
}


//=========================================================
_BOOL DBDatabaseSQLServer::_Query(const Stringz&SQL)
{
	ASSERT(connected);
	dbcmd(dbproc, (_CHAR*)SQL);
    dbsqlexec(dbproc);
    if (dbresults (dbproc) != SUCCEED)
    {
		dprintf("SQLServer::sqlexec\n");
		return FALSE;
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseSQLServer::_Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	ASSERT(connected);
	Stringz sql;
	VALUES.Reset();
	TYPES.Reset();
	for (_DWORD i=0; i<SQL.Len(); i++)
	{
		if (SQL[i] == '?')
		{
			if (VALUES.End() || TYPES.End())
			{
				dprintf("SQLServer:: too few values\n");
				return FALSE;
			}
			sql += VALUES.Next().Format(TYPES.Next());
		}
		else
			sql += SQL[i];
		i++;
	}
	dbcmd(dbproc, (_CHAR*)sql);
    dbsqlexec(dbproc);
    if (dbresults (dbproc) != SUCCEED)
    {
		dprintf("SQLServer::sqlexec\n");
		return FALSE;
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseSQLServer::Query(const Stringz&SQL)
{
	return _Query(SQL);
}


//=========================================================
_BOOL DBDatabaseSQLServer::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	;
	return _Query(SQL, TYPES, VALUES);
}


//=========================================================
_BOOL DBDatabaseSQLServer::Query(const Stringz&SQL, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	ASSERT(connected);
	if (_Query(SQL))
		return FetchResults(COLS, RECORDS);
	return FALSE;
}


//=========================================================
_BOOL DBDatabaseSQLServer::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	if (_Query(SQL, TYPES, VALUES))
		return FetchResults(COLS, RECORDS);
	return FALSE;
}


//=========================================================
Stringz DBDatabaseSQLServer::LastError()
{
	return error;		     
}


//=========================================================
_LONG DBDatabaseSQLServer::SQL2DB(SHORT T) const
{
	switch (T)
	{
	case SQLBIT:
		return DBType::BOOL;
	case SQLBINARY:
	case SQLVARBINARY:
	case SQLIMAGE:
		return DBType::DATA;
	case SQLINT1:
		return DBType::BYTE;
	case SQLCHAR:
	case SQLVARCHAR:
	case SQLTEXT:
		return DBType::STRING;
	case SQLDATETIM4:
	case SQLDATETIMN:
	case SQLDATETIME:
		return DBType::DATETIME;
	case SQLFLT4:
		return DBType::FLOAT;
	case SQLFLT8:
	case SQLFLTN:
		return DBType::DOUBLE;
	case SQLINTN:
	case SQLINT4:
		return DBType::LONG;
	case SQLINT2:
		return DBType::INT;
	case SQLMONEY4:
	case SQLMONEY:
	case SQLMONEYN:
		return DBType::CURRENCY;
	case SQLNUMERIC:
	case SQLDECIMAL:
		return DBType::DECIMAL;
	default:
		dprintf("Unknow SQLType\n");
		ASSERT(FALSE);
	}
	return DBType::DATA;
}


//=========================================================
_BOOL DBDatabaseSQLServer::FetchResults(Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	_DWORD ncols = dbnumcols(dbproc);
	if (ncols == 0) return TRUE;

	DBCOL dbcol;
	DBRecord record(ncols);
	COLS.Clear();
	COLS.Dim(ncols);
	Array<_PTR> ptrs(ncols);
	for (_DWORD i=1; i<=ncols; i++)
	{
		dbcolinfo(dbproc, CI_REGULAR, i, 0, &dbcol);
		COLS[i].name = dbcol.Name;
		COLS[i].type = SQL2DB(dbcol.Type);
		COLS[i].len = dbcol.MaxLength;		
		switch (dbcol.Type)
		{
		case SQLBIT:
			ptrs[i] = new DBBIT; 
			dbbind(dbproc, i, BITBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLBINARY:
			ptrs[i] = new DBBINARY; 
			dbbind(dbproc, i, BINARYBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLVARBINARY:
		case SQLIMAGE:
			ptrs[i] = new DBVARYBIN; 
			dbbind(dbproc, i, VARYBINBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLCHAR:
			ptrs[i] = new DBCHAR; 
			dbbind(dbproc, i, CHARBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLVARCHAR:
		case SQLTEXT:
			ptrs[i] = new DBVARYCHAR; 
			dbbind(dbproc, i, NTBSTRINGBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLDATETIM4:
			ptrs[i] = new DBDATETIM4; 
			dbbind(dbproc, i, SMALLDATETIBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLDATETIMN:
		case SQLDATETIME:
			ptrs[i] = new DBDATETIME; 
			dbbind(dbproc, i, DATETIMEBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLFLT4:
			ptrs[i] = new DBFLT4; 
			dbbind(dbproc, i, FLT4BIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLFLT8:
		case SQLFLTN:
			ptrs[i] = new DBFLT8; 
			dbbind(dbproc, i, FLT8BIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLINT1:
			ptrs[i] = new DBTINYINT; 
			dbbind(dbproc, i, BINARYBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLINTN:
		case SQLINT4:
			ptrs[i] = new DBINT; 
			dbbind(dbproc, i, INTBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLINT2:
			ptrs[i] = new DBSMALLINT; 
			dbbind(dbproc, i, SMALLBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLMONEY4:
			ptrs[i] = new DBMONEY4; 
			dbbind(dbproc, i, SMALLMONEYBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLMONEY:
		case SQLMONEYN:
			ptrs[i] = new DBMONEY; 
			dbbind(dbproc, i, MONEYBIND, 0, (LPBYTE)ptrs[i]);
			break;
		case SQLNUMERIC:
		case SQLDECIMAL:
			ptrs[i] = new DBNUMERIC; 
			dbbind(dbproc, i, NUMERICBIND, 0, (LPBYTE)ptrs[i]);
			break;
		}

	}

	while (dbnextrow(dbproc) != NO_MORE_ROWS)
	{
		DBRecord *rec = new DBRecord(ncols);
		RECORDS.Add(rec);
		for (i = 1; i <= ncols; i++)
		{
			switch(dbcoltype(dbproc, i))
			{
			case SQLBIT:
				(*rec)[i].vBool = ((DBBOOL)ptrs[i]);
				break;
			case SQLBINARY:
				(*rec)[i].vData.Dim(dbcollen(dbproc, i));
				(*rec)[i].vData[1] = ((DBBINARY)ptrs[i]);
				break;
			case SQLVARBINARY:
			case SQLIMAGE:
				(*rec)[i].vData.Dim(dbcollen(dbproc, i));
				MemCopy((*rec)[i].vData.Address(), 
						((DBVARYBIN*)ptrs[i])->array,
						((DBVARYBIN*)ptrs[i])->len);
				break;
			case SQLINT1:
				break;
			case SQLCHAR:
				break;
			case SQLVARCHAR:
			case SQLTEXT:
				break;
			case SQLDATETIM4:
				break;
			case SQLDATETIMN:
			case SQLDATETIME:
				break;
			case SQLFLT4:
				break;
			case SQLFLT8:
			case SQLFLTN:
				break;
			case SQLINTN:
			case SQLINT4:
				break;
			case SQLINT2:
				break;
			case SQLMONEY4:
				break;
			case SQLMONEY:
			case SQLMONEYN:
				break;
			case SQLNUMERIC:
			case SQLDECIMAL:
				break;
			}
		}
    }
	return TRUE;
}


//=========================================================
Stringz DBDatabaseSQLServer::SQLType(_LONG TYPE, _DWORD LEN) const
{
	Stringz res;
	switch (TYPE)
	{
	case DBType::BYTE:
		res = "TINYINT"	;
		break;
	case DBType::BOOL:
		res = "BIT"	;	
		break;
	case DBType::INT:
		res = "SMALLINT";
		break;
	case DBType::LONG:
		res = "INTEGER";
		break;
	case DBType::FLOAT:
		res = "FLOAT";
		break;
	case DBType::DOUBLE:
		res = "DOUBLE";
		break;
	case DBType::STRING:
		res.Format("CHAR(%d)", LEN);
		break;
	case DBType::DATETIME:
		res = "DATETIME";
		break;
	case DBType::DATA:		
		res.Format("BINARY(%d)", LEN);
		break;
	default:
		dprintf("Unknow DBType\n");
		ASSERT(FALSE);
	}
	return Stringz(res);
}


//=========================================================
_BOOL DBDatabaseSQLServer::Tables(List<Stringz>&NAMES, List<Stringz>&TYPES)
{
/*	ASSERT(connected);
	HSTMT hstmt;
	RETCODE retcode;

	if (SQLAllocStmt(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	
	if (SQLTables(hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0) == SQL_ERROR)
	{
		dprintf("ODBC::SQLTables\n");
		Error(hstmt);
		return FALSE;
	}

	_CHAR name[128];
	SDWORD namelen = 128;
	_CHAR type[128];
	SDWORD typelen = 128;
	SQLBindCol(hstmt, 3, SQL_C_CHAR, name, namelen, &namelen);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, type, typelen, &typelen);
	while (TRUE)
	{
		retcode = SQLFetch(hstmt);
		if (retcode == SQL_NO_DATA_FOUND) break;
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			dprintf("ODBC::SQLFetch\n");
			Error(hstmt);
			return FALSE;
		}
		NAMES.Add(new Stringz(name));
		TYPES.Add(new Stringz(type));
	}

	SQLFreeStmt(hstmt, SQL_DROP);*/
	return TRUE;
}



//=========================================================
_BOOL DBDatabaseSQLServer::DriverInfos(List<Stringz>&NAMES, List<Stringz>&VALUES)
{
	NAMES.Add(new Stringz("ID"));
	VALUES.Add(new Stringz("SQLServer"));
	NAMES.Add(new Stringz("Version"));
	VALUES.Add(new Stringz("0000"));
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseSQLServer::Columns(const Stringz&NAME, List<DBColumn>&COLS)
{
/*	ASSERT(connected);
	HSTMT hstmt;
	RETCODE retcode;

	if (SQLAllocStmt(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	
	if (SQLColumns(hstmt, NULL, 0, NULL, 0, (UCHAR*)(_CHAR*)NAME, SQL_NTS, NULL, 0) == SQL_ERROR)
	{
		dprintf("ODBC::SQLColumns\n");
		Error(hstmt);
		return FALSE;
	}

	_CHAR name[128];
	SDWORD lenname = 128;
	SWORD type;
	SDWORD lentype = sizeof(type);
	SDWORD len;
	SDWORD lenlen = sizeof(len);

	SQLBindCol(hstmt, 4, SQL_C_CHAR, name, lenname, &lenname);
	SQLBindCol(hstmt, 5, SQL_C_SSHORT, &type, lentype, &lentype);
	SQLBindCol(hstmt, 8, SQL_C_SLONG, &len, lenlen, &lenlen);
	while (TRUE)
	{
		retcode = SQLFetch(hstmt);
		if (retcode == SQL_NO_DATA_FOUND) break;
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			dprintf("ODBC::SQLFetch\n");
			Error(hstmt);
			return FALSE;
		}
		COLS.Add(new DBColumn(name, SQL2DB(type), len));
	}
	SQLFreeStmt(hstmt, SQL_DROP);*/
	return TRUE;
}



//=========================================================
int DBDatabaseSQLServer::err_handler (PDBPROCESS dbproc, INT severity,
					INT dberr, INT oserr, LPCSTR dberrstr, LPCSTR oserrstr)
{
    error.Format("DB-Library Error %i: %s\n", dberr, dberrstr);
    if (oserr != DBNOERR)
    {
        error.Format("%s. Operating System Error %i: %s\n", (_CHAR*)error,
					oserr, oserrstr);
    }
    return (INT_CANCEL);

}


#endif // _WIN32
