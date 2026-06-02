// $Id: HelloWorld-GUI.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>

class HelloApp: public WindowApplication
{
public:
	HelloApp(const UStringz T): WindowApplication(T) {};
	
private:
	GWindow mywindow;

	void OnQuit(GWidget&, GEvent*)
	{
		mywindow.Close ();
	}

	void OnCreate()
	{
		if (!Add(mywindow)) { return; }
		mywindow.Widget().SetPadding(5);
		GWidget *w = mywindow.Widget().Add(new GButton(), GWidget::BOTTOMCENTER, "70,40,RAISED");
		w->Add(new GText("Quit"), GWidget::CENTER);
		w->Connect(GEvent::OnClick, this, GEventFunction(&HelloApp::OnQuit));
		mywindow.Widget().Add(new GText("Hello world !"), GWidget::CENTER);
		mywindow.SetSize(200, 100);
		mywindow.ScreenCenter();
		mywindow.Show();
	}	
};

DebugOutput(NULL);
CreateApplication(HelloApp, "NMO GUI Hello World");

