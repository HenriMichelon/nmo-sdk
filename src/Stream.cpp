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
			
$Id: Stream.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#define BLKSIZE 8192

Stream::ExcStream::ExcStream(const Stringz&MSG) : NMOException("Stream: " + MSG) {}
Stream::ExcStreamCreate::ExcStreamCreate() : ExcStream("Create") {}
Stream::ExcStreamRead::ExcStreamRead() : ExcStream("Read") {}
Stream::ExcStreamWrite::ExcStreamWrite() : ExcStream("Write") {}

//------------------------------------------------------------------
void Stream::SaveToStream(Stream & STRM)
{
	Seek(0, START);
	_DWORD nbytes;
	_BYTE *buff = new _BYTE[BLKSIZE];
	do {
		nbytes = Read(buff, BLKSIZE);
		STRM.Write(buff, nbytes);
	} while (nbytes == BLKSIZE);
	delete []buff;
}


//------------------------------------------------------------------
void Stream::LoadFromStream(Stream &STRM)
{
	STRM.SaveToStream(*this);
}


//------------------------------------------------------------------
_DWORD Stream::Write(Stream&STRM, const _DWORD SIZE)
{
	_DWORD pos = STRM.Position();
	_DWORD nbytes;
	_DWORD nblocks = SIZE / BLKSIZE;
	_DWORD remains = SIZE - (nblocks * BLKSIZE);
	_BYTE *buff = new _BYTE[BLKSIZE];
	if (!buff) { return 0; }
	for (_DWORD idx = 0; idx < nblocks; idx ++) {
		nbytes = STRM.Read(buff, BLKSIZE);
		Write(buff, nbytes);
		if (nbytes != BLKSIZE) { break; }
	}
	if (remains > 0) {
		nbytes = STRM.Read(buff, remains);
		Write(buff, nbytes);
	}
	delete []buff;
	return (STRM.Position() - pos);
}
