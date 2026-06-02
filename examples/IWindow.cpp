/*
$Id: IWindow.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;

const _WORD UNICODETEXT[] = { '-',0x00E9,'-',0x03C0,'-',0x00F1,'-',0x00BF,'-',0 };
// must print: eacute, PI, n with tilde, inverted question mark

//---------------------------------------------------------
class awindowWin: public IWindow
{
private:
	void EventCreate();
	void EventDestroy();
	_BOOL EventQueryDestroy();
	void EventDraw(const IRect&);
	void EventShow();
	void EventHide();
	void EventMove();
	void EventResize();
 	void EventKeybDown(IKeyb::Key);
 	void EventKeybUp(IKeyb::Key);
	void EventMouseDown(IMouse::Button, _LONG, _LONG);
	void EventMouseUp(IMouse::Button, _LONG, _LONG);
 	void EventMouseMove(IMouse::Button, _LONG, _LONG);
	void EventGotFocus();
	void EventLostFocus();
};


//---------------------------------------------------------
class awindowApp: public WindowApplication
{
public:
	awindowApp(const UStringz&N): WindowApplication(N) {}

private:
	awindowWin window;
	//awindowWin child;

	void OnCreate();
	void OnDestroy();
};


//---------------------------------------------------------
DebugOutput(NULL);
CreateApplication(awindowApp, "IWindow: one window creation");


//---------------------------------------------------------
void awindowApp::OnCreate()
{
	dprintf("App OnCreate\n");
	if (!Add(window, IWindow::SIZEABLE)) {
		throw NMOException("Error creating window");
	}
	window.SetSize(200, 200);
	window.ScreenCenter();
	window.Show();
}


//---------------------------------------------------------
void awindowApp::OnDestroy()
{
	dprintf("App OnDestroy\n");
}


//---------------------------------------------------------
void awindowWin::EventCreate()
{ dprintf("Win EventCreate\n"); }

void awindowWin::EventDestroy()
{
	dprintf("Win EventDestroy\n");
}

	
_BOOL awindowWin::EventQueryDestroy()
{
	dprintf("Win EventQueryDestroy\n");
	return TRUE;
}

void awindowWin::EventShow()
{ dprintf("Win EventShow\n"); }

void awindowWin::EventHide()
{ dprintf("Win EventHide\n"); }

void awindowWin::EventMove()
{
	Debug(dprintf("Win EventMove. New pos is %dx%d\n", Left(), Top());)
}

void awindowWin::EventResize()
{
	Debug(dprintf("Win EventResize. new size is %dx%d\n", Width(), Height());)
}

void awindowWin::EventKeybDown(IKeyb::Key K)
{  
	Debug(dprintf("Win EventKeybDown: %d/%c\n", K, K);)
	IRect r(0, 0, Rect().width, Rect().height);
	//EventDraw(r);
	Display().Refresh();
	//App().Terminate();
}

void awindowWin::EventKeybUp(IKeyb::Key K)
{ 
	Debug(dprintf("Win EventKeybUp %d/%c\n", K, K);)
}

void awindowWin::EventMouseMove(IMouse::Button btn, _LONG X, _LONG Y)
{ 
	Debug(dprintf("Win EventMouseMove: %dx%d (%d)\n", X, Y, btn); )
}

void awindowWin::EventMouseDown(IMouse::Button btn, _LONG X, _LONG Y)
{ 
	Debug(dprintf("Win EventMouseDown: %dx%d (%d)\n", X, Y, btn); )
	
	if (btn == IMouse::LEFT) {
		/*awindowWin *childwindow = new awindowWin;
		if (!App().Add(childwindow, IWindow::NORMAL)) {
			throw NMOException("Error creating child window");
		}
		childwindow->SetRect(400, 150, 100, 100);
		childwindow->SetTitle("Child window");
		childwindow->Show();*/
	}
	else if (btn == IMouse::RIGHT) {
		Close();
	}

}

void awindowWin::EventMouseUp(IMouse::Button btn, _LONG X, _LONG Y)
{ 
	Debug(dprintf("Win EventMouseUp: %dx%d (%d)\n", X, Y, btn); )
}

void awindowWin::EventGotFocus()
{ dprintf("Win EventGotFocus\n"); }

void awindowWin::EventLostFocus()
{ dprintf("Win EventLostFocus\n"); }

void awindowWin::EventDraw(const IRect&R)
{
	Debug(dprintf("Win EventDraw rect is (%d,%d)x(%d,%d)\n",
				R.left, R.top, R.width, R.height);)
	IRGBColor black(0, 0, 0);
	IRGBColor grey(0x88, 0x88, 0x88);
	IRGBColor white(0xe0, 0xe0, 0xe0);
	IRGBColor red(0xff, 0, 0);
	IRGBColor blue(0, 0, 0xff);
	IRGBColor green(0, 0xff, 0);
	IRGBColor yellow(198, 117, 236);
	
	_DWORD w = Width() / 2;
	_DWORD h = Height() / 2;

	Display().SetPenColor(white);
	Display().Clear();
	Display().SetPenColor(yellow);
	Display().DrawCircle(w / 2, h / 2, MIN(w / 2, h / 2), FALSE);
	Display().DrawHLine(0, h / 2, w);

	Display().SetClip(10, 10, Width() - 20, Height() - 20);
	Display().SetPenColor(red);
	Display().DrawLine(0, 0, w, h);
	Display().SetPenColor(blue);
	Display().DrawLine(w, 0, 0, h);
	Display().SetPenColor(green);
	Display().DrawRect(w, h, w, h);
	Display().SetPenColor(black);
	Display().DrawPixel(w + w / 2, h / 2);
	
	/*IRect rect (0, 0, w, h);
	_BYTE *pixmap = Display().GetRect(rect);
	if (pixmap) {
		rect.top += h;
		Display().DrawTransClip(pixmap, rect);
		delete []pixmap;
	}*/
	
	IFont *font = IFont::Create();
	if (font) {
		UStringz str("Text");
		font->Draw(Display(), str, w, 10);
		font->SetTextColor(red);
		font->Draw(Display(), UNICODETEXT, w, 20);
		delete font;
	}

}
