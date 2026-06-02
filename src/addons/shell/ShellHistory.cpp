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

$Id: ShellHistory.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include "nmo/addons/Shell.hpp"
using namespace NMO;

//------------------------------------------------------
ShellHistory::ShellHistory(): 
_dwCurrent(0), _sFileName("") 
{}



//------------------------------------------------------
ShellHistory::~ShellHistory()
{
	File *file = File::Open(_sFileName, File::CREATE);
	if (file)
	{
		ListIterator<Stringz> history(_history);
		history.Reset();
		_LONG nonStored = history.Count()-HISTORYMAX;
		for (;nonStored>0; nonStored--) history.Next();
		while (!history.End())
			file->Writeln("%s\012", (char*)history.Next());
		delete file;
	}
	//Debug(else dprintf("ShellHistory: can't create history file\n");)
	_history.Clear();
}


//------------------------------------------------------
void ShellHistory::FileName(const Stringz&FNAME)
{
	_sFileName = FNAME;
	File *file = File::Open(_sFileName);
	if (file) {
		Stringz line;
		while (file->Readln(line)) {
			Add(line);
		}
		delete file;
	}
}


//------------------------------------------------------
void ShellHistory::Add(Stringz &S)
{
	_DWORD i=0;
	while (i<S.Len())
	{
		if (S[i] < ' ')
			S[i] = ' ';
		i++;
	}
	_history.Add(new Stringz(S));
}


//------------------------------------------------------
const Stringz ShellHistory::Prev()
{
	if (_dwCurrent>1) {
		ListIterator<Stringz> history(_history);
		return history[--_dwCurrent];
	}
	_dwCurrent = 0;
	return "";
}
 

//------------------------------------------------------
const Stringz ShellHistory::Next()
{
	if (_dwCurrent < _history.Count()) {
		ListIterator<Stringz> history(_history);
		return history[++_dwCurrent];
	}
	_dwCurrent = _history.Count() + 1;
	return "";
}


//------------------------------------------------------
void ShellHistory::Reset(ResetFrom RF)
{
	if (RF == END)
		_dwCurrent = _history.Count()+1;
	else
		_dwCurrent = 0;
}
