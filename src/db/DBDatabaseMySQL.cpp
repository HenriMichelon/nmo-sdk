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

	(c) 1999-2002 Henri Michelon

$Id: DBDatabaseMySQL.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_MYSQL
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;
#include "DBDatabaseMySQL.hpp"
#include <stdlib.h>

#define DBMYSQLVERSION "0001"

ModMySQL *DBDatabaseMySQL::module = NULL;
_DWORD DBDatabaseMySQL::nInst = 0;

//=========================================================
DBDatabaseMySQL::DBDatabaseMySQL(): DBDatabase()
{
	if (!nInst++) module = ModMySQL::Create();
	if (!module) { throw ExcLoadingDriver(); }
}


//=========================================================
DBDatabaseMySQL::~DBDatabaseMySQL()
{
	Disconnect();
	if (!--nInst) delete module;
}


//=========================================================
_BOOL DBDatabaseMySQL::Connect(const Stringz&S, const Stringz&B, const Stringz&U, const Stringz&P)
{
	Debug( if (connected) throw NMOException("MySQL::Connect: database is already connected\n"); )

	server = S;
	base = B;
	user = U;
	passwd = P;
	if (!server.Len()) server = "localhost";
	if (!user.Len()) user = SysInfo::Username();

	if (!(*module->mysql_connect)(&mysql, (_CHAR*)server, (_CHAR*)user, (_CHAR*)passwd))
	{
		dprintf("mysql_connect\n");
		return FALSE;
	}

	if ((*module->mysql_select_db)(&mysql, (_CHAR*)B))
	{
		dprintf("mysql_select_db\n");
		//mysql_close(&mysql);
		return FALSE;
	}

	connected = TRUE;
	return TRUE;
}


//=========================================================
void DBDatabaseMySQL::Disconnect()
{
	if (connected)
	{
		(*module->mysql_close)(&mysql);
		connected = FALSE;
	}
}


//=========================================================
_BOOL DBDatabaseMySQL::Query(const Stringz&SQL)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)
	return ((*module->mysql_query)(&mysql, (_CHAR*)SQL) == 0);
}


//=========================================================
_BOOL DBDatabaseMySQL::Query(const Stringz&SQL, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)

	if ((*module->mysql_query)(&mysql, (_CHAR*)SQL))
	{
		dprintf("MySQL: Error mysql_query (" + SQL + ")\n");
		return FALSE;
	}

	MYSQL_RES *res= (*module->mysql_store_result)(&mysql);
	if (!res) return TRUE;

	_DWORD ncols = (*module->mysql_num_fields)(res);
	COLS.Dim(ncols);

	_DWORD i = 0;
	MYSQL_FIELD **field = new (MYSQL_FIELD*)[ncols+1];
	while ((field[i] = (*module->mysql_fetch_field)(res)))
	{
		COLS[i+1].name = field[i]->name;
		COLS[i+1].type = MySQL2DBType(field[i]->type);
		COLS[i+1].len = field[i]->length;
		i++;
	}

	MYSQL_ROW row;
	while ((row = (*module->mysql_fetch_row)(res)))
	{
		DBRecord *rec = new DBRecord(ncols);
		for (i=0; i<ncols; i++)
			Convert(field[i]->type, (_BYTE*)row[i], field[i]->length, (*rec)[i+1]);
		RECORDS.Add(rec);
	}
	delete []field;

	return TRUE;
}


//=========================================================
_BOOL DBDatabaseMySQL::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)
	Stringz sql(SQL);
	FormatQuery(sql, TYPES, VALUES);
	return Query(sql);
}


//=========================================================
_BOOL DBDatabaseMySQL::Query(const Stringz&SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES, Array<DBColumn>&COLS, List<DBRecord>&RECORDS)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)
	Stringz sql(SQL);
	FormatQuery(sql, TYPES, VALUES);
	return Query(sql, COLS, RECORDS);
}


//=========================================================
Stringz DBDatabaseMySQL::LastError()
{
	return (*module->mysql_error)(&mysql);
}


//=========================================================
void DBDatabaseMySQL::FormatQuery(Stringz &SQL, Iterator<_LONG>&TYPES, Iterator<DBValue>&VALUES) const
{
	_LONG pos;
	TYPES.Reset();
	VALUES.Reset();
	while (TRUE)
	{
		pos = SQL.InStr('?');
		if (pos == -1) break;
		Stringz fmt;
		_LONG &t = TYPES.Next();
		switch (t)
		{
		case DBType::DATETIME:
			fmt = "%yyyy-%mm-%dd %hh:%mm:%ss";
			break;
		case DBType::TIME:
			fmt = "%hh:%mm:%ss";
			break;
		case DBType::DATE:
			fmt = "%yyyy-%mm-%dd";
			break;
		case DBType::STRING:
			SQL = SQL.Copy(0, pos) + '\'' + VALUES.Next().Format(t, fmt) + '\'' +
					SQL.Copy(pos+1, SQL.Len()-pos-1);
			continue;
			break;
		}
		SQL = SQL.Copy(0, pos) + VALUES.Next().Format(t, fmt) + 
				SQL.Copy(pos+1, SQL.Len()-pos-1);
	}
}


//=========================================================
DateTime::Time DBDatabaseMySQL::ConvertTime(const Stringz&STR) const
{
	DateTime::Time time;
	Stringz str;
	_DWORD idx = 0;
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 3)
		time.hour = (str[0l] - '0') * 100 + (str[1l] - '0') * 10 + (str[2l] - '0');
	else if (str.Len() == 2)
		time.hour = (str[0l] - '0') * 10 + (str[1l] - '0');
	else 
		time.hour = (str[0l] - '0');
	idx++;
	str = "";
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 2)
		time.minute = (str[0l] - '0') * 10 + (str[1l] - '0');
	else 
		time.minute = (str[0l] - '0');
	idx++;
	str = "";
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 2)
		time.second = (str[0l] - '0') * 10 + (str[1l] - '0');
	else 
		time.second = (str[0l] - '0');
	return time;
}


//=========================================================
DateTime::Date DBDatabaseMySQL::ConvertDate(const Stringz&STR) const
{
	DateTime::Date date;
	Stringz str;
	_DWORD idx = 0;
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 2)
	{
		date.year = (str[0l] - '0') * 10 + (str[1l] - '0');
		if (date.year < 70) 
			date.year += 2000;
		else
			date.year += 1900;
	}
	else
		date.year = (str[0l] - '0') * 1000 + (str[1l] - '0') * 100 +
					(str[2l] - '0') * 10 + (str[3l] - '0');
	idx++;
	str = "";
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 1)
		date.month = (str[0l] - '0');
	else
		date.month = (str[0l] - '0') * 10 + (str[1l] - '0');
	idx++;
	str = "";
	while ((idx < STR.Len()) && (STR[idx] >= '0') && (STR[idx] <='9'))
		str += STR[idx++];
	if (str.Len() == 1)
		date.dayOfMonth = (str[0l] - '0');
	else
		date.dayOfMonth = (str[0l] - '0') * 10 + (str[1l] - '0');
	return date;
}


//=========================================================
void DBDatabaseMySQL::Convert(enum_field_types TYPE, _BYTE *STR, _DWORD LEN, DBValue&VAL) const
{
	if (STR == NULL) { return; }
	switch (TYPE)
	{
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_SET:
	case FIELD_TYPE_NULL:
	case FIELD_TYPE_TIMESTAMP:
	case FIELD_TYPE_YEAR:
	case FIELD_TYPE_NEWDATE:
	case FIELD_TYPE_INTERVAL:
			  dprintf("Unsupported data type\n");
			  ASSERT(FALSE);
			  break;
	case FIELD_TYPE_FLOAT:
		VAL.vFloat = (_FLOAT) atof((_CHAR*)STR);
		break;
	case FIELD_TYPE_DOUBLE:
		VAL.vDouble = (_DOUBLE) atof((_CHAR*)STR);
		break;
	case FIELD_TYPE_TINY:
		VAL.vShort = (_SHORT) atoi((_CHAR*)STR);
		break;
	case FIELD_TYPE_SHORT:
		VAL.vInt = (_INT) atoi((_CHAR*)STR);
		break;
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_INT24:
		VAL.vLong = (_LONG) atol((_CHAR*)STR);
		break;
	case FIELD_TYPE_DATETIME:
		{
			Stringz datetime((_CHAR*)STR);
			_LONG pos = datetime.InStr(' ');
			if (pos > -1)
			{
				Stringz str = datetime.Left(pos);
				VAL.vDateTime.date = ConvertDate(str);
				str = datetime.Right(str.Len()+1);
				VAL.vDateTime.time = ConvertTime(str);
			}
		}
		break;
	case FIELD_TYPE_DATE:
		VAL.vDateTime.date = ConvertDate((_CHAR*)STR);
		break;
	case FIELD_TYPE_TIME:
		VAL.vDateTime.time = ConvertTime((_CHAR*)STR);
		break;
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_STRING:
		VAL.vString = (_CHAR*)STR;
		break;
	case FIELD_TYPE_BLOB:
	case FIELD_TYPE_TINY_BLOB:
	case FIELD_TYPE_MEDIUM_BLOB:
	case FIELD_TYPE_LONG_BLOB:
		VAL.vData.Dim(LEN);
		Memory::Copy(VAL.vData.Address(), STR, LEN);
		break;
	default:
		break;
	}
}


//=========================================================
_LONG DBDatabaseMySQL::MySQL2DBType(enum_field_types TYPE) const
{
	switch (TYPE)
	{
	case FIELD_TYPE_DECIMAL:
		return DBType::DECIMAL;
	case FIELD_TYPE_FLOAT:
	case FIELD_TYPE_DOUBLE:
		return DBType::FLOAT;
	case FIELD_TYPE_TINY:
		return DBType::SHORT;
	case FIELD_TYPE_SHORT:
		return DBType::INT;
	case FIELD_TYPE_LONG:
	case FIELD_TYPE_LONGLONG:
	case FIELD_TYPE_INT24:
		return DBType::LONG;
	case FIELD_TYPE_DATE:
		return DBType::DATE;
	case FIELD_TYPE_TIME:
		return DBType::TIME;
	case FIELD_TYPE_DATETIME:
		return DBType::DATETIME;
	case FIELD_TYPE_BLOB:
	case FIELD_TYPE_TINY_BLOB:
	case FIELD_TYPE_MEDIUM_BLOB:
	case FIELD_TYPE_LONG_BLOB:
		return DBType::DATA;
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_STRING:
		return DBType::STRING;
	default:	
		return DBType::STRING;
	}
}


//=========================================================
Stringz DBDatabaseMySQL::SQLType(_LONG TYPE, _DWORD LEN) const
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
		res = "INT";
		break;
	case DBType::FLOAT:
		res = "FLOAT";
		break;
	case DBType::STRING:
		res.Format("VARCHAR(%d)", LEN);
		break;
	case DBType::DATETIME:
		res = "DATETIME";
		break;
	case DBType::TIME:
		res = "TIME";
		break;
	case DBType::DATE:
		res = "DATE";
		break;
	case DBType::DATA:		
		res.Format("LONGBLOB(%d)", LEN);
		break;
	}
	return Stringz(res);
}



//=========================================================
_BOOL DBDatabaseMySQL::Tables(List<Stringz>&NAMES, List<Stringz>&TYPES)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)
	Array<DBColumn> cols;
	List<DBRecord> rows;
	Stringz SQL = "SHOW TABLES";
	if (!Query(SQL, cols, rows)) return FALSE;
	ListIterator<DBRecord> list(rows);
	while (!list.End())
	{
		DBRecord &rec = list.Next();
		NAMES.Add(new Stringz(rec[1].vString));
		TYPES.Add(new Stringz("TABLE"));
	}
	return TRUE;
}



//=========================================================
_BOOL DBDatabaseMySQL::DriverInfos(List<Stringz>&NAMES, List<Stringz>&VALUES)
{
	NAMES.Add(new Stringz("ID"));
	VALUES.Add(new Stringz("MySQL"));
	NAMES.Add(new Stringz("Version"));
	VALUES.Add(new Stringz(DBMYSQLVERSION));
	NAMES.Add(new Stringz("MySQL API Version"));
	VALUES.Add(new Stringz(MYSQL_SERVER_VERSION));

	MYSQL mysql;
	if ((*module->mysql_connect)(&mysql, "localhost", NULL, NULL))
	{
		NAMES.Add(new Stringz("Local server version"));
		VALUES.Add(new Stringz((*module->mysql_get_server_info)(&mysql)));
		(*module->mysql_close)(&mysql);
	}
	return TRUE;
}


//=========================================================
_BOOL DBDatabaseMySQL::Columns(const Stringz&NAME, List<DBColumn>&COLS)
{
	Debug(if (!connected) throw NMOException("MySQL::Query: not connected to a database");)
	_LONG pos;
	Array<DBColumn> cols;
	List<DBRecord> rows;
	Stringz SQL = "SHOW COLUMNS FROM " + NAME;
	if (!Query(SQL, cols, rows)) return FALSE;
	ListIterator<DBRecord> list(rows);
	while (!list.End())
	{
		DBRecord &rec = list.Next();
		DBColumn *col = new DBColumn;
		col->name = rec[1].vString;
		pos = rec[2].vString.InStr('(');
   		col->len = atoi(rec[2].vString.Copy(pos+1, rec[2].vString.InStr(')')-pos-1));
		col->type = MySQL2DBType(rec[2].vString);
		COLS.Add(col);
	}
	return TRUE;
}

//=========================================================
_LONG DBDatabaseMySQL::MySQL2DBType(Stringz&TYPE) const
{
	TYPE.UCase();
	if (TYPE.InStr("TINYINT") != -1)
		return DBType::SHORT;
	else if (TYPE.InStr("SMALLINT") != -1)
		return DBType::INT;
	else if (TYPE.InStr("INT") != -1)
		return DBType::LONG;
	else if (TYPE.InStr("FLOAT") != -1)
		return DBType::FLOAT;
	else if (TYPE.InStr("DOUBLE") != -1)
		return DBType::FLOAT;
	else if (TYPE.InStr("REAL") != -1)
		return DBType::FLOAT;
	else if (TYPE.InStr("DECIMAL") != -1)
		return DBType::FLOAT;
	else if (TYPE.InStr("NUMERIC") != -1)
		return DBType::FLOAT;
	else if (TYPE.InStr("DATETIME") != -1)
		return DBType::DATETIME;
	else if (TYPE.InStr("DATE") != -1)
		return DBType::DATE;
	else if (TYPE.InStr("TIME") != -1)
		return DBType::TIME;
	else if (TYPE.InStr("CHAR") != -1)
		return DBType::STRING;
	else if (TYPE.InStr("TEXT") != -1)
		return DBType::STRING;
	else if (TYPE.InStr("BLOB") != -1)
		return DBType::DATA;
	else if (TYPE.InStr("ENUM") != -1)
		return DBType::STRING;
	else if (TYPE.InStr("SET") != -1)
		return DBType::STRING;
	return DBType::STRING;
}
#endif
