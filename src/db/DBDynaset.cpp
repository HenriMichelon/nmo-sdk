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

$Id: DBDynaset.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBDynaset::DBDynaset(DBDatabase&DB, const Stringz&NAME):
	DBSet(DB, NAME), edit(FALSE)
{}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBDynaset *Create(DBDatabase&DB, const Stringz&NAME, NMO::Collection<DBColumn>&COLS, NMO::Collection<DBValue>&VALUES)
{
	Stringz name = NAME;
	name.Trim();
	if ((!DB.Connected()) || (!name.Len())) return NULL;

	_DWORD i;
	DBDynaset *table = new DBDynaset(DB, name);
	Stringz SQL((_CHAR*)"SELECT ");
	for (i=1; i<COLS.Count(); i++)
	{
		if (i != 1)
			SQL += ',';
		SQL += COLS[i].name + ' ';
	}
	SQL += (_CHAR*)"FROM ";
	SQL += name;
	SQL += (_CHAR*)"WHERE ";
	for (i=1; i<VALUES.Count(); i++)
	{
		if (i != 1)
			SQL += (_CHAR*)" AND ";
		SQL += COLS[i].name + '=';
		SQL += VALUES[i].Format(COLS[i].type);
	}
	if (DB.Query(SQL, table->Columns(), table->Records()))
		return table;
	delete table;
	return NULL;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBDynaset::Refresh()
{
	VALIDATETHIS;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
void DBDynaset::Delete()
{
	Debug(
		VALIDATETHIS;
		if (edit) throw NMOException("DBDynaset::Delete: close Edit() session first");
		if (!columns.Count()) throw NMOException("DBDynaset::Edit: 0 column set");
	)
		ASSERT(FALSE);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBDynaset::Edit()
{
	Debug(
		VALIDATETHIS;
		if (edit) throw NMOException("DBDynaset::Edit: close Edit() session first");
		if (!columns.Count()) throw NMOException("DBDynaset::Edit: 0 column set");
	)
	edited = Current();
	edit = TRUE;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBDynaset::Update()
{
	Debug(
		VALIDATETHIS;
		ASSERT(database);
		if (!edit) throw NMOException("DBDynaset::Update: no Add() or Edit() session opened");
	)

	_DWORD i;
	_DWORD ncol = columns.Count();
	Stringz SQL;
	Array<_LONG>	types(ncol);
	Array<DBValue>	values(ncol);
	DBRecord &rec = lstRecords.Current();

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


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef __DEBUG

void DBDynaset::Reset()
{ 
	Debug(
		VALIDATETHIS;
		if (edit) throw NMOException("DBDynaset::Reset: close Edit() session first");
	)
	DBSet::Reset(); 
}

DBRecord& DBDynaset::Next()
{ 
	Debug(
		VALIDATETHIS;
		if (edit) throw NMOException("DBDynaset::Next: close Edit() session first");
	)
	return DBSet::Next(); 
}

#endif
