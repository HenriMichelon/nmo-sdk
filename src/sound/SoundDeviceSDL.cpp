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

$Id: SoundDeviceSDL.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/config.h>
#ifdef HAVE_SDL
#include <nmo/NMO.hpp>
#include <nmo/Sound.hpp>
#include <nmo/sound/WaveTrack.hpp>
#include <nmo/sound/SoundDevice.hpp>
#include "SoundDeviceSDL.hpp"
#include "SoundBufferRaw.hpp"
using namespace NMO;

#include <SDL.h>
#include <SDL_audio.h>


//--------------------------------------------------------------------------
void _nmo_SDL_callback(void *DEV, Uint8 *stream, int len)
{
	SoundDeviceSDL *device = (SoundDeviceSDL*)DEV;
	device->QueueData(stream, len);
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::QueueData(_BYTE*BUFFER, _DWORD SIZE)
{
	if (!playing) { return; }
	if (WaveDataPlayed == 0) {
		dprintf("SoundDevice: stopping...\n");
		Stop();
		return;
	}
	if (tracker) { tracker->OnQueue(*this); }
	if (WaveDataPlayed < SIZE) {
		if (loop) {
			WaveDataPlayed = maxTrackSize;
		}
		else {
			SIZE = WaveDataPlayed; 
			WaveDataPlayed = 0;
		}
	}	
	else {
		WaveDataPlayed -= SIZE;
	}
	//dprintf("SoundDevice: queueing %d/%d bytes...\n", SIZE, WaveDataPlayed );
	MixBuffers(BUFFER, SIZE);
}



//--------------------------------------------------------------------------
SoundDeviceSDL::SoundDeviceSDL()
{
    devices.Add(new Stringz("SDL 1.2"));
}


//--------------------------------------------------------------------------
SoundDeviceSDL::~SoundDeviceSDL()
{
	Close();
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Close()
{
	Stop();
	SoundDevice::Close();
	SDL_CloseAudio();
	SDL_Quit();
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceSDL::Open(const Stringz&N, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER )) {
        dprintf("Error SDL_Init\n");
		return FALSE;
	}

	if (!SoundDevice::Open(N, NC, NSPS, NBPS)) { return FALSE; }
	prepared = FALSE;

	SDL_AudioSpec fmt; 

	switch (BitsPerSample()) {
	case 8:
		fmt.format = AUDIO_U8;
		break;
	case 16:
#ifdef _BIGENDIAN
		fmt.format = AUDIO_S16MSB;
#else
		fmt.format = AUDIO_S16LSB;
#endif
		break; 
	default:
		return FALSE;
	}

	fmt.freq = SamplesPerSec();
	fmt.channels = Channels();
	if (sampleduration < 100) {
		fmt.samples = 512;
	}
	else if (sampleduration < 200) {
		fmt.samples = 1024;
	}
	else if (sampleduration < 400) {
		fmt.samples = 2048;
	}
	else {
		fmt.samples = 4096;
	}
	fmt.callback = _nmo_SDL_callback;
	fmt.userdata = (_PTR)this;
    if ( SDL_OpenAudio(&fmt, &fmt) < 0 ) {
        dprintf("Error SDL_OpenAudio\n");
        return FALSE;
    }
	WaveBufSize = fmt.size;
	sampleduration = (WaveBufSize * 1000) / SamplesPerSec();
	Debug(dprintf("SoundDevice: buffer len=%d, size=%d\n", sampleduration, WaveBufSize);)

	SDL_PauseAudio(1);
	SDL_LockAudio();
	GetVolume();
	prepared = TRUE;
	return TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	if (!prepared) { return; }
	SoundDevice::Prepare();

	if (buffers.Count() == 0) { return; }

	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Reset(); }
	if (STARTPOS < maxTrackSize - STARTPOS) {
		loop = LOOP;
		WaveDataPlayed = maxTrackSize;
		if (STARTPOS) { Seek(STARTPOS); }
		if (tracker) { tracker->OnPlay(*this); }
		playing = TRUE;
		SDL_PauseAudio(0);
		SDL_UnlockAudio();
	}
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Stop()
{
	if (playing) {
		SDL_LockAudio();
		SDL_PauseAudio(1);
		playing = FALSE;
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		WaveDataPlayed = 0;
		if (tracker != NULL) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Pause()
{
	if (!prepared) { return ; }
	SDL_PauseAudio(1);
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Continue()
{
	if (!prepared) { return ; }
	SDL_PauseAudio(0);
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceSDL::Position()
{
    if (playing) {
        return (maxTrackSize - WaveDataPlayed);
    }
    else {
       return 0;
    }
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::Seek(_DWORD POS)
{
    if (POS % 2) { POS++; }
    WaveDataPlayed = maxTrackSize - POS;
    ListIterator<SoundBuffer> list(buffers);
    while (!list.End()) { list.Next().Seek(POS); }
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::SetVolume(_DWORD VOL)
{
	if (!prepared) { return; }
	if (pan > 0) {
	}
	else {
	}
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceSDL::GetVolume()
{
	if (!prepared) { return 0; }
	volume = 0;
	pan = 0;
	return volume;
}


//--------------------------------------------------------------------------
void SoundDeviceSDL::SetPan(_LONG PAN)
{
	pan = PAN;
	SetVolume(volume);
}


//--------------------------------------------------------------------------
_LONG SoundDeviceSDL::GetPan()
{
	GetVolume();
	return pan;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceSDL::BufferSize() const
{
    if (playing) {
        return (WaveDataPlayed < WaveBufSize ? WaveDataPlayed : WaveBufSize);
    }
    else {
        return WaveBufSize;
    }
}


#endif
