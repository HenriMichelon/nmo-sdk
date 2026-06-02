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

	(c) 2000-2002 SCS 361

$Id: SoundDevice.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Sound.hpp>
#include <nmo/sound/WaveTrack.hpp>
#include <nmo/sound/SoundDevice.hpp>
#include "SoundBufferRaw.hpp"
#include <math.h>


#ifdef _WIN32
	#ifndef _MSC_VER
		#include <mmsystem.h>
		#include <dsound.h>
	#endif
	#include "SoundDeviceWinMM.hpp"
	#include "src/interface/WinApp.hpp"
	#include "SoundDeviceDirectSound.hpp"
#elif defined(_MACOS)
	#include "SoundDeviceMacOS.hpp"
#endif


#if defined(_LINUX) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	#define _DEVDSP
	#include <nmo/Thread.hpp>
	#include "SoundDeviceDsp.hpp"
#endif

#ifdef HAVE_SDL
	#include "SoundDeviceSDL.hpp"
#endif


const _DWORD defaultBufferLength = 200;
const _DWORD SoundDevice::VOLUMEMIN = 0;
const _DWORD SoundDevice::VOLUMEMAX = 65535;
const _LONG SoundDevice::PANMAX = 65535;

//--------------------------------------------------------------------------
void SoundDevice::Drivers(List<Stringz>&LIST)
{
	Stringz drivers;
#ifdef _WIN32
	LIST.Add(new Stringz("Windows Multimedia"));
	LIST.Add(new Stringz("DirectSound"));
#elif defined(_MACOS)
	LIST.Add(new Stringz("Sound Manager"));
#elif defined(_DEVDSP)
	LIST.Add(new Stringz("/dev/dsp"));
#endif
#ifdef HAVE_SDL
	LIST.Add(new Stringz("Simple DirectMedia Layer"));
#endif
	LIST.Add(new Stringz("Stream"));
}


//--------------------------------------------------------------------------
SoundDevice* SoundDevice::Create(const Stringz& DRIVER)
{
	Stringz name = DRIVER;
	SoundDevice *device = NULL;
	if (name == "Stream") {
		device = new SoundDeviceStream;
	}
#ifdef HAVE_SDL
	else if (name == "Simple DirectMedia Layer") {
		device = new SoundDeviceSDL;
	};
#endif
	if (device == NULL) {
#if defined(_WIN32)
		if (name.Len() == 0) { name = "Windows Multimedia"; }
		if (name == "Windows Multimedia") {
			device = new SoundDeviceWinMM;
		}
		else if (name == "DirectSound") {
			device =  new SoundDeviceDirectSound;
		};
#elif defined(_DEVDSP)
		name = "/dev/dsp";
		device = new SoundDeviceDsp;
#elif defined(_MACOS)
		if (name.Len() == 0) { name = "Sound Manager"; }
		if (name == "Sound Manager") {
			device = new SoundDeviceMacOS;
		}
#endif
	}
	if (device != NULL) {
		device->drivername = name;
		Debug(dprintf("SoundDevice: opening driver %s\n", (_CHAR*)name);)
	}
	return device;
}



//--------------------------------------------------------------------------
SoundDevice::SoundDevice(): 
		freeze(FALSE), 
		channels(0), samplespersec(0), bitspersample(0),
		volume(0), pan(0), 
		loop(FALSE), paused(FALSE), 
		playing(FALSE), prepared(FALSE)
{
	tracker = NULL;	
	sampleduration = defaultBufferLength;
}


//--------------------------------------------------------------------------
SoundDevice::~SoundDevice()
{
	// Be sure that the double buffer is finishing playing
	Timer::Sleep(sampleduration * 2);
}


//--------------------------------------------------------------------------
void SoundDevice::Add(WaveTrack&TRACK)
{
	if (prepared) { 
		SoundBuffer *buf = CreateBuffer();
		buf->Attach(TRACK);
		buffers.Add(buf);
	}
}


//--------------------------------------------------------------------------
void SoundDevice::Add(WaveTrack*TRACK)
{
	if (prepared) { 
		SoundBuffer *buf = CreateBuffer();
		buf->Attach(TRACK);
		buffers.Add(buf);
	}
}


//--------------------------------------------------------------------------
void SoundDevice::Replace(_DWORD IDX, WaveTrack*TRACK)
{
	if (prepared) { 
		SoundBuffer *buf = CreateBuffer();
		buf->Attach(TRACK);
		buffers.Replace(IDX, buf);
	}
}


//--------------------------------------------------------------------------
void SoundDevice::Clear()
{
	Stop();
	buffers.Clear();
	maxTrackSize = 0;
}


//--------------------------------------------------------------------------
void SoundDevice::Close()
{
	Clear();
	prepared = FALSE;
}


//--------------------------------------------------------------------------
void SoundDevice::SetBufferLength(_DWORD MS)
{
	sampleduration = MS;
}


//--------------------------------------------------------------------------
_BOOL SoundDevice::Open(const Stringz&NAME, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	Close();

	ListIterator<Stringz> list(devices);
	deviceIndex = list.SearchFor(NAME);
	if ((deviceIndex == 0) && (devices.Count() > 0)) {
		deviceIndex = 1;
	}

	if (deviceIndex > 0) {
		channels = NC;
		samplespersec = NSPS;
		bitspersample = NBPS;
		prepared = channels && samplespersec && bitspersample;
	}
	Debug(dprintf("SoudDevice: opening device %s: %d\n", (_CHAR*)NAME, prepared);)
	return prepared;
}


//--------------------------------------------------------------------------
const Stringz& SoundDevice::DriverName() const
{
	return drivername;
}


//--------------------------------------------------------------------------
Stringz SoundDevice::Name()
{
	if ((deviceIndex > 0) && (devices.Count() > 0)) {
		ListIterator<Stringz> list(devices);
		return list[deviceIndex];
	}
	return "";
}


//--------------------------------------------------------------------------
void SoundDevice::Prepare()
{
	Stop();
	maxTrackSize = MaxTrackSize();
	freeze = FALSE;
}


//--------------------------------------------------------------------------
_DWORD SoundDevice::MaxTrackSize()
{
	_DWORD res = 0;
	WaveTrack *bigger = NULL;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) {
		WaveTrack &track = list.Next().Track();
		if (track.Size() > res) {
			res = track.Size();
			bigger = &track;
		}
	}
	if (bigger) {
		if ((bigger->Channels() == 2) && (Channels() == 1)) {
			return res / 2;
		}
		if ((bigger->Channels() == 1) && (Channels() == 2)) {
			return res * 2;
		}
	}
	return res;
}


//--------------------------------------------------------------------------
void SoundDevice::MixBuffers(_BYTE*BUFFER, _DWORD SIZE)
{
	_DWORD idx;
	_DWORD ntrack = 0;

	Memory::Fill((_PTR)BUFFER, 0, SIZE);
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) {
		ntrack++;
		SoundBuffer &sbuffer = list.Next();
		_BYTE *buffer = (_BYTE*)sbuffer.Next(SIZE);
		_DWORD size = MIN(sbuffer.Size(), SIZE);
		if ((buffer == NULL) || (size == 0)) { continue; };
		if (tracker) { 
			if (tracker->OnMix(*this, ntrack, buffer, 
							size, sbuffer.Track().Channels(), 
							BUFFER)) {
				continue;
			}
		}
		if (bitspersample == 8) {
			_BYTE nc = (_BYTE)buffers.Count();
			if (channels == 1) {
				for (idx = 0; idx < size; idx++) {
					BUFFER[idx] += buffer[idx] / nc;
				}
			}
			else if (channels == 2) {
				if (sbuffer.Track().Channels() == 1) {
					for (idx = 0; idx < size; idx+=2) {
						_INT s = (_BYTE(buffer[idx >> 1])  / nc);
						BUFFER[idx] += s;
						BUFFER[idx+1] += s;
					}
				}
				else {
					_BYTE* sbuf = buffer;
					_BYTE* dbuf = BUFFER;
					for (; size; size--) {
						(*(dbuf++)) += (*(sbuf++)) / nc;
					}
				}
			}
		}
		else if (bitspersample == 16) {
			_INT nc = (_INT)buffers.Count();
			_INT *sbuf = (_INT*)buffer;
			_INT *dbuf = (_INT*)BUFFER;
			if (channels == 1) {
				size = size >> 1;
				for (idx = 0; idx < size; idx++) {
					dbuf[idx] += (sbuf[idx]) / nc;
				}
			}
			else if (channels == 2) {
				if (sbuffer.Track().Channels() == 1) {
					for (idx = 0; idx < size; idx += 2) {
						_INT s = (_INT(sbuf[idx >> 1])  / nc);
						dbuf[idx] += s;
						dbuf[idx+1] += s;
					}
				}
				else {
					size = size >> 1;
					for (; size; size--) {
						(*(dbuf++)) += (*(sbuf++)) / nc;
						//dbuf[idx] += _INT(sbuf[idx]) / nc;
						//dbuf[idx+1] += _INT(sbuf[idx+1]) / nc;
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------
SoundBuffer* SoundDevice::CreateBuffer()
{
	return new SoundBufferRaw;
}



//--------------------------------------------------------------------------
void SoundDevice::TimeTotal(NMO::DateTime::Time&TIME)
{
	_DWORD seconds = maxTrackSize / (samplespersec * ((channels * bitspersample) / 8));
	TIME.hour = (_BYTE)seconds / 3600;
	seconds -= TIME.hour * 3600;
	TIME.minute = (_BYTE)seconds / 60;
	TIME.second = (_BYTE)seconds - TIME.minute * 60;
}


//--------------------------------------------------------------------------
void SoundDevice::TimePosition(NMO::DateTime::Time&TIME)
{
	_DWORD seconds = Position() / (samplespersec * ((channels * bitspersample) / 8));
	TIME.hour = (_BYTE)seconds / 3600;
	seconds -= TIME.hour * 3600;
	TIME.minute = (_BYTE)seconds / 60;
	TIME.second = (_BYTE)seconds - TIME.minute * 60;
}


//--------------------------------------------------------------------------
SoundDeviceTracker* SoundDevice::SetTracker(SoundDeviceTracker*TRACKER)
{
	SoundDeviceTracker* old = tracker;
	tracker = TRACKER;
	return old;
}


//--------------------------------------------------------------------------
_DWORD SoundDevice::GetVolume() { return volume; }

//--------------------------------------------------------------------------
_LONG SoundDevice::GetPan() { return pan; }

//--------------------------------------------------------------------------
_DWORD SoundDevice::BufferLength() const { return sampleduration; }

//--------------------------------------------------------------------------
NMO::List<Stringz>& SoundDevice::Devices() const { return (List<Stringz>&)devices; }
