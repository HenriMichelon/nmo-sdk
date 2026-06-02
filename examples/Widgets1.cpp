/*

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: Widgets1.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;

//---------------------------------------------------------
class MyWin: public GWindow
{
private:
	GBox		pnlTop;
	GBox		pnlLeft;
	GBox		pnlBottom;
	GBox		boxBtn;
	GBox		boxBnt1;
	GBox		boxBnt2;
	GBox		boxBnt3;
	GBox		boxBnt4;

	void OnCreate();
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
DebugOutput(NULL);
//DebugOutput("debug.txt");
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
	GPanel &frame = (GPanel&) SetWidget(new GPanel);
	frame.Add(pnlTop, GWidget::TOP, "52,52,RAISED", 1);
	frame.Add(pnlLeft, GWidget::LEFT, "52,52,RAISED", 1);
	frame.Add(pnlBottom, GWidget::BOTTOM, "52,52,RAISED", 1);
	frame.Add(boxBtn, GWidget::CLIENT, ",,RAISED");
	pnlTop.Add(boxBnt1, GWidget::LEFT, "50,50,RAISED");
	pnlTop.Add(boxBnt2, GWidget::LEFT, "50,50,RAISED");
	pnlLeft.Add(boxBnt3, GWidget::TOP, "50,50,RAISED");
	pnlLeft.Add(boxBnt4, GWidget::TOP, "50,50,RAISED");
}
