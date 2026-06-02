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

	(c) 2000-2002 Henri Michelon

$Id: IStdDialogPhoton.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#ifdef _QNX
#include <nmo/Interface.hpp>
using namespace NMO;


//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileOpen :: Execute()
{
	return FALSE;
}



//---------------------------------------
_BOOL IStdDialogFileSave :: Execute()
{
	return FALSE;
}



//---------------------------------------
_BOOL IStdDialogMessage :: Execute()
{
	return FALSE;
}

#endif
