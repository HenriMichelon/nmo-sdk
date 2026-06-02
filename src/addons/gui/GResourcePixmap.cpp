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

$Id: GResourcePixmap.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include "GResourcePixmap.hpp"
using namespace NMO;
 

//----------------------------------------------
GResourcePixmap::GResourcePixmap(AllPixmapsResources *R,const Stringz&RES, 
								IRGBColor&BG, const Stringz&BASEDIR):
	GResource(RES), setsize(FALSE), tiled(FALSE), multi(FALSE), many(FALSE), width(0), height(0), 
					pixmap(NULL), texture(NULL), allpixmaps(R)
{
	basedir = BASEDIR;
	if ((basedir.Len()> 0) && (basedir[basedir.Len()-1] != PATHSEP)){
		basedir += PATHSEP;
	}
	List<Stringz> parts;
	RES.Split(parts, ',');
	ListIterator<Stringz> lstParts(parts);
	if (parts.Count() > 0) { name = lstParts[1]; }

	if (parts.Count() > 1) { 
		if (lstParts[2] == "NOPIXMAP") {
			if (parts.Count() > 2) { width = _LONG(lstParts[3]); }
			if (parts.Count() > 3) { height = _LONG(lstParts[4]); }
		}
		else {
			tiled = (lstParts[2] == "TILED");
			multi = (lstParts[2] == "MULTI");
			many = (lstParts[2] == "MANY");
		}
	}

	if (name.Len() && (!many)) { allpixmaps->Load(name, basedir); }

	if (tiled) {
		if (parts.Count() > 2) { width = _LONG(lstParts[3]); }
		if (parts.Count() > 3) { height = _LONG(lstParts[4]); }
		texture = new GTexture(allpixmaps->GetPixmap(name), BG);
	}
	else if (multi) {
		IPixmap *pxm = allpixmaps->GetPixmap(name);
		if (pxm) {
			width = pxm->Width();
			height = pxm->Height();
			setsize = TRUE;
		}
		pixmaps.Add(pxm);
		for (_DWORD idx=3; idx <= parts.Count(); idx++) {			
			allpixmaps->Load(lstParts[idx], basedir);
			pixmaps.Add(allpixmaps->GetPixmap(lstParts[idx]));
		}
	}
	else if (many) {
		Stringz fname;
		_DWORD start = _DWORD(lstParts[3]);
		_DWORD count = _DWORD(lstParts[4]);
		for (_DWORD idx = start; idx < (start + count); idx++) {
			fname.Format(name, idx);
			allpixmaps->Load(fname, basedir);			
			pixmaps.Add(allpixmaps->GetPixmap(fname));
		}
		fname.Format(name, start);
		IPixmap *pxm = allpixmaps->GetPixmap(fname);
		if (pxm) {
			width = pxm->Width();
			height = pxm->Height();
			setsize = TRUE;
		}
		delete pxm;
	}
	else if (name.Len()) {
		pixmap = allpixmaps->GetPixmap(name);
		if (pixmap) {
			width = pixmap->Width();
			height = pixmap->Height();
			setsize = TRUE;
		}
	}

}


//----------------------------------------------
GResourcePixmap::~GResourcePixmap()
{
	if (texture) { delete texture; }
	if (pixmap) { delete pixmap; }
}


//----------------------------------------------
void GResourcePixmap::AllPixmapsResources::Clear()
{
	ListIterator<pixmapres> list(*this);
	while (!list.End()) {
		delete list.Next().pixmap; 
	}
}


//----------------------------------------------
GResourcePixmap::AllPixmapsResources::~AllPixmapsResources()
{
}


//----------------------------------------------
void GResourcePixmap::AllPixmapsResources::Load(const Stringz&NAME,
												const Stringz&BASEDIR)
{
	if (!Get(NAME)) { 
		Stringz name = NAME;
		if (name.Left(4) != "res:") { 
			name = BASEDIR + name;
		}
		IPixmap *pixmap = dtype.Load(name);
		if (pixmap) {
			pixmap->Remap();
			pixmapres *res = new pixmapres;
			res->name = NAME;
			res->basedir = BASEDIR;
			res->pixmap = pixmap;
			Add(res);
		}
		Debug(
			else {
				dprintf("GResourcePixmap: error loading " + BASEDIR + NAME + "\n");
			}
		)
	}
}


//----------------------------------------------
IPixmap* GResourcePixmap::AllPixmapsResources::GetPixmap(const Stringz&NAME)
{
	_DWORD index = Get(NAME);
	if (index) {
		ListIterator<pixmapres> list(*this);
		return list[index].pixmap->Clone(); 
	}
	return NULL;
}


//----------------------------------------------
_DWORD GResourcePixmap::AllPixmapsResources::Get(const Stringz&NAME)
{
	ListIterator<pixmapres> list(*this);
	while (!list.End()) {
		pixmapres &res = list.Next();
		if (res.name == NAME) { return list.Index() - 1; }
	}
	return 0;
}
