/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2000-2001 Henri Michelon

$Id: UChar.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

UChar::CharacterSets UChar::mappings;

//---------------------------------------------------
UChar& UChar::From(CharacterSetName CSET)
{
	if (CSET == "USASCII") { 
		_char = (_char & 0x7F);
	}
	else if (CSET == "ISO8859_1") {
		_char = (_char & 0xFF);
	}
	else {
		ListIterator<CharacterSet> lstsets(mappings.sets);
		while (!lstsets.End()) {
			CharacterSet&set = lstsets.Next();
			if (set.name == CSET) {
				_char = set.char2uni[_char];
				break;
			}
		}
	}
	return *this;
}



//---------------------------------------------------
UChar& UChar::To(CharacterSetName CSET)
{ 
	if (CSET == "USASCII") { 
		_char = (_char & 0x7F);
	}
	else if (CSET == "ISO8859_1") {
		_char = (_char & 0xFF);
	}
	else {
		ListIterator<CharacterSet> lstsets(mappings.sets);
		while (!lstsets.End()) {
			CharacterSet&set = lstsets.Next();
			if (set.name == CSET) {
				_char = set.uni2char[_char];
				break;
			}
		}
	}
	return *this;
}


//---------------------------------------------------
UChar::CharacterSet::CharacterSet(const Stringz &NAME, _DWORD SIZE)
{
	_DWORD size = 1 << (SIZE*4);
	name = NAME;
	char2uni = new _WORD[size];
	Memory::Fill(char2uni, 0, size);
	Memory::Fill(uni2char, 0, 65536);
}



//---------------------------------------------------
UChar::CharacterSet::~CharacterSet()
{
	delete []char2uni;
}


//---------------------------------------------------
UChar::CharacterSets::CharacterSets()
{
	List<Stringz> file;
	if (FileTxt::Load(Dir::Current() + PATHSEP + "unicode.mappings", file)) {
		ListIterator<Stringz> list(file);
		if (!list.End()) { list.Next();  } // Skip header
		_DWORD nmaps = 0;
		if (!list.End()) { nmaps = list.Next(); } // count
		while (nmaps--) { AddSet(list);	}
	}
}


//---------------------------------------------------
void UChar::CharacterSets::AddSet(ListIterator<Stringz>&LIST)
{
	Stringz name;
	_DWORD nchars = 0;
	_DWORD size = 0;
	if (!LIST.End()) { name = LIST.Next(); }
	if (!LIST.End()) { size = LIST.Next(); }
	if (!LIST.End()) { nchars = LIST.Next(); }
	if (nchars > 0) {
		Stringz &line = LIST.Next();
		if (line.Len() == (nchars * 10)) {
			UChar::CharacterSet *set = new UChar::CharacterSet(name, size);
			Stringz l(_DWORD(10));
			while (nchars--) {
				l = line.Left(5);
				_DWORD cchar = l;
				l = line.Copy(5, 5);
				_DWORD unicode = l;
				line = line.Right(line.Len() - 10);
				set->uni2char[unicode] = (_WORD)cchar;
				set->char2uni[cchar] = (_WORD)unicode;
			}
			sets.Add(set);
		}
	}
}
