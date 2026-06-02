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

$Id: GLayoutPixmap.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include "nmo/addons/gui/GLayout.hpp"
#include "GResourcePixmap.hpp"
#include "GLayoutPixmap.hpp"
using namespace NMO;


//----------------------------------------------
_BOOL GLayoutPixmap::Init() 
{ 
	background = IRGBColor(192, 192, 230);
	if (!(font = IFont::Create())) { return FALSE; }
	font->SetBgColor(background);
	return TRUE;
}


//----------------------------------------------
GLayoutPixmap::~GLayoutPixmap()
{
	pixmaps.Clear();
}


//----------------------------------------------
void GLayoutPixmap::SetResources(ResourcesTable&RESTABLE)
{
	GLayout::SetResources(RESTABLE);
	pixmaps.dtype.SetResources(RESTABLE);
}


//----------------------------------------------
void GLayoutPixmap::DeleteResource(GResource*RES)
{
	ASSERT(RES);
	delete (GResourcePixmap*)RES;
}


//----------------------------------------------
void GLayoutPixmap::AddResource(GWidget&W, const Stringz&RES)
{
	List<Stringz> ress;
	RES.Split(ress, ';');

	Stringz basedir = Option("resources_dir");
	GResourcePixmap *res;
	ListIterator<Stringz> lstRess(ress);

	switch (W.Type()) {
	case GWidget::PROGRESSBAR:
		if (ress.Count() == 2) {
			GProgressBar &bar = (GProgressBar &)W;
			bar.SetResources(lstRess[1]);
			res = new GResourcePixmap(&pixmaps, lstRess[2], background, basedir);
		}
		break;
	case GWidget::TRACKBAR:
		if (ress.Count() == 2) {
			GTrackBar &bar = (GTrackBar &)W;
			bar.SetResources(lstRess[2]);
			res = new GResourcePixmap(&pixmaps, lstRess[1], background, basedir);
		}
		break;
	case GWidget::SCROLLBAR:
		{
			GScrollBar &scroll = ((GScrollBar&)W);
			if (ress.Count() == 4) {
				scroll.SetResources(lstRess[1], lstRess[2], lstRess[3], lstRess[4]);
			}
			res = new GResourcePixmap(&pixmaps, "", background, basedir);
			res->width = scroll.btnUp.Width() + scroll.liftArea.Width() +
						scroll.btnDown.Width();
			res->height = MAX(scroll.btnUp.Height(),
							MAX(scroll.liftArea.Height(),
								MAX(scroll.btnDown.Height(), 
									scroll.liftCage.Height())));
			res->setsize = TRUE;
		}
		break;
	default:
		if (ress.Count() > 0) {
			res = new GResourcePixmap(&pixmaps, lstRess[1], background, basedir);
		}
		else {
			res = new GResourcePixmap(&pixmaps, "", background, basedir);
		}
		break;
	}
	if (res) {
		//W.Transparent() |= (res->pixmap && (res->pixmap->Frame(0)->trans));
		W.SetResource(*(GResource*)res);
		W.SetSize(res->width, res->height);
	}
	else {
		//W.Transparent() = TRUE;
	}
}


//----------------------------------------------
void GLayoutPixmap::Resize(GWidget&W, IRect&R, GResource&RES)
{
	GResourcePixmap &res = (GResourcePixmap &)RES;
	if (res.setsize) {
		R.width = res.width;
		R.height = res.height;
	}
	switch (W.Type()) {
	case GWidget::BOX:
	case GWidget::BUTTON:
		W.SetVBorder(2);
		W.SetHBorder(2);
		break;
	/*case GWidget::LINE: 
		{
			GLine &L = (GLine&)W;
			if (L.Style() == GLine::HORIZ) {
				R.height = 2;
			}
			else if (L.Style() == GLine::VERT) {
				R.width = 2;
			}
		}
		break;*/
	case GWidget::FRAME: 
		W.SetHBorder(4);
		W.SetVBorder(W.Font().Height()+2);
		break;
	default:
		break;	
	}
}


//----------------------------------------------
void GLayoutPixmap::Draw(GWidget&W, GResource&RES, const IRect&R, _BOOL BEFORE)
{
	GResourcePixmap &res = (GResourcePixmap &)RES;
	ListIterator<IPixmap> lstPixmaps(res.pixmaps);
	if ((!W.Visible()) || (!W.Display())) { return; }
	IDisplay &D = *W.Display();
	if (BEFORE) {
		switch (W.Type()) {
		case GWidget::TEXT:
			DrawText((GText&)W, D, res, R); break;
		case GWidget::TOGGLEBUTTON:
			DrawToggleButton((GToggleButton&)W, D, res, R); break;
		case GWidget::ROUNDBUTTON:
			if (res.many) {
				GValueSelect &gv = (GValueSelect&)W;
				if (res.pixmaps.Count() > 0) {
					IPixmap *pxm = &lstPixmaps[(gv.Value() - gv.Min()) / (((gv.Max() - gv.Min()) / res.pixmaps.Count()) + 1) + 1 ];
					if (pxm) { pxm->Draw(D, W.Left(), W.Top(), TRUE, pxm->Frame(0)->trans); }
				}
				break;
			}
		default:
			if (res.multi) {
				if (W.Pushed() && (res.pixmaps.Count() > 1)) {
					lstPixmaps[2].Draw(D, W.Left(), W.Top(), lstPixmaps[2].Frame(0)->trans, TRUE);
				}
				else if ((!W.Enabled()) && (res.pixmaps.Count() > 2)) {
					lstPixmaps[3].Draw(D, W.Left(), W.Top(), lstPixmaps[3].Frame(0)->trans, TRUE);
				}
				else if (res.pixmaps.Count() > 0){
					lstPixmaps[1].Draw(D, W.Left(), W.Top(), lstPixmaps[1].Frame(0)->trans, TRUE);
				}
			}
			else if (res.tiled) {
				res.texture->Draw(D, W.Left(), W.Top(), W.Width(), W.Height());
			}
			else if (res.pixmap) {
				res.pixmap->Draw(D, W.Left(), W.Top(), res.pixmap->Frame(0)->trans, TRUE);
			}
			break;
		}
	}
	else {
		switch (W.Type()) {
		case GWidget::PROGRESSBAR:
			DrawProgressBar((GProgressBar&)W, D, res, R); break;
		default:
			break;
		}
	}
}


//----------------------------------------------
void GLayoutPixmap::DrawText(GText&W, IDisplay&D, GResourcePixmap&, const IRect&)
{
	if ((!W.Width()) && W.Text().Len()) { 
		W.ComputeSize(); 
	}
	W.Font().Draw(D, W.Text(), W.Left(), W.Top());
}

//----------------------------------------------
void GLayoutPixmap::DrawProgressBar(GProgressBar&W, IDisplay&D, GResourcePixmap&RES, const IRect&)
{
	if (W.Max() > W.Min()) {
		if (W.GProgressBar::Type() == GProgressBar::HORIZ)	{
			IPixmap *pxm = RES.pixmap->GetRect(
				0, 0, 
				(W.Value() - W.Min()) * (W.Width() - 1*2) / (W.Max() - W.Min()),
				RES.pixmap->Height());
			pxm->Draw(D, W.Left(), W.Top(), pxm->Frame(0)->trans, TRUE);
			delete pxm;
		}
		else
		{	
			//_DWORD size = (W.Value()- W.Min()) * (W.Height()-1*2) / (W.Max()-W.Min());
		}
	}
}



//----------------------------------------------
void GLayoutPixmap::DrawToggleButton(GToggleButton&W, IDisplay&D, GResourcePixmap&RES, const IRect&)
{
	if (!RES.multi) { return; }
	ListIterator<IPixmap> lstPixmaps(RES.pixmaps);
	if (((W.State() == GCheckWidget::CHECK) || (W.Pushed()) &&
		(RES.pixmaps.Count() > 1))) {
		lstPixmaps[2].Draw(D, W.Left(), W.Top(), lstPixmaps[2].Frame(0)->trans, TRUE);
	}
	else if (RES.pixmaps.Count() > 0) {
		lstPixmaps[1].Draw(D, W.Left(), W.Top(), lstPixmaps[1].Frame(0)->trans, TRUE);
	}
}
