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

$Id: UStringz.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

//------------------------------------------------------
UStringz& UStringz::From(UChar::CharacterSetName CSET)
{
	if (CSET == "USASCII") {
		UChar *tmp = Str;
		while (tmp->_char) { 
			tmp->_char = tmp->_char & 0xFF;
			tmp++;
		}
	}
	else if (CSET == "ISO8859_1") {
		FromLatin1();
	}
	else {
		ListIterator<UChar::CharacterSet> lstsets(UChar::mappings.sets);
		while (!lstsets.End()) {
			UChar::CharacterSet&set = lstsets.Next();
			if (set.name == CSET) {
				UChar *tmp = Str;
				while (tmp->_char) { 
					tmp->_char = set.char2uni[tmp->_char];
					tmp++;
				}
				break;
			}
		}
	}
	return *this;
}


//------------------------------------------------------
UStringz& UStringz::To(UChar::CharacterSetName CSET)
{
	if (CSET == "USASCII") {
		UChar *tmp = Str;
		while (tmp->_char) { 
			tmp->_char = tmp->_char & 0xFF;
			tmp++;
		}
	}
	else if (CSET == "ISO8859_1") {
		ToLatin1();
	}
	else {
		ListIterator<UChar::CharacterSet> lstsets(UChar::mappings.sets);
		while (!lstsets.End()) {
			UChar::CharacterSet&set = lstsets.Next();
			if (set.name == CSET) {
				UChar *tmp = Str;
				while (tmp->_char) { 
					tmp->_char = set.uni2char[tmp->_char];
					tmp++;
				}
				break;
			}
		}
	}
	return *this;
}


//------------------------------------------------------
UStringz& UStringz::FromLatin1()
{
	if (Str) {
		UChar *tmp = Str;
		while (tmp->_char) { 
			tmp->_char = tmp->_char & 0xFF;
			tmp++;
		}
	}
	return *this;
}


//------------------------------------------------------
UStringz& UStringz::ToLatin1()
{
	if (Str) {
		UChar *tmp = Str;
		while (tmp->_char) { 
			tmp->_char = tmp->_char & 0xFF;
			tmp++;
		}
	}
	return *this;
}


//------------------------------------------------------
void UStringz::Join(List<UStringz>&LIST, UChar C) {
	if (Str) { 
		delete []Str; 
		len = 0;
		Str = NULL;
	}	
	ListIterator<UStringz> list(LIST);
	while (!list.End()) {
		*this += list.Next();
		if (!list.End()) { *this += C; }
	}
}


//------------------------------------------------------
void UStringz::Split(List<UStringz>&LIST, UChar C) const {
	_LONG pos;
	_LONG start = 0;
	while ((pos = InStr(C, start)) != -1) {
		LIST.Add(new UStringz(Copy(start, pos - start)));
		start = pos+1;
	}
	LIST.Add(new UStringz(Right(Len() - start)));
}


//------------------------------------------------------
UStringz& UStringz::Trim()
{
	RTrim();
	return LTrim();
}


//------------------------------------------------------
UStringz& UStringz::LTrim()
{
	if (Str) {
		UChar *tmp;
		while (Len() && (Str[0]._char <= _WORD(0x0020))) {
			tmp = new UChar[Len()];
			if (tmp) Strcpy(tmp, &Str[1]);
			delete []Str;
			Str = tmp;
			len--;
		}
	}
	return *this;
}


//------------------------------------------------------
UStringz& UStringz::RTrim()
{
	if (Str) {
		while (Len() && (Str[Len()-1]._char <= _WORD(0x0020))) {
			Str[Len()-1]._char = 0;
			len--;
		}
	}
	return *this;
}


//------------------------------------------------------
/*void UStringz :: LCase()
{
	if (Len()) {
		for (_DWORD i = 0; i <= Len(); i++) {
			Str[i] = UChar(tolower(Str[i]));
		}
	}
}


//------------------------------------------------------
void UStringz :: UCase()
{
	if (!Len()) return;
	for (_DWORD i = 0; i <= Len(); i++) {
		Str[i] = UChar(toupper(Str[i]));
	}
}*/


//------------------------------------------------------
_LONG UStringz :: InStr(const UChar& c, _LONG start) const
{
	if (Str && Len()) {
		if (start < 0) {
			for (_DWORD i = Len(); i > 0; i--) {
				if (Str[i-1] == c) {
					return i-1;
				}
			}
		}
		else {
			for (_DWORD i = start; i < Len(); i++) {
				if (Str[i] == c) {
					return i;
				}
			}
		}
	}
	return -1;
}


//------------------------------------------------------
_LONG UStringz :: InStr(const UStringz&S, _LONG start) const
{
	if (Str && S.Str && (Len() > S.Len())) {
		_DWORD max = Len()-S.Len();
		for (_DWORD i = start; i < max; i++) {
			_BOOL found = TRUE;
			for (_DWORD j = 0; j < S.Len(); j++) {
				if (_WORD(Str[i+j]) != S.Str[j]) {
					found = FALSE;
					break;
				}
			}
			if (found) { return i; }
		}
	}
	return -1;
}



//------------------------------------------------------
UStringz :: UStringz(const UChar *S)
{
	Str = NULL;
	len = 0;
	if (S) {
		len = Strlen(S);
		if (len) {
			Str = new UChar[len + 1];
			Strcpy(Str, S);
		}
	}
}


//------------------------------------------------------
UStringz :: UStringz(const _WORD *S)
{
	Str = NULL;
	len = 0;
	if (S) {
		len = Strlen(S);
		if (len) {
			Str = new UChar[len + 1];
			Strcpy(Str, S);
		}
	}
}


//------------------------------------------------------
UStringz :: UStringz(const _CHAR *S)
{
	Str = NULL;
	len = 0;
	if (S && S[0]) {
		len = Stringz::Strlen(S);
		if (len) {
			Str = new UChar[len + 1];
			_CHAR *tmp = (_CHAR*)S;
			UChar *dst = Str;
			while ( ((dst++)->_char = (*(tmp++))) ) {};
		}
	}
}


//------------------------------------------------------
UStringz :: ~UStringz()
{
	if (Str) { delete []Str; }
}


//------------------------------------------------------
UStringz :: UStringz(UChar S)
{
	Str = new UChar[2];
	Str[0]._char = S._char;
	Str[1]._char = _WORD(0x0000);
	len = 1;
}


//------------------------------------------------------
UStringz :: UStringz(const UStringz &S)
{
	if (S.Str && S.Len()) {
		len = S.Len();
		Str = new UChar[len + 1];
		Strcpy(Str, S.Str);
	}
	else {
		Str = NULL;
		len = 0;
	}
}


//------------------------------------------------------
UStringz UStringz::Copy(_DWORD start, _DWORD L) const
{
	ASSERTMSG(start <= Len(), 
			  "UStringz::Copy: start after the end of the string");
	if (!L) { return UStringz(); }
	L = MIN(Len() - start, L);
	L = MIN(L, (_DWORD)Len());
	UChar *res = new UChar[L+1];
	Strncpy(res, &Str[start], L);
	res[L]._char = _WORD(0x0000);
	UStringz r = res;
	delete [] res;
	return UStringz(r);
}


//------------------------------------------------------
UStringz :: UStringz()
{
	Str = NULL;
	len = 0;
}


//------------------------------------------------------
UStringz UStringz :: Right(_DWORD L) const
{ return Copy(Len()-L, L); }


//------------------------------------------------------
UStringz UStringz :: Left(_DWORD L) const
{ return Copy(0, L); }


//------------------------------------------------------
_DWORD UStringz::Strlen(const UChar*STR) const
{
	if (STR) {
		_DWORD L = 0;
		while (STR[L++]._char > _WORD(0x0000)) {};
		return (L-1);
	}
	return 0;
}


//------------------------------------------------------
void UStringz::Strcpy(const UChar*DST, const UChar*SRC) const
{
	if (DST && SRC) {
		UChar *src = (UChar*)SRC;
		UChar *dst = (UChar*)DST;
		while ( ( ((dst++)->_char) = ((src++)->_char) ) != _WORD(0x0000) ) {};
	}
}


//------------------------------------------------------
_DWORD UStringz::Strlen(const _WORD*STR) const
{
	if (STR) {
		_DWORD L = 0;
		while (STR[L++] > _WORD(0x0000)) {};
		return (L-1);
	}
	return 0;
}


//------------------------------------------------------
void UStringz::Strcpy(const UChar*DST, const _WORD*SRC) const
{
	if (DST && SRC) {
		_WORD *src = (_WORD*)SRC;
		UChar *dst = (UChar*)DST;
		while ( ( ((dst++)->_char) = (*(src++)) ) != _WORD(0x0000) ) {};
	}
}


//------------------------------------------------------
void UStringz::Strncpy(const UChar*DST, const UChar*SRC, _DWORD L) const
{
	if (DST && SRC) {
		UChar *src = (UChar*)SRC;
		UChar *dst = (UChar*)DST;
		while ( (L--) && ( ( ((dst++)->_char) = ((src++)->_char) ) != _WORD(0x0000) ) ) {};
	}
}


//------------------------------------------------------
_LONG UStringz::Strcmp(const UChar*A, const UChar*B) 
{
	if (!A) {
		return (B ? -1 : 0);
	}
	else if (!B) {
		return (A ? 1 : 0);
	}
	else {
		UChar *a = (UChar*)A;
		UChar *b = (UChar*)B;
		while ( (a->_char) && (b->_char) ) {
			if (a->_char < b->_char) { return -1; }
			if ((a++)->_char > (b++)->_char) { return 1; }
		}
		if (a->_char) { return 1; }
		if (b->_char) { return -1; }
		return 0;
	}
}


//------------------------------------------------------
void UStringz::Strcat(const UChar*DST, const UChar*SRC) const
{
	if (DST && SRC) {
		UChar *src = (UChar*)SRC;
		UChar *dst = (UChar*)DST;
		dst += Strlen(dst);
		while ( ( ((dst++)->_char) = ((src++)->_char) ) != _WORD(0x0000) ) {};
	}
}


//------------------------------------------------------
void UStringz::Strcat(const UChar*DST, const _WORD*SRC) const
{
	if (DST && SRC) {
		_WORD *src = (_WORD*)SRC;
		UChar *dst = (UChar*)DST;
		dst += Strlen(dst);
		while ( ( ((dst++)->_char) = (*(src++)) ) != _WORD(0x0000) ) {};
	}
}


namespace NMO {


//------------------------------------------------------
UStringz operator + (const UStringz &S1, const UStringz &S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
UStringz operator + (const UStringz &S1, const UChar *S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
UStringz operator + (const UChar *S1, const UStringz &S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
UStringz operator + (UChar S1, const UStringz &S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
UStringz operator + (const UStringz &S1, UChar S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}



//------------------------------------------------------
UStringz operator + (const UStringz &S1, const _WORD *S2)
{
	UStringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
const UStringz &UStringz :: operator += (const UChar *S)
{
	if (S) {
		if (!Str) {
			*this = S;
		}
		else {
			len = Strlen(S) + Len();
			UChar *N = new UChar[len + 1];
			Strcpy(N, Str);
			Strcat(N, S);
			if (Str) { delete []Str; }
			Str = N;
		}
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator += (const _WORD *S)
{
	if (S) {
		if (!Str) {
			*this = S;
		}
		else {
			len = Strlen(S) + Len();
			UChar *N = new UChar[len + 1];
			Strcpy(N, Str);
			Strcat(N, S);
			if (Str) { delete []Str; }
			Str = N;
		}
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator += (UChar S)
{
	if (!Str) {
		*this = S;
	}
	else {
		len ++;
		UChar *N = new UChar[len + 1];
		Strcpy(N, Str);
		N[len-1] = S;
		N[len]._char = _WORD(0x0000);
		if (Str) delete []Str;
		Str = N;
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator += (const UStringz &S)
{
	if (!Str) {
		*this = S;
	}
	else {
		len = Len() + S.Len();
		UChar *N = new UChar[len + 1];
		Strcpy(N, Str);
		Strcat(N, S.Str);
		if (Str) delete []Str;
		Str = N;
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator = (const UChar *S)
{
	if (Str) {
		delete []Str;
		len = 0;
		Str = NULL;
	}
	if (S) {
		len = Strlen(S);
		Str = new UChar[len + 1];
		Strcpy(Str, S);
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator = (const _WORD *S)
{
	if (Str) {
		delete []Str;
		len = 0;
		Str = NULL;
	}
	if (S) {
		len = Strlen(S);
		Str = new UChar[len + 1];
		Strcpy(Str, S);
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator = (const UStringz &S)
{
	if (Str == S.Str) { return *this; }
	if (Str) { 
		delete []Str; 
		len = 0;
		Str = NULL;
	}
	len = S.Len();
	if (S.Str && len) {
		Str = new UChar[len + 1];
		Strcpy(Str, S.Str);
	}
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator = (UChar S)
{
	if (Str) { delete []Str; }
	len = 1;
	Str = new UChar[2];
	Str[0]._char = S;
	Str[1]._char = _WORD(0x0000);
	return *this;
}


//------------------------------------------------------
const UStringz &UStringz :: operator = (const Stringz &S)
{
	if (Str) { delete []Str; }
	len = S.Len();
	if (len > 0) {
		Str = new UChar[len + 1];
		_CHAR *src = (_CHAR*)S;
		UChar *dst = Str;
		while ( ( ((dst++)->_char) = _WORD(*(src++)) ) != _WORD(0x0000) ) {};
	}
	else {
		Str = NULL;
	}
	return *this;
}


//------------------------------------------------------
UStringz :: operator Stringz () const
{
	if (Str) {
		_CHAR *tmp = new _CHAR[Len() + 1];
		_CHAR *dst = tmp;
		UChar *src = Str;
		while ( ((*(dst++)) = _BYTE(((src++)->_char) & 0xFF)) ) {};
		Stringz res = tmp;
		delete []tmp;
		return res;
	}
	else {
		return "";
	}
}


} // namespace NMO
