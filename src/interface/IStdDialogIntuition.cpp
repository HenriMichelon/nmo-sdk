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

    (c) 2000 Christophe Decanini

$Id: IStdDialogIntuition.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <exec/exec.h>
#include <clib/exec_protos.h>
#include <intuition/intuition.h>
#include <clib/intuition_protos.h>
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;
#include <exec/types.h>
#include <exec/libraries.h>
#include <libraries/asl.h>
#include <clib/asl_protos.h>



//---------------------------------------
_BOOL IStdDialogFile :: Execute()
{
    return FALSE;
}


//---------------------------------------
_BOOL IStdDialogFileOpen :: Execute()
{
#if ! defined(AUTO_LIB) 	
    if (!IntuitionBase) {
        IntuitionBase = (struct IntuitionBase*) OpenLibrary(INTUITIONNAME,
                                                                            INTUITIONVER);
    }
    if (!IntuitionBase) {
         Debug(dprintf("Can't open \"%s\" version %ld or newer.", INTUITIONNAME, INTUITIONVER);)
      return FALSE;
    }            
#endif

    Stringz filter = _filter;
    Stringz title;
    if (_title.Len())
        title = Stringz(_title);
    else
        title = Stringz(Application::App().Name());
    
    struct Library *AslBase = NULL;
    struct FileRequester *fr;

    _BOOL res = FALSE;

    IHandle mainwnd;

    if (IWindow::Link::handles.Count() > 0) {
        ListIterator<IWindow::Link> lst(IWindow::Link::handles);
        if (lst[1].nmo->Visible()) {
            mainwnd = lst[1].handle;
        }
    }

    else
        mainwnd = NULL;

        if (AslBase = OpenLibrary("asl.library", 37L)) 
            {if (fr = (struct FileRequester *)
                    AllocAslRequestTags(ASL_FileRequest,
                    ASL_Hail, (_CHAR *) title,
                    ASL_Dir,  (_CHAR *) _curdir,
                    ASL_File, (_CHAR *) _filename,
                    ASL_Pattern, (_CHAR *) filter,
                    ASL_FuncFlags, FILF_PATGAD,
                    ASL_Window, mainwnd,
                    TAG_DONE))
            {if (AslRequest(fr, NULL))
                {
                    _filename = Stringz(fr->rf_Dir) + PATHSEP + Stringz(fr->rf_File);
                    res = TRUE;
                }
             FreeAslRequest(fr);

             if ((_options & FO_FILEMUSTEXISTS) && (!File::Exists(_filename)))
                {
                FreeAslRequest(fr);
                return Execute();
                }

            }

        }   
    CloseLibrary(AslBase);
    return res;
  
}




//---------------------------------------
_BOOL IStdDialogFileSave :: Execute()
{       
#if ! defined(AUTO_LIB) 	
    if (!IntuitionBase) {
        IntuitionBase = (struct IntuitionBase*) OpenLibrary(INTUITIONNAME,
                                                                            INTUITIONVER);
    }
    if (!IntuitionBase) {
         Debug(dprintf("Can't open \"%s\" version %ld or newer.", INTUITIONNAME, INTUITIONVER);)
        return FALSE;
    }            
#endif

    Stringz filter = _filter;
    Stringz title;
    if (_title.Len())
        title = Stringz(_title);
    else
        title = Stringz(Application::App().Name());
    
    struct Library *AslBase = NULL;
    struct FileRequester *fr;

    _BOOL res = FALSE;

    IHandle mainwnd;

    if (IWindow::Link::handles.Count() > 0) {
        ListIterator<IWindow::Link> lst(IWindow::Link::handles);
        if (lst[1].nmo->Visible()) {
            mainwnd = lst[1].handle;
        }
    }
    else
        mainwnd = NULL;



        if (AslBase = OpenLibrary("asl.library", 37L)) 
            {if (fr = (struct FileRequester *)
                    AllocAslRequestTags(ASL_FileRequest,
                    ASL_Hail, (_CHAR *) title,
                    ASL_Dir,  (_CHAR *) _curdir,
                    ASL_File, (_CHAR *) _filename,
                    ASL_Pattern, (_CHAR *) filter,
                    ASL_FuncFlags,FILF_SAVE | FILF_PATGAD,
                    ASL_Window, mainwnd,
                    TAG_DONE))
            {if (AslRequest(fr, NULL))
                {
                    _filename = Stringz(fr->rf_Dir) + PATHSEP + Stringz(fr->rf_File);
                    res = TRUE;
                }
             FreeAslRequest(fr);
            }

        }
        CloseLibrary(AslBase);
        return res;

}



//---------------------------------------
_BOOL IStdDialogMessage :: Execute()
{
#if ! defined(AUTO_LIB) 
    if (!IntuitionBase) {
        IntuitionBase = (struct IntuitionBase*) OpenLibrary(INTUITIONNAME,
                                                                            INTUITIONVER);
    }
    if (!IntuitionBase) {
         Debug(dprintf("Can't open \"%s\" version %ld or newer.", INTUITIONNAME, INTUITIONVER);)
        return FALSE;
    }            
#endif

    struct EasyStruct myES;
    int result;
    
    
    if (!_title.Len()) 
        {_title = Application::App().Name();
        }

    Stringz title = _title;
    Stringz message = _message;
    myES.es_StructSize= sizeof(myES);
    myES.es_Flags = 0;
    myES.es_Title = (UBYTE *)(_CHAR *) title;
    myES.es_TextFormat = (UBYTE *) "%s";
    myES.es_GadgetFormat = (UBYTE *) "%s";

    if (_options & BTN_OK)
        {result = EasyRequest(NULL, &myES, NULL, (UBYTE *)(_CHAR *) message, "OK");
         if (result == 0) _result = RES_OK; //Is the if usefull ?
        }
    if (_options & BTN_OKCANCEL) 
        {result = EasyRequest(NULL, &myES, NULL, (UBYTE *)(_CHAR *) message, "OK|CANCEL");
         if (result == 0) _result = RES_CANCEL;
         else  _result = RES_OK;
        }
    if (_options & BTN_YESNO)
        {result = EasyRequest(NULL, &myES, NULL, (UBYTE *)(_CHAR *) message, "YES|NO");
         if (result == 0) _result = RES_NO;
         else  _result = RES_YES;
        }
    if (_options & BTN_YESNOCANCEL) 
        {result = EasyRequest(NULL, &myES, NULL, (UBYTE *)(_CHAR *) message, "YES|NO|CANCEL");
        if (result == 0) _result = RES_CANCEL;
        if (result == 1) _result = RES_YES;
        if (result == 2) _result = RES_NO;
        }

    return TRUE;
}

#endif
