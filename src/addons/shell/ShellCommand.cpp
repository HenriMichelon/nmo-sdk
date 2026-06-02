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

$Id: ShellCommand.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include "nmo/addons/Shell.hpp"
using namespace NMO;

//-------------------------------------
ShellCommand::ShellCommand(const Stringz&N,const char*H, const Stringz&A):
	_sName(N), _sAlias(A)
{ _sHelp = (char*)H; }

const char* ShellCommand::Help() const
{ return _sHelp; }

const Stringz& ShellCommand::Name() const
{ return _sName; }

Stringz& ShellCommand::Name()
{ return _sName; }

const Stringz& ShellCommand::Alias() const
{ return _sAlias; }


//-------------------------------------
ShellCommands::ShellCommands(const ShellApplication&SHELL)
{
	shell = (ShellApplication*)&SHELL;
	Add(new CmdH);
	Add(new CmdAlias);
	Add(new CmdUnalias);
	Add(new CmdSet);
	Add(new CmdUnset);
	Add(new CmdExit);
	Add(new CmdLogout);
	Add(new CmdHistory);
	Add(new CmdEnv);
	Add(new CmdH2);
	Add(new CmdHelp);
	Add(new CmdExec);
	Add(new CmdSource);
}


_DWORD ShellCommands::Add(ShellCommand*CMD)
{
	ASSERT(CMD);
	ASSERT(shell);
	CMD->shell = shell;
	return List<ShellCommand>::Add(CMD);
}


//-------------------------------------
void CmdAlias::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	_BOOL found = FALSE;
	ListIterator<ShellAlias> lstAliases(shell->Aliases());
	lstAliases.Reset();
	args.Reset();
	if (!args.End())
	{
		while (!lstAliases.End())
		{
			ShellAlias &alias = lstAliases.Next();
			if (shell->Write("%s=%s\n", (char*)alias.Name(), (char*)alias.Command()))
				return;
		}
		if (shell->Write("\n%s\n", LC_SHELL_BUILTINALIAS))
			return;

		ListIterator<ShellCommand> lstCommands(shell->Commands());
		lstCommands.Reset();
		while (!lstCommands.End())
		{
			ShellCommand &command = lstCommands.Next();
			if (command.Help()== NULL)
				if (shell->Write("%s=%s\n", (_CHAR*)command.Name(), (_CHAR*)command.Alias()))
					return;
		}
	}
	else 
	{
		Stringz cmd;
		args.Next();
		while (!args.End()) {
				cmd += ' ' + args.Next();
		}
		ShellAlias newalias;
		if (!newalias.Set(cmd)) 
		{
			shell->Write(LC_USAGEALIAS);
			return;
		}
		while (!lstAliases.End())
		{
			ShellAlias &alias = lstAliases.Next();
			if (alias.Name() == newalias.Name())
			{
				found = TRUE;
				alias.Command() = newalias.Command();
				break;
			}
		}
		if (!found)
			shell->Aliases().Add(new ShellAlias(newalias));
	}
}


//-------------------------------------
void CmdUnalias::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGEUNALIAS);
	else
	{
		ListIterator<ShellAlias> lstAliases(shell->Aliases());
		lstAliases.Reset();
		while (!lstAliases.End())
		{
			ShellAlias &alias = lstAliases.Next();
			if (alias.Name() == args[2])
			{
				shell->Aliases().Drop(lstAliases.Index() - 1);
				break;
			}
		}
	}
}


//-------------------------------------
void CmdSet::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() == 1)
	{
		Stringz val;
		List<Stringz> names;
		Env::Environ(names);
		ListIterator<Stringz> lstNames(names);
		lstNames.Reset();
		while (!lstNames.End())
		{
			Stringz &name = lstNames.Next();
			if (Env::Read(name, val))
				if (shell->Write("%s=%s\n", (char*)name, (char*)val))
					return;
		}
	}
	else
	{
		Stringz cmd;
		for (_DWORD i = 2; i<=args.Count(); i++)
			cmd += ' ' + args[i];
		_LONG pos = cmd.InStr('=');
		if (pos != -1)
		{
			Stringz name = cmd.Copy(0, pos).Trim();
			if (name.Len())
			{
				Stringz val = cmd.Copy(pos+1, cmd.Len()-pos-1).Trim();
				if (val.Len())
				{
					Env::Write(name, val);
					return;
				}
			}
		}
		shell->Write(LC_USAGESET);
	}
}


//-------------------------------------
void CmdUnset::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGEUNSET);
	else
		Env::Delete(args[2]);
}


//-------------------------------------
void CmdExit::Execute(ListIterator<Stringz>&)
{
	ASSERT(shell);
	shell->Quit();
}


//-------------------------------------
void CmdHistory::Execute(ListIterator<Stringz>&)
{
	ASSERT(shell);
	ShellHistory &history = shell->History();
	history.Reset(ShellHistory::START);
	Stringz h = history.Next();
	while (h.Len())
	{
		if (shell->Write("%s\n", (char*)h)) break;
		h = history.Next();
	}
}


//-------------------------------------
void CmdHelp::Execute(ListIterator<Stringz>&)
{
	ASSERT(shell);
	ListIterator<ShellCommand> lstCommands(shell->Commands());
	lstCommands.Reset();
	while (!lstCommands.End())
	{
		ShellCommand &command = lstCommands.Next();
		if (command.Help() != NULL)
			if (shell->Write("%s\t: %s\n", (char*)command.Name(), command.Help()))
				break;
	}
}


//-------------------------------------
void CmdExec::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGEEXEC);
	else
		if (!shell->ExecuteScript(args[2]))
			shell->Write(LC_ERREXEC, (_CHAR*)args[2]);
}
