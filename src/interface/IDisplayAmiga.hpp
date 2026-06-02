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

    Super class for all AmigaOS display types
                       
    (c) 1998-2001 Henri Michelon

$Id: IDisplayAmiga.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_AMIGA_DISP_H_
#define _NMO_AMIGA_DISP_H_


namespace NMO {

class IDisplayAmiga : public IDisplayChunky
{
public:
   static IDisplayAmiga* Create();

   virtual ~IDisplayAmiga();
        
   void Refresh();
   void Refresh(const IRect&);
        
   virtual _BOOL CreateBuffers();
   virtual void DestroyBuffers();

   void SetWindow(Window*);
   virtual _BOOL OpenWBScreen();

   static Screen *sScreen;

protected:
    RastPort	*mRastPort;
    BitMap		*mDrawBitmap;
    Window		*mWindow;
   _DWORD		mBorderTop;
   _DWORD		mBorderLeft;
        
   IDisplayAmiga(DisplayType);
        
private:
   static _DWORD sInst;

};


}

#endif
