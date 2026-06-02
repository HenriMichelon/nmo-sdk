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

	(c) 1998-2002 Henri Michelon

$Id: X11Win.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_X11WIN_H_
#define _NMO_X11WIN_H_

namespace NMO {

class X11Win
{
public:
	static _DWORD	nInstance;
	static Display	*display;
	static int		screen_num;
	static	_BOOL	eventloopstarted;

	static _BOOL OpenScreen();
};

}

#endif
