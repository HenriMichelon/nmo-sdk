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

$Id: InternetConnectionWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
#include <nmo/Net.hpp>
using namespace NMO;
#include <shellapi.h>
#include <Wininet.h>

//--------------------------------------------------------------------------
InternetConnection::InternetConnection() 
{
	mInternetConnectionAbstract = NULL;
}


//--------------------------------------------------------------------------
InternetConnection::~InternetConnection()
{
	if (mInternetConnectionAbstract) {
		::InternetHangUp((_DWORD)mInternetConnectionAbstract, 0);
	}
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::OpenURL(const Stringz&URL)
{
	return ((int)::ShellExecuteA(NULL, "open", (_CHAR*)URL, NULL, NULL, SW_SHOWNORMAL) > 32);
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::CanConnect(const Stringz&URL)
{
	return ::InternetCheckConnectionA((_CHAR*)URL,
										FLAG_ICC_FORCE_CONNECTION,
										0);
}


//--------------------------------------------------------------------------
void InternetConnection::ConnectionsList(const List<Stringz>&)
{
}


//--------------------------------------------------------------------------
_BOOL InternetConnection::Connect(const Stringz&, const Stringz&, const Stringz&)
{
	DWORD handle = 0;
	DWORD res = ::InternetDial(NULL,
					NULL,
					0, //INTERNET_AUTODIAL_FORCE_PROMPT,
					&handle,
					0);
	if (res == ERROR_SUCCESS) {
		mInternetConnectionAbstract = (_PTR)handle;
		return TRUE;
	}
	return FALSE;
}


#endif
