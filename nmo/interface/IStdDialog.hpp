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
#ifndef _INTERFACE_ISTDDIALOG_H_
#define _INTERFACE_ISTDDIALOG_H_

namespace NMO {

/*!
\brief		"Standard" dialog boxes


\author		(c) 1998-2001 Henri Michelon
\version	$Id: IStdDialog.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IStdDialog: public NMOObject
{
public:
	/*! Create a new standard dialog 
	\param UStringz	: optional title of the dialog */
	IStdDialog(const UStringz & = "");

	virtual ~IStdDialog() {};

	/*! Set the title of the application 
	\param UStringz : optional title. A default title
					  will be used (the application name,
					  or an OS pre-defined dialog title) */
	virtual void SetTitle(const UStringz & = "");

	/*! Execute (display) the dialog box.
	\return TRUE if user press OK, FALSE otherwise. */
	virtual _BOOL Execute() = 0;

public:	/* public for GTK wrappers */
	_PTR		abstract; 
protected:
	UStringz	_title;
};


}

#endif
