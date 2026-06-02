/*
(c) 1999-2002 Henri Michelon

$Id: sqlcmd.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include <nmo/addons/Shell.hpp>

#include "LC/LC.hpp"
#include "consts.hpp"
#include "sqlcmd.hpp"
#include "session.hpp"
#include "commands.hpp"
#include "statements.hpp"

using namespace NMO;

DebugOutput(NULL);
CreateApplication(sqlcmd, "SQL Command");


//////////////////////////////////////////////
sqlcmd::sqlcmd(const UStringz&N): ShellApplication(N, "sql")
{
	_sVersion = LC_SQL_VERSION;
	_sStartupBanner.Format(LC_SQL_BANNER, (_CHAR*)_sVersion);

	_commands->Add(new CmdOpen);
	_commands->Add(new CmdClose);
	_commands->Add(new CmdShow);
	_commands->Add(new CmdDesc);
	_commands->Add(new StmtCreate);
	_commands->Add(new StmtSelect);
	_commands->Add(new StmtInsert);
	_commands->Add(new StmtUpdate);
	_commands->Add(new StmtDrop);
	_commands->Add(new StmtAlter);
	_commands->Add(new StmtDelete);
}


//////////////////////////////////////////////
void sqlcmd::OnInit()
{
	Write(LC_SQL_AVAILCONNECT);
	List<Stringz> &drivers = DBDatabase::Drivers();
	ListIterator<Stringz> lstdrivers(drivers);
	while (!lstdrivers.End()) Write("%s ", (_CHAR*)lstdrivers.Next());
	Write("\n");
}


//////////////////////////////////////////////
void sqlcmd::OnDone()
{
	if (session::database) delete session::database;
}


//////////////////////////////////////////////
void sqlcmd::Execute(const Stringz&CMD, System::Args&, System::ExecMode )
{
	Write(LC_SQL_UNKCOMMAND, (_CHAR*)CMD);
}


//////////////////////////////////////////////
void sqlcmd::ExtraCompletion(Stringz&, Stringz&, List<Stringz>&)
{
}


//////////////////////////////////////////////
void sqlcmd::DisplayPrompt()
{
	if (session::database)
		Write("%s:%s:%s>", SQL_DEFPROMT, (_CHAR*)session::driver,
			  (_CHAR*)session::database->Name());
	else
		Write("%s>", SQL_DEFPROMT);
}
