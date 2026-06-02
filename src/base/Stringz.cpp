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

	(c) 1998-2001 Henri Michelon

$Id: Stringz.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _POSIX
#include <string.h>
#endif
using namespace NMO;

_CHAR* Stringz :: emptystring = "";
const _DWORD STRINGZAVGSIZE = 60;

//------------------------------------------------------
void Stringz::Join(List<Stringz>&LIST, _CHAR C) {
	*this = "";
	ListIterator<Stringz> list(LIST);
	while (!list.End()) {
		*this += list.Next();
		if (!list.End()) { *this += C; }
	}
}

//------------------------------------------------------
void Stringz::Split(List<Stringz>&LIST, _CHAR C) const {
	_LONG pos;
	_LONG start = 0;
	while ((pos = InStr(C, start)) != -1) {
		LIST.Add(new Stringz(Copy(start, pos - start)));
		start = pos+1;
	}
	LIST.Add(new Stringz(Right(Len() - start)));
}


//------------------------------------------------------
Stringz& Stringz::Trim()
{
	RTrim();
	return LTrim();
}


//------------------------------------------------------
Stringz& Stringz::LTrim()
{
	ASSERT(Str);
	while (Len() && (Str[0] <= ' ')) {
		_DWORD l = Len();
		Strcpy(Str, Str + 1); // we keep the same buffer to avoid new/delete operation on small buffers
		Str[l] = 0;
	}
	return *this;
}


//------------------------------------------------------
Stringz& Stringz::RTrim()
{
	ASSERT(Str);
	while (Len() && (Str[Len()-1] <= ' '))	{
		Str[Len()-1] = 0;
	}
	return *this;
}


//------------------------------------------------------
void Stringz :: LCase()
{
	ASSERT(Str);
	if (Len()) {
		for (_DWORD i = 0; i <= Len(); i++) {
			Str[i] = _CHAR(tolower(Str[i]));
		}
	}
}


//------------------------------------------------------
void Stringz :: UCase()
{
	ASSERT(Str);
	if (Len()) {
		for (_DWORD i = 0; i <= Len(); i++) {
			Str[i] = _CHAR(toupper(Str[i]));
		}
	}
}


//------------------------------------------------------
void Stringz :: Sub(const Stringz&SRC, const Stringz&DST)
{
	ASSERT(Str);
	_DWORD srclen = SRC.Len();
	_DWORD dstlen = DST.Len();
	_LONG pos = 0;
	while ( (pos = InStr(SRC, pos)) != -1) {
		_DWORD l = Len();
		bufsize = l - srclen + dstlen + 1;
		_CHAR *tmp = new _CHAR[bufsize];
		Strncpy(tmp, Str, pos); // Left part
		Strcpy(tmp + pos, (_CHAR*)DST); // replace
		Strcpy(tmp + pos + dstlen, Str + pos + srclen); // right part
		//tmp[l + srclen + dstlen] = 0;
		delete []Str; // can't be emptystring is InStr() have found something
		Str = tmp;
	}
}


//------------------------------------------------------
void Stringz :: Sub(_CHAR SRC, _CHAR DST)
{
	ASSERT(Str);
	if (Len()) {
		for (_DWORD i = 0; i < Len(); i++) {
			if (Str[i] == SRC) {
				Str[i] = DST;
			}
		}
	}
}


//------------------------------------------------------
_LONG Stringz :: InStr(_CHAR c, _LONG start) const
{
	ASSERT(Str);
	if (Len()) {
		if (start < 0) {
			for (_DWORD i = Len(); i > 0; i--) {
				if (Str[i-1] == c) {
					return i-1;
				}
			}
		}
		else {
			for (_LONG i = start; i < (_LONG)Len(); i++) {
				if (Str[i] == c) {
					return i;
				}
			}
		}
	}
	return -1;
}


//------------------------------------------------------
_LONG Stringz :: InStr(const Stringz&S, _LONG start) const
{
	ASSERT(Str);
	ASSERT(S.Str);
	if ((Len() > S.Len()))
	{
		_DWORD max = Len()-S.Len();
		for (_DWORD i = start; i < max; i++) {
			_BOOL found = TRUE;
			for (_DWORD j = 0; j < S.Len(); j++) {
				if (Str[i+j] != S.Str[j]) {
					found = FALSE;
					break;
				}
			}
			if (found) { return i; }
		}
	}
	return -1;
}


#ifdef _MANAGED
Stringz :: Stringz(String*STR)
{
	bufsize = STR->get_Length()+1;
	Str = new _CHAR[bufsize];
	for (_DWORD idx=0; idx < STR->get_Length(); idx++) {
		Str[idx] = STR->get_Chars(idx);
	}
	Str[idx] = 0;
}
#endif


//------------------------------------------------------
Stringz :: Stringz(_CHAR *S)
{
	if (S && S[0]) {
		bufsize = (Strlen(S) + 1) + STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Strcpy(Str, S);
	}
	else {
		bufsize = STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Str[0] = 0;
	}
}


//------------------------------------------------------
Stringz :: Stringz(const _CHAR *S)
{
	if (S && S[0]) {
		bufsize = (Strlen(S) + 1) + STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Strcpy(Str, S);
	}
	else {
		bufsize = STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Str[0] = 0;
	}
}


//------------------------------------------------------
Stringz :: ~Stringz()
{
	if (Str != emptystring) { 
		delete []Str; 
		Str = emptystring;
		bufsize = 0;
	}
}


//------------------------------------------------------
Stringz :: Stringz()
{ 
	bufsize = 0;
	Str = emptystring;
}


//------------------------------------------------------
Stringz :: Stringz(_DWORD LEN)
{
	bufsize = LEN;
	Str = new _CHAR[bufsize];
	Str[0] = 0;
}


//------------------------------------------------------
Stringz :: Stringz(_CHAR S)
{
	bufsize = STRINGZAVGSIZE;
	Str = new _CHAR[bufsize];
	Str[0] = S;
	Str[1] = 0;
}


//------------------------------------------------------
Stringz :: Stringz(const Stringz &S)
{
	ASSERT(S.Str);
	if (S.Str && S.Len()) {
		bufsize = (S.Len() + 1) + STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Strcpy(Str, S.Str);
	}
	else {
		bufsize = STRINGZAVGSIZE;
		Str = new _CHAR[bufsize];
		Str[0] = 0;
	}
}


//------------------------------------------------------
Stringz Stringz::Copy(_DWORD start, _DWORD len) const
{
	ASSERT(Str);
	ASSERTMSG(start <= Len(), 
			  "Stringz::Copy: start after the end of the string");
	if (!len) return Stringz("");
	len = MIN(Len() - start, len);

	Stringz r;
	len = MIN(len, (_DWORD)Len());
	r.bufsize = (len+1 + STRINGZAVGSIZE);
	r.Str = new _CHAR[r.bufsize];
	if (r.Str) {
		Strncpy(r.Str, &Str[start], len);
		r.Str[len] = 0;
	}
	return r;
}



//------------------------------------------------------
Stringz& Stringz::Format(const _CHAR *S, ...)
{
	ASSERT(Str);
	if (!S) { return *this; }

	va_list arg;
	va_start(arg, S);
	_DWORD l = Strlen(S)*2 + 4000;
	_CHAR *tmp;
	if ((Str == emptystring) || (l > Len())) {
		bufsize = l;
		tmp = new _CHAR[bufsize];
		if (Str != emptystring) { delete []Str; }
		Str = tmp;
	}
	else {
		tmp = Str;
	}
#if defined(HAVE_VSNPRINTF) || defined(__BCPLUSPLUS__)
	vsnprintf(tmp, l, S, arg);
#elif defined(_WIN32) && defined(_MSC_VER)
	_vsnprintf(tmp, l, S, arg);
#else
#ifdef _BSD_VA_LIST_	
	vsprintf(tmp, S, (_BSD_VA_LIST_)arg);
#else
	vsprintf(tmp, S, arg);
#endif
#endif
	va_end(arg);

	return *this;
}


//------------------------------------------------------
_DWORD Stringz::Strlen(const _CHAR*STR)
{
	if (STR && (STR != emptystring)) {
		_DWORD len = 0;
		while (STR[len++]) {};
		return (len-1);
	}
	return 0;
}


//------------------------------------------------------
void Stringz::Strcpy(const _CHAR*DST, const _CHAR*SRC)
{
	if (DST && SRC && (DST != emptystring)) {
		_CHAR *src = (_CHAR*)SRC;
		_CHAR *dst = (_CHAR*)DST;
		while ( ((*(dst++)) = (*(src++))) ) {};
	}
}


//------------------------------------------------------
void Stringz::Strncpy(const _CHAR*DST, const _CHAR*SRC, _DWORD N)
{
	if (DST && SRC && (DST != emptystring)) {
		_CHAR *src = (_CHAR*)SRC;
		_CHAR *dst = (_CHAR*)DST;
		while ( (N--) && ( (*(dst++)) = (*(src++)) ) ) {};
	}
}


//------------------------------------------------------
void Stringz::Strcat(_CHAR*DST, const _CHAR*SRC)
{
	if (DST && SRC && (DST != emptystring)) {
		_CHAR *src = (_CHAR*)SRC;
		_CHAR *dst = (_CHAR*)DST;
		dst += Strlen(dst);
		while ( ( (*(dst++)) = (*(src++)) ) != 0 ) {};
	}
}


//------------------------------------------------------
_LONG Stringz::Strcmp(const _CHAR*A, const _CHAR*B)
{
	if (!A) {
		return (B ? -1 : 0);
	}
	else if (!B) {
		return (A ? 1 : 0);
	}
	else {
		_CHAR *a = (_CHAR*)A;
		_CHAR *b = (_CHAR*)B;
		while ( (*a) && (*b) ) {
			if ((*a) < (*b)) { return -1; }
			if ((*(a++)) > (*(b++))) { return 1; }
		}
		if (*a) { return 1; }
		if (*b) { return -1; }
		return 0;
	}
}


//------------------------------------------------------
_LONG Stringz::Strncmp(const _CHAR*A, const _CHAR*B, _DWORD N)
{
	if (!A) {
		return (B ? -1 : 0);
	}
	else if (!B) {
		return (A ? 1 : 0);
	}
	else {
		_CHAR *a = (_CHAR*)A;
		_CHAR *b = (_CHAR*)B;
		while ( (*a) && (*b) && (N--)) {
			if ((*a) < (*b)) { return -1; }
			if ((*(a++)) > (*(b++))) { return 1; }
		}
		if (!N) {
			if (*a) { return 1; }
			if (*b) { return -1; }
		}
		return 0;
	}
}


namespace NMO {

//------------------------------------------------------
Stringz operator + (const Stringz &S1, const Stringz &S2)
{
	Stringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
Stringz operator + (const Stringz &S1, const _CHAR *S2)
{
	Stringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
Stringz operator + (const _CHAR *S1, const Stringz &S2)
{
	Stringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
Stringz operator + (_CHAR S1, const Stringz &S2)
{
	Stringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
Stringz operator + (const Stringz &S1, _CHAR S2)
{
	Stringz t(S1);
	t += S2;
	return t;
}


//------------------------------------------------------
Stringz operator + (const Stringz &S1, _LONG L)
{
	Stringz t;
	t.Format("%s%d", (_CHAR*)S1, L);
	return t;
}


//------------------------------------------------------
const Stringz &Stringz :: operator += (const _CHAR *S)
{
	ASSERT(Str);
	if (S) {
		if (Str == emptystring) { 
			*this = S; 
		}
		else {
			_CHAR *N;
			_DWORD newsize = Strlen(S) + Len() + 1;
			if (newsize > bufsize) {
				bufsize = newsize;
				N = new _CHAR[bufsize];
				Strcpy(N, Str);
				delete []Str;
				Str = N;
			}
			else {
				N = Str;
			}
			Strcat(N, S);
		}
	}
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator += (_CHAR S)
{
	ASSERT(Str);
	if (Str == emptystring) {
		*this = S;
	}
	else
	{
		_CHAR *N;
		_DWORD len = Len();
		_DWORD newsize = len + 2;
		if (newsize > bufsize) {
			bufsize = newsize;
			N = new _CHAR[bufsize];
			Strcpy(N, Str);
			delete []Str;
			Str = N;
		}
		else {
			N = Str;
		}
		N[len] = S;
		N[len+1] = 0;
	}
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator += (const Stringz &S)
{
	ASSERT(Str);
	if (Str == emptystring) {
		*this = S;
	}
	else if (S.Str) {
		_DWORD l = (Len() + S.Len() + 1 + STRINGZAVGSIZE);
		if (l > 1) {
			_CHAR *N;
			if (l > bufsize) {
				bufsize = l;
				N = new _CHAR[bufsize];
				Strcpy(N, Str);
				delete []Str;
				Str = N;
			}
			else {
				N = Str;
			}
			Strcat(N, S.Str);
		}
	}
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator = (const _CHAR *S)
{
	if (S && S[0]) {
		_DWORD l = (Strlen(S) + 1 + STRINGZAVGSIZE);
		if (l > 1) {
			if (l > bufsize) {
				if (Str != emptystring) { delete []Str; }
				bufsize = l;
				Str = new _CHAR[bufsize];
			}
			Strcpy(Str, S);
		}
		else {
			bufsize = STRINGZAVGSIZE;
			Str = new _CHAR[bufsize];
			Str[0] = 0; 
		}
	}
	else if (Str != emptystring) {
		if (S == NULL) {
			delete []Str;
			Str = emptystring;
		}
		else {
			Str[0] = 0;
		}
	}
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator = (const Stringz &S)
{
	if (Str == S.Str) { return *this; }
	if (S.Str && S.Len()) {
		_DWORD newsize = (S.Len() + 1 + STRINGZAVGSIZE);
		if (newsize > bufsize) {
			if (Str != emptystring) { delete []Str; }
			bufsize = newsize;
			Str = new _CHAR[bufsize];
		}
		Strcpy(Str, S.Str);
		Str[newsize - 1] = 0;
	}
	else if (Str != emptystring) {
		Str[0] = 0;
	}
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator = (_LONG L)
{
	if (Str != emptystring) {
		delete []Str; 
	}
	bufsize = 100;
	Str = new _CHAR[bufsize];
#if defined(HAVE_SNPRINTF)
	snprintf(Str, 100, "%d", (int)L);
#elif defined(_WIN32) && defined(_MSC_VER)
	_snprintf(Str, 100, "%d", L);
#else
	sprintf(Str, "%d", L);
#endif
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator = (_DWORD L)
{
	if (Str != emptystring) {
		delete []Str; 
	}
	bufsize = 100;
	Str = new _CHAR[bufsize];
#if defined(HAVE_SNPRINTF)
	snprintf(Str, 100, "%d", (int)L);
#elif defined(_WIN32) && defined(_MSC_VER)
	_snprintf(Str, 100, "%d", L);
#else
	sprintf(Str, "%d", L);
#endif
	return *this;
}


//------------------------------------------------------
const Stringz &Stringz :: operator = (_CHAR S)
{
	if (2 > bufsize) {
		if (Str != emptystring) { delete []Str; }
		bufsize = (2 + STRINGZAVGSIZE);
		Str = new _CHAR[bufsize];
	}
	Str[0] = S;
	Str[1] = 0;
	return *this;
}


//------------------------------------------------------
Stringz::operator _LONG() const
{ return (Str ? atol(Str) : 0); }


//------------------------------------------------------
Stringz::operator _FLOAT() const
{ 
	ASSERT(Str);
	if (Str != emptystring) {
		double f;
		sscanf(Str,"%le",&f);
		return (_FLOAT)f;
	}
	return 0.0;
}


//------------------------------------------------------
Stringz::operator _DOUBLE() const
{ 
	ASSERT(Str);
	if (Str != emptystring) {
		_DOUBLE f;
		sscanf(Str,"%le",&f);
		return f;
	}
	return 0.0;
}


//------------------------------------------------------
Stringz::operator _LDOUBLE() const
{ 
	ASSERT(Str);
	if (Str != emptystring) {
		double f;
		sscanf(Str,"%le",&f);
		return (_LDOUBLE)f;
	}
	return 0.0;
}


//------------------------------------------------------
Stringz::operator _DWORD() const
{ 
	if (Str != emptystring) {
		return strtoul(Str, NULL, 10);
	}
	else {
		return 0;
	}
}


} // namespace NMO
