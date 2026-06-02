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
#ifndef _GUI_TEXTURE_H_
#define _GUI_TEXTURE_H_

namespace NMO {

/*!
\brief		Manage texture for background mapping

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GTexture.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GTexture: public GObject
{
public:
	GTexture(IPixmap*, IRGBColor&);
	virtual ~GTexture();
	void Clear();

	IPixmap* Get(IDisplay&);
	IPixmap* Create(IDisplay&);
	void Draw(IDisplay&, _LONG, _LONG, _DWORD, _DWORD);
	void Draw(IDisplay&, const IRect&);

private:
	class DisplayTexture: public GObject
	{
	public:
		IDisplay	*display;
		IPixmap		*texture;
	};

	IRGBColor 	color;
	IPixmap 	*texture;
	List<DisplayTexture> textures;
};

}

#endif
