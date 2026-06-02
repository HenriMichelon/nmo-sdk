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

				(c) 1999,2000 Henri Michelon

TODO: prevent recursive aliasing

$Id: ShellAlias.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include "nmo/addons/Shell.hpp"
using namespace NMO;


//-------------------------------------
Stringz& ShellAlias::Name() 
{ return _sName; };


//-------------------------------------
const Stringz& ShellAlias::Name() const
{ return _sName; };


//-------------------------------------
Stringz& ShellAlias::Command()
{ return _sCommand; };


//-------------------------------------
_BOOL ShellAlias::Set(const Stringz&NAME)
{
	_LONG	pos;
	pos = NAME.InStr('=');
	if (pos == -1) return FALSE;
	_sCommand = NAME.Copy(pos+1, NAME.Len()-pos-1).Trim();
	if (!_sCommand.Len()) return FALSE;
	if (_sCommand.Len()>1)
	{
		if ((_sCommand[(_LONG)0] == '\'') || (_sCommand[(_LONG)0l] == '"'))
		{
			_sCommand = _sCommand.Copy(1, _sCommand.Len()-1);
			if ((_sCommand[_sCommand.Len()-1] == '\'') ||
				(_sCommand[_sCommand.Len()-1] == '"'))
				_sCommand = _sCommand.Copy(0, _sCommand.Len()-1);
		}
	}
	_sName = NAME.Copy(0, pos).Trim();
	return (_sName.Len() != 0);
}
