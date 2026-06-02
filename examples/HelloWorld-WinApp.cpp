// $Id: HelloWorld-WinApp.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>

class HelloApp: public WindowApplication
{
public:
	HelloApp(const UStringz T): WindowApplication(T) {};
	
private:
	
	class MyWindow: public IWindow
	{
	private:
		void EventDraw(const IRect&)
		{
			IFont *font = IFont::Create();
			font->Draw(Display(), "Hello world!", 70, 35);
			delete font;
		}
	};

	MyWindow mywindow;

	void OnCreate()
	{
		if (!Add(mywindow)) { return; }
		mywindow.SetSize(200, 100);
		mywindow.ScreenCenter();
		mywindow.Show();	
	}	
};

DebugOutput(NULL);
CreateApplication(HelloApp, "NMO WinApp Hello World");
