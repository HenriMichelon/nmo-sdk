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

$Id: Exc.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

UStringz NMOException :: sTitle;

void NMOException :: SetTitle(const UStringz&T) { sTitle = T; }

//--------------------------------------------------------
NMOException :: NMOException (const char*fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
#if defined(HAVE_VSNPRINTF) || defined(__BCPLUSPLUS__)
	vsnprintf(mMsg, EXCMSGMAXLEN, fmt, arg);
#elif defined(_WIN32) && defined(_MSC_VER)
	_vsnprintf(mMsg, EXCMSGMAXLEN, fmt, arg);
#else
#ifdef _BSD_VA_LIST_
	vsprintf(mMsg, fmt, (_BSD_VA_LIST_)arg);
#else
	vsprintf(mMsg, fmt, arg);
#endif
#endif
	va_end(arg);
}


//--------------------------------------------------------
void NMOException :: SetMessage(const char*fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
#if defined(HAVE_VSNPRINTF) || defined(__BCPLUSPLUS__)
	vsnprintf(mMsg, EXCMSGMAXLEN, fmt, arg);
#elif defined(_WIN32) && defined(_MSC_VER)
	_vsnprintf(mMsg, EXCMSGMAXLEN, fmt, arg);
#else
#ifdef _BSD_VA_LIST_
	vsprintf(mMsg, fmt, (_BSD_VA_LIST_)arg);
#else
	vsprintf(mMsg, fmt, arg);
#endif
#endif
	va_end(arg);
}


//--------------------------------------------------------
const char* NMOException :: Message () const
{
	return mMsg;
}
