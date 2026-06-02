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

$Id: ShellApplication.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include "nmo/addons/Shell.hpp"
using namespace NMO;

//------------------------------------------------------
ShellApplication::ShellApplication(const UStringz&AN, const Stringz &N): 
	ConsoleApplication(AN), _bQuit(FALSE)
{
	_sNickName = N;
	_sVersion = LC_SHELL_VERSION;
	_sStartupBanner.Format(LC_SHELL_BANNER, (_CHAR*)_sVersion);
	_commands = new ShellCommands(*this);
}

//------------------------------------------------------
void ShellApplication::OnDestroy()
{
#if (__GNUC__ == 2 && __GNUC_MINOR__ < 92)
	delete (_PTR)_commands;
#else
	delete _commands;
#endif
	Unredirect(INTERRUPT);
}


//------------------------------------------------------
void ShellApplication::OnCreate()
{
	ASSERT(_commands);

	System::Args args;
	_BOOL		keycode = FALSE;
	_BOOL		historyPassed = FALSE;
	_BOOL		completion1Pass = FALSE;
	_BOOL		completion2Pass = FALSE;
	Stringz		line = "";
	Stringz		startFileName = SysInfo::Userhome() + PATHSEP + '.' + _sNickName + '-';

	_history.FileName(startFileName + (_CHAR*)"history");
	if (_sStartupBanner.Len()) Write((_CHAR*)_sStartupBanner);

	OnInit();	
	Redirect(INTERRUPT);
	ExecuteScript(startFileName + (_CHAR*)"login");

	while (!_bQuit)
	{
		_history.Reset();
		if (!historyPassed)
		{
			DisplayPrompt();
			if (!completion1Pass) 
				line = "";
			else
				completion1Pass = FALSE;
			keycode = Readln(line);
		}
		else
			historyPassed = FALSE;

		if (keycode)
		{
			switch (line[0ul])
			{
			case TAB:
				{
					_LONG len = line.Len()-1;
					line = line.Copy(1, len);
					completion1Pass = Completion(line, completion2Pass);
					if (completion1Pass) continue;
					historyPassed = TRUE;
					MoveCursorBack(len);
					keycode = Readln(line);
					continue;
				}
				break;
			case CTRLD:
				Write("^D\n");
				_bQuit = TRUE;
				break;
			case CTRLC:
				Write("^C\n");
				break;
			case ARROW_UP:
			case ARROW_DOWN:
				historyPassed = TRUE;
				do
				{
					_DWORD len = line.Len()-1;
					if (line[0ul] == ARROW_UP)
						line = _history.Prev();
					else
						line = _history.Next();
					MoveCursorBack(len);
					for (_DWORD i=0; i<len; i++) Write(" ");
					MoveCursorBack(len);
					keycode = Readln(line);
				} while ((keycode) &&  ((line[0ul] == ARROW_UP) || 
						(line[0ul] == ARROW_DOWN)));
				continue;
			}
		}
		else if (line.Len())
		{
			Write("\n");
			if (line[0ul] == '!')
			{
				Stringz hline;
				line = line.Copy(1, line.Len()-1);	
				if (!line.Len()) continue;
				_history.Reset();
				do
				{
					hline = _history.Prev();
					if (hline.Copy(0, line.Len()) == line)
					{
						line = hline;
						break;
					}
				} while (hline.Len());
				if (hline.Len())
					Write("%s\n", (_CHAR*)line);
				else
				{
					Write(LC_SHELL_ERROREVENT, (_CHAR*)line);
					continue;
				}
			}
			_history.Reset();
			if (line != _history.Prev())
				_history.Add(line);
			Analyze(line);
		}
		else
			Write("\n");
		completion2Pass = FALSE;
	}
	ExecuteScript(startFileName + (_CHAR*)"logout");
	OnDone();
}


//------------------------------------------------------
void ShellApplication::Analyze(const Stringz& L)
{
	Stringz line = L;
	System::Args args;
	System::ExecMode mode = System::EXEC_NORMAL;

	// Expand evironnement variables
	Env::Expand(line);

	// Tokenize
	while (line.Len())
	{
		Stringz arg;
		line.Trim();
		while (line.Len() && (line[0ul] != ' '))
		{
			arg += line[0ul];
			line = line.Right(line.Len()-1);
		}
		args.Add(new Stringz(arg));
	}

	ListIterator<Stringz> lstArgs(args);
	ListIterator<ShellAlias> lstAliases(_aliases);
	lstAliases.Reset();
	while (!lstAliases.End())
	{
		ShellAlias &alias = lstAliases.Next();
		if (lstArgs[1] == alias.Name())
		{
			Stringz newline(alias.Command());
			lstArgs.Reset();
			lstArgs.Next();
			while (!lstArgs.End())
			{
				newline += " ";
				newline += lstArgs.Next();
			}
			Analyze(newline);
			return;
		}
	}

	ExpandCommandLine(args);

	ListIterator<ShellCommand> lstCommands(*_commands);
	lstCommands.Reset();
	while (!lstCommands.End())
	{
		ShellCommand &command = lstCommands.Next();
		if (command.Name() == lstArgs[1])
		{
			StartPage();
			command.Execute(lstArgs);
			StopPage();
			return;
		}
	}

#ifdef USEDRIVENAME
	if (lstArgs[1][lstArgs[1].Len()-1] == USEDRIVENAME)
	{
		if (!Dir::Change(lstArgs[1]))
			Write((char*)LC_SHELL_ERRORCHDRIVE, (char*)lstArgs[1]);
	}
	else
#endif
	{
		if (lstArgs[args.Count()] == (_CHAR*)"&")
		{
			mode = System::EXEC_BACKGROUND;
			args.Drop(args.Count());
		}
		//Unredirect(INTERRUPT);
		StartPage();
		Execute(lstArgs[1], args, mode);
		StopPage();
		//Redirect(INTERRUPT);
	}
}


//------------------------------------------------------
_BOOL ShellApplication::Completion(Stringz&line, _BOOL&completion)
{
	_BOOL r;
	_LONG pos;
	Stringz sline;
	List<Stringz> names;
	ListIterator<Stringz> lstNames(names);

	// environment variable completion
	pos = line.InStr('$');
	if (pos != -1)
	{
		sline = line.Copy(pos+1, line.Len()-pos-1);
		if (sline.InStr(' ') == -1)
		{
			Env::Environ(names);
			r = TryCompletion(sline, lstNames, completion);
			line = line.Copy(0, pos+1) + sline;
			return r;
		}
	}
	pos = line.InStr(' ', -1);
	sline = line;
	ExtraCompletion(line, sline, names);
	if (pos == -1) {
		if (line.Len() && ((sline != line) || (line[0ul] == PATHSEP))) {
			line = line.Copy(0, line.Len()-sline.Len());
			r = TryCompletion(sline, lstNames, completion);
			line += sline;
		}
		else {
			// Aliases completion
			ListIterator<ShellAlias> lstAliases(_aliases);
			lstAliases.Reset();
			while (!lstAliases.End())
				names.Add(lstAliases.Next().Name());

			// Bundled commands completion
			ListIterator<ShellCommand> lstCommands(*_commands);
			lstCommands.Reset();
			while (!lstCommands.End())
				names.Add(lstCommands.Next().Name());
			r = TryCompletion(sline, lstNames, completion);
		}
	}
	else {
		line = line.Copy(0, line.Len()-sline.Len());
		r = TryCompletion(sline, lstNames, completion);
		line += sline;
	}
	return r;
}


//------------------------------------------------------
_BOOL ShellApplication::TryCompletion(Stringz&line, Iterator<Stringz>&names, _BOOL&completion)
{
	List<Stringz> result;
	ListIterator<Stringz> results(result);

	names.Reset();
	while (!names.End())
	{
		Stringz &name = names.Next();
		if (line == name.Copy(0, line.Len()))
			result.Add(name);
	}
	if (!result.Count())
		Beep();
	else if (result.Count() == 1)
		line = results[1];
	else if (!completion)
	{
		completion = 1;
		Beep();
		for (_DWORD i=1; i<=result.Count(); i++)
		{
			Stringz name = results[i];
			for (_DWORD j=(line.Len()+1); j<name.Len(); j++)
			{
				Stringz nline = name.Copy(0, j);
				results.Reset();
				while (!results.End())
				{
					if (nline != results.Next().Copy(0, nline.Len()))
					{
						line = nline.Copy(0, j-1);
						return FALSE;
					}
				}
			}
		}
	}
	else
	{
		completion = 0;
		if (result.Count() > 100)
			Write(LC_SHELL_TOOMANY, result.Count());
		else
		{
			Write("\n");
			results.Reset();
			while (!results.End())
				Write("%s ", (_CHAR*)results.Next());
			Write("\n");
		}
		return TRUE;
	}
	return FALSE;
}


//------------------------------------------------------
_BOOL ShellApplication::ExecuteScript(const Stringz&FNAME)
{
	List<Stringz> file;
	ListIterator<Stringz> files(file);
	if (!FileTxt::Load(FNAME, file, TRUE)) return FALSE;

	files.Reset();
	while (!files.End())
	{
		Stringz &line = files.Next();
		line.Trim();
		if (line[0ul] != '#') {
			Analyze(line);
		}
	}
	return TRUE;
}


//------------------------------------------------------
ShellHistory& ShellApplication::History() 
{
	return _history;
}


//------------------------------------------------------
ShellCommands& ShellApplication::Commands() 
{
	return *_commands;
}


//------------------------------------------------------
ShellAliases& ShellApplication::Aliases()
{
	return _aliases;
}


//------------------------------------------------------
void ShellApplication::Quit()
{
	_bQuit = TRUE;
}


//------------------------------------------------------
void ShellApplication::OnSignal(SignalEvent::Signal)
{
	SendBreak();
}


//------------------------------------------------------
_BOOL ShellApplication::Readln(Stringz&R)
{
	char	c;
	Stringz	RRight;
	_DWORD	pos = R.Len();
	
	Write(R);

	do
	{
		if (GetChar(c))
		{
			switch (c)
			{
			case TAB:
			case ARROW_UP:
			case ARROW_DOWN:
				R = c + R;
				c = KEYCODE;
				break;
			case CTRLC:
			case CTRLD:
				R = c;
				c = KEYCODE;
				break;
			case DEL:
				if (pos == R.Len()) break;
				RRight = R.Copy(pos+1, R.Len()-pos-1);
				R = R.Copy(0, pos);
				R += RRight;
				Write(RRight);
				PutChar(' ');
				MoveCursorBack(RRight.Len()+1);
				pos--;
				break;
			case BACKSPACE:
				if (!pos) break;
				RRight = R.Copy(pos, R.Len()-pos);
				R = R.Copy(0, pos-1);
				R += RRight;
				MoveCursorBack(1);
				Write(RRight);
				PutChar(' ');
				MoveCursorBack(RRight.Len()+1);
				pos--;
				break;
			case ENTER:
				R += '\0';
				break;
			case ARROW_LEFT:
				c = ' ';
				if (!pos) break;
				PutChar('\010');
				pos--;
				break;
			case ARROW_RIGHT:				
				c = ' ';
				if (pos == (_DWORD)R.Len()) break;
				PutChar(R[pos]);
				pos++;
				break;
			}
		}
		else
		{
			RRight = R.Copy(pos, R.Len()-pos);
			R = R.Copy(0, pos);
			R += c;
			R += RRight;
			PutChar(c);
			Write(RRight);
			MoveCursorBack(RRight.Len());
			pos++;
		}
	}
	while ((c != ConsoleApplication::ENTER) && (c != KEYCODE));

	return (c == KEYCODE);
}
