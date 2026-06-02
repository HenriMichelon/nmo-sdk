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

$Id: NetID.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;

#ifdef _BSDSOCKET
#include "NetIDIP.hpp"
#endif

_DWORD			NetID::nInst = 0;
NMO::List<Stringz>	NetID::drivers;

//---------------------------------------------------------------------------
NetID::~NetID()
{
	if (!--nInst) 
		drivers.Clear();
}


//---------------------------------------------------------------------------
NetID* NetID::Create(FileCfg& CFG)
{
	return Create(CFG.Read("NET", "driver"), CFG);
}


//---------------------------------------------------------------------------
NetID* NetID::Create(const Stringz&DRIVER, FileCfg& CFG)
{
	if (!nInst++)
	{
#ifdef _BSDSOCKET
		drivers.Add(new Stringz(UDPDRIVERNAME));
		drivers.Add(new Stringz(TCPDRIVERNAME));
#endif
	}

	NetID *tmp = NULL;
	ListIterator<Stringz> list(drivers);
	_LONG driver = list.SearchFor(DRIVER);

	if (!driver) {
		Debug(dprintf("Unknown driver %s\n", (_CHAR*)DRIVER);)
		return NULL;
	}

	try {
#ifdef _BSDSOCKET
		tmp = (NetID*) new NetIDIP(CFG);
#endif
	}
	catch (NMOException) {
		delete tmp;
		tmp = NULL;
		//Debug(dprintf("%s\n", e.Message());)
	}

	if (tmp) {
		List<Stringz> lst;
		CFG.Read(lst);
		ListIterator<Stringz> list(lst);
		tmp->cfg.Load(list);
		tmp->driver = driver;
	}
	return tmp;
}


//---------------------------------------------------------------------------
const Stringz& NetID::Driver() 
{
	ListIterator<Stringz> list(drivers);
	return list[driver];
}
