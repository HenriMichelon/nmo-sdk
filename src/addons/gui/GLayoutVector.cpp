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

$Id: GLayoutVector.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include "nmo/addons/gui/GLayout.hpp"
#include "GResourceVector.hpp"
#include "GLayoutVector.hpp"
using namespace NMO;
 
//----------------------------------------------
GLayoutVector::GLayoutVector(): GLayout(), texture(NULL)
{
}


//----------------------------------------------
GLayoutVector::~GLayoutVector()
{
	if (texture) { delete texture; }
}


//----------------------------------------------
_BOOL GLayoutVector::Init() 
{ 
	if (!(font = IFont::Create())) { return FALSE; }
	SetOption("color_focus", "");
	SetOption("color_shadow_dark", "");
	SetOption("color_shadow_light", "");
	SetOption("color_foreground_up", "");
	SetOption("color_foreground_down", "");
	SetOption("color_background", "192,192,230");
	SetOption("color_font_textcolor", "");
	SetOption("texture", "vectorbg.png");
	return TRUE;
}



//----------------------------------------------
IRGBColor GLayoutVector::ExtractColor(const Stringz&OPT, _BYTE R, _BYTE G, _BYTE B)
{
	List<Stringz> rgb;
	Stringz opt = Option(OPT);
	if (opt.Len() > 0) {
		opt.Split(rgb, ',');
		if (rgb.Count() == 3) {
			ListIterator<Stringz> lstrgb(rgb);
			return IRGBColor(_BYTE(_DWORD(lstrgb[1])), 
							_BYTE(_DWORD(lstrgb[2])), 
							_BYTE(_DWORD(lstrgb[3])));
		}
	}
	return IRGBColor(R, G, B);
}


//----------------------------------------------
void GLayoutVector::UpdateOptions()
{
	focus = ExtractColor("color_focus", 2, 2, 2);
	shadowDark = ExtractColor("color_shadow_dark", 175-100, 175-100, 207-100);
	shadowBright = ExtractColor("color_shadow_light", 0xf0, 0xf0, 0xf0);
	fgUp = ExtractColor("color_foreground_up", 175, 175, 207);
	fgDown = ExtractColor("color_foreground_down", 193, 216, 193);
	background = ExtractColor("color_background", 192, 192, 230);
	font->SetBgColor(fgUp);
	font->SetTextColor(ExtractColor("color_font_textcolor", 0, 0, 0));

	if (texture != NULL) { delete texture; }
	if (Option("texture").Len() > 0) {
		DatatypePixmap dtype;
		texture = new GTexture(dtype.Load(Option("texture")),
							   background);
	}
}


//----------------------------------------------
void GLayoutVector::DeleteResource(GResource*RES)
{
	ASSERT(RES);
	delete (GResourceVector*)RES;
}


//----------------------------------------------
void GLayoutVector::AddResource(GWidget&W, const Stringz&RES)
{
	GResourceVector *res = new GResourceVector(RES);
	W.SetResource(*(GResource*)res);
	W.SetSize(res->width, res->height);
	switch (W.Type()) {
	case GWidget::BOX:
		if (res->flat) { W.Transparent() = TRUE; }
		break;
	case GWidget::UPDOWN:
		((GUpDown&)W).SetResources(RES, RES);
		break;
	case GWidget::TRACKBAR:
		((GTrackBar&)W).SetResources("12,12,RAISED");
		break;
	case GWidget::SCROLLBAR:
		((GScrollBar&)W).SetResources(RES, RES,
			Stringz(",,LOWERED") + (res->flat ? ",FLAT" : ""),
			Stringz(",,RAISED") + (res->flat ? ",FLAT": ""));
		break;
	case GWidget::PROGRESSBAR:
		((GProgressBar&)W).SetResources(RES);
		break;
	case GWidget::LISTBOX:
		((GListBox&)W).SetResources(Stringz(",,LOWERED") + (res->flat ? ",FLAT" : ""),
									Stringz("18,18,RAISED") + (res->flat ? ",FLAT" : ""),
									"");
		break;
	case GWidget::SCROLLBOX:
		((GScrollBox&)W).SetResources(Stringz(",,LOWERED") + (res->flat ? ",FLAT" : ""),
									Stringz("18,18,RAISED") + (res->flat ? ",FLAT" : ""),
									Stringz("18,18,RAISED") + (res->flat ? ",FLAT" : ""));
		break;
	case GWidget::TEXTEDIT:
		((GTextEdit&)W).SetResources(RES);
		break;
	case GWidget::TABS:
        ((GTabs&)W).SetResources(",,RAISED");
		break;
	case GWidget::GRID:
		((GGrid&)W).SetResources(",,LOWERED");
	default:
		break;
	}
}


//----------------------------------------------
void GLayoutVector::Resize(GWidget&W, IRect&R, GResource&)
{
	switch (W.Type()) {
	case GWidget::BOX:
	case GWidget::BUTTON:
	case GWidget::TABBUTTON:
		W.SetVBorder(2);
		W.SetHBorder(2);
		break;
	case GWidget::LINE:
		{
			GLine &L = (GLine&)W;
			if (L.Style() == GLine::HORIZ) {
				R.height = 2;
			}
			else if (L.Style() == GLine::VERT) {
				R.width = 2;
			}
		}
		break;
	case GWidget::FRAME:
		W.SetHBorder(4);
		W.SetVBorder(W.Font().Height()+2);
		break;
	default:
		break;
	}
}


//----------------------------------------------
void GLayoutVector::Draw(GWidget&W, GResource&RES, const IRect&R, _BOOL BEFORE)
{
	GResourceVector &res = (GResourceVector &)RES;
	if (!W.Visible()) { return; }
	if (!W.Display()) { return; }
	IDisplay &D = *W.Display();
	if (BEFORE) {
		switch (W.Type()) {
			//case GWidget::TEXTEDIT: 
			//case GWidget::UPDOWN:
			//case GWidget::SCROLLBAR:
			//case GWidget::PROGRESSBAR:
			case GWidget::PANEL:
				  DrawPanel((GPanel&)W, D, res, R);
				  break;
			case GWidget::BOX:
			case GWidget::SCROLLBOX:
				  DrawBox(W, D, res, R);
				  break;
			case GWidget::GRIDCELL: 
				  DrawGridCell((GGridCell&)W, D, res, R);
				  break;
			case GWidget::LINE: 
				  DrawLine((GLine&)W, D, res, R);
				  break;
			case GWidget::FRAME: 
				  DrawFrame((GFrame&)W, D, res, R);
				  break;
			case GWidget::ARROW: 
				  DrawArrow((GArrow&)W, D, res, R);
				  break;
			case GWidget::BUTTON: 
				  DrawButton((GButton&)W, D, res, R);
				  break;
			case GWidget::CHECKMARK: 
				  DrawCheckmark((GCheckmark&)W, D, res, R);
				  break;
			case GWidget::TEXT: 
				  DrawText((GText&)W, D, res, R);
				  break;
			case GWidget::SELECTION: 
				  DrawSelection((GSelection&)W, D, res, R);
				  break;
			case GWidget::TRACKBAR:
				  DrawTrackBar((GTrackBar&)W, D, res, R);
				  break;
			case GWidget::ROUNDBUTTON:
				  DrawRoundButton((GRoundButton&)W, D, res, R);
				  break;
			case GWidget::TOGGLEBUTTON: 
				  DrawToggleButton((GToggleButton&)(GCheckWidget&)W, D, res, R);
				  break;
			case GWidget::TABBUTTON: 
				  DrawTabButton((GTabButton&)W, D, res, R);
				  break;
			case GWidget::PICTURE: 
				{
					GPicture &pic = (GPicture&)W;
					if (pic.Pixmap()) { pic.Pixmap()->Draw(D, pic.Left(), pic.Top(), pic.Transparency(), TRUE); }
				}
				break;
			default:
				break;
		}
	}
	else {
		switch (W.Type()) {
			case GWidget::TEXTEDIT:
				  DrawTextEdit((GTextEdit&)W, D, res, R);
				  break;
			case GWidget::PROGRESSBAR: 
				  DrawProgressBar((GProgressBar&)W, D, res, R);
				  break;
			case GWidget::TABS:
				  DrawTabs((GTabs&)W, D, res, R);
				  break;
			default:
				break;
		}
	}
}


//----------------------------------------------
void GLayoutVector::DrawPanel(GPanel&W, IDisplay&D, GResourceVector&, const IRect&)
{
	if (W.DrawBackground()) {
		texture->Draw(D, W.Rect());
	}
}


//----------------------------------------------
void GLayoutVector::DrawBox(GWidget&W, IDisplay&D, GResourceVector&RES, const IRect&)
{
	if ((W.Width()<4) || (W.Height()<4)) { return; }
	_DWORD l = W.Rect().left;
	_DWORD t = W.Rect().top;
	_DWORD w = W.Rect().width;
	_DWORD h = W.Rect().height;
	if (RES.style != GResourceVector::FLAT) {
		w--;
		h--;
		switch (RES.style) {
		case GResourceVector::RAISED:
			D.SetPenColor(shadowBright);
			break;
		case GResourceVector::LOWERED:
			D.SetPenColor(shadowDark);
			break;
		default:
			break;
		}
		D.DrawLine(l, t, l+w, t);
		D.DrawLine(l, t, l, t+h);
		switch (RES.style) {
		case GResourceVector::LOWERED:
			D.SetPenColor(shadowBright);
			break;
		case GResourceVector::RAISED:
			D.SetPenColor(shadowDark);
			break;
		default:
			break;
		}
		D.DrawLine(l+w, t, l+w, t+h);
		D.DrawLine(l+w, t+h, l, t+h);

		l += 1;
		t += 1;	
		w -= 1;
		h -= 1;	
	}
	if (W.DrawBackground())	{
		if (W.Pushed() && W.MoveChildsOnPush()) {
			D.SetPenColor(fgDown);
		}
		else {
			D.SetPenColor(fgUp);
		}
		D.DrawRect(l, t, w, h);
	}
}


//----------------------------------------------
void GLayoutVector::DrawLine(GLine&W, IDisplay&D, GResourceVector&RES, const IRect&)
{
	IRGBColor c1, c2;
	switch (RES.style) {
	case GResourceVector::LOWERED:
		c1 = shadowDark;
		c2 = shadowBright;
		break;
	case GResourceVector::RAISED:
		c1 = shadowBright;
		c2 = shadowDark;
		break;
	default:
		c1 = shadowDark;
		c2 = shadowDark;
		break;
	}
	IRect rect = W.Rect();
	D.SetPenColor(c1);
	if (W.Style() == GLine::HORIZ)
		D.DrawLine(rect.left, rect.top, rect.left+rect.width-1, rect.top);
	else if (W.Style() == GLine::VERT)
		D.DrawLine(rect.left, rect.top, rect.left, rect.top+rect.height-1);
	D.SetPenColor(c2);
	if (W.Style() == GLine::HORIZ)
		D.DrawLine(rect.left, rect.top+1, rect.left+rect.width-1, rect.top+1);
	else if (W.Style() == GLine::VERT)
		D.DrawLine(rect.left+1, rect.top, rect.left+1, rect.top+rect.height-1);
}




//----------------------------------------------
void GLayoutVector::DrawFrame(GFrame&W, IDisplay&D, GResourceVector&RES, const IRect&)
{
	const _DWORD LEFTOFFSET = 8;

	/*if ((W.Width() < (W.HBorder()*2)) ||
		(W.Height() < (W.VBorder()*2))) {
		return;
	}*/

	IRect rect = W.Rect();

	_DWORD fh = W.Font().Height();
	_DWORD fw = W.Font().Width(W.Text());
	_DWORD l1 = rect.left;
	_DWORD t1 = rect.top+fh/2;
	_DWORD l2 = rect.left+rect.width-1;
	_DWORD t2 = rect.top+rect.height-1;
	IRGBColor c1;
	IRGBColor c2;
	switch (RES.style) {
	case GResourceVector::RAISED:
		  c1 = shadowBright;
		  c2 = shadowDark;
		  break;
	case GResourceVector::LOWERED:
		  c1 = shadowDark;
		  c2 = shadowBright;
		  break;
	case GResourceVector::FLAT:
		  c1 = shadowDark;
		  c2 = shadowDark;
		  break;
	}
	if (W.Text().Len() && (W.Width() >= fw) && (W.Height() >= fh)) {
		D.SetPenColor(c1);
		D.DrawLine(l1, t1, l1+LEFTOFFSET-3, t1);
		D.DrawLine(l1+LEFTOFFSET+1+fw, t1,
						  l2, t1);
		D.SetPenColor(c2);
		D.DrawLine(l1+1, t1+1, l1+LEFTOFFSET-3, t1+1);
		D.DrawLine(l1+LEFTOFFSET+1+fw, t1+1,
						  l2-2, t1+1);
		W.Font().Draw(D, W.Text(), l1+LEFTOFFSET, t1-fh/2);
	}
	else {
		D.SetPenColor(c1);
		D.DrawLine(l1, t1, l2, t1);
		D.SetPenColor(c2);
		D.DrawLine(l1+1, t1+1, l2-2, t1+1);
	}

	D.SetPenColor(c1);
	D.DrawLine(l1, t2, l1, t1);
	D.DrawLine(l2-1, t1+1, l2-1, t2-1);
	D.DrawLine(l2-1, t2-1, l1+1, t2-1);
	D.SetPenColor(c2);
	D.DrawLine(l1+1, t2-2, l1+1, t1+1);
	D.DrawLine(l2, t1, l2, t2);
	D.DrawLine(l2, t2, l1, t2);
}



//----------------------------------------------
void GLayoutVector::DrawArrow(GArrow&W, IDisplay&D, GResourceVector&, const IRect&)
{
	if ((W.Width() < 4) || (W.Height() < 4)) return;
	//dprintf("Draw Arrow %x\n", &W);

	_DWORD i, w, h, xpos, ypos, left, top;
	switch (W.Kind()) {
	case GArrow::UP:
	case GArrow::DOWN:
		w = W.Width() - 4;
		if (!(w % 2)) w--;
		h = (w/2+1);
		xpos = (W.Width() - w)/2;
		ypos = (W.Width() - h)/2;
		break;
	case GArrow::LEFT:
	case GArrow::RIGHT:
	default:	
		h = W.Height() - 4;
		if (!(h % 2)) h--;
		w = (h/2+1);
		xpos = (W.Height() - w)/2;
		ypos = (W.Height() - h)/2;
		break;
	}


	left = W.Left() + xpos;
	top = W.Top() + ypos;
	if (W.Enabled())
		D.SetPenColor(shadowDark);
	else
		D.SetPenColor(fgUp);
	switch (W.Kind()) {
	case GArrow::UP:
		for (i=0; i<h; i++)
			D.DrawLine(i+left, h-i+top-1, 
					  (w-i)+left, h-i+top-1);
		break;
	case GArrow::DOWN:
		for (i=0; i<h; i++)
			D.DrawLine(i+left, i+top,
					  (w-i)+left, i+top);
		break;
	case GArrow::LEFT:
		for (i=0; i<w; i++)
			D.DrawLine(w-i+left-1, top+i,
						w-i+left-1, top+h-i);				
		break;
	case GArrow::RIGHT:
		for (i=0; i<w; i++)
			D.DrawLine(i+left+1, top+i,
						i+left+1, top+h-i);				
		break;
	}
}


//----------------------------------------------
void GLayoutVector::DrawToggleButton(GToggleButton&W, IDisplay&D, GResourceVector&RES, const IRect&R)
{
	W.SetDrawBackground(!RES.flat);
	if (W.State() == GCheckWidget::CHECK) {
		RES.style = GResourceVector::LOWERED;
		W.Pushed() = TRUE;
	}
	else {
		RES.style = GResourceVector::RAISED;
		W.Pushed() = FALSE;
	}
	DrawBox(W, D, RES, R);
}


//----------------------------------------------
void GLayoutVector::DrawButton(GButton&W, IDisplay&D, GResourceVector&RES, const IRect&R)
{
	//dprintf("Draw Button %x\n", &W);
	//W.SetDrawBackground(!RES.flat);
	if (W.Pushed()) {
		RES.style = GResourceVector::LOWERED;
	}
	else {
		if (RES.flat) {
			if (W.Pointed()) {
				RES.style = GResourceVector::RAISED;
			}
			else {
				RES.style = GResourceVector::FLAT;
			}
		}
		else {
			RES.style = GResourceVector::RAISED;
		}
	}
	DrawBox(W, D, RES, R);
}


//----------------------------------------------
void GLayoutVector::DrawCheckmark(GCheckmark&W, IDisplay&D, GResourceVector&, const IRect&)
{
	if ((W.Width() < 5) || (W.Height() < 5)) return;

	_LONG l = W.Left()+1;
	_LONG t = W.Top()+1;
	_LONG r = l+W.Width()-3;
	_LONG b = t+W.Height()-3;

	D.SetPenColor(shadowDark);
	//if (kind == CROSS)
	{
		D.DrawLine(l+1,	t,		r,		b-1);
		D.DrawLine(l,	t,		r,		b);
		D.DrawLine(l,	t+1,	r-1,	b);
		D.DrawLine(l,	b-1,	r-1,	t);
		D.DrawLine(l,	b,		r,		t);
		D.DrawLine(l+1,	b,		r,		t+1);
	}
	/*else
	{
		_LONG w = Width()/3;
		_LONG h = Height()/2;
		//display->DrawLine(l+2,	t+h,	l+w,	b-2);
		//display->DrawLine(l+w,	b-2,	r-1,	t);
		display->DrawLine(l+1,	t+h,	l+w,	b-1);
		display->DrawLine(l+w,	b-1,	r-1,	t+1);
		display->DrawLine(l+1,	t+h+1,	l+w,	b);
		display->DrawLine(l+w,	b,		r,		t+1);
	}*/
}


//----------------------------------------------
void GLayoutVector::DrawText(GText&W, IDisplay&D, GResourceVector&RES, const IRect&R)
{
	if ((!W.Width()) && W.Text().Len()) { 
		W.ComputeSize(); 
	}
	DrawPanel(W, D, RES, R);
	W.Font().Draw(D, W.Text(), W.Left(), W.Top());
}


//----------------------------------------------
void GLayoutVector::DrawTextEdit(GTextEdit&W, IDisplay&D, GResourceVector&RES, const IRect&R)
{
	if (W.HaveFocus() && (!W.ReadOnly())) {
		UStringz txt = W.DisplayedText().Left(W.SelStart() - 
											 W.FirstDisplayedChar());
		_DWORD l = W.Left() + 2 + W.Font().Width(txt);
		_DWORD t = W.Top() + 2;
		_DWORD h = W.Font().Height();
		D.SetPenColor(shadowDark);
		D.DrawHLine(l - 2, t, 5);
		D.DrawHLine(l - 2, t + h, 5);
		D.DrawLine(l, t, l, t + h);
	}
}


	
//----------------------------------------------
void GLayoutVector::DrawProgressBar(GProgressBar&W, IDisplay&D, GResourceVector&, const IRect&)
{
	D.SetPenColor(fgDown);
	if (W.Max() > W.Min()) {
		if (W.GProgressBar::Type() == GProgressBar::HORIZ)	{
			D.DrawRect(W.Left() + 1, 
					W.Top() + 1, 
					(W.Value() - W.Min()) * (W.Width() - 1*2) / 
					ABS(W.Max() - W.Min()), 
					W.Height() - 1*2);
			if (W.DisplayType() != GProgressBar::NONE) {
				UStringz text;
				/*if (W.DisplayType() == GProgressBar::PERCENT)
					text.Format("%d%%", (_DWORD)((_FLOAT(W.Value() - W.Min()) / 
								_FLOAT(W.Max()- W.Min()))*100.0));
				else if (W.DisplayType() == GProgressBar::VALUE)
					text.Format("%d", W.Value());*/
				_DWORD x = (W.Width() - 1*2 - 
							W.Font().Width(text))/2;
				_LONG y = (W.Height() - 1*2 -
							W.Font().Height())/2;
				W.Font().Draw(D, text, x + W.Left() + 1, y + W.Top() + 1);
			}
		}
		else
		{	
			_DWORD size = (W.Value()- W.Min()) * (W.Height()-1*2) / 
							ABS(W.Max()-W.Min());
			D.DrawRect(W.Left() + 1, W.Top() + W.Height() -
						size - 1, W.Width() - 1*2,
						size);
		}
	}
}


//--------------------------------------------------------------------------
void GLayoutVector::DrawSelection(GSelection&W, IDisplay&D, 
								  GResourceVector&, const IRect&)
{
	D.SetPenColor(fgDown);
	D.DrawRect(W.Rect());
}



//--------------------------------------------------------------------------
void GLayoutVector::DrawTrackBar(GTrackBar&W, IDisplay&D, 
								  GResourceVector&, const IRect&)
{
	if (W.TrackBarType() == GTrackBar::HORIZ) {
		_LONG left = W.Left() + W.tracker.Width()/2 - 1;
		_LONG right = W.Left() + W.Width() - W.tracker.Width()/2 - 1;
		D.SetPenColor(shadowBright);
		D.DrawHLine(left, W.Top() + W.Height() / 2, right - left - 1);
		D.SetPenColor(shadowDark);
		D.DrawLine(left, W.Top() + 2, left, W.Top() + W.Height() - 2);
		D.DrawLine(right, W.Top() + 2, right, W.Top() + W.Height() - 2);
		for (_LONG idx = W.Min(); idx < W.Max(); idx += W.Step()) {
			_LONG l = left + (idx - W.Min()) * 
					 (W.Width() - W.tracker.Width()) / (W.Max() - W.Min());
			D.DrawLine(l, W.Top() + W.Height()/4, l, 
						W.Top() + W.Height() - W.Height()/4);
		}	
	}
	else {
		_LONG top = W.Top() + W.tracker.Height()/2 - 1;
		_LONG bottom = W.Top() + W.Height() - W.tracker.Height()/2 - 1;
		D.SetPenColor(shadowBright);
		D.DrawLine(W.Left() + W.Width() / 2, top, W.Left() + W.Width() / 2, bottom);
		D.SetPenColor(shadowDark);
		D.DrawLine(W.Left() + 2, top, W.Left() + W.Width() - 2, top);
		D.DrawLine(W.Left() + 2, bottom, W.Left() + W.Width() - 2, bottom);
		for (_LONG idx = W.Min(); idx < W.Max(); idx += W.Step()) {
			_LONG t = top + (idx - W.Min()) * 
					 (W.Height() - W.tracker.Height()) / (W.Max() - W.Min());
			D.DrawLine(W.Left() + W.Width()/4, t, 
						W.Left() + W.Width() - W.Width()/4, t);
		}	
	}
}


//--------------------------------------------------------------------------
void GLayoutVector::DrawTabs(GTabs&W, IDisplay&D,
							GResourceVector&, const IRect&)
{
	D.SetPenColor(shadowBright);
	if (W.Childs().Count() == 0) {
		D.DrawLine(W.Left(), W.Top() + W.Height() - 1,
				   W.Left() + W.Width() - 1,  W.Top() + W.Height() - 1);
		return;
	}
	_DWORD wtab = MAX(_LONG(W.Width() / (W.Childs().Count() ) - W.Padding() * 2),
				            0l);
	_DWORD nsel = W.IndexSelected() - 1;
	_DWORD t = W.Top() + W.Height() - 1;
	D.DrawLine(W.Left(), t,
			   W.Left() + wtab * nsel - 1, t);
	D.SetPenColor(background);
	D.DrawLine(W.Left() + wtab * nsel, t,
			   W.Left() + wtab * (nsel + 1) - 2, t);
	D.SetPenColor(shadowBright);
	D.DrawLine(W.Left() + wtab * (nsel + 1) - 1, t,
			   W.Left() + W.Width() - 1, t);
}


//--------------------------------------------------------------------------
void GLayoutVector::DrawTabButton(GTabButton&W, IDisplay&D,
								  GResourceVector&RES, const IRect&)
{
	if ((W.Width()<4) || (W.Height()<4)) { return; }
	_DWORD l = W.Rect().left;
	_DWORD t = W.Rect().top;
	_DWORD w = W.Rect().width;
	_DWORD h = W.Rect().height;
	w -= 1;
	h -= 2;
	D.SetPenColor(shadowBright);
	D.DrawLine(l, t, l+w, t);
	D.DrawLine(l, t, l, t+h);
	D.SetPenColor(shadowDark);
	D.DrawLine(l+w, t, l+w, t+h);
	l += 1;
	t += 1;	
	w -= 1;
	if (W.DrawBackground() && (!RES.flat)) {
		if (W.Pushed() && W.MoveChildsOnPush()) {
			D.SetPenColor(fgDown);
			D.DrawRect(l, t, w, h);
		}
		else if (W.State() == GCheckWidget::CHECK) {
			//D.SetPenColor(background);
			texture->Draw(D, l, t, w, h+1);
		}
		else {
			D.SetPenColor(fgUp);
			D.DrawRect(l, t, w, h);
		}
	}
}


//--------------------------------------------------------------------------
void GLayoutVector::DrawRoundButton(GRoundButton&W, IDisplay&D,
								  GResourceVector&, const IRect&)
{
	_DWORD beam = MIN(W.Width() / 2, W.Height() / 2);
	_DWORD outer = beam / 3;
	beam -= outer;
	_LONG x = W.Left() + W.Width() / 2;
	_LONG y = W.Top() + W.Height() / 2;

	const _DOUBLE RAD = 1.57079632679;
	//const _DWORD OFFSET = 3;

	_DOUBLE aMin;
	switch (W.RoundButtonType()) {
	case GRoundButton::RB_TOP: aMin = RAD; break;
	case GRoundButton::RB_LEFT: aMin = RAD * 2; break;
	case GRoundButton::RB_BOTTOM: aMin = RAD * 3; break;
	case GRoundButton::RB_RIGHT: 
	default: aMin = 0.0; break;
	}

	_DOUBLE angle = ((RAD * 4 * (W.Value() - W.Min())) / (W.Max() - W.Min())) + aMin;

	D.SetPenColor(shadowBright);
	D.DrawCircle(x - 1,
				y - 1,
				beam,
				FALSE);
	D.SetPenColor(shadowDark);
	D.DrawCircle(x + 1,
				y + 1,
				beam,
				FALSE);
	//D.SetPenColor(fgUp);
	if (W.Pushed()) {
		D.SetPenColor(fgDown);
	}
	else {
		D.SetPenColor(fgUp);
	}
	D.DrawCircle(x, y, beam);

	/*D.SetPenColor(shadowBright);
	D.DrawCircle(x - 1,
				y - 1,
				beam - OFFSET,
				FALSE);
	D.SetPenColor(shadowDark);
	D.DrawCircle(x + 1,
				y + 1, 
				beam - OFFSET,
				FALSE);
	if (W.Pushed()) {
		D.SetPenColor(fgDown);
	}
	else {
		D.SetPenColor(fgUp);
	}
	D.DrawCircle(x, y, beam - OFFSET);*/

	D.SetPenColor(shadowBright);
	D.DrawCircle(x, y, beam + outer / 2 + 2, FALSE, W.Step());

	D.SetPenColor(shadowDark);
	D.DrawCircle(x, y, beam + outer / 2 + 1, FALSE, W.Step());
	D.DrawLine(x + _LONG(beam / 3 * cos(angle)),
				y - _LONG(beam / 3 * sin(angle)),
				x + _LONG((beam )  * cos(angle)),
				y - _LONG((beam )* sin(angle)));
	D.DrawLine(x + _LONG((beam + outer) * cos(aMin)),
				y - _LONG((beam + outer) * sin(aMin)),
				x + _LONG((beam + 2) * cos(aMin)),
				y - _LONG((beam + 2) * sin(aMin)));
}


//--------------------------------------------------------------------------
void GLayoutVector::DrawGridCell(GGridCell&W, IDisplay&D,
								  GResourceVector&RES, const IRect&)
{
	if ((W.Width()<2) || (W.Height()<2)) { return; }
	_DWORD l = W.Rect().left;
	_DWORD t = W.Rect().top;
	_DWORD w = W.Rect().width;
	_DWORD h = W.Rect().height;
	--w;
	--h;
	switch (RES.style) {
	case GResourceVector::LOWERED:
		D.SetPenColor(shadowBright);
		break;
	default:
		D.SetPenColor(shadowDark);
		break;
	}
	D.DrawLine(l+w, t, l+w, t+h);
	D.DrawLine(l+w, t+h, l, t+h);

	if (W.DrawBackground()) {
		if (W.Pushed() || W.Selected()) {
			D.SetPenColor(fgDown);
		}
		else {
			D.SetPenColor(fgUp);
		}
		D.DrawRect(l, t, w, h);
	}
}


