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

	(c) 1999-2001 Henri Michelon 

$Id: DatatypePixmap.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IDisplay.hpp>
#include <nmo/interface/IPixmap.hpp>
#include <nmo/datatypes/FilePixmap.hpp>
#include <nmo/datatypes/FilePng.hpp>
#ifdef HAVE_LIBUNGIF
	#include <nmo/datatypes/FileGif.hpp>
	#include "modules/Modungif.hpp"
#endif
#include <nmo/datatypes/DatatypePixmap.hpp>
using namespace NMO;


class DatatypePixmapAbstract 
{
private:
	friend class DatatypePixmap;
	class PixmapModule {
	public:
		FilePixmap	*datatype;
		DynModule 	*module;
		Stringz		fileext;
	};
	ResourcesTable *restable;

	static _DWORD nInst;
	static NMO::List<PixmapModule>	modules;

	PixmapModule* Module(const Stringz&);
	void AddModule(DynModule*, FilePixmap*, const Stringz&);	
};

#define _m(x) ((DatatypePixmapAbstract*)(this->mDatatypePixmapAbstract))->x

_DWORD DatatypePixmapAbstract::nInst = 0;
NMO::List<DatatypePixmapAbstract::PixmapModule> DatatypePixmapAbstract::modules;


//------------------------------------------------
DatatypePixmap::DatatypePixmap(ResourcesTable *RESTABLE)
{
	mDatatypePixmapAbstract = new DatatypePixmapAbstract;
	_m(restable) = RESTABLE;
	if (!(_m(nInst)++)) {
/*#ifdef HAVE_LIBUNGIF
		DynModule *mod = Modungif::Create();
		if (mod) { _m(AddModule)(mod, new FileGif, "gif"); }
#endif*/
		_m(AddModule)(NULL, new FilePng, "png");
	}
}


//------------------------------------------------
void DatatypePixmap::SetResources(ResourcesTable &RESTABLE)
{
	_m(restable) = &RESTABLE;
}


//------------------------------------------------
ResourcesTable* DatatypePixmap::Resources() const
{
	return _m(restable);
}


//------------------------------------------------
DatatypePixmap::~DatatypePixmap()
{
	if (!(--_m(nInst))) {
		ListIterator<DatatypePixmapAbstract::PixmapModule> list(_m(modules));
		while (!list.End()) {
			DatatypePixmapAbstract::PixmapModule &module = list.Next();
			delete module.module;
			delete module.datatype;
		}
		_m(modules).Clear();
	}
	delete (DatatypePixmapAbstract*)mDatatypePixmapAbstract;
}


//------------------------------------------------
void DatatypePixmapAbstract::AddModule(DynModule *MODULE, FilePixmap*FILEPXM, 
							   const Stringz&EXT)
{
	PixmapModule *pm = new PixmapModule;
	pm->fileext = EXT;
	pm->module = MODULE;
	pm->datatype = FILEPXM;
	modules.Add(pm);
}


//------------------------------------------------
DatatypePixmapAbstract::PixmapModule* DatatypePixmapAbstract::Module(const Stringz&NAME)
{
	Stringz ext(File::FileExt((Stringz&)NAME));
	if (ext.Len() == 0) {
		ext = NAME;
	}
	ext.Trim().LCase();	
	ListIterator<DatatypePixmapAbstract::PixmapModule> list(modules);
	while (!list.End()) {
		DatatypePixmapAbstract::PixmapModule &module = list.Next();
		if (module.fileext == ext) {
			return &module;
		}
	}
	return NULL;
}


//------------------------------------------------
IPixmap *DatatypePixmap::Load(const Stringz &NAME)
{
	DatatypePixmapAbstract::PixmapModule *mod = _m(Module)(NAME);
	if (mod != NULL) {
		try {
			if (_m(restable) && (NAME.Left(4) == "res:")) {
				Stringz name = NAME.Right(NAME.Len() - 4);
				_LONG vpos = name.InStr(':');
				if (vpos > -1) {
					Stringz id = name.Left(vpos);
					id.Trim();
					name = name.Right(name.Len() - vpos - 1);
					name.Trim();
					Stream *stream = _m(restable)->Get(id, name);
					if (stream) {
						stream->Seek(sizeof(_DWORD), Stream::CURRENT);
						return mod->datatype->Load(*stream, mod->module);
					}
				}
			}
			else {
				FileStream stream(NAME);
				return mod->datatype->Load(stream, mod->module);
			}
		}
		catch (Stream::ExcStream) {
			return NULL;
		}
	}
	return NULL;
}


//------------------------------------------------
IPixmap *DatatypePixmap::Load(const Stringz&NAME, Stream&STREAM)
{
	DatatypePixmapAbstract::PixmapModule *mod = _m(Module)(NAME);
	if (mod != NULL) {
		try {
			return mod->datatype->Load(STREAM, mod->module);
		}
		catch (Stream::ExcStream) {
			return NULL;
		}
	}
	return NULL;
}
