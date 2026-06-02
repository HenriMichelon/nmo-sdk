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

*/
#ifndef _INTERFACE_ISTDDIALOGMSG_H_
#define _INTERFACE_ISTDDIALOGMSG_H_

namespace NMO {

/*!
\brief		Standard message box


\author		(c) 1998-2001 Henri Michelon
\version	$Id: IStdDialogMessage.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IStdDialogMessage: public IStdDialog
{
public:
	typedef enum {
		BTN_OK = 1,
		BTN_OKCANCEL = 2,
		BTN_YESNO = 4,
		BTN_YESNOCANCEL = 8,
		PIX_QUESTION = 16,
		PIX_NOTICE = 32,
		PIX_WARNING = 64,
		PIX_ERROR = 128
	} MessageBoxOption;

	typedef enum {
		RES_OK,
		RES_CANCEL,
		RES_YES,
		RES_NO
	} MessageBoxResult;

	/*! Create a new standard dialog 
	\param UStringz	: text of the message box.
	\param UStringz	: optional title of the dialog.
	\param _DWORD	: options */
	IStdDialogMessage(const UStringz & = "", 
						const UStringz & = "",
						_DWORD = 0);

	virtual ~IStdDialogMessage() {};

	/*! Set the text of the message 
	\param UStringz : text */
	virtual void SetText(const UStringz & = "");

	/*! Set the options of the message box
	\param _DWORD : options */
	virtual void SetOptions(_DWORD = 0);

	MessageBoxResult Result();

	/*! Execute (display) the dialog box.
	\return For this kind of dialog, Execute() always return TRUE.
			Use Result() instead. */
	virtual _BOOL Execute();

protected:
	_DWORD				_options;
	UStringz			_message;
	MessageBoxResult	_result;
};

inline IStdDialogMessage::MessageBoxResult IStdDialogMessage::Result()
{ return _result; }


}

#endif
