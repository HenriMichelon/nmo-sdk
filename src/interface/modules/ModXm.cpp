/* ModXm.cpp generated on 04 August 2001, 17:14:17 by genmod
	(c)2000-2001 Henri Michelon
*/
#include <nmo/NMO.hpp>

#if defined(HAVE_X11)
#if defined(HAVE_MOTIF)

#include "ModXm.hpp"
using namespace NMO;

ModXm* ModXm::Create()
{
	_BOOL result;
	ModXm *Xm = new ModXm;
	result = Xm->Load("Xm");
	if (result) {
		result &= (Xm->XmStringCreateLtoR = (FXmStringCreateLtoR)
				Xm->GetMethodAddress("XmStringCreateLtoR")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmStringCreateLtoR in dynamic module Xm\n"); })
		result &= (Xm->XmStringGetLtoR = (FXmStringGetLtoR)
				Xm->GetMethodAddress("XmStringGetLtoR")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmStringGetLtoR in dynamic module Xm\n"); })
		result &= (Xm->XmCreateFileSelectionDialog = (FXmCreateFileSelectionDialog)
				Xm->GetMethodAddress("XmCreateFileSelectionDialog")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmCreateFileSelectionDialog in dynamic module Xm\n"); })
		result &= (Xm->XmSelectionBoxGetChild = (FXmSelectionBoxGetChild)
				Xm->GetMethodAddress("XmSelectionBoxGetChild")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmSelectionBoxGetChild in dynamic module Xm\n"); })
		result &= (Xm->XmCreateQuestionDialog = (FXmCreateQuestionDialog)
				Xm->GetMethodAddress("XmCreateQuestionDialog")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmCreateQuestionDialog in dynamic module Xm\n"); })
		result &= (Xm->XmCreateWarningDialog = (FXmCreateWarningDialog)
				Xm->GetMethodAddress("XmCreateWarningDialog")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmCreateWarningDialog in dynamic module Xm\n"); })
		result &= (Xm->XmCreateErrorDialog = (FXmCreateErrorDialog)
				Xm->GetMethodAddress("XmCreateErrorDialog")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmCreateErrorDialog in dynamic module Xm\n"); })
		result &= (Xm->XmCreateInformationDialog = (FXmCreateInformationDialog)
				Xm->GetMethodAddress("XmCreateInformationDialog")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmCreateInformationDialog in dynamic module Xm\n"); })
		result &= (Xm->XmMessageBoxGetChild = (FXmMessageBoxGetChild)
				Xm->GetMethodAddress("XmMessageBoxGetChild")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmMessageBoxGetChild in dynamic module Xm\n"); })
		result &= (Xm->XmStringCreateSimple = (FXmStringCreateSimple)
				Xm->GetMethodAddress("XmStringCreateSimple")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method XmStringCreateSimple in dynamic module Xm\n"); })
	}
	if (!result) {
		delete Xm;
		return NULL;
	}
	dprintf("Dynamic module Xm loaded\n");
	return Xm;
}

#endif /* HAVE_MOTIF */
#endif /* HAVE_X11 */

