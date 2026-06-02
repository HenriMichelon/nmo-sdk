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

	(c) 2000-2001 SCS 361

$Id: SoundBufferRaw.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Sound.hpp>
#include <nmo/sound/SoundBuffer.hpp>
#include "SoundBufferRaw.hpp"

//---------------------------------------------------------
SoundBufferRaw::SoundBufferRaw(): SoundBuffer()
{
	buffer = NULL;
}


//---------------------------------------------------------
SoundBufferRaw::~SoundBufferRaw()
{
	Done();
}


//---------------------------------------------------------
void SoundBufferRaw::Done()
{
	if (buffer != NULL) { 
		delete []buffer; 
		buffer = NULL;
	}
}


//---------------------------------------------------------
_BOOL SoundBufferRaw::Reset(_DWORD DEFSIZE)
{
	ASSERT(track);
	Done();
	size = DEFSIZE;
	track->Seek(0, Stream::START);
	if (size > 0) { buffer = new _BYTE[size]; }
	return TRUE;
}


//---------------------------------------------------------
_PTR SoundBufferRaw::Current()
{
	ASSERT(track);
	ASSERT(buffer);
	return buffer;
}
	

//---------------------------------------------------------
_PTR SoundBufferRaw::Next(_DWORD SIZE)
{
	ASSERT(track);
	if (SIZE > size) {
		size = SIZE;
		if (buffer != NULL) { 
			delete []buffer;
			buffer = NULL;
		}
	}
	if (buffer == NULL) { buffer = new _BYTE[size]; }

	currentsize = track->Read((_PTR)buffer, size);
	return buffer;
}
	

//---------------------------------------------------------
void SoundBufferRaw::Seek(_DWORD POS)
{
	ASSERT(track);
	track->Seek(POS, Stream::START);
}
