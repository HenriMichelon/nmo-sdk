/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: RegEx.cpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/StdioApplication.hpp>
using namespace NMO;


class regex: public StdioApplication
{
public:
	regex(UStringz N): StdioApplication(N) {};
	
private:
	void OnCreate()
	{
		Stringz name = Name();
		Write("%s\n", (_CHAR*)name);

		Stringz regex("\\.gif$");
		Stringz	str;
		
		str = "toto.jpg";
		Write("%s on %s: %d\n", (_CHAR*)regex,
								(_CHAR*)str,
								RegEx::Eval(str, regex));
		
		str = "toto.gif";
		Write("%s on %s: %d\n", (_CHAR*)regex,
								(_CHAR*)str,
								RegEx::Eval(str, regex));

		NMO::List<Stringz> subs;
		regex = "\\.(.*)$";
		Write("%s on %s: %d\n", (_CHAR*)regex,
								(_CHAR*)str,
								RegEx::Eval(str, regex, subs));
		ListIterator<Stringz> lstsubs(subs);
		while (!lstsubs.End()) {
			Write("\t%s\n", (_CHAR*)lstsubs.Next());
		}
	}
};


DebugOutput(NULL);
CreateApplication(regex, "Regular Expressions");
