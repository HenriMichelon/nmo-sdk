/* Modgtk12.cpp generated on 23 July 2001, 18:59:09 by genmod
	(c)2000-2002 Henri Michelon
*/
#include <nmo/NMO.hpp>

#if defined(HAVE_X11)
#if defined(HAVE_GTK)

#include "Modgtk12.hpp"
using namespace NMO;

Modgtk12* Modgtk12::Create()
{
	_BOOL result;
	Modgtk12 *gtk12 = new Modgtk12;
	result = gtk12->Load("gtk");
	if (!result) { result = gtk12->Load("gtk12"); }
	if (!result) { result = gtk12->Load("gtk-1.2"); }
	if (result) {
		result &= (gtk12->gtk_init_check = (Fgtk_init_check)
				gtk12->GetMethodAddress("gtk_init_check")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_init in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_widget_hide_all = (Fgtk_widget_hide_all)
				gtk12->GetMethodAddress("gtk_widget_hide_all")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_widget_hide_all in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_main_quit = (Fgtk_main_quit)
				gtk12->GetMethodAddress("gtk_main_quit")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_main_quit in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_file_selection_get_filename = (Fgtk_file_selection_get_filename)
				gtk12->GetMethodAddress("gtk_file_selection_get_filename")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_file_selection_get_filename in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_file_selection_new = (Fgtk_file_selection_new)
				gtk12->GetMethodAddress("gtk_file_selection_new")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_file_selection_new in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_file_selection_set_filename = (Fgtk_file_selection_set_filename)
				gtk12->GetMethodAddress("gtk_file_selection_set_filename")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_file_selection_set_filename in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_signal_connect = (Fgtk_signal_connect)
				gtk12->GetMethodAddress("gtk_signal_connect")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_signal_connect in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_main = (Fgtk_main)
				gtk12->GetMethodAddress("gtk_main")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_main in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_dialog_new = (Fgtk_dialog_new)
				gtk12->GetMethodAddress("gtk_dialog_new")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_dialog_new in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_label_new = (Fgtk_label_new)
				gtk12->GetMethodAddress("gtk_label_new")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_label_new in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_button_new_with_label = (Fgtk_button_new_with_label)
				gtk12->GetMethodAddress("gtk_button_new_with_label")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_button_new_with_label in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_container_add = (Fgtk_container_add)
				gtk12->GetMethodAddress("gtk_container_add")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_container_add in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_widget_show = (Fgtk_widget_show)
				gtk12->GetMethodAddress("gtk_widget_show")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_widget_show in dynamic module gtk12\n"); })
		result &= (gtk12->gtk_widget_show_all = (Fgtk_widget_show_all)
				gtk12->GetMethodAddress("gtk_widget_show_all")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method gtk_widget_show_all in dynamic module gtk12\n"); })
	}
	if (!result) {
		delete gtk12;
		return NULL;
	}
	dprintf("Dynamic module gtk12 loaded\n");
	return gtk12;
}

#endif /* HAVE_GTK */
#endif /* HAVE_X11 */

