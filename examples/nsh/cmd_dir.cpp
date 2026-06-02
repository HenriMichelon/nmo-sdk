/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: cmd_dir.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/Shell.hpp>
#include "LC/LC.hpp"
#include "cmd_dir.hpp"

using namespace NMO;

Lifo<Stringz> CmdPushd::pushStack;

///////////////////////////////////////
void CmdPwd::Execute(ListIterator<Stringz>&)
{
	ASSERT(shell);
	shell->Write("%s\n", (_CHAR*)Dir::Current());
}


///////////////////////////////////////
void CmdChdir::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	Stringz dir;
	if (args.Count() > 1) {
		args.GetList().Drop(1);
		dir.Join(args.GetList(), ' ');
	}
	else {
		dir = SysInfo::Userhome();
	}
	if (!Dir::Change(dir)) {
		shell->Write(LC_ERRORCHDIR, (_CHAR*)dir);
	}
}


///////////////////////////////////////
void CmdRmdir::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGERMDIR);
	else
		if (!Dir::Remove(args[2]))
			shell->Write(LC_ERRORRMDIR, (_CHAR*)args[2]);
}


///////////////////////////////////////
void CmdMkdir::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGEMKDIR);
	else
		if (!Dir::Create(args[2]))
			shell->Write(LC_ERRORMKDIR, (_CHAR*)args[2]);
}


///////////////////////////////////////
CmdPushd::~CmdPushd()
{
	while (!pushStack.Empty())
		delete &pushStack.Pop();
}


///////////////////////////////////////
void CmdPushd::Execute(ListIterator<Stringz>&args)
{
	ASSERT(shell);
	if (args.Count() != 2)
		shell->Write(LC_USAGEPUSHD);
	else
	{
		Dir::Expand(args[2]);
		pushStack.Push(*(new Stringz(args[2])));
		shell->Write("%s\n", (_CHAR*)args[2]);
	}
}


///////////////////////////////////////
void CmdPopd::Execute(ListIterator<Stringz>&)
{
	ASSERT(shell);
	if (CmdPushd::pushStack.Empty())
		shell->Write(LC_ERRORPOPD);
	else
	{
		Stringz &dir = CmdPushd::pushStack.Get();
		shell->Write("%s\n", (_CHAR*)dir);
		if (!Dir::Change(dir))
			shell->Write(LC_ERRORCHDIR, (_CHAR*)dir);
		delete &CmdPushd::pushStack.Pop();
	}
}
