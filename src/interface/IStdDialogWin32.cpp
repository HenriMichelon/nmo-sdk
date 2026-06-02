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

$Id: IStdDialogWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <Commdlg.h>
#include <Shlobj.h>
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>



//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileOpen :: Execute()
{
	Stringz title = _title;
	Stringz curdir = Dir::Current();

	IHandle parent = NULL;
	if (IWindow::Link::handles.Count() > 0) {
		ListIterator<IWindow::Link> lst(IWindow::Link::handles);
		if (lst[1].nmo->Visible()) { 
			parent = lst[1].handle; 
		}
	}

	if (_options & FO_SELECTDIRECTORY) {
		char folder[MAX_PATH];
		LPCITEMIDLIST pidl;
		BROWSEINFO bInfo;
		bInfo.hwndOwner      = IHANDLE(parent);
		bInfo.pidlRoot       = NULL;
	    bInfo.pszDisplayName = folder;
		bInfo.lpszTitle      = (_CHAR*)title;
        bInfo.ulFlags        = BIF_RETURNONLYFSDIRS;
		bInfo.lpfn           = NULL;
		if ( pidl = SHBrowseForFolder (&bInfo) ) {
			if ( SHGetPathFromIDList (pidl,folder) ) {
				if (folder[Stringz::Strlen(folder)-1] == PATHSEP) {
					folder[Stringz::Strlen(folder)-1] = 0;
				}
				_filename = folder;
				Dir::Change(curdir);
				return TRUE;
			}
        } 
		Dir::Change(curdir);
		return FALSE;
	}

	OPENFILENAME openfilename;
	Memory::Fill(&openfilename, 0, sizeof(openfilename));
	openfilename.lStructSize = sizeof(openfilename);

	openfilename.hwndOwner = IHANDLE(parent);
	openfilename.lpstrFile = new _CHAR[260];
	openfilename.lpstrFile[0] = 0;
	openfilename.nMaxFile = 260;
	if (_filter.Len()) {
		_CHAR* f = _filter;
		_DWORD l = _filter.Len();
		while (l--) {
			if (f[l] == '/') {
				f[l] = '\0';
			}
		}
		openfilename.lpstrFilter = f;
	}
	else {
		openfilename.lpstrFilter = "*.*\0*.*\0";
	}
	openfilename.nFilterIndex = 1;
	if (_title.Len()) {
		openfilename.lpstrFileTitle = title;
		openfilename.nMaxFileTitle = title.Len() + 1;
	}
	else {
		openfilename.lpstrFileTitle = NULL;
		openfilename.nMaxFileTitle = 0;
	}
	if (_filename.Len()) {
		Stringz::Strncpy(openfilename.lpstrFile, _filename, 260);
	}
	openfilename.lpstrInitialDir = _curdir;
	openfilename.Flags = OFN_HIDEREADONLY;

	if (_options & FO_FILEMUSTEXISTS) {
		openfilename.Flags |= OFN_FILEMUSTEXIST;
	}
	if (GetOpenFileName(&openfilename) == TRUE) {
		_filename = (_CHAR*)openfilename.lpstrFile;
		delete []openfilename.lpstrFile;
		Dir::Change(curdir);
		return TRUE;
	}
	delete []openfilename.lpstrFile;
	Dir::Change(curdir);
	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileSave :: Execute()
{
	Stringz title = _title;
	Stringz curdir = Dir::Current();

	OPENFILENAME openfilename;
	Memory::Fill(&openfilename, 0, sizeof(OPENFILENAME));
	openfilename.lStructSize = sizeof(OPENFILENAME);

	if (IWindow::Link::handles.Count() > 0) {
		ListIterator<IWindow::Link> lst(IWindow::Link::handles);
		if (lst[1].nmo->Visible()) {
			openfilename.hwndOwner = IHANDLE(lst[1].handle);
		}
	}
	else {
		openfilename.hwndOwner = NULL;
	}
	openfilename.lpstrFile = new _CHAR[260];
	openfilename.lpstrFile[0] = 0;
	openfilename.nMaxFile = 260;
	if (_filter.Len()) {
		_CHAR* f = _filter;
		_DWORD l = _filter.Len();
		while (l--) {
			if (f[l] == '/') {
				f[l] = '\0';
			}
		}
		openfilename.lpstrFilter = f;
	}
	else {
		openfilename.lpstrFilter = "*.*\0*.*\0";
	}
	openfilename.nFilterIndex = 1;
	if (_title.Len()) {
		openfilename.lpstrFileTitle = title;
		openfilename.nMaxFileTitle = title.Len() + 1;
	}
	else {
		openfilename.lpstrFileTitle = NULL;
		openfilename.nMaxFileTitle = 0;
	}
	if (_filename.Len()) {
		Stringz::Strncpy(openfilename.lpstrFile, _filename, 260);
	}
	openfilename.lpstrInitialDir = _curdir;
	openfilename.Flags = OFN_HIDEREADONLY;

	openfilename.Flags |= OFN_OVERWRITEPROMPT;
/*	if (_options & VALIDPATH) {
		openfilename.Flags |= OFN_PATHMUSTEXIST;
	}*/
	if (GetSaveFileName(&openfilename) == TRUE) {
		_filename = (_CHAR*)openfilename.lpstrFile;
		delete []openfilename.lpstrFile;
		Dir::Change(curdir);
		return TRUE;
	}
	delete []openfilename.lpstrFile;
	Dir::Change(curdir);
	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogMessage :: Execute()
{
	int result;
	UINT style = MB_TASKMODAL;
	if (_options & BTN_OK) { style |= MB_OK; }
	if (_options & BTN_OKCANCEL) { style |= MB_OKCANCEL; }
	if (_options & BTN_YESNO) { style |= MB_YESNO; }
	if (_options & BTN_YESNOCANCEL) { style |= MB_YESNOCANCEL; }
	if (_options & PIX_QUESTION) { style |= MB_ICONQUESTION; }
	if (_options & PIX_NOTICE) { style |= MB_ICONINFORMATION; }
	if (_options & PIX_WARNING) { style |= MB_ICONWARNING; }
	if (_options & PIX_ERROR) { style |= MB_ICONERROR; }

	if (!_title.Len()) { 
		_title = Application::App().Name();
	}

#ifdef _UNICODE
	result = MessageBoxW(NULL, (_WORD*)_message, (_WORD*)_title, _style);
#else
	Stringz title = _title;
	Stringz msg = _message;
	result = MessageBoxA(NULL, msg, title, style);
#endif
	switch (result) {
		case IDCANCEL: _result = RES_CANCEL; break;
		case IDOK: _result = RES_OK; break;
		case IDYES: _result = RES_YES; break;
		case IDNO: _result = RES_NO; break;
	};
	return TRUE;
}
#endif
