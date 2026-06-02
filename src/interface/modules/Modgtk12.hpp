/* Modgtk12.hpp generated on 23 July 2001, 18:59:09 by genmod
	(c)2000-2001 Henri Michelon
*/
#ifndef _GENMOD_MODModgtk12
#define _GENMOD_MODModgtk12

#if defined(HAVE_X11)
#if defined(HAVE_GTK)

#include <gtk/gtk.h>

namespace NMO {

class Modgtk12: public DynModule
{
public:
	static Modgtk12* Create();
	typedef void  (*Fgtk_init_check)(int *argc, char ***argv);
	typedef void  (*Fgtk_widget_hide_all)(GtkWidget *widget);
	typedef void  (*Fgtk_main_quit)(void);
	typedef gchar*  (*Fgtk_file_selection_get_filename)(GtkFileSelection *filesel);
	typedef GtkWidget*  (*Fgtk_file_selection_new)(const gchar *title);
	typedef void  (*Fgtk_file_selection_set_filename)(GtkFileSelection *filesel, const gchar *filename);
	typedef guint  (*Fgtk_signal_connect)(GtkObject *object, const gchar *name, GtkSignalFunc func, gpointer func_data);
	typedef void  (*Fgtk_main)(void);
	typedef GtkWidget*  (*Fgtk_dialog_new)(void);
	typedef GtkWidget*  (*Fgtk_label_new)(const gchar *str);
	typedef GtkWidget*  (*Fgtk_button_new_with_label)(const gchar *label);
	typedef void  (*Fgtk_container_add)(GtkContainer *container, GtkWidget *widget);
	typedef void  (*Fgtk_widget_show)(GtkWidget *widget);
	typedef void  (*Fgtk_widget_show_all)(GtkWidget *widget);

	Fgtk_init_check	gtk_init_check;
	Fgtk_widget_hide_all	gtk_widget_hide_all;
	Fgtk_main_quit	gtk_main_quit;
	Fgtk_file_selection_get_filename	gtk_file_selection_get_filename;
	Fgtk_file_selection_new	gtk_file_selection_new;
	Fgtk_file_selection_set_filename	gtk_file_selection_set_filename;
	Fgtk_signal_connect	gtk_signal_connect;
	Fgtk_main	gtk_main;
	Fgtk_dialog_new	gtk_dialog_new;
	Fgtk_label_new	gtk_label_new;
	Fgtk_button_new_with_label	gtk_button_new_with_label;
	Fgtk_container_add	gtk_container_add;
	Fgtk_widget_show	gtk_widget_show;
	Fgtk_widget_show_all	gtk_widget_show_all;
};

} // namespace NMO

#endif /* HAVE_GTK */
#endif /* HAVE_X11 */

#endif
