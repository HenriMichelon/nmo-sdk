/*
(c) 1999-2002 Henri Michelon

$Id: statements.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include <nmo/addons/Shell.hpp>

#include "LC/LC.hpp"
#include "session.hpp"
#include "statements.hpp"

using namespace NMO;


///////////////////////////////////////
void Statement::Execute(ListIterator<Stringz>&args)
{
	if (!session::database)
	{
		shell->Write(LC_SQL_NODBOPEN);
		return;
	}

	Stringz SQL;
	args.Reset();
	while (!args.End())
		SQL += args.Next() + ' ';

	Array<DBColumn> cols;
	List<DBRecord> rows;
	if (!session::database->Query(SQL, cols, rows))
	{
		shell->Write(LC_SQL_ERROR, (_CHAR*)session::database->LastError());
		return;
	}
	Display(cols, rows);
}


///////////////////////////////////////
void Statement::Display(Array<DBColumn>&cols, List<DBRecord>&rows)
{
	if (!cols.Count()) return;

	_DWORD i, j, l;
	_DWORD lcol = shell->Cols() / 4;
	_DWORD ncol = lcol / 8;

	//ListIterator<DBColumn> lstcols(cols);
	for (i=1; i<=cols.Count(); i++)
	{
		Stringz name = cols[i].name;
		if (name.Len() > lcol) name[lcol] = 0;
		shell->Write((_CHAR*)name);
		for (l=1; l<=(ncol-(name.Len()/8)); l++)
			shell->Write("\t");
	}
	shell->Write("\n");

	for (i=1; i<=cols.Count(); i++)
	{
		Stringz name = cols[i].name;
		if (name.Len() > lcol) name[lcol] = 0;
		for (j=1; j<=name.Len(); j++)
			shell->PutChar('-');
		for (l=1; l<=(ncol-(name.Len()/8)); l++)
			shell->Write("\t");
	}
	shell->Write("\n");

	ListIterator<DBRecord> lstrows(rows);
	while (!lstrows.End())
	{
		DBRecord &row = lstrows.Next();
		for (i=1; i<=cols.Count(); i++)
		{
			Stringz r = row[i].Format(cols[i].type);
			if (r.Len() > lcol) r[lcol] = 0;
			shell->Write((_CHAR*)r);
			for (l=1; l<=(ncol-(r.Len()/8)); l++)
				shell->Write("\t");
		}
		if (shell->Write("\n")) break;
	}
}


///////////////////////////////////////
void StmtCreate::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 5)
	{
		shell->Write(LC_SQL_USAGECREATE);
		shell->Write(LC_SQL_USAGECONSTR);
	}
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtSelect::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 4)
		shell->Write(LC_SQL_USAGESELECT);
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtInsert::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 5)
		shell->Write(LC_SQL_USAGEINSERT);
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtUpdate::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 6)
		shell->Write(LC_SQL_USAGEUPDATE);
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtDrop::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 3)
		shell->Write(LC_SQL_USAGEDROP);
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtAlter::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 5)
		shell->Write(LC_SQL_USAGEALTER);
	else
		Statement::Execute(args);
}


///////////////////////////////////////
void StmtDelete::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 5)
		shell->Write(LC_SQL_USAGEDELETE);
	else
		Statement::Execute(args);
}
