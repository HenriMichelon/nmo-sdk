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

$Id: InternetConnectionLinux.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------------*/
#ifdef linux
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;


//--------------------------------------------------------------------------
InternetConnection::InternetConnection() 
{
}


//--------------------------------------------------------------------------
InternetConnection::~InternetConnection()
{
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::OpenURL(const Stringz&URL)
{
	return FALSE;
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::CanConnect(const Stringz&URL)
{
	return FALSE;
}


//--------------------------------------------------------------------------
void InternetConnection::ConnectionsList(const List<Stringz>&)
{
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::Connect(const Stringz&, const Stringz&, const Stringz&)
{
	return FALSE;
}


//--------------------------------------------------------------------------
/*_BOOL InternetConnection::GetProxy(Stringz&SERVER, Stringz&PORT, 
								Stringz&USERNAME, Stringz&PASSWD)
{
	return FALSE;
}


//--------------------------------------------------------------------------
void InternetConnection::SetProxy(const Stringz&, const Stringz&, Stringz&, Stringz&, _BOOL)
{
}*/
#endif


