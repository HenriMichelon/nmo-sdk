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

$Id: GResourcePixmap.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _NMO_GUI_RESOURCEPIXMAP_H_
#define _NMO_GUI_RESOURCEPIXMAP_H_

namespace NMO {

class GResourcePixmap: public GResource
{ 
public:
	typedef struct {
		Stringz	name;
		Stringz basedir;
		IPixmap	*pixmap;
	} pixmapres;

	class AllPixmapsResources: private List<pixmapres>
	{
	public:
		DatatypePixmap dtype;
		virtual ~AllPixmapsResources();
		void Clear();
		void Load(const Stringz&, const Stringz&);
		IPixmap* GetPixmap(const Stringz&);
	private:
		_DWORD Get(const Stringz&);
	};

	_BOOL		setsize;
	_BOOL		tiled;
	_BOOL		multi;
	_BOOL		many;
	_DWORD		width;
	_DWORD		height;
	Stringz		name;
	Stringz		basedir;
	IPixmap		*pixmap;
	GTexture	*texture;
	List<IPixmap> pixmaps;
	
	GResourcePixmap(AllPixmapsResources *, const Stringz&, 
					IRGBColor&, const Stringz&);
	virtual ~GResourcePixmap();

private:
	AllPixmapsResources *allpixmaps;
};

}

#endif 
