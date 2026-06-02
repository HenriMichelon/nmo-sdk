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

$Id: SoundDeviceDirectSound.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Thread.hpp>
# include <nmo/Sound.hpp>
# include <nmo/sound/WaveTrack.hpp>
# include <nmo/sound/SoundDevice.hpp>
# include <nmo/Interface.hpp>
# include <mmsystem.h>
# include <dsound.h>
# include <math.h>
#endif
#include "SoundDeviceDirectSound.hpp"
#include "SoundBufferRaw.hpp"

//--------------------------------------------------------------------------
BOOL CALLBACK nmo_SoundDriverDirectSound_Enumerate(LPGUID lpGuid,
												  LPCSTR lpcstrDescription, 
												  LPCSTR lpcstrModule, 
												  LPVOID lpContext)
{
	if (!lpGuid) { return TRUE; }
	SoundDeviceDirectSound* driver = (SoundDeviceDirectSound*)lpContext;
	driver->devices.Add(new Stringz(lpcstrDescription));
	GUID *tmp = new GUID;
	Memory::Copy(tmp, lpGuid, sizeof(GUID));
	driver->uiddevices.Add(tmp);
	return TRUE;
}


//--------------------------------------------------------------------------
SoundDeviceDirectSound::SoundDeviceDirectSound(): 
	lpds(NULL),	lpdsbPrimary(NULL), lpdsb(NULL), lpdsNotify(NULL),
		rghEvent(NULL), rgdsbpn(NULL)
{
	DirectSoundEnumerate(nmo_SoundDriverDirectSound_Enumerate, this);	
}


//--------------------------------------------------------------------------
SoundDeviceDirectSound::~SoundDeviceDirectSound()
{
	Close();
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Close()
{
	SoundDevice::Close();
    if (lpds) { 
		lpds->Release(); 
		lpds = NULL;
	}
	if (rgdsbpn) {
		delete []rgdsbpn;
		rgdsbpn = NULL;
	}
	if (rghEvent) {
		delete []rghEvent;
		rghEvent = NULL;
	}
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceDirectSound::Open(const Stringz&N, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	if (SoundDevice::Open(N, NC, NSPS, NBPS)) {
		ListIterator<GUID> lstid(uiddevices);
		deviceID = &lstid[deviceIndex];
		prepared = FALSE;
		WaveFormat.nChannels = (WORD) Channels();
		WaveFormat.nSamplesPerSec = SamplesPerSec();
		WaveFormat.wBitsPerSample = (WORD) BitsPerSample();
		WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
		WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample)/8;
		WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
		WaveFormat.cbSize = 0;

		HWND hWnd;
		if (IWindow::Link::handles.Count() > 0) {
			ListIterator<IWindow::Link> lst(IWindow::Link::handles);
			if (lst[1].nmo->Visible()) { 
				hWnd = IHANDLE(lst[1].handle);
			}
		}
		else {
			hWnd= GetForegroundWindow();
			if (hWnd == NULL) { hWnd = GetDesktopWindow(); }
		}
		if FAILED(DirectSoundCreate(deviceID, &lpds, NULL)) { return FALSE; }
		if FAILED(lpds->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE)) { return FALSE;	}

		DSBUFFERDESC dsbdesc;
		Memory::Fill(&dsbdesc, 0, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER 
						| DSBCAPS_CTRLPAN 
						| DSBCAPS_CTRLVOLUME 
						| DSBCAPS_LOCHARDWARE;
		if FAILED(lpds->CreateSoundBuffer(&dsbdesc, &lpdsbPrimary, NULL)) { 
			dsbdesc.dwFlags &= ~DSBCAPS_LOCHARDWARE;
			if FAILED(lpds->CreateSoundBuffer(&dsbdesc, &lpdsbPrimary, NULL)) { 
				return FALSE; 
			}
		}
		lpdsbPrimary->SetFormat(&WaveFormat);

		WaveBufSize = WaveFormat.nAvgBytesPerSec / (1000 / sampleduration);
		WaveBufSize -= (((WaveBufSize/2) % 2) ? 2 : 0);

		Memory::Fill(&dsbdesc, 0, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2   // Always a good idea
						| DSBCAPS_LOCHARDWARE
						| DSBCAPS_GLOBALFOCUS         // Allows background playing
						| DSBCAPS_CTRLPOSITIONNOTIFY; // Needed for notification
		dsbdesc.dwBufferBytes = WaveBufSize * 2;
		dsbdesc.lpwfxFormat = &WaveFormat;
		if FAILED(lpds->CreateSoundBuffer(&dsbdesc, &lpdsb, NULL)) { 
			dsbdesc.dwFlags &= ~DSBCAPS_LOCHARDWARE;
			if FAILED(lpds->CreateSoundBuffer(&dsbdesc, &lpdsb, NULL)) { 
				return FALSE; 
			}
		}

		nEvents = 2;
		rghEvent = new HANDLE[nEvents];
		rgdsbpn = new DSBPOSITIONNOTIFY[nEvents];

		for (_DWORD idx=0; idx < nEvents; idx++) {
			if ((rghEvent[idx] = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL) { return FALSE; }
			rgdsbpn[idx].dwOffset = idx * WaveBufSize;
			rgdsbpn[idx].hEventNotify = rghEvent[idx];
		}
		prepared = TRUE;
		GetVolume();
		GetPan();
	}
	return prepared;
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	if (!prepared) { return; }
	SoundDevice::Prepare();

	if (buffers.Count() > 0) {
		WaveDataPlayed = maxTrackSize;
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Reset(WaveBufSize); }
		if (STARTPOS) { Seek(STARTPOS); }
		if (tracker) { tracker->OnPlay(*this); }
		loop = LOOP;
		ThThread::Start(ThThread::HIGH);
	}
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Stop()
{
	if (playing) {
		ThThread::Stop();
		lpdsbPrimary->Stop();
		lpdsb->Stop();
		if (lpdsNotify) { 
			lpdsNotify->Release(); 
			lpdsNotify = NULL;
		}
		playing = FALSE;
		ListIterator<SoundBuffer> list(buffers);
		while (!list.End()) { list.Next().Done(); }
		WaveDataPlayed = 0;
		if (tracker) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Pause()
{
	if (!prepared) { return; }
	lpdsb->Stop();
	paused = TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Continue()
{
	if (!prepared) { return; }
	lpdsb->Play(0, 0, DSBPLAY_LOOPING);
	paused = FALSE;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDirectSound::Position()
{
	if (playing) { 
		return (maxTrackSize - WaveDataPlayed);
	}
	else {
		return 0; 
	}
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::Seek(_DWORD POS)
{
	if (POS % 2) { POS++; }
	WaveDataPlayed = maxTrackSize - POS;
	ListIterator<SoundBuffer> list(buffers);
	while (!list.End()) { list.Next().Seek(POS); }
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::SetVolume(_DWORD VOL)
{
	if (!prepared) { return; }
	LONG vol = (VOL * ABS(_LONG(DSBVOLUME_MAX - DSBVOLUME_MIN))) / VOLUMEMAX;
	if FAILED(lpdsb->SetVolume(vol - ABS(_LONG(DSBVOLUME_MAX - DSBVOLUME_MIN)))) {
		Debug(dprintf("SoundDeviceDirectSound::SetVolume(%d) failed\n",
					vol - ABS(_LONG(DSBVOLUME_MAX - DSBVOLUME_MIN))));
	}
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDirectSound::GetVolume()
{
	if (!prepared) { return 0; }
	LONG vol;
	if FAILED(lpdsbPrimary->GetVolume(&vol)) {
		dprintf("SoundDeviceDirectSound::GetVolume failed\n");
		return 0;
	}
	volume = (((DSBVOLUME_MAX - DSBVOLUME_MIN) + vol) * VOLUMEMAX)
			/ ABS(_LONG(DSBVOLUME_MAX - DSBVOLUME_MIN));
	return volume;
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::SetPan(_LONG)
{
	if (!prepared) { return; }
}


//--------------------------------------------------------------------------
_LONG SoundDeviceDirectSound::GetPan()
{
	if (!prepared) { return 0; }
	pan = 0;
	return pan;
}


//--------------------------------------------------------------------------
void SoundDeviceDirectSound::OnStart()
{
	dprintf("SoundDeviceDirectSound: Thread started\n");
	playing = TRUE;
	NextSample(currentbuffer = 1);
	if FAILED(lpdsb->QueryInterface(IID_IDirectSoundNotify, (VOID **)&lpdsNotify)) { return ; }
	if FAILED(lpdsNotify->SetNotificationPositions(nEvents, rgdsbpn)) {
		lpdsNotify->Release();
		return ;
	}
	lpdsbPrimary->Play(0, 0, DSBPLAY_LOOPING); 
	lpdsb->Play(0, 0, DSBPLAY_LOOPING);

    // The message loop lasts until we get a WM_QUIT message,
    // upon which we shall return from the function.
    while (TRUE) {
		MSG		msg;
		DWORD	result;
        // Read all of the messages in this next loop, 
        // removing each message as we read it.
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
            if (msg.message == WM_QUIT) { return; }
            //DispatchMessage(&msg); 
        }

        // Wait for any message sent or posted to this queue 
        // or for one of the passed handles be set to signaled.
        result = MsgWaitForMultipleObjects( 
				nEvents,      // How many possible events
				rghEvent,       // Location of handles
				FALSE,          // Wait for all?
				INFINITE,       // How long to wait
				QS_POSTMESSAGE);   // Any message is an event


        // The result tells us the type of event we have.
        if (result != (WAIT_OBJECT_0 + nEvents))
        {
            // One of the handles became signaled. 
			 if (!NextSample(result - WAIT_OBJECT_0)) {
				 break;
			 }
        } // End of else clause.
		//Debug(dprintf("Event %d\n", result);)
	}
}



//--------------------------------------------------------------------------
_BOOL SoundDeviceDirectSound::NextSample(_DWORD dwPos)
{
	if (!playing) { return FALSE; }
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

    LPVOID  lpvPtr1; 
    DWORD   dwBytes1; 
    LPVOID  lpvPtr2; 
    DWORD   dwBytes2; 
    if SUCCEEDED(lpdsb->Lock((1 - dwPos) * WaveBufSize, 
								size, 
								&lpvPtr1, &dwBytes1, 
								&lpvPtr2, &dwBytes2, 0)) {
		//_BYTE *data = new _BYTE[size];
		//MixBuffers((_BYTE*)data, size);
		//Memory::Copy(lpvPtr1, data, dwBytes1);
        //if (lpvPtr2) { Memory::Copy(lpvPtr2, data+dwBytes1, dwBytes2); }
		MixBuffers((_BYTE*)lpvPtr1, dwBytes1);
		if (lpvPtr2) { MixBuffers((_BYTE*)lpvPtr2, dwBytes2); }
		lpdsb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
		//delete data;
	}
	return TRUE;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceDirectSound::BufferSize() const
{
	if (playing) {
		return (WaveDataPlayed < WaveBufSize ? WaveDataPlayed : WaveBufSize);
	}
	else {
		return WaveBufSize;
	}
}
#endif
