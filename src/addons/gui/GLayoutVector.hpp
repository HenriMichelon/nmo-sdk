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

$Id: GLayoutVector.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _NMO_GUI_LAYOUTVECTOR_H_
#define _NMO_GUI_LAYOUTVECTOR_H_

namespace NMO {

class GLayoutVector: public GLayout
{ 
public:
	GLayoutVector();
	~GLayoutVector();

	virtual _BOOL Init();
	virtual void Draw(GWidget&, GResource&, const IRect&, _BOOL);
	virtual void AddResource(GWidget&, const Stringz& = "");
	virtual void DeleteResource(GResource*);
	virtual void Resize(GWidget&, IRect&, GResource&);
		
private:
	IRGBColor	background;
	IRGBColor	focus;
	IRGBColor	shadowDark;
	IRGBColor	shadowBright;
	IRGBColor	fgUp;
	IRGBColor	fgDown;
	GTexture	*texture;

	void UpdateOptions();
	IRGBColor ExtractColor(const Stringz&, _BYTE, _BYTE, _BYTE);
	void DrawPanel(GPanel&, IDisplay&, GResourceVector&, const IRect&);
	void DrawBox(GWidget&, IDisplay&, GResourceVector&, const IRect&);
	void DrawLine(GLine&, IDisplay&, GResourceVector&, const IRect&);
	void DrawFrame(GFrame&, IDisplay&, GResourceVector&, const IRect&);
	void DrawArrow(GArrow&, IDisplay&, GResourceVector&, const IRect&);
	void DrawButton(GButton&, IDisplay&, GResourceVector&, const IRect&);
	void DrawCheckmark(GCheckmark&, IDisplay&, GResourceVector&, const IRect&);
	void DrawText(GText&, IDisplay&, GResourceVector&, const IRect&);
	void DrawTextEdit(GTextEdit&, IDisplay&, GResourceVector&, const IRect&);
	void DrawProgressBar(GProgressBar&, IDisplay&, GResourceVector&, const IRect&);
	void DrawSelection(GSelection&, IDisplay&, GResourceVector&, const IRect&);
	void DrawTrackBar(GTrackBar&, IDisplay&, GResourceVector&, const IRect&);
	void DrawToggleButton(GToggleButton&, IDisplay&, GResourceVector&, const IRect&);
	void DrawRoundButton(GRoundButton&, IDisplay&, GResourceVector&, const IRect&);
	void DrawTabButton(GTabButton&, IDisplay&, GResourceVector&, const IRect&);
	void DrawTabs(GTabs&, IDisplay&, GResourceVector&, const IRect&);
	void DrawGridCell(GGridCell&, IDisplay&, GResourceVector&, const IRect&);
	
};

}

#endif 
