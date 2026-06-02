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

	(c) 2000-2001 Henri Michelon

$Id: IStdDialogMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IDisplayMacOS.hpp"
using namespace NMO;

static pascal void myEventProc(NavEventCallbackMessage, 
                        NavCBRecPtr, 
                        NavCallBackUserData)
{
}



//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileOpen :: Execute()
{
	AEDesc				defaultLocation;
	AEDesc				*defaultLocationptr = NULL;
	NavReplyRecord		reply;
	NavDialogOptions	dialogOptions;
	NavEventUPP         eventProc = NewNavEventUPP(myEventProc);
	if (NavGetDefaultDialogOptions(&dialogOptions) != noErr) {
		return FALSE;
	}
	if (_title.Len()) {
		Stringz	title = _title.To("APPLE_ROMAN");
		CopyCStringToPascal(title, dialogOptions.windowTitle); 
	}
	if (_curdir.Len()) {
		FSSpec fsspec;
		Str255 curdir;
		CopyCStringToPascal(_curdir, curdir);
		if (FSMakeFSSpec(0, 0, curdir, &fsspec)) {
			return FALSE;
		}
		if (AECreateDesc(typeFSS, &fsspec,
                         sizeof(fsspec),
                         &defaultLocation ) != noErr) {
         	return FALSE;
         }
         defaultLocationptr = &defaultLocation;
	}
	dialogOptions.dialogOptionFlags ^= kNavAllowPreviews;
	dialogOptions.dialogOptionFlags |= kNavAllFilesInPopup;
	OSErr err;
	if (_options & FO_SELECTDIRECTORY) {
		err = NavChooseFolder(defaultLocationptr,
								&reply,
								&dialogOptions,
								eventProc,
								(NavObjectFilterUPP) NULL,
								NULL);
	}
	else {
		err = NavGetFile(defaultLocationptr,
								&reply,
								&dialogOptions,
								eventProc,
								(NavPreviewUPP) NULL,
								(NavObjectFilterUPP) NULL,
								(NavTypeListHandle)NULL,
								NULL);
	}
	
   	if (err == noErr && reply.validRecord) {
		long    count;
                
        err = AECountItems(&(reply.selection), &count);
        if ((err == noErr) && (count > 0)) {
	        AEKeyword   theKeyword;
    	    DescType    actualType;
        	Size        actualSize;
	        FSSpec      documentFSSpec;
                        
    	     err = AEGetNthPtr(&(reply.selection), 1,
                             typeFSS, &theKeyword,
                             &actualType,&documentFSSpec,
                             sizeof(documentFSSpec),
                             &actualSize);
	         if (err == noErr){
	         	_filename = EnvPath::ExtractPath(documentFSSpec.vRefNum,
	         									documentFSSpec.parID);
				/*Str255		dirname;
				CInfoPBRec 	pbrec;
				pbrec.dirInfo.ioNamePtr = (StringPtr)&dirname;
				pbrec.dirInfo.ioVRefNum = documentFSSpec.vRefNum;
				pbrec.dirInfo.ioDrParID = documentFSSpec.parID;
				pbrec.dirInfo.ioFDirIndex = -1;
				_filename = "";
				do {
					pbrec.dirInfo.ioDrDirID = pbrec.dirInfo.ioDrParID;
					if (::PBGetCatInfoSync(&pbrec) == noErr) {
						CopyPascalStringToC(dirname, cdirname);
						_filename = Stringz(cdirname) + PATHSEP + _filename;
					}
				} while (pbrec.dirInfo.ioDrDirID != fsRtDirID);*/
				if (!(_options & FO_SELECTDIRECTORY)) {
					_CHAR cdirname[260];
					CopyPascalStringToC(documentFSSpec.name, cdirname);
					if ((_filename.Len() > 0) && 
						(_filename[_filename.Len()-1] != PATHSEP)) {
						_filename += PATHSEP;
					}
					_filename += cdirname;
				}
        	 }
		}
		NavDisposeReply(&reply);
		if ((!(_options & FO_SELECTDIRECTORY) ) && 
			(_options & FO_FILEMUSTEXISTS) && 
			(!File::Exists(_filename))) {
			return Execute();
		}
		return TRUE;
	}
	NavDisposeReply(&reply);
 	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileSave :: Execute()
{
	AEDesc				defaultLocation;
	AEDesc				*defaultLocationptr = NULL;
	NavReplyRecord		reply;
	NavDialogOptions	dialogOptions;
	NavEventUPP         eventProc = NewNavEventUPP(myEventProc);
	if (NavGetDefaultDialogOptions(&dialogOptions) != noErr) {
		return FALSE;
	}
	if (_title.Len()) {
		Stringz	title = _title.To("APPLE_ROMAN");
		CopyCStringToPascal(title, dialogOptions.windowTitle); 
	}
	if (_curdir.Len()) {
		FSSpec fsspec;
		Str255 curdir;
		CopyCStringToPascal(_curdir, curdir);
		if (FSMakeFSSpec(0, 0, curdir, &fsspec)) {
			return FALSE;
		}
		if (AECreateDesc(typeFSS, &fsspec,
                         sizeof(fsspec),
                         &defaultLocation ) != noErr) {
         	return FALSE;
         }
         defaultLocationptr = &defaultLocation;
	}
	dialogOptions.dialogOptionFlags ^= kNavAllowPreviews;
	dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
	if (_filename.Len()) {
		CopyCStringToPascal(_filename, dialogOptions.savedFileName); 
	}
	OSErr err = NavPutFile(defaultLocationptr,
							&reply,
							&dialogOptions,
							eventProc,
							kNavGenericSignature,
							kNavGenericSignature,
							NULL);

   	if (err == noErr && reply.validRecord) {
		long    count;
                
        err = AECountItems(&(reply.selection), &count);
        if ((err == noErr) && (count > 0)) {
	        AEKeyword   theKeyword;
    	    DescType    actualType;
        	Size        actualSize;
	        FSSpec      documentFSSpec;
                        
    	     err = AEGetNthPtr(&(reply.selection), 1,
                             typeFSS, &theKeyword,
                             &actualType,&documentFSSpec,
                             sizeof(documentFSSpec),
                             &actualSize);
	         if (err == noErr){
				Str255		dirname;
				_CHAR		cdirname[255];
				CInfoPBRec 	pbrec;
				pbrec.dirInfo.ioNamePtr = (StringPtr)&dirname;
				pbrec.dirInfo.ioVRefNum = documentFSSpec.vRefNum;
				pbrec.dirInfo.ioDrParID = documentFSSpec.parID;
				pbrec.dirInfo.ioFDirIndex = -1;
				_filename = "";
				do {
					pbrec.dirInfo.ioDrDirID = pbrec.dirInfo.ioDrParID;
					if (::PBGetCatInfoSync(&pbrec) == noErr) {
						CopyPascalStringToC(dirname, cdirname);
						_filename = Stringz(cdirname) + PATHSEP + _filename;
					}
				} while (pbrec.dirInfo.ioDrDirID != fsRtDirID);
				CopyPascalStringToC(documentFSSpec.name, cdirname);
				_filename += cdirname;
        	 }
		}
/*		if ((_options & FO_VALIDPATH) && (!Dir::Exists(File::FilePath(_filename)))) {
			NavDisposeReply(&reply);
			return Execute();
		}*/
		return TRUE;
	}
	NavDisposeReply(&reply);
 	return FALSE;
}


//---------------------------------------
_BOOL IStdDialogMessage :: Execute()
{
	AlertType type;
	ConstStringPtr okbtn = NULL;
	ConstStringPtr cancelbtn = NULL;
	ConstStringPtr otherbtn = NULL;
	if (_options & BTN_OK) { 
		okbtn = (const unsigned char*)-1; 
	}
	if (_options & BTN_OKCANCEL) { 
		okbtn = (const unsigned char*)-1; 
		cancelbtn = (const unsigned char*)-1; 
	}
	if (_options & BTN_YESNO) { 
		okbtn = "\pYes"; 
		otherbtn = "\pNo";		
	}
	if (_options & BTN_YESNOCANCEL) { 
		okbtn = "\pYes"; 
		otherbtn = "\pNo";					
		cancelbtn = (const unsigned char*)-1; 
	}
	if (_options & PIX_QUESTION) { type = kAlertStopAlert; }
	if (_options & PIX_NOTICE) { type = kAlertNoteAlert; }
	if (_options & PIX_WARNING) { type = kAlertCautionAlert; }
	if (_options & PIX_ERROR) { type = kAlertStopAlert; }

	if (!_title.Len()) { 
		_title = Application::App().Name();
	}

	Str255	pmsg;
	Str255	ptitle;
	SInt16	result;
	Stringz	title = _title.To("APPLE_ROMAN");
	Stringz	msg = _message.To("APPLE_ROMAN");
	AlertStdAlertParamRec asapr;
	
	CopyCStringToPascal(msg, pmsg);
	CopyCStringToPascal(title, ptitle);

	asapr.movable = true;
	asapr.helpButton =false;
	asapr.filterProc = NULL;
	asapr.defaultText = okbtn;
	asapr.cancelText = cancelbtn;
	asapr.otherText = otherbtn;
	asapr.defaultButton = kAlertStdAlertOKButton;
	asapr.cancelButton = kAlertStdAlertCancelButton;
	asapr.position = kWindowDefaultPosition;
	StandardAlert(type,
					ptitle,
					pmsg,
					&asapr,
					&result);

	switch (result) {
		case kAlertStdAlertOKButton: 
			if ((_options & BTN_OK) || (_options & BTN_OKCANCEL)) 
				{ _result = RES_OK; }
			if ((_options & BTN_YESNO) || (_options & BTN_YESNOCANCEL)) 
				{ _result = RES_YES; }
			break;
		case kAlertStdAlertCancelButton: _result = RES_CANCEL; break;
		case kAlertStdAlertOtherButton: _result = RES_NO; break;
	};
	return TRUE;
}


#endif
