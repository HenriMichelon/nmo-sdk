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

$Id: RegEx.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#ifdef _POSIX
// Posix systems that have RegEx POSIX functions
# include <regex.h>
#else
# include "regex/regex.h"
#endif


typedef struct {
	regex_t preg;
} RegExAbstract;

#define __m(x) ((RegExAbstract*)(this->mRegExAbstract))->x


//--------------------------------------------
RegEx::RegExError::RegExError(): NMOException("RegEx") {};


//--------------------------------------------
_BOOL RegEx::Eval(const Stringz&STRING, const Stringz&REGEX, _DWORD CFLAGS)
{
	RegEx regex;
	if (regex.Compile(REGEX, CFLAGS | NOSUBS)) {
		return regex.Execute(STRING);
	}
	return FALSE;
}


//--------------------------------------------
_BOOL RegEx::Eval(const Stringz&STRING, const Stringz&REGEX, 
				  List<Stringz>&RESLIST, _DWORD CFLAGS)
{
	RegEx regex;
	if (regex.Compile(REGEX, CFLAGS)) {
		return regex.Execute(STRING, RESLIST);
	}
	return FALSE;
}


//--------------------------------------------
RegEx::RegEx() 
{
	mRegExAbstract = NULL;
}


//--------------------------------------------
RegEx::RegEx(const Stringz&REGEX, _DWORD CFLAGS)
{ 
	mRegExAbstract = NULL;
	if (!Compile(REGEX, CFLAGS)) {
		Debug(throw NMOException("RegEx::RegEx:: error compiling RE");)
	}
}


//--------------------------------------------
RegEx::~RegEx()
{
	if (mRegExAbstract) {
		regfree(&__m(preg));
		delete (RegExAbstract*)mRegExAbstract;
   	}
}


//--------------------------------------------
const Stringz RegEx::Error() const
{
	ASSERT(mRegExAbstract);
	return ""; // XXX: We have to store the result of each call
			   // into abstract to get the error message
}


//--------------------------------------------
_BOOL RegEx::Compile(const Stringz&REGEX, _DWORD CFLAGS)
{
	if (mRegExAbstract) {
		regfree(&__m(preg));
   	}
	else {
		mRegExAbstract = new RegExAbstract;
	}

	int cflags = REG_EXTENDED;
	if (CFLAGS & ICASE) {
		cflags |= REG_ICASE;
	}
/*	if (CFLAGS & NOSUBS) {
		cflags |= REG_NOSUB;
	}*/

	return (regcomp(&__m(preg), (_CHAR*)REGEX, cflags) == 0);
}


//--------------------------------------------
_BOOL RegEx::Execute(const Stringz&STRING)
{
	ASSERT(mRegExAbstract);
	return (regexec(&__m(preg), (_CHAR*)STRING, 0, NULL, 0) == 0);
}


//--------------------------------------------
_BOOL RegEx::Execute(const Stringz&STRING, List<Stringz>&RESLIST)
{
	ASSERT(mRegExAbstract);

	_DWORD subcount = __m(preg).re_nsub + 1;
	regmatch_t *pmatch = new regmatch_t[subcount];
	if (regexec(&__m(preg), (_CHAR*)STRING, subcount, pmatch, 0)) {
		delete []pmatch;
		return FALSE;
	}
	for (_DWORD i=0; i<subcount; i++) {
		if (pmatch[i].rm_so != pmatch[i].rm_eo) {
			RESLIST.Add(new Stringz(STRING.Copy(pmatch[i].rm_so,
											   pmatch[i].rm_eo - 
											   pmatch[i].rm_so)));
		}
	}
	delete []pmatch;
	return TRUE;
}

