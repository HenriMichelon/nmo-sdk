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
				
$Id: GTextEdit.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//----------------------------------------------
GTextEdit::GTextEdit(UStringz TXT): GWidget(TEXTEDIT), 
	selStart(0), selLen(0), startPos(0), nDispChar(0)
{
	transparent = TRUE;
	allowFocus = TRUE;
	text = TXT;
}


//----------------------------------------------
GTextEdit::~GTextEdit()
{ }


//----------------------------------------------
void GTextEdit::SetResources(const Stringz&BRES)
{
	Add(box, CLIENT, BRES);
	box.Add(gtext, GWidget::HCENTER);
	selStart = 0;
	startPos = 0;
	ComputeNDispChar();
}



//----------------------------------------------
void GTextEdit::SetSelStart(_DWORD S)
{
	selStart = S;
	if (startPos > selStart) startPos = selStart;
}


//----------------------------------------------
void GTextEdit::SetText(const UStringz&TEXT)
{
	if (text == TEXT) return;
	if (!text.Len()) {
		selStart = 0;
		startPos = 0;
	}
	text = TEXT;
	ComputeNDispChar();
	if (parent) { parent->Refresh(rect); }
	gtext.SetText(text.Copy(startPos, nDispChar+1));
	box.Refresh();
	Refresh();
	GEventText e(text);
	Call(GEvent::OnTextChange, &e);
}


//----------------------------------------------
IKeyb::Key GTextEdit::EventKeybDown(IKeyb::Key K)
{
	K = GWidget::EventKeybDown(K);
	if (ReadOnly()) { return K; }

	Freeze() = TRUE;
	if (K == keyb->KEY_LEFTARROW) {
		if (selStart > 0) { selStart--; }
	}
	else if (K == keyb->KEY_RIGHTARROW) {
		if (selStart < text.Len()) { selStart++; }
	}
	else if (K == keyb->KEY_END) {
		selStart = text.Len();
	}
	else if (K == keyb->KEY_HOME) {
		selStart = 0;
	}
	else if (K == keyb->KEY_BACKSPACE) {
		if (selStart > 0) {
			selStart--;
			SetText(text.Copy(0, selStart) +  text.Copy(selStart+1, 
					text.Len() - selStart - 1));
		}
	}
	else if (K== keyb->KEY_DELETE) {
		if (selStart < text.Len()) {
			SetText(text.Copy(0, selStart) +  text.Copy(selStart+1, 
					text.Len() - selStart - 1));
		}
	}
	else if ((K != keyb->KEY_SHIFTRIGHT) &&
			(K != keyb->KEY_SHIFTLEFT) &&
			(K != keyb->KEY_CTRLRIGHT) &&
			(K != keyb->KEY_CTRLLEFT) &&
			(K != keyb->KEY_ALTRIGHT) &&
			(K != keyb->KEY_ALTLEFT))
	{
		UChar c = keyb->CodeToChar(K);
		if (c >= _WORD(0x0020)) {
			SetText(text.Left(selStart) + c + 
					text.Right(text.Len() - selStart));
			selStart++;
		}
		else {
			Freeze() = FALSE;
			return K;
		}
	}
	else {
		Freeze() = FALSE;
		return K;
	}
	ComputeNDispChar();
	if (selStart < startPos) {
		startPos = selStart;
	}
	else if ( ((selStart + selLen) > (startPos + nDispChar)) &&
			  (nDispChar != text.Len()) ) {
		startPos = selStart - nDispChar;
	}
	ComputeNDispChar();
	Freeze() = FALSE;
	gtext.SetText(text.Copy(startPos, nDispChar+1));
	box.Refresh();
	Refresh();
	return K;
}

//----------------------------------------------
void GTextEdit::ComputeNDispChar()
{
	_DWORD i;
	_LONG s = box.Width() - box.HBorder()*2;
	for (i=startPos; (i<text.Len()) && (s > 0); i++) {
		if (s < _LONG(font->Width(text[i]))) { break; }
		s -= font->Width(text[i]);
	}
	nDispChar = i-startPos;
}


//----------------------------------------------
_BOOL GTextEdit::HaveFocus()
{
	GWidget *w = this;
	do	{
		if (w->Focused()) return TRUE;
		w = w->Parent();
	} while (w);
	return FALSE;
}


//----------------------------------------------
void GTextEdit :: SetReadOnly (_BOOL READONLY)
{
	readonly = READONLY;
}


//----------------------------------------------
_BOOL GTextEdit :: ReadOnly () const
{
	return readonly;
}


//----------------------------------------------
UStringz GTextEdit::DisplayedText() const
{
	return gtext.Text();
}
