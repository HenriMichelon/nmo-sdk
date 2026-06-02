/*
(c) 1999-2002 Henri Michelon

$Id: commands.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include <nmo/addons/Shell.hpp>

#include "LC/LC.hpp"
#include "session.hpp"
#include "commands.hpp"

using namespace NMO;


///////////////////////////////////////
void CmdOpen::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 3)
		shell->Write(LC_SQL_USAGEOPEN);
	else
	{
		Stringz user;
		Stringz passwd;
		session::driver = args[2];
		session::driver.UCase();

		if (session::database)
		{
			delete session::database;
			session::database = NULL;
		}

		if (args.Count() > 4)
			passwd = args[5];
		if (args.Count() > 3)
			user = args[4];
		session::database = DBDatabase::Create(session::driver);
		if (!session::database)
			shell->Write(LC_SQL_ERROPEN, (_CHAR*)session::driver);
		else
		{
			if (!session::database->Connect("", args[3], user, passwd))
			{
				shell->Write(LC_SQL_ERROR, (_CHAR*)session::database->LastError());
				delete session::database;
				session::database = NULL;
			}
		}
	}
}


///////////////////////////////////////
void CmdClose::Execute(ListIterator<Stringz>&)
{
	if (!session::database)
		shell->Write(LC_SQL_NODBOPEN);
	else
	{
		delete session::database;
		session::database = NULL;
	}
}


///////////////////////////////////////
void CmdShow::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 2)
		shell->Write(LC_SQL_USAGESHOW);
	else
	{
		ShellCommand *cmd = NULL;
		if (args[2] == (_CHAR*)"tables")
			cmd = new CmdShowTables;
		if (args[2] == (_CHAR*)"driver")
			cmd = new CmdShowDriver;
		if (cmd)
		{
			cmd->shell = shell;
			cmd->Execute(args);
			delete cmd;
		}
		else
			shell->Write(LC_SQL_USAGESHOW);
	}
}


///////////////////////////////////////
void CmdDesc::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 2)
	{
		shell->Write(LC_SQL_USAGEDESC);
		return;
	}

	if (!session::database)
	{
		shell->Write(LC_SQL_NODBOPEN);
		return;
	}

	List<DBColumn> cols;
	if (!session::database->Columns(args[2], cols))
	{
		shell->Write(LC_SQL_ERRTABLE, (_CHAR*)args[2]);
		shell->Write("%s\n", (_CHAR*)session::database->LastError());
	}
	else
	{
		_BYTE i;
		Stringz name;
		ListIterator<DBColumn> lst(cols);
		while (!lst.End())
		{
			DBColumn &col = lst.Next();
			name = col.name;
			shell->Write((_CHAR*)name);
			for (i=1; i<=(3-(name.Len()/8)); i++)
				shell->Write("\t");
			//name = session::database->SQLType(col.type, col.len);
			name = DBType::Format(col.type);
			shell->Write(": %s", (_CHAR*)name);
			for (i=1; i<=(3-((name.Len()+2)/8)); i++)
				shell->Write("\t");
			if (shell->Write("\n")) break;
		}
	}
}


///////////////////////////////////////
void CmdShowTables::Execute(ListIterator<Stringz>&)
{
	if (!session::database)
	{
		shell->Write(LC_SQL_NODBOPEN);
		return;
	}

	List<Stringz> names;
	List<Stringz> types;
	if (!session::database->Tables(names, types))
		shell->Write(LC_SQL_ERRTABLES);
	else
	{
		ListIterator<Stringz> lstnames(names);
		ListIterator<Stringz> lsttypes(types);
		while (!lstnames.End())
		{
			Stringz &name = lstnames.Next();
			shell->Write((_CHAR*)name);
			if (!lsttypes.End())
			{
				for (_BYTE i=1; i<=(4-(name.Len()/8)); i++)
					shell->Write("\t");
				shell->Write(": %s", (_CHAR*)lsttypes.Next());
			}
			if (shell->Write("\n")) break;
		}
	}
}


///////////////////////////////////////
void CmdShowDriver::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 3)
		shell->Write(LC_SQL_USAGEDRIVER);
	else
	{
		Stringz driver = args[3];
		DBDatabase *db = DBDatabase::Create(driver);

		if (!db)
			shell->Write(LC_SQL_ERROPEN, (_CHAR*)session::driver);
		else
		{
			List<Stringz> names;
			List<Stringz> values;
			if (db->DriverInfos(names, values))
			{
				ListIterator<Stringz> lstnames(names);
				ListIterator<Stringz> lstvalues(values);
				while (!lstvalues.End())
				{
					if (!lstnames.End())
					{
						Stringz &name = lstnames.Next();
						shell->Write((_CHAR*)name);
						for (_BYTE i=1; i<=(3-(name.Len()/8)); i++)
							shell->Write("\t");
						shell->Write(": ");
					}
					else
						shell->Write("\t\t  ");
					if (shell->Write("%s\n", (_CHAR*)lstvalues.Next())) break;
				}
			}
		}
		delete db;
	}
}
