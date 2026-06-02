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

$Id: ExcIntuition.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
/*#ifdef WARPUP
	#include <powerup/ppcproto/exec.h>
	#include <powerup/ppcproto/intuition.h>
#endif*/
#include <nmo/Base.hpp>
using namespace NMO;


//--------------------------------------------------------
void NMOException::Display()
{
#if ! defined(AUTO_LIB)	
	if (!IntuitionBase) {
		IntuitionBase = (struct IntuitionBase *)OpenLibrary(INTUITIONNAME, 
															INTUITIONVER);
	}
	if (!IntuitionBase) { return; }
#endif
	struct EasyStruct es;
	es.es_StructSize = sizeof(es);
	es.es_Flags = 0;
	Stringz t = sTitle;
	es.es_Title = (UBYTE*)(_CHAR*)t;
	es.es_TextFormat = (UBYTE*)mMsg;
	es.es_GadgetFormat = (UBYTE*)"Ok";
	EasyRequest(NULL, &es, NULL);
}

#endif
