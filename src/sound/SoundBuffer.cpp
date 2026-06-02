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

$Id: SoundBuffer.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#include <nmo/Sound.hpp>
#include <nmo/sound/SoundBuffer.hpp>
using namespace NMO;


//---------------------------------------------------------
SoundBuffer::SoundBuffer(): 
	track(NULL), size(0), currentsize(0), 
	destroyTrack(FALSE)
{}


//---------------------------------------------------------
void SoundBuffer::Attach(WaveTrack &TRACK)
{
	destroyTrack = FALSE;
	track = &TRACK;
}


//---------------------------------------------------------
void SoundBuffer::Attach(WaveTrack *TRACK)
{
	destroyTrack = TRUE;
	track = TRACK;
}


//---------------------------------------------------------
SoundBuffer::~SoundBuffer()
{
	if (destroyTrack) { delete track; }
}


//---------------------------------------------------------
_BOOL SoundBuffer::End() 
{ 
	return (track->End() && (!track->Loop()));
}


//---------------------------------------------------------
_DWORD SoundBuffer::Size() const 
{ 
	return currentsize; 
};
