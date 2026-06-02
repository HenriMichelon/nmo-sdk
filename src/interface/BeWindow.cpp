/*
$Id: BeWindow.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
*/
#ifdef _BEOS
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "BeWindow.hpp"


//------------------------------------------------
BeWindow :: BeWindow (IWindow *NMOWND,
					  BRect &frame, 
					  const char*title,
					  window_look LOOK)
		: BWindow(frame, title, B_TITLED_WINDOW, 0, 0)
{
	nmownd = NMOWND;
}


//------------------------------------------------
void BeWindow :: FrameMoved(BPoint)
{
	ASSERT(nmownd);
	nmownd->EventMove();
}


//------------------------------------------------
void BeWindow :: FrameResized(float W, float H)
{
	ASSERT(nmownd);
	nmownd->EventResize();
	nmownd->ResizeDisplay();
}	


//------------------------------------------------
bool BeWindow :: QuitRequested(void)
{
	ASSERT(nmownd);
	return nmownd->EventQueryDestroy();
}
#endif