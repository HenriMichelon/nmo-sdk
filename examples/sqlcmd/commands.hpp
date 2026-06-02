/*
(c) 1999-2002 Henri Michelon

$Id: commands.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SQLCMD_CMDSQL_H_
#define _SQLCMD_CMDSQL_H_

namespace NMO {


CMD("open", CmdOpen, LC_SQL_HELPOPEN);
CMD("close", CmdClose, LC_SQL_HELPCLOSE);
CMD("show", CmdShow, LC_SQL_HELPSHOW);
CMD("desc", CmdDesc, LC_SQL_HELPDESC);

CMD("", CmdShowTables, NULL);
CMD("", CmdShowPrivil, NULL);
CMD("", CmdShowDriver, NULL);


}

#endif
