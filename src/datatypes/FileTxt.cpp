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

$Id: FileTxt.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/datatypes/FileTxt.hpp>
using namespace NMO;


//------------------------------------------------
_BOOL FileTxt::Load(const Stringz &NAME, List<Stringz> &LIST, _BOOL FILTER)
{
	File *file = File::Open(NAME, File::READ);
	if (!file) return FALSE;
	//dprintf("FileTxt: opening %s\n", (_CHAR*)NAME);

	_BOOL lastiseol = FALSE;
	Stringz string(_DWORD(160));
	while (!file->Eof()) {
		_CHAR c;
		if (file->Read(&c, sizeof(_CHAR)) != _LONG(sizeof(_CHAR))) {
			break;
		}
		if (((c == 10) || (c == 13)) && (!lastiseol)) {
			if (FILTER) {
				string.Trim();
				if (string.Len())
					LIST.Add(new Stringz(string));
			}
			else {
				LIST.Add(new Stringz(string));
			}
			string = "";
			lastiseol = TRUE;
		}
		else {
			lastiseol = FALSE;
			string += c;
		}
	}
	if (FILTER) {
		string.Trim();
		if (string.Len()) {
			LIST.Add(new Stringz(string));
		}
	}
	else {
		LIST.Add(new Stringz(string));
	}
	delete file;
	return TRUE;
}



//------------------------------------------------
_BOOL FileTxt::Save(const Stringz &NAME, Iterator<Stringz> &LIST)
{
	File *file = File::Open(NAME, File::CREATE);
	if (!file) return FALSE;

	LIST.Reset();
	while (!LIST.End()) {
		file->Writeln("%s\n", (_CHAR*)LIST.Next());
	}
	delete file;
	return TRUE;
}

