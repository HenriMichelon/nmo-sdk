/* ModXm.hpp generated on 04 August 2001, 17:14:17 by genmod
	(c)2000-2001 Henri Michelon
*/
#ifndef _GENMOD_MODModXm
#define _GENMOD_MODModXm

#if defined(HAVE_X11)
#if defined(HAVE_MOTIF)

#include <Xm/Xm.h>

namespace NMO {

class ModXm: public DynModule
{
public:
	static ModXm* Create();
	typedef XmString  (*FXmStringCreateLtoR)(char *text,XmStringCharSet charset);
	typedef Boolean  (*FXmStringGetLtoR)(XmString string,XmStringCharSet charset,char **text);
	typedef Widget  (*FXmCreateFileSelectionDialog)(Widget ds_p,String name,ArgList fsb_args,Cardinal fsb_n);
	typedef Widget  (*FXmSelectionBoxGetChild)(Widget sb,unsigned int which);
	typedef Widget  (*FXmCreateQuestionDialog)(Widget parent,char *name,ArgList al,Cardinal ac);
	typedef Widget  (*FXmCreateWarningDialog)(Widget parent,char *name,ArgList al,Cardinal ac);
	typedef Widget  (*FXmCreateErrorDialog)(Widget parent,char *name,ArgList al,Cardinal ac);
	typedef Widget  (*FXmCreateInformationDialog)(Widget parent,char *name,ArgList al,Cardinal ac);
	typedef Widget  (*FXmMessageBoxGetChild)(Widget widget,unsigned int child);
	typedef XmString  (*FXmStringCreateSimple)(char *text);


	FXmStringCreateLtoR	XmStringCreateLtoR;
	FXmStringGetLtoR	XmStringGetLtoR;
	FXmCreateFileSelectionDialog	XmCreateFileSelectionDialog;
	FXmSelectionBoxGetChild	XmSelectionBoxGetChild;
	FXmCreateQuestionDialog	XmCreateQuestionDialog;
	FXmCreateWarningDialog	XmCreateWarningDialog;
	FXmCreateErrorDialog	XmCreateErrorDialog;
	FXmCreateInformationDialog	XmCreateInformationDialog;
	FXmMessageBoxGetChild	XmMessageBoxGetChild;
	FXmStringCreateSimple	XmStringCreateSimple;
};

} // namespace NMO

#endif /* HAVE_MOTIF */
#endif /* HAVE_X11 */

#endif
