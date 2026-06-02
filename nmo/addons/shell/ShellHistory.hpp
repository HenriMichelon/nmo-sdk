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
#ifndef _SHELL_HISTORY_H_
#define _SHELL_HISTORY_H_

namespace NMO {

/*!
\brief			Shell add-on, command line history

\author		(c) 1998-2000 Henri Michelon
\version	$Id: ShellHistory.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ShellHistory: public NMOObject
{
public:
	typedef enum
	{
		START,
		END
	} ResetFrom;

	ShellHistory();
	virtual ~ShellHistory();

	void Reset(ResetFrom = END);

	// Get previous line
	// return "" if there is no more line
	const Stringz Prev();

	// Get next line
	// return "" if there is no more line
	const Stringz Next();

	// Add a new line
	// CTRL chars are replaced with ' '
	void Add(Stringz&);

	void FileName(const Stringz&);

private:
	_DWORD			_dwCurrent;
	Stringz			_sFileName;
	List<Stringz>	_history;
};

}

#endif
