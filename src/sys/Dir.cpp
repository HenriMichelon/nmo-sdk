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

	(c) 1998-2002 Henri Michelon 
				
$Id: Dir.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-----------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;


//---------------------------------------------
void Dir::Expand(Stringz&NAME)
{
	_LONG pos;
#ifdef _POSIX
	// Expand tilde char on multi users system
	pos = NAME.InStr('~');
	if (pos != -1) {
		NAME = NAME.Copy(0, pos) + SysInfo::Userhome() + NAME.Copy(pos+1, NAME.Len()-pos-1);
	}
#endif
	pos = NAME.InStr('.');
	if (pos != -1) {
		if (pos == ((_LONG)NAME.Len()-1)) {
			NAME = NAME.Copy(0, NAME.Len()-1) + Current();
		}
		else if (NAME[_LONG(pos+1)] == PATHSEP)	{
			if (pos == 0) {
				NAME = Current() + NAME.Copy(1, NAME.Len()-1);
			}
			else if (NAME[_LONG(pos-1)] == PATHSEP) {
				NAME = NAME.Copy(0, pos) + Current() + NAME.Copy(pos+1, NAME.Len()-pos-1);
			}
		}
	}
}


//------------------------------------------------------
Stringz Dir::CleanFilePath(const Stringz& path)
{
	Stringz p(path);
	_LONG pos = p.InStr(PATHSEP);
	if (pos<=0) {
		return p;
	}
	else {
		Stringz res = p.Copy(pos, p.Len()-pos);
		if (res[res.Len()] == PATHSEP) {
			return res.Copy(0, res.Len()-1);
		}
		else {
			return res;
		}
	}
}


//---------------------------------------------
_BOOL Dir::CreateAll(const Stringz&PATH)
{
	_BOOL result = TRUE;
	List<Stringz> components;
	PATH.Split(components, PATHSEP);
	ListIterator<Stringz> lstpath(components);
	Stringz path(_DWORD(PATH.Len() + 1));
#ifdef USEDRIVENAME
	if (lstpath.Count() > 0) {
		path = lstpath.Next();
		_LONG pos = path.InStr(USEDRIVENAME);
		if (pos != (_LONG(path.Len()) - 1)) { Create(path.Right(path.Len() - pos)); }
	}
#endif
	while (!lstpath.End()) {
		if (path.Len() > 0) { path += PATHSEP; }
		path += lstpath.Next();
		result &= Create(path);
	}
	return result;
}
