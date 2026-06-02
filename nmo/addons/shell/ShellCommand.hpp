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

*/
#ifndef _SHELL_COMMAND_H_
#define _SHELL_COMMAND_H_

namespace NMO {

class ShellApplication;

/*!
\brief		Shell add-on, Shell bundled commands
				
			For aliases, environement vars and shell control

\author		(c) 1998-2002 Henri Michelon
\version	$Id: ShellCommand.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ShellCommand: public NMOObject
{
public:
	ShellApplication *shell;

	ShellCommand(const Stringz&, const char*, const Stringz&);
	virtual ~ShellCommand() {};

	Stringz& Name();
	const Stringz& Name() const;
	const Stringz& Alias() const;
	const char* Help() const;
	virtual void Execute(ListIterator<Stringz>&) {};


private:
	Stringz	_sName;
	Stringz	_sAlias;
	char	*_sHelp;
};


class ShellCommands: public List<ShellCommand>
{
public:
	ShellCommands(const ShellApplication&);

	_DWORD Add(ShellCommand*);

private:
	ShellApplication *shell;
};


#define CMD(n, x, h) class x: public ShellCommand { public: x(const Stringz& N = n, const char*H=h, const Stringz&A=n): ShellCommand(N, H, A) {}; void Execute(ListIterator<Stringz>&); };
#define ALIAS(n, x, y) class x: public y { public: x(): y(n, NULL) {}; };

CMD("?", CmdHelp, LC_HELPHELP);
CMD("exit", CmdExit, LC_HELPEXIT);
CMD("set", CmdSet, LC_HELPSET);
CMD("unset", CmdUnset, LC_HELPUNSET);
CMD("alias", CmdAlias, LC_HELPALIAS);
CMD("unalias", CmdUnalias, LC_HELPUNALIAS);
CMD("h", CmdHistory, LC_HELPHISTORY);
CMD("exec", CmdExec, LC_HELPEXEC);

ALIAS("env", CmdEnv, CmdSet);
ALIAS("logout", CmdLogout, CmdExit);
ALIAS("source", CmdSource, CmdExec);
ALIAS("history", CmdH, CmdHistory);
ALIAS("help", CmdH2, CmdHelp);

}

#endif
