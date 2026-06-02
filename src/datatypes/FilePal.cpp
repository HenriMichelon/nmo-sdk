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

$Id: FilePal.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/datatypes/FileIFF.hpp>
#include <nmo/datatypes/FilePal.hpp>
using namespace NMO;


typedef struct {
	FileIFF::ID chunkID;
	_DWORD	chunkSize;
	_BYTE	r1;
	_BYTE	r2;
	_BYTE	r3;
	_BYTE	r4;
} DataChunk;


typedef struct {
	FileIFF::ID chunkID;
	_DWORD	chunkSize;
	FileIFF::ID RIFFID;
} MainChunk;

typedef struct {
	_BYTE	r;
	_BYTE	g;
	_BYTE	b;
	_BYTE	a;
} RGBData;


//////////////////////////////////////////////////////////////////////
IPalette* FilePal::Load(Stream&STREAM)
{
	FileIFF iff(STREAM);
	_DWORD size = iff.FindChunk('P', 'A', 'L', ' ', 'R', 'I', 'F', 'F');
	if (size) {
		_PTR data = iff.FindSubChunk('d', 'a', 't', 'a', FALSE);
		DataChunk *dat = (DataChunk*)data;
		if (dat) {
			RGBData rgb;
			_DWORD nbcolors = (dat->chunkSize - 8) / 4;
			IPalette *pal = new IPalette(nbcolors);
			for (_DWORD idx = 0; idx < nbcolors; idx ++) {
				STREAM.Read(&rgb, 4);
				pal->Set(idx, rgb.r, rgb.g, rgb.b);
			}
			FileIFF::FreeSubChunk(data);
			return pal;
		}
		FileIFF::FreeSubChunk(data);
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
IPalette* FilePal::Load(const Stringz&FILENAME)
{
	FileStream *file = new FileStream(FILENAME);
	IPalette *res = Load(*file);
	delete file;
	return res;
}
