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
#ifndef _INTERFACE_ISTDDIALOGFILEOPEN_H_
#define _INTERFACE_ISTDDIALOGFILEOPEN_H_

namespace NMO {

/*!
\brief		"open a file" dialog boxes


\author		(c) 1998-2001 Henri Michelon
\version	$Id: IStdDialogFileOpen.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IStdDialogFileOpen: public IStdDialogFile
{
public:
	typedef enum {
		FO_NONE = 0,
		FO_FILEMUSTEXISTS = 1,
		FO_SELECTDIRECTORY = 2
	} FileOpenOption;


	/*! Create a new "open a file" dialog 
	\param UStringz	: optional title of the dialog 
	\param UStringz	: optional display filter
	\param UStringz : optional default file name
	\param UStringz : optional current directory
	\param FileOpenOption : options*/
	IStdDialogFileOpen(const UStringz & = "", const Stringz & = "", 
					const Stringz & = "", const Stringz & = "",
					FileOpenOption = FO_NONE);

	virtual ~IStdDialogFileOpen() {};

	/*! Set the dialog options
	\param UStringz	: optional current directory name.*/
	virtual void SetOptions(FileOpenOption = FO_NONE);

	virtual _BOOL Execute();

protected:
	FileOpenOption	_options;
};


}

#endif
