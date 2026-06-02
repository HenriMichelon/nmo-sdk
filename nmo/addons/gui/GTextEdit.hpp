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

*/
#ifndef _NMO_GUI_TEXTEDIT_H_
#define _NMO_GUI_TEXTEDIT_H_

namespace NMO {

/*!
\brief		A widget to edit a line of text

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GTextEdit.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GTextEdit: public GWidget
{
public:
	GTextEdit(UStringz = "");
	virtual ~GTextEdit();

	_BOOL ReadOnly() const;
	void SetReadOnly(_BOOL);
	
	const UStringz& Text() const;
	void SetText(const UStringz&);

	_DWORD SelStart() const;
	void SetSelStart(_DWORD);

	_DWORD FirstDisplayedChar() const;

	// return TRUE if this or parent have keyboard focus
	_BOOL HaveFocus();

	UStringz DisplayedText() const;

	void SetResources(const Stringz&);


protected:
	UStringz text;
	GBox 	box;
	GText	gtext;
	_BOOL	readonly;
	_DWORD	selStart;
	_DWORD	selLen;
	_DWORD	startPos;
	_DWORD	nDispChar;

	virtual IKeyb::Key EventKeybDown(IKeyb::Key);

	// Compute the number of displayed characters
	void ComputeNDispChar();

	friend class GDropEditList;
};

inline const UStringz& GTextEdit::Text() const { return (UStringz&)text; }
inline _DWORD GTextEdit::SelStart() const { return selStart; }
inline _DWORD GTextEdit::FirstDisplayedChar() const { return startPos; };

}

#endif
