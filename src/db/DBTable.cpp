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

$Id: DBTable.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBTable::DBTable(DBDatabase&DB, const Stringz&NAME): DBSet(DB, NAME),
	add(FALSE), edit(FALSE)
{}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBTable* DBTable::Open(DBDatabase&DB, const Stringz&NAME, const Stringz&ORDER)
{
	if ((!DB.Connected()) || (!NAME.Len())) return NULL;

	DBTable *table = new DBTable(DB, NAME);
	Stringz SQL("SELECT * FROM ");
	SQL += NAME;
	if (ORDER.Len()) { SQL += " ORDER BY " + ORDER + " ASC"; }
	if (DB.Query(SQL, table->Columns(), table->Records()))
		return table;
	delete table;
	return NULL;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBTable* DBTable::Create(DBDatabase&DB, const Stringz&NAME, Collection<DBColumn>&COLS, _DWORD PK)
{
	Stringz name = NAME;
	name.Trim();
	if ((!DB.Connected()) || (!name.Len())) return NULL;
		
	DBTable *table = new DBTable(DB, name);
	table->columns.Dim(COLS.Count());
	Stringz SQL("CREATE TABLE ");
	SQL += name;
	SQL += (_CHAR*) " (";
	for (_DWORD i=1; i<=COLS.Count(); i++)
	{
		if (i != 1)
			SQL += (_CHAR*)", ";
		DBColumn &col = COLS[i];
		SQL += col.name + ' ';
		SQL += DB.SQLType(col.type, col.len);
		table->columns[i] = col;
	}
	SQL += ')';
	if (DB.Query(SQL))
	{
		if (PK)
		{
			SQL = (_CHAR*)"CREATE UNIQUE INDEX idx";
			SQL += name;
			SQL += (_CHAR*)" ON ";
			SQL += name;
			SQL += (_CHAR*)" (";
			SQL += COLS[PK].name;
			SQL += (_CHAR*)" ASC) WITH PRIMARY DISALLOW NULL";
			DB.Query(SQL);
		}
		return table;
	}
	delete table;
	return NULL;	
}
	

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::Refresh()
{
	VALIDATETHIS;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::Add()
{
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::Add: close Add() session first");
		if (edit) throw NMOException("DBTable::Add: close Edit() session first");
		if (!columns.Count()) throw NMOException("DBTable::Add: 0 column table");
	)

	records.Add(new DBRecord(columns.Count()));
	add = TRUE;
}
	

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
void DBTable::Delete()
{
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::Delete: close Add() session first");
		if (edit) throw NMOException("DBTable::Delete: close Edit() session first");
		if (!columns.Count()) throw NMOException("DBTable::Edit: 0 column table");
	)
	
	_DWORD i;
	_DWORD ncol = columns.Count();
	Stringz SQL;
	
	
	SQL = (_CHAR*)"DELETE FROM ";
	SQL += name;
	SQL += (_CHAR*)" WHERE ";
	for (i=1; i<=ncol; i++)
	{
		if (i != 1) SQL += (_CHAR*)" AND ";
		SQL += columns[i].name;
		SQL += (_CHAR*) " = ";
		SQL += Current()[i].Format(columns[i].type);
	}
	database->Query(SQL);
	ASSERTMSG(FALSE, "Check for the drop!");
	records.Drop(lstRecords.Index());	
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::Edit()
{
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::Edit: close Add() session first");
		if (edit) throw NMOException("DBTable::Edit: close Edit() session first");
		if (!columns.Count()) throw NMOException("DBTable::Edit: 0 column table");
	)
	edited = Current();
	edit = TRUE;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::AddColumn(const Stringz&, DBType)
{
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::AddColumn: close Add() session first");
		if (edit) throw NMOException("DBTable::AddColumn: close Edit() session first");
	)
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::Update()
{
	Debug(
		VALIDATETHIS;
		ASSERT(database);
		if ((!add) && (!edit)) throw NMOException("DBTable::Update: no Add() or Edit() session opened");
	)

	_DWORD i;
	_DWORD ncol = columns.Count();
	Stringz SQL;
	Array<_LONG>	types(ncol);
	Array<DBValue>	values(ncol);
	DBRecord &rec = lstRecords.Current();

	if (add)
	{
		SQL = (_CHAR*)"INSERT INTO ";
		SQL += name;
		SQL += (_CHAR*)" VALUES (";
		for (i=1; i<=ncol; i++)
		{
			if (i != 1) SQL += (_CHAR*)", ";
			SQL += '?';
			types[i] = columns[i].type;
			values[i] = rec[i];
		}
		SQL += ')';
		database->Query(SQL, types, values);
		add = FALSE;
	}
	else if (edit)
	{
		SQL = (_CHAR*)"UPDATE ";
		SQL += name;
		SQL += (_CHAR*)" SET ";
		for (i=1; i<=ncol; i++)
		{
			if (i != 1) SQL += (_CHAR*)", ";
			SQL += columns[i].name;
			SQL += (_CHAR*) " = ";
			SQL += rec[i].Format(columns[i].type);
		}
		SQL += (_CHAR*)" WHERE ";
		for (i=1; i<=ncol; i++)
		{
			if (i != 1) SQL += (_CHAR*)" AND ";
			SQL += columns[i].name;
			SQL += (_CHAR*) " = ";
			SQL += edited[i].Format(columns[i].type);
		}
		database->Query(SQL);
		edit = FALSE;
	}
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBTable::Drop()
{
	VALIDATETHIS;
	ASSERT(database);

	Stringz SQL("DROP TABLE ");
	SQL += name;
	if (database->Query(SQL))
	{
		records.Clear();
		columns.Clear();
	}
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef __DEBUG

void DBTable::Reset()
{ 
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::Reset: close Add() session first");
		if (edit) throw NMOException("DBTable::Reset: close Edit() session first");
	)
	DBSet::Reset(); 
}

DBRecord& DBTable::Next()
{ 
	Debug(
		VALIDATETHIS;
		if (add) throw NMOException("DBTable::Next: close Add() session first");
		if (edit) throw NMOException("DBTable::Next: close Edit() session first");
	)
	return DBSet::Next(); 
}

#endif
