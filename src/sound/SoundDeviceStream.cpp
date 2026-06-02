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

$Id: SoundDeviceStream.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#include <nmo/Sound.hpp>
#include "SoundBufferRaw.hpp"
using namespace NMO;

class SoundDeviceStreamAbstract
{
private:
	friend class SoundDeviceStream;
	_BOOL	quit;
	Stream	*stream;
	_DWORD	wavedataplayed;
	
};

#define _m(x) ((SoundDeviceStreamAbstract*)(this->mSoundDeviceStreamAbstract))->x


//--------------------------------------------------------------------------
SoundDeviceStream::SoundDeviceStream()
{
	mSoundDeviceStreamAbstract = new SoundDeviceStreamAbstract;
	sampleduration = 50;
	_m(stream) = NULL;
	volume = VOLUMEMAX;
	pan = 0;
	_m(quit) = TRUE;
	devices.Add(new Stringz("RIFF PCM"));
}


 //--------------------------------------------------------------------------
SoundDeviceStream::~SoundDeviceStream()
{
	delete (SoundDeviceStreamAbstract*)mSoundDeviceStreamAbstract;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::SetStream(Stream&STREAM)
{
	_m(stream) = &STREAM;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::Close()
{
	Stop();
	SoundDevice::Close();
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceStream::Open(const Stringz&N,_DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	if (SoundDevice::Open(N, NC, NSPS, NBPS)) {
		prepared = (_m(stream) != NULL);
	}
	return prepared;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	ASSERT(_m(stream));
	if (!prepared) { return; }
	if (buffers.Count() == 0) { return; }
	_m(quit) = FALSE;
	SoundDevice::Prepare();

	_DWORD buffersize = (((Channels() * BitsPerSample())/8) *
						SamplesPerSec()) / (1000 / sampleduration);
	buffersize -= (((buffersize/2) % 2) ? 2 : 0);

	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Reset(buffersize); }
	
	loop = LOOP;
	_m(wavedataplayed) = maxTrackSize;
	if (STARTPOS) { Seek(STARTPOS); }
	if (tracker) { tracker->OnPlay(*this); }
	playing = TRUE;

	_BYTE *buffer = new _BYTE[buffersize];
	while ((!_m(quit)) && QueueWaveData(buffer, buffersize)) {};
	delete []buffer;
	Stop();
}



//--------------------------------------------------------------------------
_BOOL SoundDeviceStream::QueueWaveData(_BYTE*BUFFER, _DWORD SIZE)
{
	ASSERT(_m(stream));
	if (!playing) { return FALSE; };
	if (tracker) { tracker->OnQueue(*this); }
	if (!_m(wavedataplayed)) { return FALSE; }

	freeze = TRUE;
	_DWORD size = SIZE;
	if (_m(wavedataplayed) < size) {
		size = _m(wavedataplayed); 
		_m(wavedataplayed) = 0;
	}	
	else {
		_m(wavedataplayed) -= size;
	}
	MixBuffers(BUFFER, size);
	
#ifdef _BIGENDIAN
	if (bitspersample == 16) {
		_INT *block = (_INT*)BUFFER;
		_DWORD bsize = size;
		while (bsize) {
			block[0] = SWAP_INT(block[0]);
			block++;
			bsize -= sizeof(_INT);
		}
	}
#endif
	_m(stream)->Write(BUFFER, size);
	
	freeze = FALSE;
	return TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::Stop()
{
	if (playing) {
		_m(quit) = TRUE;
		playing = FALSE;
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		if (tracker) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceStream::Pause() { }
void SoundDeviceStream::Continue() { }


//--------------------------------------------------------------------------
_DWORD SoundDeviceStream::Position()
{
	if (!prepared) { return 0; }
	return (maxTrackSize - _m(wavedataplayed));
}


//--------------------------------------------------------------------------
void SoundDeviceStream::Seek(_DWORD POS)
{
	freeze = TRUE;
	if (POS % 2) { POS++; }
	_m(wavedataplayed) = maxTrackSize - POS;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Seek(POS); }
	freeze = FALSE;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::SetVolume(_DWORD VOL) 
{
	volume = VOL;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceStream::GetVolume()
{
	return volume;
}


//--------------------------------------------------------------------------
void SoundDeviceStream::SetPan(_LONG PAN)
{
	pan = PAN;
}


//--------------------------------------------------------------------------
_LONG SoundDeviceStream::GetPan()
{
	return pan;
}

//--------------------------------------------------------------------------
_DWORD SoundDeviceStream::BufferSize() const
{
	return (_m(wavedataplayed) < samplespersec ? _m(wavedataplayed) : samplespersec);
}
