/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: cmd_file.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/Shell.hpp>
#include <nmo/LC/LC.hpp>
#include <ctype.h>

#include "LC/LC.hpp"
#include "cmd_file.hpp"

using namespace NMO;


///////////////////////////////////////
void CmdLs::Execute(ListIterator<Stringz>&args)
{
	_DWORD nopt = 0;
	List<Stringz> names;

	opt_l = FALSE;

	args.Reset();
	args.GetList().Drop(args.Current());
	while (!args.End())
	{
		Stringz &arg = args.Next();
		if (arg[0ul] != '-')
			break;
		else
		{
			nopt++;
			switch (arg[1ul])
			{
			case 'l':
				opt_l = TRUE;
				break;
			}
		}
	}

	if ((shell->Cols() / 5) < 2) return;
	args.Reset();
	while (nopt--) args.GetList().Drop(args.Current());
	switch (args.Count())
	{
	case 0:
		ListDir(Stringz(Dir::Current()));
		break;
	case 1:
		{
			Stringz &name = args.Next();
			if ((name[name.Len()-1] == PATHSEP) || Dir::Exists(name))
				ListDir(name);
			else
				Print(name);
		}
		break;
	default:
		args.GetList().Sort();
		args.Reset();
		while (!args.End())
		{
			Stringz &name = args.Next();
			if (Print(name)) break;
		}
		break;
	}
}


///////////////////////////////////////
_BOOL CmdLs::ListDir(Stringz path)
{
	DirSearch::FindData findData;
	DirSearch dirSearch;
	List<Stringz> names;

	if (path[path.Len()-1] != PATHSEP)
		path += PATHSEP;

	_BOOL found = dirSearch.FindFirstDir(path, findData);
	while (found)
	{
		names.Add(new Stringz(path+findData.fileName));
		found = dirSearch.FindNextDir(findData);
	}
	dirSearch.FindCloseDir(findData);

	if (path[path.Len()-1] != PATHSEP)
			path += PATHSEP;

	found = dirSearch.FindFirstFile(path+ALLFILES, findData);
	while (found)
	{
		names.Add(new Stringz(path+findData.fileName));
		found = dirSearch.FindNextFile(findData);
	}
	dirSearch.FindCloseFile(findData);

	names.Sort();
	ListIterator<Stringz> lstnames(names);
	while (!lstnames.End())
	{
		Stringz &name = lstnames.Next();
		if (Print(name)) return TRUE;
	}
	if (!opt_l) shell->Write("\n");
	return FALSE;
}


///////////////////////////////////////
_BOOL CmdLs::Print(const Stringz&NAME)
{
	Stringz name;
	_BOOL e = FALSE;
	_LONG pos = NAME.InStr(PATHSEP, -1);

	if (pos != -1)
		name = NAME.Copy(pos+1, NAME.Len()-pos-1);
	else
		name = NAME;

	Stringz line;
	if (opt_l)
	{
		File::FileStats stats;
		if (File::Stats(NAME, stats))
		{
			if (stats.readAccess)
				line += 'r';
			else
				line += '-';
			if (stats.writeAccess)
				line += 'w';
			else
				line += '-';
			if (stats.execAccess)
				line += 'x';
			else
				line += '-';

#ifdef USERSECURITY
			Stringz user;
			if (stats.user.Len())
				user = stats.user.Copy(0, 8);
			else
				user = LC_NSH_EVERYONE;
			line.Format("%s  %8s", (_CHAR*)line, (_CHAR*)user);

			if (stats.group.Len())
				user = stats.group.Copy(0, 8);
			else
				user = LC_NSH_EVERYONE;
			line.Format("%s  %8s", (_CHAR*)line, (_CHAR*)user);
#endif
			line += "  ";

			DateTime::Date current = DateTime::Date::Current();
			if (stats.modifDate.date.year == current.year)
			{
				if (stats.modifDate.date.month == current.month)
				{
					_BYTE decal = current.dayOfMonth - stats.modifDate.date.dayOfMonth;
					if (decal < 7)
					{
						if (decal == 0) 
							line += LC_NSH_TODAY;
						else if (decal == 1) 
							line += LC_NSH_YESTERDAY;
						else {
							line += "        ";
							line += LC_NMO_SDAYOFWEEK[stats.modifDate.date.dayOfWeek];
						}
					}
					else {
						line += "     ";
						line += LC_NMO_SDAYOFWEEK[stats.modifDate.date.dayOfWeek];
						line.Format("%s %02d", (_CHAR*)line, stats.modifDate.date.dayOfMonth);
					}
				}
				else
					line.Format("%s %s %02d-%s", (_CHAR*)line,
								LC_NMO_SDAYOFWEEK[stats.modifDate.date.dayOfWeek],
								stats.modifDate.date.dayOfMonth,
								LC_NMO_SMONTH[stats.modifDate.date.month-1]);
			}
			else
				line.Format("%s%02d-%s-%04d", (_CHAR*)line,
							stats.modifDate.date.dayOfMonth,
							LC_NMO_SMONTH[stats.modifDate.date.month-1],
							stats.modifDate.date.year);

			line.Format("%s  %02d:%02d", (_CHAR*)line,
						stats.modifDate.time.hour,
						stats.modifDate.time.minute);

			if (stats.size < (1024*1024)) {
				line.Format("%s  %6d", (_CHAR*)line, stats.size);
			}
			else if (stats.size < 1024*1024*1024)
				line.Format("%s   %3d%s", (_CHAR*)line, stats.size / (1024*1024), LC_NMO_MB);
			else
				line.Format("%s%6d%s", (_CHAR*)line, stats.size / (1024*1024*1024), LC_NMO_GB);
			line += "  ";
			line += name;
			if (stats.isDirectory)
				line += '/';
			else if (stats.execAccess)
				line += '*';
			line += '\n';			
		}
	}
	else
	{
		_DWORD ncol = shell->Cols() / 5;
		if (name.Len() > ncol) name[ncol-1] = 0;
		ncol = ncol / 8;
		line = name;
		for (_BYTE l=1; l<=(ncol-(name.Len()/8)); l++) {
			line += '\t';
		}
	}
	return shell->Write(line);
}


///////////////////////////////////////
void CmdRm::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 2)
	{
		shell->Write(LC_USAGERM);
		return;
	}

	_BOOL opt_y = FALSE;
	_DWORD nopt = 0;

	args.Reset();
	args.GetList().Drop(args.Current());
	while (!args.End())
	{
		Stringz &arg = args.Next();
		if (arg[0ul] != '-')
			break;
		else
		{
			nopt++;
			switch (arg[1ul])
			{
			case 'y':
				opt_y = TRUE;
				break;
			}
		}
	}

	args.Reset();
	while (nopt--) args.GetList().Drop(args.Current());

	while (!args.End())
	{
		Stringz &fname = args.Next();
		if (opt_y)
		{
			_CHAR c;
			shell->Write(LC_CONFIRMRM, (_CHAR*)fname);
			_BOOL stop = shell->GetChar(c);
			shell->Write("\n");
			if (stop)
			{
				if (c == ConsoleApplication::CTRLC)
					return;
			}
			if (tolower(c) != LC_CONFIRMYES)
				continue;
		}
		if (!File::Remove(fname))
			shell->Write(LC_ERRORRM, (_CHAR*)fname);
	}
}



///////////////////////////////////////
void CmdCat::Execute(ListIterator<Stringz>&args)
{
	if (args.Count() < 2)
	{
		shell->Write(LC_USAGECAT);
		return;
	}
	File *file;
	args.Reset();
	args.Next();
	while (!args.End())
	{
		Stringz &fname = args.Next();
		file = File::Open(fname);
		if (!file)
		{
			shell->Write(LC_ERRORCAT, (_CHAR*)fname);
			return;
		}

		Stringz line;
		while (!file->Eof())
		{
			file->Readln(line);
			if (shell->Write("%s\n", (_CHAR*)line)) 
			{
				delete file;
				return;
			}
		}
		delete file;
	}
}


///////////////////////////////////////
void CmdWl::Execute(ListIterator<Stringz>&args)
{
	shell->Write("%d args\n", args.Count());
	args.Reset();
	while (!args.End())
		if (shell->Write("\t%s\n", (_CHAR*)args.Next())) return;
}
