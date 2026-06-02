/*
$Id: Widgets2.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;

//---------------------------------------------------------
class MyWin: public GWindow
{
private:
	GBox			pnlTop;
	GBox			pnlLeft;
	GBox			pnlBottom;
	GPanel			pnlClient;
	GWidget			pnlText;
	GText			txtClient;
	GButton			btnQuit;
	GToggleButton	btnPic;
	//GPicture		picture;

	void OnCreate();

	void OnBtnQuit(GWidget&, GEvent*);
};


//---------------------------------------------------------
class MyApp: public WindowApplication
{
public:
	MyApp(const UStringz&N): WindowApplication(N) {};

private:
	MyWin window;
	void OnCreate();
};


//---------------------------------------------------------
//DebugOutput("debug.txt");
DebugOutput(NULL);
CreateApplication(MyApp, "MyApp with a window and some widgets");


//---------------------------------------------------------
void MyApp::OnCreate()
{
	if (!Add(window, IWindow::SIZEABLE)) throw NMOException("Error creating main window");
	window.SetSize(400, 200);
	window.ScreenCenter();
	window.Show();
}



//---------------------------------------------------------
void MyWin::OnCreate()
{
	DatatypePixmap dtype; 
	GPanel &frame = (GPanel&) SetWidget(new GPanel);

	frame.Add(pnlTop, GWidget::TOP, "22,50,RAISED", 1);
	frame.Add(pnlLeft, GWidget::LEFT, "22,22,RAISED", 1);
	frame.Add(pnlBottom, GWidget::BOTTOM, "22,22,RAISED", 1);
	frame.Add(pnlClient, GWidget::CLIENT);

	pnlTop.Add(btnQuit, GWidget::LEFT, "60,45,RAISED");
	btnQuit.Add(new GPicture(dtype.Load("flower.png")), GWidget::TOPCENTER);
	btnQuit.Add(new GText("Quit!"), GWidget::TOPCENTER);
	btnQuit.Connect(GEvent::OnClick, this, GEventFunction(&MyWin::OnBtnQuit));
	pnlTop.SetSize(0, btnQuit.Height()+2);
	
	pnlClient.Add(txtClient, GWidget::LEFT);
	txtClient.SetText("A text Label");
	
	pnlClient.Add(btnPic, GWidget::CENTER, "60,60,RAISED");
}


void MyWin::OnBtnQuit(GWidget &, GEvent*)
{
	Close();
}
