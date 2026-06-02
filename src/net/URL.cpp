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

	(c) 2000-2001 Henri Michelon

$Id: URL.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;


//------------------------------------------------------------------
_BOOL URL::Split(const Stringz&URL, Stringz&PROTO, 
								  Stringz&SERVER, Stringz&PORT, Stringz&DIR, 
								  Stringz&FILE, Stringz&PARAMS)
{
	Stringz url(URL);
	List<Stringz> 	subs;
	ListIterator<Stringz> 	lstsubs;

	// Extract PROTO, SERVER & PORT
	Stringz	regex = "^([a-z]+)+://([^:/]+):?([0-9]*)";
	if (!RegEx::Eval(url, regex, subs)) {
		return FALSE;
	}
	if (subs.Count() < 3) {
		return FALSE;
	}
	PROTO = lstsubs[2];
	SERVER = lstsubs[3];
	if (lstsubs.Count() > 3) {
		PORT = lstsubs[4];
	}
	if (url.Len() == lstsubs[1].Len()) {
		return TRUE;
	}
	// Remove extracted part from url
	url = url.Right(url.Len() - lstsubs[1].Len());
	// Extract GET params
	_LONG pos = url.InStr('?');
	if (pos >= 0) {
		PARAMS = url.Right(url.Len() - pos - 1);
		url = url.Left(pos);
	}
	// Extract DIR & FILE
	regex = "([^/]+\\.[^/]*)$";
	subs.Clear();
	if (RegEx::Eval(url, regex, subs)) {
		FILE = lstsubs[2];
		url = url.Left(url.Len() - lstsubs[1].Len());
	}
	DIR = url;
	return TRUE;
}


