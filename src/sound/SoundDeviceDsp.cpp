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

	(c) 2000-2002 Henri Michelon

$Id: SoundDeviceDsp.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#if defined(_BSD) || defined(_LINUX)
#include <nmo/Thread.hpp>
#include <nmo/Sound.hpp>
#include <nmo/sound/WaveTrack.hpp>
#include <nmo/sound/SoundDevice.hpp>
#include "SoundDeviceDsp.hpp"
#include "SoundBufferRaw.hpp"
using namespace NMO;

#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <errno.h>
#include <string.h>
#if defined(__FreeBSD__)
	#include <machine/soundcard.h>
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	#include <soundcard.h>
#elif defined(_LINUX)
	#include <linux/soundcard.h>
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__) 
	#define DEVICE		"/dev/audio"
	#define ALL_DEVICE	"/dev/audio*"
#else
	#define DEVICE		"/dev/dsp"
	#define ALL_DEVICE	"/dev/dsp*"
#endif

//--------------------------------------------------------------------------
SoundDeviceDsp::SoundDeviceDsp()
{
	dsp = -1;
	devices.Add(new Stringz(DEVICE));
   /* DirSearch   dir;
    DirSearch::FindData     find;
    if (dir.FindFirstFile(ALL_DEVICE, find)) {
        do {
			if ((find.fileName.Len() > 3)  && 
				((find.fileName[3l] < '0') ||
				 (find.fileName[3l] > '9'))) {
				continue;
			}
            devices.Add(new Stringz("/dev/" + find.fileName));
        } while (dir.FindNextFile(find));
    }
    dir.FindCloseFile(find);*/
	buffer[0] = NULL;
	buffer[1] = NULL;
}


//--------------------------------------------------------------------------
SoundDeviceDsp::~SoundDeviceDsp()
{
	Close();
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::Close()
{
	Stop();
	SoundDevice::Close();
	if (dsp != -1) { close(dsp); }
	if (buffer[0] != NULL) {
		delete []buffer[0];
		delete []buffer[1];
		buffer[0] = NULL;
		buffer[1] = NULL;
	}
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceDsp::Open(const Stringz&N, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	int val;

	if (!SoundDevice::Open(N, NC, NSPS, NBPS)) { return FALSE; }
	prepared = FALSE;

	if ((dsp = open (Name(), O_WRONLY, 0)) == -1) {
		return FALSE;
	}

	WaveBufSize = SamplesPerSec() / (1000 / sampleduration);
	WaveBufSize -= (((WaveBufSize/2) % 2) ? 2 : 0);

#ifdef SNDCTL_DSP_SETBLKSIZE
	val = WaveBufSize;
	if (ioctl (dsp, SNDCTL_DSP_SETBLKSIZE, &val) == -1) { return FALSE; }
#endif

	switch (BitsPerSample()) {
	case 8:
		val = AFMT_U8;
		break;
	case 16:
		val = AFMT_S16_LE;
		break; 
	default:
		return FALSE;
	}
    if (ioctl(dsp, SNDCTL_DSP_SETFMT, &val) == -1) { return FALSE; }
    val = Channels();
    if (ioctl(dsp, SNDCTL_DSP_CHANNELS, &val) == -1) { return FALSE; }

	val = BitsPerSample();
    if (ioctl(dsp, SNDCTL_DSP_SAMPLESIZE, &val) == -1) { return FALSE; }
	val = SamplesPerSec();
    if (ioctl(dsp, SNDCTL_DSP_SPEED, &val) == -1) { return FALSE; }

	/*Debug(	
      audio_buf_info info;
	  ioctl(dsp, SNDCTL_DSP_GETOSPACE, &info);
	  dprintf("fragments = %d\n", info.fragments);
	  dprintf("fragstotal = %d\n", info.fragstotal);
	  dprintf("fragsize = %d\n", info.fragsize);
	  dprintf("bytes = %d\n", info.bytes);
	)*/

	if (ioctl(dsp, SNDCTL_DSP_GETBLKSIZE, &val) == -1) { return FALSE; }
	WaveBufSize = val;
	WaveBufSize -= (((WaveBufSize/2) % 2) ? 2 : 0);
	Debug(dprintf("SoundDevice: buffer len=%d, size=%d\n", sampleduration, WaveBufSize);)
	buffer[0] = new _BYTE[WaveBufSize];
	buffer[1] = new _BYTE[WaveBufSize];

	GetVolume();
	prepared = TRUE;
	return TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::OnStart()
{
	dprintf("SoundDevice: start queueing...\n");
	readBuffer = 0;
	if (playing) {
		if (WaveDataPlayed == 0) { 
			Stop();
			return; 
		}
		QueueWaveData();
	}
	while (playing && (WaveDataPlayed > 0)) {
		WriteBuffer(buffer[readBuffer], 
					bufsize[readBuffer]);
		readBuffer = 1 - readBuffer;
		QueueWaveData();
		//ioctl(dsp, SNDCTL_DSP_POST, 0); // SNDCTL_DSP_SYNC block all application threads
	}
	dprintf("SoundDevice: end of play\n");
	playing = FALSE;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Done(); }
	WaveDataPlayed = 0;
	if (tracker != NULL) { tracker->OnStop(*this); }
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::QueueWaveData()
{
	_DWORD size = WaveBufSize;
	if (tracker) { tracker->OnQueue(*this); }
	if (WaveDataPlayed < size) {
		if (loop) {
			WaveDataPlayed = maxTrackSize;
		}
		else {
			size = WaveDataPlayed; 
			WaveDataPlayed = 0;
		}
	}	
	else {
		WaveDataPlayed -= size;
	}
	bufsize[readBuffer] = size;
	/*dprintf("SoundDevice: queueing %d/%d bytes in buffer %d...\n",
			size, WaveDataPlayed, readBuffer);*/
	MixBuffers(buffer[readBuffer], size);
}

	
//--------------------------------------------------------------------------
void SoundDeviceDsp::WriteBuffer(_BYTE*BUFFER, _DWORD SIZE)
{
   _DWORD 	remain = SIZE;
    int 	res;

    while (remain) {
		if ((res = write(dsp, BUFFER, remain)) == -1) {
			if (errno != EWOULDBLOCK) { break; }
			continue;
		}
		if (res > int(remain)) { break; }
	    remain -= res;
		BUFFER += res;
    }
}

	
//--------------------------------------------------------------------------
void SoundDeviceDsp::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	if (!prepared) { 
		dprintf("SoundDevice: unprepared device! \n");
		return; 
	}
	SoundDevice::Prepare();
	if (buffers.Count() == 0) { return; }

	WaveDataPlayed = maxTrackSize;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Reset(WaveBufSize); }
	if ((maxTrackSize - STARTPOS) > WaveBufSize) {
		loop = LOOP;
		if (STARTPOS > 0) { Seek(STARTPOS); }
		if (tracker) { tracker->OnPlay(*this); }
		playing = TRUE;
		ThThread::Start(ThThread::HIGH);
	}
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::Stop()
{
	if (playing) {
		playing = FALSE;
		ThThread::Wait();
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		WaveDataPlayed = 0;
		if (tracker != NULL) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::Pause()
{
	if (!prepared) { return; }
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::Continue()
{
	if (!prepared) { return; }
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDsp::Position()
{
    if (playing) {
        return (maxTrackSize - WaveDataPlayed);
    }
    else {
       return 0;
    }
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::Seek(_DWORD POS)
{
    if (POS % 2) { POS++; }
    WaveDataPlayed = maxTrackSize - POS;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Seek(POS); }
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::SetVolume(_DWORD VOL)
{
	if (!prepared) { return; }
	if (pan > 0) {
	}
	else {
	}
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDsp::GetVolume()
{
	if (!prepared) { return 0; }
	volume = 0;
	pan = 0;
	return volume;
}


//--------------------------------------------------------------------------
void SoundDeviceDsp::SetPan(_LONG PAN)
{
	pan = PAN;
	SetVolume(volume);
}


//--------------------------------------------------------------------------
_LONG SoundDeviceDsp::GetPan()
{
	GetVolume();
	return pan;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDsp::BufferSize() const
{
    if (playing) {
        return (WaveDataPlayed < WaveBufSize ? WaveDataPlayed : WaveBufSize);
    }
	else {
		return WaveBufSize;
    }
}

#endif
