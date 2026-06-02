/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: cmd_file.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NSH_CMD_LS_H_
#define _NSH_CMD_LS_H_

namespace NMO {


class CmdLs: public ShellCommand
{
public:
	CmdLs(): ShellCommand("ls", LC_HELPLS, "ls") {};
	void Execute(ListIterator<Stringz>&);

private:
	_BOOL opt_l;

	_BOOL ListDir(Stringz);
	_BOOL Print(const Stringz&);
};

CMD("wl", CmdWl, "");
CMD("rm", CmdRm, LC_HELPRM);
CMD("cat", CmdCat, LC_HELPCAT);

ALIAS("del", CmdDel, CmdRm);
ALIAS("type", CmdType, CmdCat);

}

#endif
