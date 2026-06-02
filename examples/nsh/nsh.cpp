/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: nsh.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/Shell.hpp>
#include <nmo/LC/LC.hpp>

#include "nsh.hpp"
#include "LC/LC.hpp"
#include "cmd_dir.hpp"
#include "cmd_file.hpp"

using namespace NMO;


// Environnement variable for prompt configuration
const char PSPROMPT[] = "NPROMPT";

// Default prompt
const char DEFAULTPSPROMPT[] = "[\\n]\\w>";

DebugOutput(NULL);
CreateApplication(nsh, "NMO Shell");

//////////////////////////////////////////////
nsh::nsh(const UStringz&N): ShellApplication(N), _dwBackground(0)
{
	ASSERT(_commands);
	_sVersion = LC_NSH_VERSION;
	_sStartupBanner.Format(LC_NSH_BANNER, (_CHAR*)_sVersion, LC_SHELL_VERSION);

	_commands->Add(new CmdLs);
// use OS rm until rm -R is written
#ifndef _POSIX
	_commands->Add(new CmdRm);
#endif
	_commands->Add(new CmdCat);
	_commands->Add(new CmdType);
	_commands->Add(new CmdDel);
	//_commands->Add(new CmdWl);
	_commands->Add(new CmdPwd);
	_commands->Add(new CmdChdir);
	_commands->Add(new CmdCd);
	_commands->Add(new CmdRmdir);
	_commands->Add(new CmdRd);
	_commands->Add(new CmdMkdir);
	_commands->Add(new CmdMd);
	_commands->Add(new CmdPushd);
	_commands->Add(new CmdPopd);
}


//////////////////////////////////////////////
void nsh::Execute(const Stringz&CMD, System::Args&ARGS, System::ExecMode MODE)
{
	Stringz cmd(CMD);
	_BOOL found ;

	found = File::Exists(cmd);
	if (!found)
	{
		found = InPath(cmd);
#ifdef _WIN32
		if (!found) {
			Stringz EXEEXT = ".exe";
			found = InPath(cmd + EXEEXT);
			if (!found) {
				EXEEXT = ".com";
				found = InPath(cmd + EXEEXT);
			}
			if (!found) {
				EXEEXT = ".bat";
				found = InPath(cmd + EXEEXT);
			}
		}
#endif
	}
	if (found)
	{
		if (Dir::Exists(CMD))
			Dir::Change(CMD);
		else
		{
			_PID pid;
			System::Execute(cmd, ARGS, MODE, &pid);
			if (MODE == System::EXEC_BACKGROUND)
			{
				_dwBackground++;
				Write((char*)LC_NSH_STARTPID, _dwBackground, pid);
			}
			SetName(Name());
		}
	}
	else
		Write((char*)LC_NSH_ERRCMD, (char*)cmd);
}


//////////////////////////////////////////////
_BOOL nsh::InPath(Stringz&CMD)
{
	Stringz cmd;
	if (CMD.InStr(PATHSEP) == -1)
	{
		cmd = Dir::Current();
		cmd += PATHSEP;
		cmd += CMD;
	}
	else
		cmd = CMD;
	if (File::Exists(cmd)) return TRUE;

	_LONG pos;
	Stringz path;
	if (!Env::Read("PATH", path)) return FALSE;
	while (path.Len())
	{
		//Write("path: %s\n", (char*)path);
		pos = path.InStr(VARSEP);
		if (pos != -1)
		{
			cmd = path.Copy(0, pos);
			path = path.Copy(pos+1, path.Len()-pos);
		}
		else
		{
			cmd = path;
			path = "";
		}
		cmd += PATHSEP + CMD;
		if (File::Exists(cmd)) 
		{
			CMD = cmd;
			return TRUE;
		}
	}
	return FALSE;
}


//////////////////////////////////////////////
void nsh::ExtraCompletion(Stringz&line, Stringz&testline, List<Stringz>&names)
{
	DirSearch::FindData	findData;
	DirSearch	dirSearch;

	Stringz path;
	_LONG pos = line.InStr(' ', -1);
	if (pos != -1)
		path = line.Copy(pos+1, line.Len()-pos-1);
	else 
		path = line;
	if (!path.Len())
	{
		path = Dir::Current();
		if (path[path.Len()-1] != PATHSEP)
			path += PATHSEP;
	}
	pos = path.InStr(PATHSEP, -1);
	if (pos == -1) {
		testline = path;
		path = Dir::Current();
	}
	else  {
		testline = path.Copy(pos+1, path.Len()-pos-1);
		if (path.Len() > 1) {path = path.Copy(0, pos+1); }
	}

	_BOOL found = dirSearch.FindFirstDir(path, findData);
	while (found)
	{
		names.Add(new Stringz(findData.fileName + PATHSEP));
		found = dirSearch.FindNextDir(findData);
	}
	dirSearch.FindCloseDir(findData);

	if (path[path.Len()-1] != PATHSEP)
			path += PATHSEP;
	path += ALLFILES;
	//dprintf("\npath: %s\n", (char*)path);
	found = dirSearch.FindFirstFile(path, findData);
	while (found)
	{
		names.Add(new Stringz(findData.fileName));
		found = dirSearch.FindNextFile(findData);
	}
	dirSearch.FindCloseFile(findData);
}


//////////////////////////////////////////////
void nsh::ExpandCommandLine(System::Args&ARGS)
{
	System::Args args;
	ListIterator<Stringz> lst(ARGS);
	lst.Next();
	while (!lst.End())
	{
		Stringz &arg = lst.Current();
		if ((arg.InStr('*') != -1) || 
#ifdef _AMIGA
			(arg.InStr('#') != -1) ||
#endif
			(arg.InStr('?') != -1))
		{
			args.Add(new Stringz(arg));
			args.Drop(lst.Current());
		}
		else
			lst.Next();
	}
	lst = args;
	while (!lst.End())
	{
		Stringz &arg = lst.Next();

		_LONG		pos;
			Stringz		path;
			Stringz		ppath;
			DirSearch::FindData	findData;
			DirSearch	dirSearch;

			pos = arg.InStr(PATHSEP, -1);
			if (pos == -1)
			{
				ppath = Dir::Current() + PATHSEP;
				path = ppath + arg;
			}
			else
			{
				ppath = arg.Copy(0, pos+1);
				path = arg;
			}

			_BOOL found = dirSearch.FindFirstDir(path, findData);
			while (found)
			{
				ARGS.Add(new Stringz(ppath + findData.fileName));
				found = dirSearch.FindNextDir(findData);
			}
			dirSearch.FindCloseDir(findData);

			found = dirSearch.FindFirstFile(path, findData);
			while (found)
			{
				ARGS.Add(new Stringz(ppath + findData.fileName));
				found = dirSearch.FindNextFile(findData);
			}
			dirSearch.FindCloseFile(findData);
	}
}


//////////////////////////////////////////////
void nsh::DisplayPrompt()
{
	Stringz psprompt(DEFAULTPSPROMPT);
	Env::Read(PSPROMPT, psprompt);

	char prev = ' ';
	_LONG nchar=0;
	_LONG len = psprompt.Len();
	while (nchar < len)
	{
		char c = psprompt[nchar]; 
		switch (c)
		{
		case '"':
			if (prev == '"')
				Write("%c", c);
			break;
		case '\'':
			if (prev == '\'')
				Write("%c", c);
			break;
		case '\\':
			nchar++;
			if (nchar >= len) break;
			switch (psprompt[nchar])
			{
			case 's':
				Write(_sStartupBanner);
				break;
			case 'n':
				Write(_sNickName);
				break;
			case 'v':
				Write(_sVersion);
				break;
			case 'o':
				Write(SysInfo::OSName());
				break;
			case 'r':
				Write(SysInfo::OSRelease());
				break;
			case 'm':
				Write(SysInfo::Hardware());
				break;
			case 'u':
				Write(SysInfo::Username());
				break;
			case 'h':
				Write(SysInfo::Hostname());
				break;
			case '\\':
				Write("%c", c);
				break;
			case 'w':
				Write((char*)Dir::Current());
				break;
			case 'd':
					Write("%s\n", (_CHAR*)DateTime::Date::Current().Format("$w $dd $mmm $yy"));
				break;
			case 't':
					Write("%s\n", (_CHAR*)DateTime::Time::Current().Format("$HH:$MM:$SS"));
				break;
			default:
				break;
			}
			break;
		default:
			Write("%c", c);
			break;
		}
		nchar++;
	}
}

