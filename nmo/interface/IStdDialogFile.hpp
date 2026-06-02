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
#ifndef _INTERFACE_ISTDDIALOGFILE_H_
#define _INTERFACE_ISTDDIALOGFILE_H_

namespace NMO {

/*!
\brief		File manipulation dialog boxes


\author		(c) 1998-2001 Henri Michelon
\version	$Id: IStdDialogFile.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IStdDialogFile: public IStdDialog
{
public:
	/*! Create a new file dialog 
	\param UStringz	: optional title of the dialog 
	\param UStringz	: optional display filter
	\param UStringz : optional default file name
	\param UStringz : optional current directory */
	IStdDialogFile(const UStringz & = "", const Stringz & = "",
					const Stringz & = "", const Stringz & = "");

	virtual ~IStdDialogFile() {};

	/*! Set the display (file selection) filter string
	\param UStringz	: optional display filter.
						Format depend on operating system.
						default is "show all files"*/
	virtual void SetDisplayFilter(const Stringz & = "");

	/*! Set the default file name
	\param UStringz	: optional default file name.*/
	virtual void SetDefaultFileName(const UStringz & = "");

	/*! Set the current directory
	\param UStringz	: optional current directory name.*/
	virtual void SetCurrentDirectory(const UStringz & = "");

	virtual _BOOL Execute();

	/*! Return the name of the selected file.
		Valid only after a successful call to Execute().
	\return name (with path) of the selected file.*/
	virtual Stringz FileName();

	/* must be public for dialog callbacks functions on some OS */
	Stringz	_filename;
	
protected:
	Stringz	_curdir;
	Stringz	_filter;
};


}

#endif
