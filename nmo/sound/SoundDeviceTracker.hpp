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

*/
#ifndef _SOUND_SOUNDDEVICETRACKER_H_
#define _SOUND_SOUNDDEVICETRACKER_H_

namespace NMO {

class SoundDevice;

/*!
\brief		This class help you tracking the state of an audio device
			represented by the SoundDevice class.

 			With a tracker, your application can react when a device
			start or stop playing and can apply real-time effects
			to audio datas before the device mix them and send them to
 			the hardware.

 			For example, a FIR filter can be applied in the OnMix()
			event to emulate an equalizer.
 
			Only one tracker is attached to a device, but multiple
			devices can be tracked with one tracker.

\author		(c) 2000-2001 SCS 361
\version	$Id: SoundDeviceTracker.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class SoundDeviceTracker: public SoundObject
{
public:
	virtual ~SoundDeviceTracker() {};

	/*! This event is called during Play() just before the first sample
		is queued (all tracks & devices are prepared).
		\param SoundDevice	: event target device
	*/
	virtual void OnPlay(SoundDevice&) {};

	/*! This event is called during Stop() just after the playback
		have been stopped (before destructions of any buffer or data).
		\param SoundDevice	: event target device
	*/
	virtual void OnStop(SoundDevice&) {};

	/*! This event is called when a new sample is queued.
		Useful to update playing state/timing (before data is
	 	read from disk/memory).
		\param SoundDevice	: event target device
	*/
	virtual void OnQueue(SoundDevice&) {};

	/*! This event is called when a track sample is extracted, just
		before mixing with others tracks.  
		Useful to apply real-time filter effects.
		\param SoundDevice	: event target device.
		\param _DWORD		: track number.
		\param _PTR			: buffer with datat to mix.
		\param _DWORD		: size of the buffer with data to mix.
		\param _DWORD		: total number of tracks.
		\param _PTR			: destination buffer for the mix (optional use).
		\return TRUE if the destination buffer is managed by the function.
	*/
	virtual _BOOL OnMix(SoundDevice&, _DWORD, _PTR, _DWORD, _DWORD, _PTR) { return FALSE; };
};
}

#endif
