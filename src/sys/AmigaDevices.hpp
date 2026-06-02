/*

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

		Generic device class

(c)  1998-2000 Henri Michelon
$Id: AmigaDevices.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
*/
#if defined(_AMIGA) || defined(amigaos)
#ifndef _AMIGA_DEVICE_H_
#define _AMIGA_DEVICE_H_

namespace NMO {

class AmigaDevices
{
public:
	static void Open(char *, ULONG, struct IORequest **, ULONG, ULONG);
	static void Close(struct IORequest *);
	static void DoCommand (struct IORequest *, UWORD);
	static void DoIO(struct IORequest*);
};

}

#endif // _AMIGA_DEVICE_H_
#endif
