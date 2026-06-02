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

	(c) 1999-2001 Henri Michelon 

$Id: CGIApplication.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/CGIApplication.hpp>
using namespace NMO;
#include <stdlib.h>
#include <stdio.h>


//------------------------------------------------------------------
CGIApplication::CGIApplication(const UStringz&N): Application(N)
{
	Stringz query = GetVar("QUERY_STRING");
	
	_CHAR code[3] = {'\0'};
	_CHAR car;

	List<Stringz> p;
	ListIterator<Stringz> lstp(p);
	query.Split(p, '&');
	lstp.Reset();
	while (!lstp.End()) {
		Stringz &param = lstp.Next();
		_LONG pos = param.InStr('=');
		if (pos > -1) {
			Stringz value = param.Right(param.Len() - pos - 1);
			_DWORD i=0;
			while (i<value.Len()) {
				switch (value[i]) {
					case '+':
						value[i] = ' ';
						break;
					case '%':
						code[0] = value[i+1];
						code[1] = value[i+2];
						car = (_CHAR)strtol(code, (_CHAR**)NULL, 16);
						value = value.Copy(0, i) + car + 
							    value.Copy(i+3, value.Len() - i - 3);
						break; 
				}
				i++;
			}
			Param *newparam = new Param;
			newparam->name = param.Left(pos);
			newparam->value = value;
			params.Add(newparam);
		}
	}
	

}



//------------------------------------------------------------------
void CGIApplication::Terminate()
{
	exit(0);
}


//------------------------------------------------------------------
void CGIApplication::Write(const char*FMT, ...)
{
	ASSERT(FMT);
	va_list arg;
	va_start(arg, FMT);
#ifdef _BSD_VA_LIST_
	vprintf(FMT, (_BSD_VA_LIST_)arg);
#else
	vprintf(FMT, arg);
#endif
	va_end(arg);
}


//------------------------------------------------------------------
void CGIApplication::SetName(const Stringz&)
{
}


//------------------------------------------------------------------
Stringz CGIApplication::GetVar(const Stringz&NAME)
{
	Stringz res;
	if (Env::Read(NAME, res)) return res;
	return "";
}
	

//------------------------------------------------------------------
Stringz CGIApplication::GetParam(const Stringz&NAME, const Stringz&DEF)
{
	ListIterator<Param> list(params);
	while (!list.End()) {
		Param &param = list.Next();
		if (param.name == NAME) { return param.value; }
	}
	return DEF;
}

