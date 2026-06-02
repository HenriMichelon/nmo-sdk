/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

			LEVEL 1 widgets test

$Id: Widgets3.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>

//---------------------------------------------------------
class MyWin: public GWindow
{
private:
	GUpDown 		upd;
	GScrollBar 		scr;
	GProgressBar 	prg;

	void OnCreate();
	void OnBtnQuit(GWidget&, GEvent*);
	void OnUpdChange(GWidget&, GEvent*);
	void OnLstSelect(GWidget&, GEvent*);
	void OnTxtChange(GWidget&, GEvent*);
};


//---------------------------------------------------------
class MyApp: public WindowApplication
{
public:
	MyApp(const UStringz N): WindowApplication(N) {};

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
	window.SetSize(500, 400);
	window.ScreenCenter();
	window.Show();
}


//---------------------------------------------------------
void MyWin::OnCreate()
{
	DatatypePixmap datatype;
	GWidget &frame = SetWidget(new GPanel);
//	GMenu *menu = (GMenu*)frame.Add(new GMenu, GWidget::TOP);

	GWidget *pnlTop = frame.Add(new GBox, GWidget::TOP, ",40,RAISED", 4);
	GWidget *pnl1 = frame.Add(new GPanel, GWidget::TOP, ",50", 4);
	GWidget *fpnlLeft = frame.Add(new GPanel, GWidget::LEFT, "120,", 2);
	GWidget *pnlLeft = fpnlLeft->Add(new GFrame("My Frame"), GWidget::CLIENT, ",,RAISED", 2);

	GButton *btnQuit = (GButton*) pnlTop->Add(new GButton, GWidget::LEFT, "60,,RAISED", 2);
	btnQuit->Add(new GPicture(datatype.Load("exit.png")), GWidget::LEFTCENTER);
	btnQuit->Add(new GText("Quit"), GWidget::LEFTCENTER);
	//pnlTop->Add(new GText("Right click here for popup"), GWidget::RIGHTCENTER);

	GButton *btnQuitFlat = (GButton*) pnlTop->Add(new GButton, GWidget::LEFT, "40,,,FLAT");
	btnQuitFlat->Add(new GPicture(datatype.Load("flower.png")), GWidget::CENTER);
	
	pnl1->Add(upd, GWidget::LEFT, "20,40");
	upd.SetRange(10, 60, 2);
	pnl1->Add(scr, GWidget::LEFTCENTER, "150,20");
	scr.SetRange(upd.Min(), upd.Max());
	pnl1->Add(prg, GWidget::LEFTCENTER, "150,20,RAISED");
	prg.SetRange(upd.Min(), upd.Max());
	//prg.SetDisplay(GProgressBar::PERCENT);

	/*GPopupMenu *entry = menu->AddItem("File");
	entry->AddItem("Open", datatype.Load("open.gif"));
	entry->AddItem("Save", DatatypePixmap::Load("save.gif"));
	entry->AddItem("Close", btnQuitFlat->Pixmap());
	entry->AddSeparator();
	entry->AddItem("Quit", btnQuit->Pixmap()).Connect(GEvent::OnClick, this,
								   GEventFunction(OnBtnQuit));
	menu->AddItem("Edit");
	menu->AddItem("Something");
	menu->AddItem("Window");
	menu->AddItem("Help", GWidget::RIGHT);*/
	
	GList *lst;
/*	lst = (GList*) pnlLeft->Add(new GDropList, GWidget::TOP, 0, 20);
	lst->AddItem("azerty");
	lst->AddItem("totor");
	lst->AddItem("xxxxx");
	lst->AddItem("shesbtcm");
	lst->AddItem("poiuytreza dee aezaza");
	lst->AddItem("qsdfgh");
	lst->AddItem("nbvcxw");

	lst = (GList*) pnlLeft->Add(new GDropEditList("DropEdit"), GWidget::TOP, 0, 20);
	lst->AddItem("azerty");
	lst->AddItem("totor");
	lst->AddItem("shesbtcm");
	lst->AddItem("poiuytreza dee aezaza");
	lst->AddItem("qsdfgh");
	lst->AddItem("nbvcxw");*/

	lst = (GList*)pnlLeft->Add(new GListBox, GWidget::TOP, "0,80");
	lst->Add(new GText("azerty"));
	lst->Add(new GText("totor"));
	lst->Add(new GText("xxxxx"));
	lst->Add(new GText("shesbtcm"));
	lst->Add(new GText("poiuytreza dee aezaza"));
	lst->Add(new GText("qsdfgh"));
	lst->Add(new GText("nbvcxw"));
	lst->Add(new GText("azerty"));
	lst->Add(new GText("totor"));
	lst->Add(new GText("shesbtcm"));
	lst->Add(new GText("poiuytreza dee aezaza"));
	lst->Add(new GText("qsdfgh"));
	lst->Add(new GText("nbvcxw"));
	lst->Add(new GText("azerty"));
	lst->Add(new GText("totor"));
	lst->Add(new GText("shesbtcm"));
	lst->Add(new GText("poiuytreza dee aezaza"));
	lst->Add(new GText("qsdfgh"));
	lst->Add(new GText("nbvcxw"));

	/*GTextEdit *txt = (GTextEdit*)pnl1->Add(new GTextEdit, GWidget::LEFTCENTER, "100,20,RAISED");
	txt->SetText(Title());*/

	/*GPopupMenu *popup = pnlTop->SetPopupMenu(new GPopupMenu);
	popup->AddItem("Nothing");
	popup->AddItem("About", datatype.Load("flower.gif"));
	popup->AddSeparator();
	popup->AddItem("Quit", btnQuit->Pixmap()).Connect(GEvent::OnClick, this,
								   GEventFunction(&MyWin::OnBtnQuit));*/

	upd.Connect(GEvent::OnValueChange, this, GEventFunction(&MyWin::OnUpdChange));
	scr.Connect(GEvent::OnValueChange, this, GEventFunction(&MyWin::OnUpdChange));
	//lst->Connect(GEvent::OnSelectItem, this, GEventFunction(&MyWin::OnLstSelect));
	//txt->Connect(GEvent::OnTextChange, this, GEventFunction(&MyWin::OnTxtChange));
	btnQuit->Connect(GEvent::OnClick, this, GEventFunction(&MyWin::OnBtnQuit));
	btnQuitFlat->Connect(GEvent::OnClick, this, GEventFunction(&MyWin::OnBtnQuit));
}


void MyWin::OnUpdChange(GWidget&W, GEvent*)
{
	GValueSelect &vs = (GValueSelect&)W;
	upd.SetValue(vs.Value());
	prg.SetValue(vs.Value());
	scr.SetValue(vs.Value());
	//Debug(dprintf("upd 0x%x %d -> %d (%d) = %d\n", &vs, vs.Min(), vs.Max(), vs.Step(), vs.Value());)
}


void MyWin::OnBtnQuit(GWidget&, GEvent*)
{
	/*IStdDialogMessage msgbox("Are you sure you want to quit ?", "",
							 IStdDialogMessage::BTN_YESNO);
	msgbox.Execute();
	if (msgbox.Result() ==IStdDialogMessage::RES_YES)*/
		{Close();}
}


void MyWin::OnLstSelect(GWidget&, GEvent*e)
{
	GEventItem *evt = (GEventItem*)e;
	//Debug(dprintf("Select %d: %s\n", evt->index, (_CHAR*)evt->item);)
}


void MyWin::OnTxtChange(GWidget&W, GEvent*)
{
	GTextEdit &text = (GTextEdit&)W;
//	Debug(dprintf("Text: %s\n", (_CHAR*)text.Text());)
}
