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

	(c) 2000-2002 Henri Michelon

$Id: IStdDialogGTK.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/Base.hpp>
#ifdef HAVE_GTK
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

#include "modules/Modgtk12.hpp"
#include <gtk/gtk.h>

class StdDialogGtkWrapper
{
public:
	IStdDialog	*dialog;
	Modgtk12	*gtk;
	StdDialogGtkWrapper(IStdDialog *D, Modgtk12 *M):
		dialog(D), gtk(M) {};
};

#define GTK_GTK_OBJECT (GtkObject*)
#define GTK_GTK_FILE_SELECTION (GtkFileSelection*)
#define GTK_GTK_CONTAINER (GtkContainer*)
#define GTK_GTK_DIALOG (GtkDialog*)

void nmo_stddialog_GTK_OK(GtkWidget*, gpointer user_data) 
{
	StdDialogGtkWrapper *wrapper = (StdDialogGtkWrapper*)user_data;
	(*wrapper->gtk->gtk_widget_hide_all)((GtkWidget*)wrapper->dialog->abstract);
	wrapper->dialog->abstract = (_PTR)1;
	(*wrapper->gtk->gtk_main_quit)();
}


void nmo_stddialog_GTK_YES(GtkWidget*, gpointer user_data) 
{
	StdDialogGtkWrapper *wrapper = (StdDialogGtkWrapper*)user_data;
	(*wrapper->gtk->gtk_widget_hide_all)((GtkWidget*)wrapper->dialog->abstract);
	wrapper->dialog->abstract = (_PTR)2;
	(*wrapper->gtk->gtk_main_quit)();
}


void nmo_stddialog_GTK_NO(GtkWidget*, gpointer user_data) 
{
	StdDialogGtkWrapper *wrapper = (StdDialogGtkWrapper*)user_data;
	(*wrapper->gtk->gtk_widget_hide_all)((GtkWidget*)wrapper->dialog->abstract);
	wrapper->dialog->abstract = (_PTR)3;
	(*wrapper->gtk->gtk_main_quit)();
}


void nmo_stddialog_GTK_CANCEL(GtkWidget*, gpointer user_data) 
{
	StdDialogGtkWrapper *wrapper = (StdDialogGtkWrapper*)user_data;
	(*wrapper->gtk->gtk_widget_hide_all)((GtkWidget*)wrapper->dialog->abstract);
	wrapper->dialog->abstract = (_PTR)0;
	(*wrapper->gtk->gtk_main_quit)();
}


void nmo_stddialog_STORE_FILENAME(GtkFileSelection *, gpointer user_data) 
{
	StdDialogGtkWrapper *wrapper = (StdDialogGtkWrapper*)user_data;
	IStdDialogFileOpen* dialog = (IStdDialogFileOpen*)wrapper->dialog;
	GtkFileSelection* selector = (GtkFileSelection*)dialog->abstract;
	dialog->_filename = (*wrapper->gtk->gtk_file_selection_get_filename)(GTK_GTK_FILE_SELECTION(selector));
}


//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
	Modgtk12 *gtk = Modgtk12::Create();
	if (!gtk) {
		dprintf("IStdDialogFile: can't open GTK 1.2 library\n");
		return FALSE; 
	}
	(*gtk->gtk_init_check)(Application::App().Argv().argc(), 
						   NULL);
	Stringz title = _title;

	GtkWidget *file_selector;
	file_selector = (*gtk->gtk_file_selection_new)(title);

	Stringz dir(_curdir);
	if (dir.Len() == 0) {
		dir = Dir::Current();
	}
	if (dir.Right(1) != PATHSEP) {
		dir += PATHSEP;
	}
	(*gtk->gtk_file_selection_set_filename)(GTK_GTK_FILE_SELECTION(file_selector),
									dir + _filename);

	StdDialogGtkWrapper wrapper(this, gtk);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT ((GTK_GTK_FILE_SELECTION(file_selector))->ok_button),
						"clicked", GTK_SIGNAL_FUNC (nmo_stddialog_STORE_FILENAME), &wrapper);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT ((GTK_GTK_FILE_SELECTION(file_selector))->ok_button),
						"clicked", GTK_SIGNAL_FUNC (nmo_stddialog_GTK_OK), &wrapper);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT ((GTK_GTK_FILE_SELECTION(file_selector))->cancel_button),
						"clicked", GTK_SIGNAL_FUNC (nmo_stddialog_GTK_CANCEL), &wrapper);
	
	abstract = (_PTR)file_selector;
	(*gtk->gtk_widget_show)(file_selector);
	(*gtk->gtk_main)();
	delete gtk;
	return ((_DWORD)abstract) == 1;
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
	Modgtk12 *gtk = Modgtk12::Create();
	if (!gtk) { 
		dprintf("IStdDialogMessage: can't open GTK 1.2 library\n");
		return FALSE; 
	}
	(*gtk->gtk_init_check)(Application::App().Argv().argc(), 
						   NULL);
	/*(*gtk->gtk_init_check)(Application::App().Argv().argc(), 
						   Application::App().Argv().argv());*/
	// from http://developer.gnome.org/doc/API/gtk/gtkdialog.html
	GtkWidget *dialog, *label;
	GtkWidget *btnok, *btncancel, *btnyes, *btnno;
	/* Create the widgets */
	dialog = (*gtk->gtk_dialog_new)();
	Stringz title = _title;
	Stringz msg = _message;
	label = (*gtk->gtk_label_new)(msg);
	btnok = (*gtk->gtk_button_new_with_label)("Okay");
	btncancel = (*gtk->gtk_button_new_with_label)("Cancel");
	btnyes = (*gtk->gtk_button_new_with_label)("Yes");
	btnno = (*gtk->gtk_button_new_with_label)("No");
	/* Ensure that the dialog box is destroyed when the user clicks ok. */
	StdDialogGtkWrapper wrapper(this, gtk);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT (btnok), "clicked",
					   GTK_SIGNAL_FUNC (nmo_stddialog_GTK_OK), 
					   &wrapper);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT (btncancel), "clicked",
					   GTK_SIGNAL_FUNC (nmo_stddialog_GTK_CANCEL), 
					   &wrapper);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT (btnyes), "clicked",
					   GTK_SIGNAL_FUNC (nmo_stddialog_GTK_YES), 
					   &wrapper);
	(*gtk->gtk_signal_connect)(GTK_GTK_OBJECT (btnno), "clicked",
					   GTK_SIGNAL_FUNC (nmo_stddialog_GTK_NO), 
					   &wrapper);

    if ((_options & BTN_OK) || (!_options)) { 
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnok);
	}
    if (_options & BTN_OKCANCEL) { 
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnok);
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btncancel);
	}
    if (_options & BTN_YESNO) { 
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnyes);
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnno);
	}
    if (_options & BTN_YESNOCANCEL) { 
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnyes);
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btnno);
		(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->action_area),
						   btncancel);
	}

	/* Add the label, and show everything we've added to the dialog. */
	(*gtk->gtk_container_add)(GTK_GTK_CONTAINER ((GTK_GTK_DIALOG(dialog))->vbox),
					   label);
	abstract = (_PTR)dialog;
	(*gtk->gtk_widget_show_all)(dialog);
	(*gtk->gtk_main)();
    switch ((_DWORD)abstract) {
        case 0: _result = RES_CANCEL; break;
        case 1: _result = RES_OK; break;
        case 2: _result = RES_YES; break;
        case 3: _result = RES_NO; break;
	};
	delete gtk;
	return TRUE;
}

#endif // HAVE_GTK
