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

#ifndef _SHELL_APP_H_
#define _SHELL_APP_H_

namespace NMO {

/*!

\brief			Shell addon application super class

\author		(c) 1998-2000 Henri Michelon

\version	$Id: ShellApplication.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/

//--------------------------------------------------------------------------

class ShellApplication: public ConsoleApplication, private SignalEvent
{
public:
	ShellCommands& Commands();
	ShellAliases& Aliases();
	ShellHistory& History();

	// Force the shell to exit
	// (send a CTRL-D)
	void Quit();

	// Execute a script
	//	String	: file name
	_BOOL ExecuteScript(const Stringz&);

protected:
	_BOOL			_bQuit;
	Stringz			_sStartupBanner;
	Stringz			_sVersion;
	Stringz			_sNickName;
	ShellAliases	_aliases;
	ShellHistory	_history;
	ShellCommands	*_commands;

	// Initialize the shell with a nickname
	// this nick name is used for aliases,
	// history, login and logout files names.
	//	String	: application name
	//	String	: nickname
	ShellApplication(const UStringz&, const Stringz& = DEFAULTNICK);

	void OnCreate();
	void OnDestroy();

	// Analyze command line
	// Parse line, replace aliases
	// and execute command
	//	_String	: command line
	void Analyze(const Stringz&);

	// Read a line from the user input
	// return TRUE if a control key have been pressed	
	//	String	: (input/output) command line
	_BOOL Readln(Stringz&R);

private:

	// Complete command line on TAB press
	//	String	: line to complete
	//	_BOOL	: (input/output) comletion pass
	//	return TRUE if line have been modified
	_BOOL Completion(Stringz&, _BOOL&);

	// Try to complete a line with one of the string of the list
	//	String	: (input/output) line to complete
	//	Iterator: (input) list if string for completion
	//	_BOOL	: (input/output) completion pass
	_BOOL TryCompletion(Stringz&, Iterator<Stringz>&, _BOOL&);

	// Signal handler for CTRL-C
	void OnSignal(Signal);

	// replace OnCreate() event
	virtual void OnInit() {};

	// replace OnDestroy() event
	virtual void OnDone() {};

	// Display a prompt before user input
	virtual void DisplayPrompt() = 0;

	// Modify command line arguments
	virtual void ExpandCommandLine(System::Args&) {};

	// Execute an external command
	virtual void Execute(const Stringz&, System::Args&, System::ExecMode) {};

	// Fill the completion strings list
	// with external completion
	virtual void ExtraCompletion(Stringz&, Stringz&, List<Stringz>&) {};

};

}

#endif
