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

	(c) 1998-2001 Henri Michelon 

$Id: GPicture.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include "nmo/NMO.hpp"
#include "nmo/addons/GUI.hpp"
using namespace NMO;

//----------------------------------------------
GPicture::GPicture(IPixmap *I): 
	GWidget(PICTURE), pixmap(I), trans(TRUE), 
		animSpeed(10), autoSize(TRUE)
{
	transparent = TRUE;
	if (pixmap) {
		pixmap->Remap();
	}
}


//----------------------------------------------
void GPicture::SetPixmap(IPixmap*B)
{
	if (pixmap == B) return;
	pixmap = B;
	if (pixmap) {
		pixmap->Remap();
		if (autoSize) {
			SetSize(pixmap->Width(), pixmap->Height());
		}
		else {
			Refresh();
		}
		Call(GEvent::OnPictureChange);
	}
}


//----------------------------------------------
void GPicture::SetAutoSize(_BOOL A)
{
	if (autoSize == A) return;
	autoSize = A;
	if (autoSize && pixmap) {
		SetSize(pixmap->Width(), pixmap->Height());
	}
}


//----------------------------------------------
GPicture::~GPicture()
{
	if (pixmap) delete pixmap;
}


//----------------------------------------------
void GPicture::EventCreate()
{
	if (autoSize && pixmap) {
		SetSize(pixmap->Width(), pixmap->Height());
	}
	GWidget::EventCreate();
}


//----------------------------------------------
IPixmap* GPicture::Pixmap() const
{ 
	return pixmap; //->Clone();
}


//----------------------------------------------
void GPicture::SetTransparency(_BOOL T)
{
	trans = T;
}
