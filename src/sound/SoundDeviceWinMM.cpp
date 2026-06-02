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

$Id: SoundDeviceWinMM.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
	#include <nmo/Sound.hpp>
	#include <nmo/sound/WaveTrack.hpp>
	#include <nmo/sound/SoundDevice.hpp>
	#include <mmsystem.h>
#endif
#include "SoundDeviceWinMM.hpp"
#include "SoundBufferRaw.hpp"


//--------------------------------------------------------------------------
void CALLBACK WaveOutProc(HWAVEOUT , UINT uMsg, DWORD dwInstance, 
						  DWORD dwParam1, DWORD )
{
	if (uMsg == MM_WOM_DONE) {
		if (!((SoundDeviceWinMM*)dwInstance)->QueueWaveData((WAVEHDR*)dwParam1)) {
			((SoundDeviceWinMM*)dwInstance)->StopQueueing();
		}
	}
}


//--------------------------------------------------------------------------
SoundDeviceWinMM::SoundDeviceWinMM(): hMixer(NULL), HWaveOut(NULL)
{
	WAVEOUTCAPS caps;
	UINT nbdev = waveOutGetNumDevs();
	for (UINT idx = 0; idx < nbdev; idx++) {
		if (waveOutGetDevCaps(idx, &caps, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR) {
			if ((caps.wChannels > 1) &&
				(caps.dwFormats & WAVE_FORMAT_4S16) &&
				(caps.dwFormats & WAVE_FORMAT_2S16) &&
				(caps.dwFormats & WAVE_FORMAT_2M16) &&
				(caps.dwFormats & WAVE_FORMAT_4M16) &&
				(caps.dwSupport & WAVECAPS_VOLUME)) {
				devices.Add(new Stringz(caps.szPname));
				_DWORD *id = new _DWORD;
				*id = idx;
				devicesid.Add(id);
			}
		}
	}
}


//--------------------------------------------------------------------------
SoundDeviceWinMM::~SoundDeviceWinMM()
{
	Close();
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Close()
{
	SoundDevice::Close();
	if (HWaveOut) {
		waveOutClose(HWaveOut);
		HWaveOut = NULL;
	}
	if (hMixer) {
		mixerClose(hMixer);
		hMixer = NULL;
	}
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceWinMM::Open(const Stringz&N, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	if (SoundDevice::Open(N, NC, NSPS, NBPS)) {
		WaveFormat.nChannels = (WORD) Channels();
		WaveFormat.nSamplesPerSec = SamplesPerSec();
		WaveFormat.wBitsPerSample = (WORD) BitsPerSample();
		WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
		WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample)/8;
		WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
		WaveFormat.cbSize = 0;
		ListIterator<_DWORD> lstid(devicesid);
		DWORD err = waveOutOpen(&HWaveOut, lstid[deviceIndex], &WaveFormat, 
								(DWORD)WaveOutProc, (DWORD)this, CALLBACK_FUNCTION);
		prepared = (err == MMSYSERR_NOERROR);
		if (prepared) {
			WaveDataPlayed = 0;
			err = mixerOpen(&hMixer, lstid[deviceIndex], 0, 0, MIXER_OBJECTF_WAVEOUT);
			prepared = (err == MMSYSERR_NOERROR);
			if (!prepared) { 
				Close(); 
			}
			else {
				WaveBufSize = WaveFormat.nAvgBytesPerSec / (1000 / sampleduration);
				WaveBufSize -= (((WaveBufSize/2) % 2) ? 2 : 0);
				GetVolume();
			}
		}
	}
	return prepared;
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	if (!prepared) { return; }
	SoundDevice::Prepare();

	if (buffers.Count() > 0) {
		DWORD	err;
		Memory::Fill(WaveHeader, 0, sizeof(WAVEHDR)*2);

		if (!(WaveHeader[0].lpData = (char *)VirtualAlloc(0, 
															WaveBufSize<<1, 
															MEM_COMMIT, 
															PAGE_READWRITE))) {
			Debug(dprintf("ERROR: Can't allocate memory for WAVE buffer!\n");)
			waveOutClose(HWaveOut);
			return;
		}

		WaveHeader[1].lpData = WaveHeader[0].lpData + WaveBufSize;
		WaveHeader[1].dwBufferLength = WaveHeader[0].dwBufferLength = WaveBufSize;

		if ((err = waveOutPrepareHeader(HWaveOut, &WaveHeader[0], sizeof(WAVEHDR)))) {
			Debug(dprintf("ERROR: preparing WAVEHDR 1! -- %08X\n", err);)
		}
		else {
			if ((err = waveOutPrepareHeader(HWaveOut, &WaveHeader[1], sizeof(WAVEHDR)))) {		
				Debug(dprintf("ERROR: preparing WAVEHDR 2! -- %08X\n", err);)
			}
			else {
				Pause();
				ListIterator<SoundBuffer> list(buffers);
				while (!list.End()) { list.Next().Reset(WaveBufSize); }
				WaveDataPlayed = maxTrackSize;
				if ((maxTrackSize - STARTPOS) > WaveBufSize) {
					if (STARTPOS) { Seek(STARTPOS); }
					loop = LOOP;
					if (tracker) { tracker->OnPlay(*this); }
					playing = TRUE;
					if (QueueWaveData(&WaveHeader[0], TRUE)) {
						if (QueueWaveData(&WaveHeader[1])) {
							Continue();
							return;
						}
					}
				}
				playing = FALSE;
				list.Reset();
				while (!list.End()) {
					list.Next().Done();
				}
				WaveDataPlayed = 0;
				if (tracker) { tracker->OnStop(*this); }
				waveOutUnprepareHeader(HWaveOut, &WaveHeader[1], sizeof(WAVEHDR));
			}
			waveOutUnprepareHeader(HWaveOut, &WaveHeader[0], sizeof(WAVEHDR));
		}
		VirtualFree(WaveHeader[0].lpData, WaveBufSize<<1, MEM_FREE);
	}
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::StopQueueing ()
{
	// This code is duplicated from Stop() but
	// without waveOutReset() which cause the
	// application to 'hang' with some audio
	// drivers when the device is already stopped.
	if (playing) {
		playing = FALSE;
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		WaveDataPlayed = 0;
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[1], sizeof(WAVEHDR));
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[0], sizeof(WAVEHDR));
		VirtualFree(WaveHeader[0].lpData, WaveBufSize<<1, MEM_FREE);
		if (tracker) { tracker->OnStop(*this); }
		freeze = TRUE;
	}
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Stop()
{
	if (playing) {
		playing = FALSE;
		waveOutReset(HWaveOut);
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		WaveDataPlayed = 0;
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[1], sizeof(WAVEHDR));
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[0], sizeof(WAVEHDR));
		VirtualFree(WaveHeader[0].lpData, WaveBufSize<<1, MEM_FREE);
		if (tracker) { tracker->OnStop(*this); }
		freeze = TRUE;
	}
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Pause()
{
	if (!prepared) { return; }
	waveOutPause(HWaveOut);
	paused = TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Continue()
{
	if (!prepared) { return; }
	waveOutRestart(HWaveOut);
	paused = FALSE;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceWinMM::Position()
{
	if (playing) {
		return (maxTrackSize - WaveDataPlayed);
	}
	else {
		return 0;
	}
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::Seek(_DWORD POS)
{
	freeze = TRUE;
	if (POS % 2) { POS++; }
	WaveDataPlayed = maxTrackSize - POS;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Seek(POS); }
	freeze = FALSE;
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::SetVolume(_DWORD VOL)
{
	MIXERLINE mixerline;
	Memory::Fill(&mixerline, 0, sizeof(MIXERLINE));
	mixerline.cbStruct = sizeof(MIXERLINE);
	mixerline.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	if (mixerGetLineInfo((HMIXEROBJ)hMixer, &mixerline, MIXER_GETLINEINFOF_COMPONENTTYPE) == MMSYSERR_NOERROR) {
		MIXERCONTROL mixerctrl;
		MIXERLINECONTROLS mixerlinectrl;
		Memory::Fill(&mixerctrl, 0, sizeof(MIXERCONTROL));
		Memory::Fill(&mixerlinectrl, 0, sizeof(MIXERLINECONTROLS));
		mixerlinectrl.cbStruct = sizeof(MIXERLINECONTROLS);
		mixerlinectrl.dwLineID = mixerline.dwLineID;
		mixerlinectrl.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		mixerlinectrl.cControls = 1;
		mixerlinectrl.cbmxctrl = sizeof(MIXERCONTROL);
		mixerlinectrl.pamxctrl = &mixerctrl;
		if (mixerGetLineControls((HMIXEROBJ)hMixer, &mixerlinectrl, MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
			MIXERCONTROLDETAILS mixerctrldet;
			MIXERCONTROLDETAILS_UNSIGNED myvolume[2];
			Memory::Fill(&mixerctrldet, 0, sizeof(MIXERCONTROLDETAILS));
			mixerctrldet.cbStruct = sizeof(MIXERCONTROLDETAILS);
			mixerctrldet.dwControlID = mixerctrl.dwControlID;
			mixerctrldet.cChannels = 2;
			mixerctrldet.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
			mixerctrldet.paDetails = myvolume;
			volume = VOL;
			myvolume[1].dwValue = MAX(_LONG(volume) + MIN(pan, 1l), 1l);
			myvolume[0].dwValue = MAX(_LONG(volume) - MAX(pan, 1l), 1l);
			mixerSetControlDetails((HMIXEROBJ)hMixer, &mixerctrldet, MIXER_GETCONTROLDETAILSF_VALUE);
		}
	}
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceWinMM::GetVolume()
{
	MIXERLINE mixerline;
	Memory::Fill(&mixerline, 0, sizeof(MIXERLINE));
	mixerline.cbStruct = sizeof(MIXERLINE);
	mixerline.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	if (mixerGetLineInfo((HMIXEROBJ)hMixer, &mixerline, MIXER_GETLINEINFOF_COMPONENTTYPE) == MMSYSERR_NOERROR) {
		MIXERCONTROL mixerctrl;
		MIXERLINECONTROLS mixerlinectrl;
		Memory::Fill(&mixerctrl, 0, sizeof(MIXERCONTROL));
		Memory::Fill(&mixerlinectrl, 0, sizeof(MIXERLINECONTROLS));
		mixerlinectrl.cbStruct = sizeof(MIXERLINECONTROLS);
		mixerlinectrl.dwLineID = mixerline.dwLineID;
		mixerlinectrl.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		mixerlinectrl.cControls = 1;
		mixerlinectrl.cbmxctrl = sizeof(MIXERCONTROL);
		mixerlinectrl.pamxctrl = &mixerctrl;
		if (mixerGetLineControls((HMIXEROBJ)hMixer, &mixerlinectrl, MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
			MIXERCONTROLDETAILS mixerctrldet;
			MIXERCONTROLDETAILS_UNSIGNED *myvolume = new MIXERCONTROLDETAILS_UNSIGNED[mixerline.cChannels+1];
			Memory::Fill(&mixerctrldet, 0, sizeof(MIXERCONTROLDETAILS));
			mixerctrldet.cbStruct = sizeof(MIXERCONTROLDETAILS);
			mixerctrldet.dwControlID = mixerctrl.dwControlID;
			mixerctrldet.cChannels = mixerline.cChannels;
			mixerctrldet.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED)*mixerline.cChannels;
			mixerctrldet.paDetails = myvolume;
			if (mixerGetControlDetails((HMIXEROBJ)hMixer, &mixerctrldet, MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {
				/*if ((mixerline.cChannels > 1) &&
					(myvolume[1].dwValue <= 0xFFFF)) {
					volume = MAX(myvolume[0].dwValue, myvolume[1].dwValue);
					pan = myvolume[1].dwValue - myvolume[0].dwValue;
				}
				else {*/
					volume = myvolume[0].dwValue;
					pan = 0;
				//}
				delete []myvolume;
				return volume;
			}
			delete []myvolume;
		}
	}
	return 0;
}


//--------------------------------------------------------------------------
void SoundDeviceWinMM::SetPan(_LONG PAN)
{
	pan = MAX(MIN(PAN, PANMAX), -PANMAX);
	SetVolume(volume);
}


//--------------------------------------------------------------------------
_LONG SoundDeviceWinMM::GetPan()
{
	GetVolume();
	return pan;
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceWinMM::QueueWaveData(WAVEHDR *waveHeader, _BOOL first)
{
	if (!playing) { return FALSE; };
	if (freeze) { 
		dprintf("SoundDeviceWinMM: device freezed !\n");
		return TRUE;
	};
	freeze = TRUE;
	if (tracker) { tracker->OnQueue(*this); }

	_DWORD size = WaveBufSize;
	if (!WaveDataPlayed) { return FALSE; }
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
	//dprintf("queue %d - %d - %d\n", size, WaveDataPlayed, maxTrackSize);
	waveHeader->dwBufferLength = size;
	MixBuffers((_BYTE*)waveHeader->lpData, size);
	waveOutWrite(HWaveOut, waveHeader, sizeof(WAVEHDR)); 
	freeze = FALSE;
	return WaveDataPlayed > 0;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceWinMM::BufferSize() const
{
	if (playing) {
		return (WaveDataPlayed < WaveBufSize ? WaveDataPlayed : WaveBufSize);
	}
	else {
		return WaveBufSize;
	}
}
#endif
