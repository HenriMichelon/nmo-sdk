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

$Id: GTexture.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//---------------------------------------------
GTexture::GTexture(IPixmap*TEXTURE, IRGBColor&COLOR):
	color(COLOR), texture(TEXTURE)
{ }


//---------------------------------------------
GTexture::~GTexture()
{
	if (texture) { delete texture; }
	Clear();
}



//---------------------------------------------
void GTexture::Draw(IDisplay&D, const IRect&R)
{
	Draw(D, R.left, R.top, R.width, R.height);
}

//---------------------------------------------
void GTexture::Draw(IDisplay&D, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	if (texture) {
		IPixmap *T = Get(D);
		if (T != NULL) {
			IPixmap *B = T->GetRect(X, Y, W, H);
			if (B != NULL) {
				B->DrawNoTransClip(D, X, Y);
				delete B;
			}
		}
	}
	else {
		D.SetPenColor(color);
		D.DrawRect(X, Y, W, H);
	}
}

	
//---------------------------------------------
void GTexture::Clear()
{
	ListIterator<DisplayTexture> list(textures);
	while (!list.End()) {
		DisplayTexture &t = list.Next();
		if (t.texture) { delete t.texture; }
	}
	textures.Clear();
}


//---------------------------------------------
IPixmap* GTexture::Create(IDisplay&D)
{
	if (!texture) { return NULL; }
	// Copy & map texture
	IPixmap *t = texture->Copy();
	t->Remap();
	IPixmap::IFrame *fsrc = t->Frame(0);
	// create a new texture, window wide
	IPixmap *b = new IPixmap;
	IPixmap::IFrame *f = b->AddFrame(0);
	f->next = f;
	f->bpp = fsrc->bpp;
	f->width = D.Width();
	f->height = D.Height();
	f->pal = fsrc->pal->Copy();
	f->sleepTimeTotal = fsrc->sleepTimeTotal;
	f->sleepTimeRemain = fsrc->sleepTimeRemain;
	f->pic = new _BYTE[f->width*f->height*f->bpp];
	// map tiled texture into window texture
	_DWORD x, y;
	for (y=0; y<=f->height / fsrc->height; y++) {
		for (x=0; x<=f->width / fsrc->width; x++) {
			b->Draw(t, x*fsrc->width, y*fsrc->height);
		}
	}
	delete t;
	return b;
}


//---------------------------------------------
IPixmap* GTexture::Get(IDisplay&D)
{
	if (!texture) { return NULL; };
	ListIterator<DisplayTexture> list(textures);
	while (!list.End()) {
		DisplayTexture &t = list.Next();
		if (t.display == &D) {
			if ((t.texture->Width() != D.Width()) ||
				(t.texture->Height() != D.Height())) {
				delete t.texture;
				t.texture = Create(D);
			}
			return t.texture;
		}
	}
	DisplayTexture *t = new DisplayTexture;
	t->display = &D;
	t->texture = Create(D);
	textures.Add(t);
	return t->texture;
}

