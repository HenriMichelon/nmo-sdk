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

$Id: ThCriticalSection.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;

NMO::List<ThCriticalSection> ThCriticalSection::cs;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
ThCriticalSection *ThCriticalSection::Create(const Stringz &NAME)
{
	ListIterator<ThCriticalSection> list(cs);
	while (!list.End()) {
		ThCriticalSection &section = list.Next();
		if (section.Name() == NAME) 	{
			Debug(dprintf("ThCriticalSection: using %s\n", (_CHAR*)NAME);)
			return &section;
		}
	}
	ThCriticalSection *section = new ThCriticalSection(NAME);
	if (section->Init()) {
		cs.Add(section);
		Debug(dprintf("ThCriticalSection: creating %s\n", (_CHAR*)NAME);)
		return section;
	}
	delete section;
	return NULL;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Stringz ThCriticalSection::Name()
{
	return name;
}
