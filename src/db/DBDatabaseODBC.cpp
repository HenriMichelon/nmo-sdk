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

$Id: DBDatabaseODBC.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/DB.hpp>
# include <windows.h>
#endif

#include <sqlext.h>
#include <odbcinst.h>
#include "DBDatabaseODBC.hpp"
#include "modules/ModODBCCP.hpp"

#define DBODBCVERSION "0001"

ModODBC* DBDatabaseODBC::odbc = NULL;
_DWORD DBDatabaseODBC::instance = 0;
HENV DBDatabaseODBC::henv = SQL_NULL_HENV;

#define SQLBIND(n, v, l, tc, ts) (*odbc->SQLBindParameter)(hstmt, n, SQL_PARAM_INPUT, tc, ts, l, 0, v, 0, NULL);


//=========================================================
DBDatabaseODBC::~DBDatabaseODBC()
{
	Disconnect();
}


//=========================================================
_BOOL DBDatabaseODBC::Connect(const Stringz&, const Stringz&B, const Stringz&U, const Stringz&P)
{
	ASSERT(!connected);

	if (!instance++)
	{
		odbc = ModODBC::Create();
		if (!odbc) return FALSE;
		(*odbc->SQLAllocEnv)(&henv);
	}

	RETCODE retcode;

	base = B;
	user = U;
	passwd = P;

	if ((*odbc->SQLAllocConnect)(henv, &hdbc) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocConnect\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}

	(*odbc->SQLSetConnectOption)(hdbc, SQL_LOGIN_TIMEOUT, 5);

	if (user == (_CHAR*)"")
		retcode = (*odbc->SQLConnect)(hdbc,
									(UCHAR*)(_CHAR*)base, SQL_NTS, 
									NULL, SQL_NULL_DATA,
									NULL, SQL_NULL_DATA);
	else if (passwd == (_CHAR*)"")
		retcode = (*odbc->SQLConnect)(hdbc,
									(UCHAR*)(_CHAR*)base, SQL_NTS, 
									(UCHAR*)(_CHAR*)user, SQL_NTS,
									NULL, SQL_NULL_DATA);
	else
		retcode = (*odbc->SQLConnect)(hdbc,
									(UCHAR*)(_CHAR*)base, SQL_NTS, 
									(UCHAR*)(_CHAR*)user, SQL_NTS,
									(UCHAR*)(_CHAR*)passwd, SQL_NTS);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		dprintf("ODBC::SQLConnect\n");
		Error(SQL_NULL_HSTMT);
		(*odbc->SQLFreeConnect)(hdbc);
		return FALSE;
	}
	connected = TRUE;
	return TRUE;
}


//=========================================================
void DBDatabaseODBC::Disconnect()
{
	if (connected)
	{
		(*odbc->SQLDisconnect)(hdbc);
		(*odbc->SQLFreeConnect)(hdbc);
	}
	if (!--instance)
	{
		(*odbc->SQLFreeEnv)(henv);
		if (odbc) delete odbc;
	}
}


//=========================================================
_BOOL DBDatabaseODBC::_Query(HSTMT hstmt, const Stringz&SQL)
{
	RETCODE retcode = (*odbc->SQLExecDirect)(hstmt, (UCHAR*)(_CHAR*)SQL, SQL_NTS);
	if (retcode == SQL_ERROR)
	{
		dprintf("ODBC::SQLExecDirect\n");
		Error(hstmt);
		return FALSE;
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseODBC::_Query(HSTMT hstmt, const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	if ((*odbc->SQLPrepare)(hstmt, (UCHAR*)(_CHAR*)SQL, SQL_NTS) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLPrepare\n");
		Error(hstmt);
		return FALSE;
	}

	_WORD idx = 0;
	VALUES.Reset();
	TYPES.Reset();
	List<_PTR> vals;
	while ((!VALUES.End()) && (!TYPES.End()))
	{
		idx++;
		DBValue &VAL = VALUES.Next();
		switch (TYPES.Next())
		{
		case DBType::BYTE:
			SQLBIND(idx, &VAL.vByte, 0, SQL_C_UTINYINT, SQL_CHAR);
			break;
		case DBType::BOOL:
			SQLBIND(idx, &VAL.vBool, 0, SQL_C_BIT, SQL_BIT);
			break;
		case DBType::SHORT:
			SQLBIND(idx, &VAL.vShort, 0, SQL_C_STINYINT, SQL_TINYINT);
			break;
		case DBType::INT:
			SQLBIND(idx, &VAL.vInt, 0, SQL_C_SSHORT, SQL_SMALLINT);
			break;
		case DBType::LONG:
			SQLBIND(idx, &VAL.vLong, 0, SQL_C_SLONG, SQL_INTEGER);
			break;
		case DBType::DECIMAL:
			dprintf("Unsupported DBType\n");
			ASSERT(FALSE);
			break;
		case DBType::FLOAT:
			SQLBIND(idx, &VAL.vFloat, 0, SQL_C_FLOAT, SQL_REAL);
			break;
		case DBType::DOUBLE:
			SQLBIND(idx, &VAL.vDouble, 0, SQL_C_DOUBLE, SQL_DOUBLE);
			break;
		case DBType::STRING:
			SQLBIND(idx, (_CHAR*)VAL.vString, VAL.vString.Len(), SQL_C_CHAR, SQL_LONGVARCHAR);
			break;
		case DBType::DATETIME:
			{
				TIMESTAMP_STRUCT *time = new TIMESTAMP_STRUCT;
				vals.Add((_PTR*)time);
				time->day = VAL.vDateTime.date.dayOfMonth;
				time->year = VAL.vDateTime.date.year;
				time->month = VAL.vDateTime.date.month;
				time->hour = VAL.vDateTime.time.hour;
				time->minute = VAL.vDateTime.time.minute;
				time->second = VAL.vDateTime.time.second;
				SQLBIND(idx, time, 0, SQL_C_TIMESTAMP, SQL_TIMESTAMP);
			}
			break;
		case DBType::DATA:
			SQLBIND(idx, VAL.vData.Address(), 0, SQL_C_BINARY, SQL_LONGVARBINARY);
			break;
		default:
			dprintf("Unknow DBType\n");
			ASSERT(FALSE);
		}
	}

	if ((*odbc->SQLExecute)(hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLExecute\n");
		Error(hstmt);
		return FALSE;
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseODBC::Query(const Stringz&SQL)
{
	ASSERT(connected);
	HSTMT hstmt;
	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	_BOOL r = _Query(hstmt, SQL);
	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return r;
}


//=========================================================
_BOOL DBDatabaseODBC::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	ASSERT(connected);
	HSTMT hstmt;
	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	_BOOL r = _Query(hstmt, SQL, TYPES, VALUES);
	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return r;
}


//=========================================================
_BOOL DBDatabaseODBC::Query(const Stringz&SQL, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	ASSERT(connected);
	HSTMT hstmt;
	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	_BOOL r = _Query(hstmt, SQL);
	if (r)
		r = FetchResults(hstmt, COLS, RECORDS);
	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return r;
}


//=========================================================
_BOOL DBDatabaseODBC::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	ASSERT(connected);
	HSTMT hstmt;
	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	_BOOL r = _Query(hstmt, SQL, TYPES, VALUES);
	if (r)
		r = FetchResults(hstmt, COLS, RECORDS);
	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return r;
}


//=========================================================
Stringz DBDatabaseODBC::LastError()
{
	return error;		     
}


//=========================================================
void DBDatabaseODBC::Error(HSTMT hstmt)
{
	UCHAR	sqlState[6];
	SDWORD	nativeError;
	UCHAR	errorMsg[SQL_MAX_MESSAGE_LENGTH-1];
	SWORD	errorMsgl;
	if (connected)
		(*odbc->SQLError)(henv, hdbc, hstmt, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &errorMsgl);
	else
		(*odbc->SQLError)(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &errorMsgl);
	error = (_CHAR*)errorMsg;
}


//=========================================================
_LONG DBDatabaseODBC::SQL2DB(SWORD T) const
{
	switch (T)
	{
	case SQL_BIGINT:
		return DBType::LONG;
	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		return DBType::DATA;
	case SQL_BIT:
		return DBType::BYTE;
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
		return DBType::STRING;
	case SQL_DATE:
	case SQL_TIME:
	case SQL_TIMESTAMP:
		return DBType::DATETIME;
	case SQL_FLOAT:
		return DBType::FLOAT;
	case SQL_DOUBLE:
	case SQL_REAL:
		return DBType::DOUBLE;
	case SQL_DECIMAL:
		return DBType::DECIMAL;
	case SQL_INTEGER:
		return DBType::LONG;
	case SQL_SMALLINT:
		return DBType::INT;
	case SQL_TINYINT:
		return DBType::SHORT;
	default:
		dprintf("Unknow SQLType\n");
		ASSERT(FALSE);
	}
	return DBType::DATA;
}


//=========================================================
void DBDatabaseODBC::Bind(HSTMT hstmt, _WORD N, _LONG TYPE, _PTR&VAL, _DWORD LEN, List<SDWORD>&OUTLEN)
{
	SDWORD *outlen = new SDWORD;
	OUTLEN.Add(outlen);
	switch (TYPE)
	{
	case DBType::BYTE:
	case DBType::BOOL:
		VAL = new unsigned char;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_UTINYINT, VAL, sizeof(unsigned char), outlen);
		break;
	case DBType::SHORT:
		VAL = new signed char ;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_STINYINT, VAL, sizeof(signed char), outlen);
		break;
	case DBType::INT:
		VAL = new short int;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_SSHORT, VAL, sizeof( short int), outlen);
		break;
	case DBType::LONG:
		VAL = new long int;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_SLONG, VAL, sizeof(long int), outlen);
		break;
	case DBType::DECIMAL:
		dprintf("Unsupported data type\n");
		break;
	case DBType::DOUBLE:
		VAL = new double;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_DOUBLE, VAL, sizeof(double), outlen);
		break;
	case DBType::FLOAT:
		VAL = new float;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_FLOAT, VAL, sizeof(float), outlen);
		break;
	case DBType::STRING:
		VAL = new unsigned char[LEN+1];
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_CHAR, VAL, LEN+1, outlen);
		break;
	case DBType::DATETIME:
		VAL = new TIMESTAMP_STRUCT;
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_TIMESTAMP, VAL, sizeof(TIMESTAMP_STRUCT), outlen);
		break;
	case DBType::DATA:		
		VAL = new unsigned char[LEN];
		(*odbc->SQLBindCol)(hstmt, N, SQL_C_BINARY, VAL, LEN, outlen);
		break;
	default:
		dprintf("Unknow DBType\n");
		ASSERT(FALSE);
	}
}

//=========================================================
_BOOL DBDatabaseODBC::FetchResults(HSTMT hstmt, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	SWORD	i;
	SWORD   ncols;
	SWORD   coltype;
	SWORD   colnamelen;
	SWORD   nullable;
	SWORD   scale;
	UDWORD  collen;
	UCHAR	colName[64];
	RETCODE	retcode;

	_LONG			dbtype;
	List<SDWORD>	outlen;

	if ((*odbc->SQLNumResultCols)(hstmt, &ncols) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLNumResultCols\n");
		Error(hstmt);
		return FALSE;
	}
	if (ncols == 0) return TRUE;
	Array<_PTR> vals(ncols);
	COLS.Clear();
	COLS.Dim(ncols);
	for (i=1; i<=ncols; i++)
	{
		(*odbc->SQLDescribeCol)(hstmt, i, 
			colName, (SWORD)sizeof(colName),
            &colnamelen, &coltype, 
			&collen, &scale, &nullable);
		dbtype = SQL2DB(coltype);
		COLS[i].name = (_CHAR*)colName;
		COLS[i].type = dbtype;
		COLS[i].len = collen;
		Bind(hstmt, i, dbtype, vals[i], collen, outlen);
	}
	while (TRUE)
	{
		retcode = (*odbc->SQLFetch)(hstmt);
		if (retcode == SQL_NO_DATA_FOUND) break;
        if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			dprintf("ODBC::SQLFetch\n");
			Error(hstmt);
			return FALSE;
		}
		DBRecord *rec = new DBRecord(ncols);
		RECORDS.Add(rec);
		for (i = 1; i <= ncols; i++)
		{
			switch(COLS[i].type)
			{
			case DBType::BYTE:
				(*rec)[i].vByte = *((unsigned char*)vals[i]);
				break;
			case DBType::BOOL:
				(*rec)[i].vBool = *((unsigned char*)vals[i]);
				break;
			case DBType::SHORT:
				(*rec)[i].vShort = *((signed char*)vals[i]);
				break;
			case DBType::INT:
				(*rec)[i].vInt = *((short int*)vals[i]);
				break;
			case DBType::LONG:
				(*rec)[i].vLong = *((long int*)vals[i]);
				break;
			case DBType::FLOAT:
				(*rec)[i].vFloat = *((float*)vals[i]);
				break;
			case DBType::DOUBLE:
				(*rec)[i].vDouble = *((double*)vals[i]);
				break;
			case DBType::STRING:
				(*rec)[i].vString = (_CHAR*)(unsigned char*)vals[i];
				break;
			case DBType::DATA:		
				(*rec)[i].vData.Dim(COLS[i].len);
				Memory::Copy((*rec)[i].vData.Address(), vals[i], COLS[i].len);
				break;
			case DBType::DATETIME:
				{
					TIMESTAMP_STRUCT *time = (TIMESTAMP_STRUCT*)vals[i];
					(*rec)[i].vDateTime.date.year = time->year;
					(*rec)[i].vDateTime.date.month = _BYTE(time->month);
					(*rec)[i].vDateTime.date.dayOfMonth = _BYTE(time->day);
					(*rec)[i].vDateTime.time.hour = _BYTE(time->hour);
					(*rec)[i].vDateTime.time.minute = _BYTE(time->minute);
					(*rec)[i].vDateTime.time.second = _BYTE(time->second);
				}
				break;
			}
		}
    }
	for (i=1; i<=ncols; i++)
		delete[] vals[i];
	return TRUE;
}


//=========================================================
Stringz DBDatabaseODBC::SQLType(_LONG TYPE, _DWORD LEN) const
{
	Stringz res;
	switch (TYPE)
	{
	case DBType::BYTE:
		res = "BIT"	;	
		break;
	case DBType::BOOL:
		res = "BIT"	;	
		break;
	case DBType::SHORT:
		res = "TINYINT";
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
		res.Format("VARCHAR(%d)", LEN);
		break;
	case DBType::DATETIME:
		res = "TIMESTAMP";
		break;
	case DBType::DATA:		
		res.Format("VARBINARY(%d)", LEN);
		break;
	default:
		dprintf("Unknow DBType\n");
		ASSERT(FALSE);
	}
	return Stringz(res);
}


//=========================================================
_BOOL DBDatabaseODBC::Tables(List<Stringz>&NAMES, List<Stringz>&TYPES)
{
	ASSERT(connected);
	HSTMT hstmt;
	RETCODE retcode;

	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	
	if ((*odbc->SQLTables)(hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0) == SQL_ERROR)
	{
		dprintf("ODBC::SQLTables\n");
		Error(hstmt);
		return FALSE;
	}

	_CHAR name[128];
	SDWORD namelen = 128;
	_CHAR type[128];
	SDWORD typelen = 128;
	(*odbc->SQLBindCol)(hstmt, 3, SQL_C_CHAR, name, namelen, &namelen);
	(*odbc->SQLBindCol)(hstmt, 4, SQL_C_CHAR, type, typelen, &typelen);
	while (TRUE)
	{
		retcode = (*odbc->SQLFetch)(hstmt);
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

	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return TRUE;
}



//=========================================================
_BOOL DBDatabaseODBC::DriverInfos(List<Stringz>&NAMES, List<Stringz>&VALUES)
{
	NAMES.Add(new Stringz("ID"));
	VALUES.Add(new Stringz("ODBC"));
	NAMES.Add(new Stringz("Version"));
	VALUES.Add(new Stringz(DBODBCVERSION));

	ModODBCCP *odbccp = ModODBCCP::Create();
	if (!odbccp) return TRUE;
	Stringz s;
	_WORD size = 1024;
	_CHAR *drivers = new _CHAR[size];
	if ((*(odbccp->SQLGetInstalledDrivers))(drivers, size, &size))
	{
		NAMES.Add(new Stringz("Drivers"));
		for (_WORD i=0; i<size; i++)
		{
			if (drivers[i] == 0)
			{
				VALUES.Add(new Stringz(s));
				s = "";
			}
			else
				s += drivers[i];
		}
	}
	delete []drivers;
	delete odbccp;
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseODBC::Columns(const Stringz&NAME, List<DBColumn>&COLS)
{
	ASSERT(connected);
	HSTMT hstmt;
	RETCODE retcode;

	if ((*odbc->SQLAllocStmt)(hdbc, &hstmt) != SQL_SUCCESS)
	{
		dprintf("ODBC::SQLAllocStmt\n");
		Error(SQL_NULL_HSTMT);
		return FALSE;
	}
	
	if ((*odbc->SQLColumns)(hstmt, NULL, 0, NULL, 0, (UCHAR*)(_CHAR*)NAME, SQL_NTS, NULL, 0) == SQL_ERROR)
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

	(*odbc->SQLBindCol)(hstmt, 4, SQL_C_CHAR, name, lenname, &lenname);
	(*odbc->SQLBindCol)(hstmt, 5, SQL_C_SSHORT, &type, lentype, &lentype);
	(*odbc->SQLBindCol)(hstmt, 8, SQL_C_SLONG, &len, lenlen, &lenlen);
	while (TRUE)
	{
		retcode = (*odbc->SQLFetch)(hstmt);
		if (retcode == SQL_NO_DATA_FOUND) break;
		if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
		{
			dprintf("ODBC::SQLFetch\n");
			Error(hstmt);
			return FALSE;
		}
		COLS.Add(new DBColumn(name, SQL2DB(type), len));
	}
	(*odbc->SQLFreeStmt)(hstmt, SQL_DROP);
	return TRUE;
}

#endif
