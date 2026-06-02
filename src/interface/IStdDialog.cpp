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

$Id: IStdDialog.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;


//---------------------------------------
IStdDialog :: IStdDialog(const UStringz & TITLE)
{
	_title = TITLE;
}


//---------------------------------------
void IStdDialog :: SetTitle(const UStringz & TITLE)
{
	_title = TITLE;
}


//---------------------------------------
IStdDialogFile :: IStdDialogFile(const UStringz & TITLE,
								 const Stringz & FILTER,
								const Stringz & FNAME,
								const Stringz & CURDIR):

IStdDialog(TITLE)
{
	_filename = FNAME;
	_curdir = CURDIR;
	_filter = FILTER;
}


//---------------------------------------
void IStdDialogFile :: SetDisplayFilter(const Stringz & FILTER)
{
	_filter = FILTER;
}


//---------------------------------------
Stringz IStdDialogFile :: FileName()
{
	return _filename;
}


//---------------------------------------
void IStdDialogFile :: SetDefaultFileName(const UStringz & FNAME)
{
	_filename = FNAME;
}


//---------------------------------------
void IStdDialogFile :: SetCurrentDirectory(const UStringz & CURDIR)
{
	_curdir = CURDIR;
}



//---------------------------------------
IStdDialogFileOpen :: IStdDialogFileOpen(const UStringz & TITLE,
										const Stringz & FILTER,
										const Stringz & FNAME,
										const Stringz & CURDIR,
										FileOpenOption OPTIONS):
IStdDialogFile(TITLE, FILTER, FNAME, CURDIR)
{
	_options = OPTIONS;
}


//---------------------------------------
void IStdDialogFileOpen :: SetOptions(FileOpenOption OPTIONS)
{
	_options = OPTIONS;
}




//---------------------------------------
IStdDialogFileSave :: IStdDialogFileSave(const UStringz & TITLE,
										const Stringz & FILTER,
										const Stringz & FNAME,
										const Stringz & CURDIR,
										FileSaveOption OPTIONS):
IStdDialogFile(TITLE, FILTER, FNAME, CURDIR)
{
	_options = OPTIONS;
}


//---------------------------------------
void IStdDialogFileSave :: SetOptions(FileSaveOption OPTIONS)
{
	_options = OPTIONS;
}


//---------------------------------------
IStdDialogMessage :: IStdDialogMessage(const UStringz & MSG,
									const UStringz & TITLE,
									_DWORD OPTIONS):
IStdDialog(TITLE)
{
	_message = MSG;
	_options = OPTIONS;
}


//---------------------------------------
void IStdDialogMessage :: SetText(const UStringz & MSG)
{
	_message = MSG;
}


//---------------------------------------
void IStdDialogMessage :: SetOptions(_DWORD OPTIONS)
{
	_options = OPTIONS;
}



