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

  TODO: manage word aligned boundary in subchunks

	(c) 2000-2001 Henri Michelon 

$Id: FileIFF.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/datatypes/FileIFF.hpp>
using namespace NMO;


//////////////////////////////////////////////////////////////////////
FileIFF::FileIFF(Stream& STREAM)
{
	Open(STREAM);
}


//////////////////////////////////////////////////////////////////////
FileIFF::FileIFF(): chunkpos(0), stream(NULL) { }


//////////////////////////////////////////////////////////////////////
void FileIFF::Open(Stream&STREAM)
{
	stream = &STREAM; 
	chunkpos = stream->Position();
}


//////////////////////////////////////////////////////////////////////
_DWORD FileIFF::FindChunk(_CHAR C1, _CHAR C2, _CHAR C3, _CHAR C4,
						_CHAR F1, _CHAR F2, _CHAR F3, _CHAR F4)
{
	ASSERTMSG(stream, "FileIFF: open a stream first");
	ID formname = IFFID(F1, F2, F3, F4);
	ID chunkname = IFFID(C1, C2, C3, C4);
	ID tmp;
	while (!stream->End()) {
		if (stream->Read(&tmp, 4) != 4) { break; }
		tmp = LE2DWORD(tmp);
		if (formname == tmp) { 
			_DWORD size;
			if (stream->Read(&size, 4) != 4) { break; }
			size = LE2DWORD(size);
			if (stream->Read(&tmp, 4) != 4) { break; }
			tmp = LE2DWORD(tmp);
			if (tmp == chunkname) { 
				chunkpos = stream->Position();
				return size; 
			}
			stream->Seek(-8, Stream::CURRENT);
		}
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////
_PTR FileIFF::FindSubChunk(_CHAR C1, _CHAR C2, _CHAR C3, _CHAR C4, _BOOL LOADDATA)
{
	ASSERTMSG(stream, "FileIFF: open a stream first");
	stream->Seek(chunkpos);
	ID chunkname = IFFID(C1, C2, C3, C4);
	ID tmp;
	_DWORD size;
	while (!stream->End()) {
		stream->Read(&tmp, 4);
		tmp = LE2DWORD(tmp);
		stream->Read(&size, 4);
		size = LE2DWORD(size);
		if (chunkname == tmp) {
			stream->Seek(-8, Stream::CURRENT);
			if (LOADDATA) {
				size += 8;
			}
			else {
				size = 8;
			}
			_BYTE *buf = new _BYTE[size];
			if (!buf) { return NULL; }
			stream->Read(buf, size);
			return buf;
		}
		else {
			stream->Seek(size, Stream::CURRENT);
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
void FileIFF::FreeSubChunk(_PTR PTR)  
{
	ASSERT(PTR);
	delete [](_BYTE*)PTR;
}
