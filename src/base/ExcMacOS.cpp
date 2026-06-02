/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2001 Henri Michelon

$Id: ExcMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/Base.hpp>
using namespace NMO;

//--------------------------------------------------------
void NMOException::Display()
{
	SInt16 res;
	Str255 pstr;
	Str255 pmsg;
	AlertStdAlertParamRec asapr;

	Stringz _title = sTitle;
	CopyCStringToPascal(mMsg, pmsg);
	CopyCStringToPascal(_title, pstr);

	asapr.movable = true;
	asapr.helpButton =false;
	asapr.filterProc = NULL;
	asapr.defaultText = (const unsigned char*)-1;
	asapr.cancelText = NULL;
	asapr.otherText = NULL;
	asapr.defaultButton = kAlertStdAlertOKButton;
	asapr.cancelButton = 0;
	asapr.position = kWindowDefaultPosition;
	StandardAlert(kAlertStopAlert,
					pstr,
					pmsg,
					&asapr,
					&res);
}
#endif
