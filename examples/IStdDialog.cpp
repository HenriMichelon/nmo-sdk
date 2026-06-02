/*
    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

$Id: IStdDialog.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;


//---------------------------------------------------------
class MyWin: public IWindow
{
private:
	void EventCreate();
	void EventMouseDown(IMouse::Button, _LONG, _LONG);
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
CreateApplication(MyApp, "STD Dialog");


//---------------------------------------------------------
void MyApp::OnCreate()
{
	IStdDialogMessage msgbox1("Application starting", "A Message", 
							IStdDialogMessage::BTN_OK);
	msgbox1.Execute();
	if (!Add(window, IWindow::SIZEABLE)) 
		throw NMOException("Error creating main window");
	window.SetSize(422, 250);
	window.ScreenCenter();
	window.Show();
}


//---------------------------------------------------------
void MyWin::EventCreate()
{
	IStdDialogFileOpen fileopen("File open Title", "", "myfile.txt", Dir::Current());
	if (fileopen.Execute()) {
		UStringz msg = "request OPEN file ";
		msg += UStringz(fileopen.FileName());
		IStdDialogMessage msgbox(msg, "Requester title", 
								IStdDialogMessage::BTN_OKCANCEL +
								IStdDialogMessage::PIX_NOTICE);
		msgbox.Execute();
	}
	
	IStdDialogFileSave filesave("File save Title", "", "myfile.txt", 
								Dir::Current(), IStdDialogFileSave::FS_VALIDPATH);
	if (filesave.Execute()) {	
		UStringz msg1 = UStringz("request SAVE file ");
		msg1 += UStringz(filesave.FileName());
		IStdDialogMessage msgbox1(msg1, "Requester title", 
								IStdDialogMessage::BTN_YESNO +
								IStdDialogMessage::PIX_QUESTION);
		msgbox1.Execute();
	}
}

//---------------------------------------------------------
void MyWin::EventMouseDown(IMouse::Button, _LONG, _LONG)
{
	Close();
}
