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
 
$Id: IKeybAmiga.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;


//-------------------------------------------------------
_BOOL IKeyb :: isDown(Key) 
{ 

	return FALSE;
}


//-------------------------------------------------------
_BOOL IKeyb ::isPushed(Key I)
{ 	
	
	return FALSE;
}
	
	
//-------------------------------------------------------
_CHAR IKeyb :: CodeToChar(Key K)
{
	return 0;
}


//-------------------------------------------------------
IKeyb::Key IKeyb :: CharToCode(_CHAR C)
{
	return 'A';
}


//-------------------------------------------------------
IKeyb :: IKeyb(): shift(FALSE), ctrl(FALSE), alt(FALSE),
os(FALSE), capslock(FALSE)
{
}


//-------------------------------------------------------
void IKeyb :: SetKey(Key I) 
{
	
}



//-------------------------------------------------------
void IKeyb :: UnsetKey(Key I) 
{
	
}


//-------------------------------------------------------
void IKeyb :: SetMap()
{
	KEY_NONE = 0;
}


#endif
