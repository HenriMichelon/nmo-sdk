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

	Include file for the "interface" package

	(c) 1998-2001 Henri Michelon 

$Id: Interface.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/


//-----------------------------------------------------
// AmigaOS 3.x
//-----------------------------------------------------
#ifdef _AMIGA
	/*#define INTUITION_IOBSOLETE_H
	#define DEVICES_TIMER_H
	#undef SIGN
	#include <intuition/intuition.h>*/
	#define IHandle Window*
#endif


//-----------------------------------------------------
// MacOS 8+
//-----------------------------------------------------
#ifdef _MACOS
	typedef _PTR IHandle;
	#define IHANDLE(x) (WindowRef)(x)
#endif


//-----------------------------------------------------
// MS Windows
//-----------------------------------------------------
#ifdef _WIN32
	typedef _PTR IHandle;
	#define IHANDLE(x) (HWND)(x)
#endif


//-----------------------------------------------------
// X11 R4+
//-----------------------------------------------------
#ifdef HAVE_X11
	typedef _PTR IHandle;
	#define IHANDLE(x) (Window)(x)
#endif


//-----------------------------------------------------
// QNX Photon microGUI 2.0+
//-----------------------------------------------------
#ifdef __QNX__
	typedef _PTR IHandle;
	#define IHANDLE(x) (PtWidget_t*)(x)
#endif


//-----------------------------------------------------
// BeOS R4+
//-----------------------------------------------------
#ifdef _BEOS
	#define IHandle BWindow*
#endif



//-----------------------------------------------------
// Interface package
//-----------------------------------------------------
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IColorSpace.hpp>
#include <nmo/interface/IDisplay.hpp>
#include <nmo/interface/IDisplayChunky.hpp>
#include <nmo/interface/IPixmap.hpp>
#include <nmo/interface/ICachedChar.hpp>
#include <nmo/interface/IFontEngine.hpp>
#include <nmo/interface/IFont.hpp>
#include <nmo/interface/IKeyb.hpp>
#include <nmo/interface/IMouse.hpp>
#include <nmo/interface/IScreen.hpp>
#include <nmo/interface/IWindow.hpp>
#include <nmo/interface/ICursor.hpp>

#include <nmo/datatypes/FilePal.hpp>
#include <nmo/datatypes/FilePixmap.hpp>
#include <nmo/datatypes/DatatypePixmap.hpp>

#include <nmo/interface/IStdDialog.hpp>
#include <nmo/interface/IStdDialogMessage.hpp>
#include <nmo/interface/IStdDialogFile.hpp>
#include <nmo/interface/IStdDialogFileOpen.hpp>
#include <nmo/interface/IStdDialogFileSave.hpp>
