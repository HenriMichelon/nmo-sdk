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

$Id: DBDatabasePostgres.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_POSTGRES
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include "DBDatabasePostgres.hpp"
using namespace NMO;

//=========================================================
DBDatabasePostgres::DBDatabasePostgres(): DBDatabase()
{
}


//=========================================================
DBDatabasePostgres::~DBDatabasePostgres()
{
	Disconnect();
}


//=========================================================
_BOOL DBDatabasePostgres::Connect(const Stringz&S, const Stringz&B, const Stringz&U, const Stringz&P)
{
	Debug( if (connected) throw NMOException("Postgres::Connect: database is already connected\n"); )

	server = S;
	base = B;
	user = U;
	passwd = P;

	_CHAR *s = NULL;
	_CHAR *b = NULL;
	if (server.Len()) s = (_CHAR*)server;
	if (base.Len()) b = (_CHAR*)base;

	pgconn = PQsetdb(s, NULL, NULL, NULL, b);
	connected = (PQstatus(pgconn) == CONNECTION_OK);
	return connected;
}


//=========================================================
void DBDatabasePostgres::Disconnect()
{
	if (connected)
	{
		PQfinish(pgconn);
		connected = FALSE;
	}
}


//=========================================================
_BOOL DBDatabasePostgres::Query(const Stringz&SQL)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)
	PGresult *res = PQexec(pgconn, (_CHAR*)SQL);
	if (!res) return FALSE;
	ExecStatusType status = PQresultStatus(res);
	PQclear(res);
	return ((status == PGRES_COMMAND_OK) || (status == PGRES_TUPLES_OK));
}


//=========================================================
_BOOL DBDatabasePostgres::Query(const Stringz&SQL, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)

	PGresult *res = PQexec(pgconn, (_CHAR*)SQL);
	if (!res) return FALSE;
	ExecStatusType status = PQresultStatus(res);
	if (status == PGRES_COMMAND_OK)
	{
		PQclear(res);
		return TRUE;
	}

	if (status != PGRES_TUPLES_OK)
	{
		PQclear(res);
		dprintf("PQexec\n");
		return FALSE;
	}

	_DWORD i;
	_DWORD ncols = PQnfields(res);
	COLS.Dim(ncols);
	for (i=1; i<=ncols; i++)
	{
		COLS[i].name = PQfname(res, i-1);
		COLS[i].type = PQType2DBType(PQftype(res, i-1));
		COLS[i].len = PQfsize(res, i-1);
	}

	_DWORD nrows = PQntuples(res);
	for (i=0; i<nrows; i++)
	{
		DBRecord *rec = new DBRecord(ncols);
		for (_DWORD j=1; j<=ncols; j++)
			SetValue((*rec)[j], COLS[j], PQgetvalue(res, i, j-1));
		RECORDS.Add(rec);
	}
			
	PQclear(res);
	return TRUE;
}


//=========================================================
_BOOL DBDatabasePostgres::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)
	Stringz sql(SQL);
	FormatQuery(sql, TYPES, VALUES);
	return Query(sql);
}


//=========================================================
_BOOL DBDatabasePostgres::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)
	Stringz sql(SQL);
	FormatQuery(sql, TYPES, VALUES);
	return Query(sql, COLS, RECORDS);
}



//=========================================================
Stringz DBDatabasePostgres::LastError()
{
	return PQerrorMessage(pgconn);
}


//=========================================================
void DBDatabasePostgres::FormatQuery(Stringz &SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES) const
{
	_LONG pos;
	TYPES.Reset();
	VALUES.Reset();
	while (TRUE)
	{
		pos = SQL.InStr('?');
		if (pos == -1) break;
		SQL = SQL.Copy(0, pos) + VALUES.Next().Format(TYPES.Next()) + 
				SQL.Copy(pos+1, SQL.Len()-pos-1);
	}
}


//=========================================================
Stringz DBDatabasePostgres::SQLType(_LONG TYPE, _DWORD LEN) const
{
	Stringz res;
	switch (TYPE)
	{
	case DBType::BYTE:
		res = "CHAR(1)";	
		break;
	case DBType::BOOL:
		res = "BOOLEAN";	
		break;
	case DBType::SHORT:
		res = "SMALLINT";
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
	case DBType::STRING:
		res = "TEXT";
		break;
	case DBType::DATETIME:
		res = "DATETIME";
		break;
	case DBType::DATA:		
		res.Format("CHAR(%d)", LEN);
		break;
	}
	return Stringz(res);
}


//=========================================================
_BOOL DBDatabasePostgres::Tables(List<Stringz>&NAMES, List<Stringz>&TYPES)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)
	Array<DBColumn> cols;
	List<DBRecord> rows;
	if (!Query("SELECT relname FROM pg_class", cols, rows)) return FALSE;
	rows.Reset();
	while (!rows.End())
	{
		NAMES.Add(new Stringz(rows.Next()[1].vString));
		TYPES.Add(new Stringz("CLASS"));
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabasePostgres::DriverInfos(List<Stringz>&NAMES, List<Stringz>&VALUES)
{
	NAMES.Add(new Stringz("ID"));
	VALUES.Add(new Stringz("Postgres"));
	NAMES.Add(new Stringz("Version"));
	VALUES.Add(new Stringz("0000"));
	return TRUE;
}


//=========================================================
_BOOL DBDatabasePostgres::Columns(const Stringz&NAME, List<DBColumn>&COLS)
{
	Debug(if (!connected) throw NMOException("Postgres::Query: not connected to a database");)

	Array<DBColumn> cols;
	List<DBRecord> rows;

	Stringz SQL("SELECT attname, atttypid, attlen FROM pg_class c, pg_attribute WHERE relname='");
	SQL += NAME;
	SQL += (_CHAR*)"' AND attnum>0 AND attrelid=c.oid ORDER BY attnum";

	if (!Query(SQL, cols, rows)) return FALSE;
	rows.Reset();
	while (!rows.End())
	{
		DBRecord &rec = rows.Next();
		COLS.Add(new DBColumn(rec[1].vString, PQType2DBType(rec[2].vLong), rec[3].vInt));
	}
	return TRUE;
}


//=========================================================
_LONG DBDatabasePostgres::PQType2DBType(Oid TYPE) const
{
	switch (TYPE)
	{
	case 16: //BOOLOID:
		return DBType::BOOL;
	case 17: //BYTEAOID:
		return DBType::DATA;
	case 18: //CHAROID:
		return DBType::BYTE;
	case 19: //NAMEOID:
	case 25: //TEXTOID:
	case 1042: //BPCHAROID:
	case 1043: //VARCHAROID:
		return DBType::STRING;
	case 21: //INT2OID:
		return DBType::INT;
	case 23: //INT4OID:
	case 26: //OIDOID:
		return DBType::LONG;
	case 700: //FLOAT4OID:
	case 701: //FLOAT8OID:
		return DBType::FLOAT;
	case 1082: //DATEOID:
	case 1083: //TIMEOID:
	case 1184: //DATETIMEOID:
	case 1186: //TIMESPANOID:
	case 1296: //TIMESTAMPOID:
		return DBType::STRING;
		//return DBType::DATETIME;
	default:
		return DBType::STRING;
	}
}


//=========================================================
void DBDatabasePostgres::SetValue(DBValue&VAL, const DBColumn&COL, const char*STR) const
{
	switch (COL.type)
	{
	case DBType::BYTE:
		VAL.vByte = (_BYTE)atoi(STR);
		break;
	case DBType::BOOL:
		VAL.vBool = ((!strcasecmp(STR, "y")) ||
					(!strcasecmp(STR, "t")) ||
					(!strcasecmp(STR, "yes")) ||
					(!strcasecmp(STR, "true")));
		break;
	case DBType::SHORT:
		VAL.vShort = (_SHORT)atoi(STR);
		break;
	case DBType::INT:
		VAL.vInt = (_INT)atoi(STR);
		break;
	case DBType::LONG:
		VAL.vLong = (_LONG)atol(STR);
		break;
	case DBType::FLOAT:
		VAL.vFloat = (_FLOAT)atof(STR);
		break;
	case DBType::STRING:
	case DBType::DATETIME:
		VAL.vString = STR;
		break;
	case DBType::DATA:
		VAL.vData.Dim(COL.len);
		MemCopy(VAL.vData.Address(), STR, COL.len);
		break;
	}
}


#endif
