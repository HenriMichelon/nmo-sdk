/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: cmd_dir.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NSH_CMD_DIR_H_
#define _NSH_CMD_DIR_H_

namespace NMO {


CMD("pwd", CmdPwd, LC_HELPPWD);
CMD("chdir", CmdChdir, LC_HELPCHDIR);
CMD("rmdir", CmdRmdir, LC_HELPRMDIR);
CMD("mkdir", CmdMkdir, LC_HELPMKDIR);
CMD("popd", CmdPopd, LC_HELPPOPD);

class CmdPushd: public ShellCommand
{
public:
	static Lifo<Stringz> pushStack;

	CmdPushd(): ShellCommand("pushd", LC_HELPPUSHD, "pushd") {};
	~CmdPushd();
	void Execute(ListIterator<Stringz>&);
};


ALIAS("cd", CmdCd, CmdChdir);
ALIAS("rd", CmdRd, CmdRmdir);
ALIAS("md", CmdMd, CmdMkdir);


}
#endif
