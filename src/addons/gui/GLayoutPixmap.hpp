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
			
$Id: GLayoutPixmap.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _NMO_GUI_LAYOUTPIXMAP_H_
#define _NMO_GUI_LAYOUTPIXMAP_H_

namespace NMO {

class GLayoutPixmap: public GLayout
{ 
public:
	virtual ~GLayoutPixmap();
	virtual _BOOL Init();
	virtual void Draw(GWidget&, GResource&, const IRect&, _BOOL);
	virtual void AddResource(GWidget&, const Stringz& = "");
	virtual void DeleteResource(GResource*);
	virtual void Resize(GWidget&, IRect&, GResource&);
	virtual void SetResources(ResourcesTable&);

private:
	IRGBColor		background;
	GResourcePixmap::AllPixmapsResources pixmaps;

	void UpdateOptions() {};
	void DrawText(GText&, IDisplay&, GResourcePixmap&, const IRect&);
	void DrawProgressBar(GProgressBar&, IDisplay&, GResourcePixmap&, const IRect&);
	void DrawToggleButton(GToggleButton&, IDisplay&, GResourcePixmap&, const IRect&);
};

}

#endif 
