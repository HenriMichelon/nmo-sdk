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

$Id: SoundDeviceMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Sound.hpp>
#include "CarbonSndPlayDB.h"
#include "SoundDeviceMacOS.hpp"
#include "SoundBufferRaw.hpp"

class SoundDeviceMacOSAbstract
{
public:
	#define MAX_NODETOBEFILLED 10

	class NodeToBeFilled
	{
	public:
		_BOOL				empty;
		SoundDeviceMacOS	*device;
		SndDoubleBufferPtr	buffer;
		
		NodeToBeFilled(): empty(FALSE), device(NULL), buffer(NULL) {};
	};
	
	static _BOOL fifoLocked;
	static _BOOL timerLocked;
	static NodeToBeFilled emptyNodes[MAX_NODETOBEFILLED];
	static EventLoopTimerRef pollTimer;
	static _DWORD nInstPollTimer;
	void FlushNodes(SoundDevice*);

private:
	SndChannelPtr			sndchannel;
	SndDoubleBufferHeader2	snddbheader;
	_DWORD					wavedataplayed;
	_DWORD					wavebufsize;

	friend class SoundDeviceMacOS;
};

_BOOL SoundDeviceMacOSAbstract::fifoLocked = FALSE;
_BOOL SoundDeviceMacOSAbstract::timerLocked = FALSE;
SoundDeviceMacOSAbstract::NodeToBeFilled SoundDeviceMacOSAbstract::emptyNodes[MAX_NODETOBEFILLED];
EventLoopTimerRef SoundDeviceMacOSAbstract::pollTimer = NULL;
_DWORD SoundDeviceMacOSAbstract::nInstPollTimer = 0;

#define _m(x) ((SoundDeviceMacOSAbstract*)(this->mSoundDeviceMacOSAbstract))->x


//--------------------------------------------------------------------------
pascal void MySndCallBackCallback (SndChannelPtr, SndDoubleBufferPtr snddb)
{
	SoundDeviceMacOSAbstract::fifoLocked = TRUE;
	for (_DWORD idx = 0; idx < MAX_NODETOBEFILLED; idx ++) {
		if (!SoundDeviceMacOSAbstract::emptyNodes[idx].empty) {
			// New empty buffer to fill
			SoundDeviceMacOSAbstract::emptyNodes[idx].device = (SoundDeviceMacOS*)snddb->dbUserInfo[0];
			SoundDeviceMacOSAbstract::emptyNodes[idx].buffer = snddb;
			SoundDeviceMacOSAbstract::emptyNodes[idx].empty = TRUE;
			//dprintf("MySndCallBackCallback: new empty buffer\n");
			break;
		}
	}
	SoundDeviceMacOSAbstract::fifoLocked = FALSE;
}


//--------------------------------------------------------------------------
pascal void MyEventLoopTimer(EventLoopTimerRef, void*)
{
	if (SoundDeviceMacOSAbstract::fifoLocked || SoundDeviceMacOSAbstract::timerLocked) { return; }
	SoundDeviceMacOSAbstract::timerLocked = TRUE;
	SoundDeviceMacOS *device = NULL;
	SndDoubleBufferPtr buffer = NULL;
	// Search for an unused slot
	for (_DWORD idx = 0; idx < MAX_NODETOBEFILLED; idx ++) {
		if (SoundDeviceMacOSAbstract::fifoLocked) { 
			SoundDeviceMacOSAbstract::timerLocked = FALSE;
			return; 
		}
		if (SoundDeviceMacOSAbstract::emptyNodes[idx].empty) {
			// New empty buffer to fill
			//dprintf("PollEmptyNodes:: find a buffer to fill\n");
			device = SoundDeviceMacOSAbstract::emptyNodes[idx].device;
			buffer = SoundDeviceMacOSAbstract::emptyNodes[idx].buffer;
			SoundDeviceMacOSAbstract::emptyNodes[idx].empty = FALSE;
			// Really fill the buffer
			if (device && buffer && device->IsPlaying()) {
				device->QueueWaveData(buffer);
			}
			break; // Fill only one buffer at a time
		}
	}
	SoundDeviceMacOSAbstract::timerLocked = FALSE;
}



//--------------------------------------------------------------------------
void SoundDeviceMacOSAbstract::FlushNodes(SoundDevice *DEV)
{
	SoundDeviceMacOSAbstract::fifoLocked = TRUE;
	for (_DWORD idx = 0; idx < MAX_NODETOBEFILLED; idx ++) {
		if (DEV == SoundDeviceMacOSAbstract::emptyNodes[idx].device) {
			SoundDeviceMacOSAbstract::emptyNodes[idx].empty = FALSE;
			SoundDeviceMacOSAbstract::emptyNodes[idx].buffer = NULL;
			SoundDeviceMacOSAbstract::emptyNodes[idx].device = NULL;
		}
	}
	SoundDeviceMacOSAbstract::fifoLocked = FALSE;
}


//--------------------------------------------------------------------------
SoundDeviceMacOS::SoundDeviceMacOS()
{
	mSoundDeviceMacOSAbstract = new SoundDeviceMacOSAbstract;
	NumVersion version = SndSoundManagerVersion();
	Stringz device;
	device.Format("Sound Manager %d",version.majorRev);
	devices.Add(new Stringz(device));
	_m(sndchannel) = NULL;
	for (_DWORD idx = 0; idx < 2; idx ++) {
		_m(snddbheader).dbhBufferPtr[idx] = NULL;
	}
}


//--------------------------------------------------------------------------
SoundDeviceMacOS::~SoundDeviceMacOS()
{
	Close();
	if (mSoundDeviceMacOSAbstract) {
		delete (SoundDeviceMacOSAbstract*)mSoundDeviceMacOSAbstract;
		mSoundDeviceMacOSAbstract = NULL;
	}
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Close()
{
	Stop();
	SoundDevice::Close();
	for (_DWORD idx = 0; idx < 2; idx ++) {
		if (_m(snddbheader).dbhBufferPtr[idx] != NULL) {
			delete [] (_BYTE*)_m(snddbheader).dbhBufferPtr[idx];
			_m(snddbheader).dbhBufferPtr[idx] = NULL;
		}
	}
	SndDisposeChannel(_m(sndchannel), true);
	_m(sndchannel) = NULL;
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceMacOS::Open(const Stringz&NAME, _DWORD NC, _DWORD NSPS, _DWORD NBPS)
{
	if (SoundDevice::Open(NAME, NC, NSPS, NBPS)) {
		prepared = FALSE;
		OSErr err = SndNewChannel(&_m(sndchannel),
								sampledSynth,
								(NC == 1 ? initMono : initStereo),
								NULL);
		if (err != noErr) {
			Debug(dprintf("SoundDev: error SndNewChannel %d\n", err);)
			return FALSE;
		}
		_m(wavebufsize) = (samplespersec * 
					   (( channels * bitspersample ) / 8 ) )
						/ (1000 / sampleduration);
		_m(wavebufsize) -= (((_m(wavebufsize)/2) % 2) ? 2 : 0);
		for (_DWORD idx = 0; idx < 2; idx ++) {
			_m(snddbheader).dbhBufferPtr[idx] = (SndDoubleBufferPtr)  new _BYTE[sizeof(SndDoubleBuffer) + _m(wavebufsize)];
		}
		_m(snddbheader).dbhNumChannels = NC;
		_m(snddbheader).dbhSampleSize = NBPS;
		if (NBPS == 8) {
			_m(snddbheader).dbhFormat = kSoundNotCompressed; // k8BitOffsetBinaryFormat;
			_m(snddbheader).dbhCompressionID = notCompressed;
		}
		else {
			_m(snddbheader).dbhFormat = k16BitBigEndianFormat;
			_m(snddbheader).dbhCompressionID = fixedCompression;;
		}	
		_m(snddbheader).dbhPacketSize = 0;
		switch (NSPS) {
		case 11025: _m(snddbheader).dbhSampleRate = rate11025hz; break;
		case 22050: _m(snddbheader).dbhSampleRate = rate22050hz; break;
		case 44100: _m(snddbheader).dbhSampleRate = rate44khz; break;
		default: _m(snddbheader).dbhSampleRate = rate22khz; break;
		}
		_m(snddbheader).dbhDoubleBack = SndDoubleBackUPP(MySndCallBackCallback);
		_m(wavedataplayed) = 0;
		prepared = TRUE;
		GetVolume();
	}
	return prepared;
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Play(_DWORD STARTPOS, _BOOL LOOP)
{
	if (!prepared) { return; }
	SoundDevice::Prepare();

	if (buffers.Count() > 0) {
		ListIterator<SoundBuffer> lst(buffers);
		while (!lst.End()) { lst.Next().Reset(_m(wavebufsize)); }
		_m(wavedataplayed) = maxTrackSize;
		if ((maxTrackSize - STARTPOS) > _m(wavebufsize)) {
			loop = LOOP;
			for (_DWORD idx = 0; idx < 2; idx ++) {
				Memory::Fill(_m(snddbheader).dbhBufferPtr[idx], 0, sizeof(SndDoubleBuffer) + _m(wavebufsize));
				_m(snddbheader).dbhBufferPtr[idx]->dbNumFrames = (_m(wavebufsize) * 8) /  
																(Channels() * BitsPerSample());
				_m(snddbheader).dbhBufferPtr[idx]->dbFlags = dbBufferReady;
				_m(snddbheader).dbhBufferPtr[idx]->dbUserInfo[0] = (long)this;
			}
			if (STARTPOS > 0) { Seek(STARTPOS); }
			if (tracker) { tracker->OnPlay(*this); }
			dprintf("SoundDev: start queueing...\n");
			playing = TRUE;
			CarbonSndPlayDoubleBuffer(_m(sndchannel), (SndDoubleBufferHeaderPtr)&_m(snddbheader));
			if ((SoundDeviceMacOSAbstract::nInstPollTimer++) == 0) {
				InstallEventLoopTimer(GetMainEventLoop(),
									kEventDurationMillisecond * 50.0,
									kEventDurationMillisecond * 50.0,
									NewEventLoopTimerUPP(MyEventLoopTimer),
									NULL,
									&SoundDeviceMacOSAbstract::pollTimer);
					dprintf("SoundDev: installed polling timer\n");
			}
			return;			
		}
		playing = FALSE;
		lst = buffers;
		while (!lst.End()) { lst.Next().Done(); }
		//dprintf("SoundDev: fall back at the end of Play()\n");
		if (tracker) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Stop()
{
	if (playing) {
		playing = FALSE;
		SndCommand sndcmd;
		sndcmd.cmd = quietCmd;
		sndcmd.param1 = 0;
		sndcmd.param2 = 0;
		OSErr err = MySndDoImmediate(_m(sndchannel), &sndcmd);
		Debug(if (err != noErr) {
			dprintf("SoundDev: error SndDoImmediate %d\n", err);
		})
		if ((--SoundDeviceMacOSAbstract::nInstPollTimer) == 0) {
			RemoveEventLoopTimer(SoundDeviceMacOSAbstract::pollTimer);	
			dprintf("SoundDev: removed polling timer\n");		
		}
		_m(FlushNodes(this));
		sndcmd.cmd = flushCmd;
		err = MySndDoImmediate(_m(sndchannel), &sndcmd);
		Debug(if (err != noErr) {
			dprintf("SoundDev: error SndDoImmediate %d\n", err);
		})
		ListIterator<SoundBuffer> lst(buffers);
		while (!lst.End()) { lst.Next().Done();	}
		if (tracker) { tracker->OnStop(*this); }
	}
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Pause()
{
	if (!prepared) { return; }
	SndCommand sndcmd;
	sndcmd.cmd = pauseCmd;
	sndcmd.param1 = 0;
	sndcmd.param2 = 0;
	OSErr err = MySndDoImmediate(_m(sndchannel), &sndcmd);
	Debug(if (err != noErr) {
		dprintf("SoundDev: error SndDoImmediate %d\n", err);
	})
	paused = TRUE;
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Continue()
{
	if (!prepared) { return; }
	SndCommand sndcmd;
	sndcmd.cmd = resumeCmd;
	sndcmd.param1 = 0;
	sndcmd.param2 = 0;
	OSErr err = MySndDoImmediate(_m(sndchannel), &sndcmd);
	Debug(if (err != noErr) {
		dprintf("SoundDev: error SndDoImmediate %d\n", err);
	})
	paused = FALSE;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceMacOS::Position()
{
	if (!prepared) { return 0; }
	return (maxTrackSize - _m(wavedataplayed));
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::Seek(_DWORD POS)
{
	freeze = TRUE;
	if (POS % 2) { POS++; }
	_m(wavedataplayed) = maxTrackSize - POS;
	ListIterator<SoundBuffer> lst(buffers);
	while (!lst.End()) { lst.Next().Seek(POS); }
	_m(FlushNodes(this));
	for (_DWORD idx = 0; idx < 2; idx ++) {
		_m(snddbheader).dbhBufferPtr[idx]->dbFlags = dbBufferReady;
	}
	freeze = FALSE;
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::SetVolume(_DWORD VOL)
{
	_DWORD vol = (VOL * (kFullVolume - kNoVolume)) / (VOLUMEMAX - VOLUMEMIN);
	_DWORD volleft = MAX(_LONG(vol) + MIN(pan, 1l), 1l);
	_DWORD volright = MAX(_LONG(vol) - MAX(pan, 1l), 1l);
	if (prepared) {
		SndCommand sndcmd;
		sndcmd.cmd = volumeCmd;
		sndcmd.param1 = 0;
		sndcmd.param2 = volright + (volleft << 16);
		OSErr err = MySndDoImmediate(_m(sndchannel), &sndcmd);
		Debug(if (err != noErr) {
			dprintf("SoundDev: error SndDoImmediate %d\n", err);
		})
	}
	else {
		SetDefaultOutputVolume(volright + (volleft << 16));
	}
	volume = VOL;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceMacOS::GetVolume()
{
	long vol;
	if (prepared) {
		SndCommand sndcmd;
		sndcmd.cmd = getVolumeCmd;
		sndcmd.param1 = 0;
		sndcmd.param2 = (long)&vol;
		OSErr err = MySndDoImmediate(_m(sndchannel), &sndcmd);
		Debug(if (err != noErr) {
			dprintf("SoundDev: error SndDoImmediate %d\n", err);
		})
	}
	else {
		GetDefaultOutputVolume(&vol);
	}
	_DWORD volleft = (vol >> 16) & 0xFFFF;
	_DWORD volright = vol  & 0xFFFF;	
	vol = MAX(volleft, volright);
	pan = ((volright - volleft) * (VOLUMEMAX - VOLUMEMIN)) / (kFullVolume - kNoVolume);
	volume = (vol * (VOLUMEMAX - VOLUMEMIN)) /  (kFullVolume - kNoVolume);
	return volume;
}


//--------------------------------------------------------------------------
void SoundDeviceMacOS::SetPan(_LONG PAN)
{
	pan = PAN;
	SetVolume(volume);;
}


//--------------------------------------------------------------------------
_LONG SoundDeviceMacOS::GetPan()
{
	GetVolume();
	return pan;
}


//--------------------------------------------------------------------------
_BOOL SoundDeviceMacOS::QueueWaveData(_PTR SNDPTR)
{
	SndDoubleBufferPtr snddb = (SndDoubleBufferPtr)SNDPTR;
	if (!playing) {
		snddb->dbFlags = dbBufferReady | dbLastBuffer;
		return FALSE; 
	};
	if (tracker) { tracker->OnQueue(*this); }
	if (!_m(wavedataplayed)) { 
		snddb->dbFlags = dbBufferReady | dbLastBuffer;
		playing = FALSE;
		ListIterator<SoundBuffer> lst(buffers);
		while (!lst.End()) { lst.Next().Done();	}
		if (tracker) { tracker->OnStop(*this); }
		Memory::Fill((_BYTE*)snddb->dbSoundData, 0, _m(wavebufsize));
		return FALSE; 
	}

	_DWORD size = _m(wavebufsize);
	if (_m(wavedataplayed) < size) {
		if (loop) {
			_m(wavedataplayed) = maxTrackSize;
		}
		else {
			size = _m(wavedataplayed); 
			_m(wavedataplayed) = 0;
		}
	}	
	else {
		_m(wavedataplayed) -= size;
	}
	//Debug(dprintf("queue %d - %d - %d\n", size, _m(wavedataplayed), maxTrackSize);)
	MixBuffers((_BYTE*)snddb->dbSoundData, size);
	snddb->dbFlags = dbBufferReady;
	return TRUE;
}


//--------------------------------------------------------------------------
_DWORD SoundDeviceMacOS::BufferSize() const
{
    if (playing) {
        return (_m(wavedataplayed) < _m(wavebufsize) ? _m(wavedataplayed) : _m(wavebufsize));
    }
	else {
		return _m(wavebufsize);
    }
}
#endif
