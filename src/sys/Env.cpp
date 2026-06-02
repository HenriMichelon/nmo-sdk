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

$Id: Env.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#ifndef _MSC_VER
# include <stdlib.h> 
#endif

#ifndef _WIN32
extern char **environ;
#endif

#ifndef _AMIGA
//------------------------------------------------------
_BOOL Env::Read(const Stringz &S, Stringz&R)
{
	char *r = getenv((char*)S);
	if (r != NULL)
		R = r;
	return (r != NULL);
}


//------------------------------------------------------
void Env::Environ(List<Stringz> &LIST)
{
#ifndef macintosh
	Stringz name;
	_LONG pos;
	_LONG i = 0;
	while (environ[i])
	{
		name = environ[i++];
		pos = name.InStr('=');
		if (pos > 0)
		{
			name = name.Copy(0, pos);
			LIST.Add(new Stringz(name));
		}
	}
#endif
}
#endif


//------------------------------------------------------
void Env::Expand(Stringz&S)
{
	_LONG pos = 0;
	Stringz name;
	Stringz value;
	do
	{
		name = "";
		pos = S.InStr(VARPRE, pos);
		if (pos == -1) break;
		for (_DWORD i=(pos+1); i<S.Len(); i++)
		{
			name += S[i];
			if (Read(name, value))
			{
				S = S.Copy(0, pos) + value +
					S.Copy(pos+1+name.Len(), S.Len()-pos-1-name.Len());
				pos += value.Len();
				break;
			}
		}
		pos++;
	} while (pos < (_LONG)S.Len());
}
