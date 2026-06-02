/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2000-2001 Henri Michelon

	TODO: recall when FILE_MUST_EXISTS

$Id: IStdDialogMotif.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_MOTIF
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;
#define XMSTRINGDEFINES
#include "modules/ModXm.hpp"
#include <Xm/Xm.h>

class XmCBDataMessage
{
public:
	ModXm *xm;
	IStdDialogMessage *dlg;
	int *quit;
	int btn;
	IStdDialogMessage::MessageBoxResult *result;
	XmCBDataMessage(ModXm *X, IStdDialogMessage*D, int*Q,
					IStdDialogMessage::MessageBoxResult*R, int B):
		xm(X), dlg(D), quit(Q), btn(B), result(R) {};
};

class XmCBDataFile
{
public:
	ModXm *xm;
	IStdDialogFile *dlg;
	int *quit;
	int btn;
	int *result;
	XmCBDataFile(ModXm *X, IStdDialogFile*D, int*Q,
				int*R, int B):
		xm(X), dlg(D), quit(Q), btn(B), result(R) {};
};

void dialogCB(Widget w,
		XmCBDataMessage* client_data,
		XmAnyCallbackStruct *call_data)
{
	XtUnmanageChild(w);
	(*client_data->quit) = 1;
	(*client_data->result) = (IStdDialogMessage::MessageBoxResult)client_data->btn;
}

void dialogCBFile(Widget w,
		XmCBDataFile* client_data,
		XmFileSelectionBoxCallbackStruct *selection)
{
	XtUnmanageChild(w);
	(*client_data->quit) = 1;
	(*client_data->result) = client_data->btn;

	char *filename;
	(*client_data->xm->XmStringGetLtoR)(selection->value,
					XmSTRING_DEFAULT_CHARSET,
					&filename);
	client_data->dlg->_filename = filename;
}

typedef  void (*TXmCallBack)(_WidgetRec *, void *, void *);



//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
	ModXm *xm = ModXm::Create();
	if (!xm) {
		dprintf("IStdDialogFile: can't open Motif (Xm) library\n");
		return FALSE;
	}

	Widget toplevel, dialog, button;
	Arg al[10];
	int ac = 0;
	int xm_mainloop_exit = 0;

	int argc = 0;
	char argv[1][1] = { "" };
	toplevel = XtInitialize(argv[0], "",NULL,0, &argc, (char**)argv);

	Stringz dir(_curdir);
	if (dir.Len() == 0) { dir = Dir::Current();	}
	if (dir.Right(1) != PATHSEP) { dir += PATHSEP; }
	XtSetArg(al[ac], XmNdirectory, (*xm->XmStringCreateSimple)(dir));
	ac++;

	Stringz fname(_filename);
	if (fname.Len() > 0) {
		XtSetArg(al[ac], XmNdirSpec, (*xm->XmStringCreateSimple)(fname));
		ac++;
	}

	Stringz filter(_filter);
	if (filter.Len() > 0) {
		XtSetArg(al[ac], XmNdirMask, (*xm->XmStringCreateSimple)(filter));
		ac++;
	}

	int _result;
	dialog = (*xm->XmCreateFileSelectionDialog)(toplevel,"dialog", al, ac);
	XmCBDataFile ok_data(xm, this, &xm_mainloop_exit, &_result, TRUE);
	XmCBDataFile cancel_data(xm, this, &xm_mainloop_exit, &_result, FALSE);
	XtAddCallback (dialog, XmNokCallback, (TXmCallBack)dialogCBFile, (void*)&ok_data);
	XtAddCallback (dialog, XmNcancelCallback, (TXmCallBack)dialogCBFile, (void*)&cancel_data);
	button = (*xm->XmSelectionBoxGetChild)(dialog, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild(button);
	XtManageChild(dialog);
	XtRealizeWidget(dialog);
	int myexit = 0;
	while (!myexit) {
		 XEvent event;
		 XtNextEvent(&event);
		 myexit = xm_mainloop_exit;
		 XtDispatchEvent(&event);
	}
	if (_filename.Len() == 0) { _filename = fname; }
	XtUnmanageChild(dialog);
	delete xm;
	return _result;
}



//---------------------------------------
_BOOL IStdDialogFileOpen :: Execute()
{
	_BOOL result = IStdDialogFile::Execute();
	if (_options & FO_SELECTDIRECTORY) {
		_filename = File::FilePath(_filename);
	}
	return result;
}



//---------------------------------------
_BOOL IStdDialogFileSave :: Execute()
{
	return IStdDialogFile::Execute();
}



//---------------------------------------
_BOOL IStdDialogMessage :: Execute()
{
	ModXm *xm = ModXm::Create();
	if (!xm) {
		dprintf("IStdDialogFile: can't open Motif (Xm) library\n");
		return FALSE;
	}

	Widget toplevel, dialog, button;
	Arg al[2];
	int ac = 0;
	int xm_mainloop_exit = 0;

	int argc = 0;
	char argv[1][1] = { "" };
	toplevel = XtInitialize(argv[0], "",NULL,0, &argc, (char**)argv);
	Stringz msg = _message;
	XtSetArg(al[ac], XmNmessageString,
			(*xm->XmStringCreateLtoR)(msg, XmSTRING_DEFAULT_CHARSET));
	ac++;
	if (_options & PIX_QUESTION) {
		dialog = (*xm->XmCreateQuestionDialog)(toplevel,"dialog", al, ac);
	}
	else if (_options & PIX_WARNING) {
		dialog = (*xm->XmCreateWarningDialog)(toplevel,"dialog", al, ac);
	}
	else if (_options & PIX_ERROR) {
		dialog = (*xm->XmCreateErrorDialog)(toplevel,"dialog", al, ac);
	}
	else {
		dialog = (*xm->XmCreateInformationDialog)(toplevel,"dialog", al, ac);
	}
	XmCBDataMessage ok_data(xm, this, &xm_mainloop_exit, &_result, RES_OK);
	XmCBDataMessage cancel_data(xm, this, &xm_mainloop_exit, &_result, RES_CANCEL);
	XtAddCallback (dialog, XmNokCallback, (TXmCallBack)dialogCB, (void*)&ok_data);
	XtAddCallback (dialog, XmNcancelCallback, (TXmCallBack)dialogCB, (void*)&cancel_data);
	button = (*xm->XmMessageBoxGetChild)(dialog, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild(button);
	XtManageChild(dialog);
	XtRealizeWidget(dialog);
	int myexit = 0;
	while (!myexit) {
		 XEvent event;
		 XtNextEvent(&event);
		 myexit = xm_mainloop_exit;
		 XtDispatchEvent(&event);
	}
	if ((_options & BTN_YESNO)  || (_options & BTN_YESNOCANCEL)) {
		if (_result == RES_OK) { _result = RES_YES; }
		if (_result == RES_CANCEL) { _result = RES_NO; }
	}
	XtUnmanageChild(dialog);
	delete xm;
	return TRUE;
}

#endif // HAVE_MOTIF
