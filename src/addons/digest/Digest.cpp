/* Derivated from:
 * mdXhl.c
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@login.dkuug.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 *
 * $FreeBSD: src/lib/libmd/mdXhl.c,v 1.13 1999/08/28 00:05:07 peter Exp $
 *
 */
/*                                ---------
                                  [NMO-SDK]
                                  ---------

$Id: Digest.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/Digest.hpp>
using namespace NMO;

const _DWORD BUFSIZE = 8096*2;
const _LONG LENGTH = 16;

//-------------------------------------------------------------
const Stringz Digest::String(const Stringz&STR) {
	return Data((_CHAR*)STR, STR.Len());
}


//-------------------------------------------------------------
const Stringz Digest::End()
{
	int i;
    _BYTE digest[LENGTH];
    _CHAR hex[]="0123456789abcdef";

    _CHAR *buf = new _CHAR[2*LENGTH + 1];

	Final(digest);

    for (i = 0; i < LENGTH; i++) {
		buf[i+i] = hex[digest[i] >> 4];
		buf[i+i+1] = hex[digest[i] & 0x0f];
    }
    buf[i+i] = '\0';

	Stringz result(buf);
	delete []buf;
    return result;
}


//-------------------------------------------------------------
const Stringz Digest::File(NMO::File &FILE)
{
    int i;

  	_BYTE buffer[BUFSIZE];

    Init();
    while ((i = FILE.Read(buffer,sizeof buffer)) > 0) {
	   	Update(buffer,i);
    }
    if (i < 0) return "";
    return End();
}


//-------------------------------------------------------------
const Stringz Digest::Data(_PTR data, _DWORD len)
{
	Init();
   	Update(data,len);
	return End();
}

