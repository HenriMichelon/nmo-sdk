/*
$Id: BeWindow.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
*/

#ifndef _NMO_BEOS_BEWINDOWS_H_
#define _NMO_BEOS_BEWINDOWS_H_

class BeWindow: public BWindow
{
public:
	BeWindow(IWindow*, BRect&, const char*, window_look);
	
	virtual void FrameResized(float, float);
	virtual void FrameMoved(BPoint);
	virtual bool QuitRequested(void);

private:
	IWindow *nmownd;
};

#endif