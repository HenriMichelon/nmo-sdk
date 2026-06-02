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

$Id: X11Win.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include <X11/Xlib.h>
#include "X11Win.hpp"
#include <stdio.h>
#include <stdlib.h>


_BOOL	 X11Win::eventloopstarted = FALSE;
_DWORD	 X11Win::nInstance = 0;
Display* X11Win::display = NULL;
int		 X11Win::screen_num = -1;


_BOOL X11Win::OpenScreen()
{
	if (!nInstance) {
		char *displayName = getenv("DISPLAY");
		if (displayName == NULL) {
			fprintf(stderr, "Please set the DISPLAY variable\n");
			return FALSE;
		}
		if ( (display = XOpenDisplay(displayName)) == NULL) {
			fprintf(stderr, "Cannot connect to %s\n", displayName);
			return FALSE;
		}
		screen_num = DefaultScreen(display);
		IScreen::SetSize(DisplayWidth(display, screen_num),
						DisplayHeight(display, screen_num));
		Debug(dprintf("Connected to %s\n", displayName);)
		nInstance++;
	}
	return TRUE;
}

#endif
