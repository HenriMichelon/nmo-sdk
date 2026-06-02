/*
$Id: GScrollBox.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
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
	GWidget			pnlClient;
	GButton			btnQuit;

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
CreateApplication(MyApp, "ScrollBox");


//---------------------------------------------------------
void MyApp::OnCreate()
{
	if (!Add(window, IWindow::SIZEABLE)) throw NMOException("Error creating main window");
	window.SetSize(600, 410);
	window.ScreenCenter();
	window.Show();
}



//---------------------------------------------------------
void MyWin::OnCreate()
{
	DatatypePixmap dtype;
	GPanel &frame = (GPanel&) SetWidget(new GPanel);

	frame.Add(pnlTop, GWidget::TOP, "22,50,RAISED", 1);
	frame.Add(pnlBottom, GWidget::BOTTOM, "22,22,RAISED", 1);
	frame.Add(pnlLeft, GWidget::LEFT, "22,22,RAISED", 1);
	frame.Add(pnlClient, GWidget::CLIENT, "", 5);

	pnlTop.Add(btnQuit, GWidget::LEFT, "60,30,RAISED");
	btnQuit.Add(new GText("Quit"), GWidget::CENTER);
	btnQuit.Connect(GEvent::OnClick, this, GEventFunction(&MyWin::OnBtnQuit));
	pnlTop.SetSize(0, btnQuit.Height()+2);


	GWidget *box = pnlClient.Add(new GScrollBox, GWidget::CLIENT, ",,LOWERED", 5);
	for (_DWORD x = 0; x < 31; x ++) {
		Stringz fmt;
		GWidget *btn = box->Add(new GToggleButton, GWidget::TOPLEFT, "750,30,RAISED");
		btn = btn->Add(new GBox, GWidget::CENTER, "100,24,RAISED");
		fmt.Format("String n %d", x+1);
		btn->Add(new GText(UStringz(fmt)), GWidget::CENTER);
	}
}


void MyWin::OnBtnQuit(GWidget &, GEvent*)
{
	Close();
}
