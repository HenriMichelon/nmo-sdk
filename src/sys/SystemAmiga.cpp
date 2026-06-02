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

    (c) 1999-2001 Henri Michelon

$Id: SystemAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
---------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <utility/tagitem.h>
#include <clib/dos_protos.h>
#include <nmo/NMO.hpp>
using namespace NMO;


//------------------------------------------------------
_BOOL NMO::System::Execute(const Stringz &PATH, Args&ARGS, ExecMode MODE, _PID *PID)
{
    switch (MODE) {
    case EXEC_NORMAL:
    case EXEC_BACKGROUND:
        {
            Stringz path = PATH;
            ListIterator<Stringz> list(ARGS);
            while (!list.End()) {
                path += ' ' + list.Next();
            }
            TagItem taglist[] = { { TAG_END, 0 } };
            ::SystemTagList((_CHAR*)path, taglist);
        }
        break;
    case EXEC_OVERLAY:
        break;
    }
    return TRUE;
}


//------------------------------------------------------
_BOOL NMO::System :: Shutdown (System::ShutdownMode)
{
    return FALSE;
}


//------------------------------------------------------
_LONG NMO::System::Rand()
{
    return FastRand(_LONG_MAX);
}


//------------------------------------------------------
void NMO::System::InitRand()
{
}


#endif
