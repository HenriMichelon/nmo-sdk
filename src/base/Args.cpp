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

	(c) 1998-2001 Henri Michelon 

$Id: Args.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;


//--------------------------------------------
Args::Args(_LONG C, char**V[])
{
	Set(C, V);
}


//--------------------------------------------
void Args::Set(_LONG C, char**V[])
{
	m_argc = C;
	m_argv = V;
	for (_LONG i=0; i<C; i++) {
		Add(new Stringz((*V)[i]));
	}
}


//--------------------------------------------
_DWORD Args::IndexOf(const Stringz&NAME)
{
	_DWORD len = NAME.Len();
	ListIterator<Stringz> list(*this);
	list.Next(); // first arg: name of the command
	while (!list.End()) {
		Stringz name(list.Current());
		_CHAR c = name[0L];
		if ((c == '-') || (c == '/')) {
			name = name.Right(name.Len()-1);
		}
		if ((name.Len() == len) && (name == NAME)) {
			return list.Index();
		}
		if (name.Len() > len) {
			_CHAR sep = name[len];
			if ((sep == '=') || (sep == ':')) {
				if (name.Left(len) == NAME) {
					return list.Index();
				}
			}
		}
		list.Next();
	}
	return 0;
}


//--------------------------------------------
_BOOL Args::Exists(const Stringz&NAME)
{
	ListIterator<Stringz> list(*this);
	return (list.SearchFor(NAME) != 0);
}


//--------------------------------------------
Stringz Args::Value(const Stringz&NAME, const Stringz&DEFAULT)
{
	_DWORD c = Count();
	ListIterator<Stringz> list(*this);
	Stringz res(DEFAULT);
	_DWORD idx = IndexOf(NAME);
	if (idx != 0) {
		res = list[idx];
		_LONG pos = res.InStr('=');
		if (pos == -1) {
			pos = res.InStr(':');
		}
		if (pos != -1) {
			res = res.Copy(pos+1, res.Len()-pos-1);
		}
		else if (idx < c) {
			res = list[idx+1];
			_CHAR r = res[0ul];
			if ((r == '-') || (r == '/')) {
				res = DEFAULT;
			}
		}
	}
	return Stringz(res);
}


//--------------------------------------------
void Args::Unamed(List<Stringz>&LIST, _BOOL NODIVARGS)
{
	_BOOL havename = FALSE;
	ListIterator<Stringz> list(*this);
	list.Next(); // first arg: name of the command
	while (!list.End()) {
		Stringz &arg = list.Next();
		_CHAR a = arg[0L];
		if ((a == '-') || ((a == '/') && (!NODIVARGS))) {
			havename = ((arg.InStr('=') == -1) && (arg.InStr(':') == -1));
		}
		else {
			if (havename) {
				havename = FALSE;
			}
			else {
				LIST.Add(new Stringz(arg));
			}
		}
	}
}


//--------------------------------------------
void Args::Operands(List<Stringz>&LIST, _BOOL NODIVARGS)
{
	ListIterator<Stringz> list(*this);
	list.Next(); // first arg: name of the command
	while (!list.End()) {
		Stringz &arg = list.Next();
		_CHAR a = arg[0L];
		if (!((a == '-') || ((a == '/') && (!NODIVARGS)))) {
			LIST.Add(new Stringz(arg));
		}
	}
}


//--------------------------------------------
Stringz&  Args::operator [](_DWORD IDX)
{
	ListIterator<Stringz> list(*this);
	return list [IDX];
}


//--------------------------------------------
int * Args::argc() { return &m_argc; }

//--------------------------------------------
char *** Args::argv() { return m_argv; }

