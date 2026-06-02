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
#ifndef _SOUND_SOUNDDEVICE_H_
#define _SOUND_SOUNDDEVICE_H_

namespace NMO {

/*!
\brief		A sound device to play digital sounds.

  			The audio format used in tracks is the same
			as the Microsoft Pulse Code Modulation (PCM) format
			used in WAVE RIFF files.

			Currently supported format are:
				8-Bit Mono PCM
				8-Bit Stereo PCM
				16-Bit Mono PCM
				16-Bit Stereo PCM

			There is only one way to use a custom data format now:
			write your own SoundDevice derived class (or derived
			from NMO internal SoundDeviceXxx class) and rewrite
			the MixBuffers() method.
			There is plan to introduce a "SoundMixer" class in the futur.


\author		(c) 2000-2002 SCS 361
\version	$Id: SoundDevice.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class SoundDevice: public SoundObject
{
public:
	static const _DWORD	VOLUMEMIN;
	static const _DWORD	VOLUMEMAX;
	static const _LONG	PANMAX;
	
	/*! Return a list with the names of all the avaible drivers. */
	static void Drivers(List<Stringz>&);

	/*! Create a sound device by opening a specific sound driver
		\param Stringz	: name of the driver. Is name == "", then
						  the first driver of the driver list is selected
		\return NULL on error
	*/
	static SoundDevice *Create(const Stringz& = "") ;

	virtual ~SoundDevice();

	/*! Return a list with the names of all avaible devices */
	List<Stringz>& Devices() const;

	/*! Attach a tracker to the device.
		Pass NULL to disable device tracking. 
		\return old tracker of NULL*/
	SoundDeviceTracker* SetTracker(SoundDeviceTracker*);

	/*! Open & prepare device for playing
		\param	Stringz	: device name.Is name == "", then
						  the first device of the device list is selected
		\param	_DWORD	: number of channels.
		\param	_DWORD	: number of samples.
		\param	_DWORD	: number of bits per sample.
		\return FALSE if the device can't be opened or does not support
				selected format.
	*/
	virtual _BOOL Open(const Stringz&, _DWORD, _DWORD, _DWORD);

	/*! Return the mixing buffer length in milliseconds */
	_DWORD BufferLength() const;

	/*! Set the mixing buffer length in milliseconds.
		Stop and restart device to apply changes mades during play.
	*/
	void SetBufferLength(_DWORD);

	/*! Return the device wide mixing buffer size in bytes.
		Note: not all tracks have the same buffer size.*/
	virtual _DWORD BufferSize() const = 0;

	/*! Release all datas allocated by Open() */
	virtual void Close();

	/*! Add a wave track to mix & play.
		The total number of track isn't limited, but remember 
		that mixing all tracks requiere computing time.
		Track INS'T destroyed on track list clear & track droping.
		\param WaveTrack : track to add
	*/
	virtual void Add(WaveTrack&);

	/*! Add a wave track to mix & play.
		The total number of track isn't limited, but remember 
		that mixing all tracks requiere computing time.
		Track IS destroyed on track list clear & track droping.
		\param WaveTrack : track to add
	*/
	virtual void Add(WaveTrack*);

	/*! Replace a wave track
		\param _DWORD : track index, 1 based
		\param WaveTrack : new track */
	virtual void Replace(_DWORD, WaveTrack*);

	/*! Clear all tracks & buffers */
	virtual void Clear();

	/*! Play all the tracks simultaneously.
		\param _DWORD	: start position.
		\param _BOOL	: loop.
	*/
	virtual void Play(_DWORD = 0, _BOOL = FALSE) = 0;

	/*! Stop playing */
	virtual void Stop() = 0;

	/*! Pause playing */
	virtual void Pause() = 0;

	/*! Resume after a Pause() */
	virtual void Continue() = 0;

	/*! Set the master volume.
	  	Values goes from VOLUMEMIN to VOLUMEMAX.
		\param _DWORD : master volume for right and left audio channels
	*/
	virtual void SetVolume(_DWORD) = 0;

	/*! Return the current master volume 
	  	Values goes from VOLUMEMIN to VOLUMEMAX.
	*/
	virtual _DWORD GetVolume();

	/*! Set the relative volume between the left and right channels. 
	  	Values goes from -PANMAX (left only) to +PANMAX (right only)
		\param _DWORD : Relative volume between the left and right channels
	*/
	virtual void SetPan(_LONG) = 0;

	/*! Return the relative volume between the left and right audio channels. */
	virtual _LONG GetPan();

	//! Number of channels
	_DWORD Channels() const;

	//! Number of samples per second
	_DWORD SamplesPerSec() const;

	//! Number of bits per sample
	_DWORD BitsPerSample() const;

	/*! Return TRUE is the device has been opened with Open() */
	_BOOL IsOpen() const;

	/*! Return TRUE if the device is current playing sounds */
	virtual _BOOL IsPlaying();

	/*! Return TRUE if the device is current paused */
	virtual _BOOL IsPaused();

	/*! Set the current playing position (in bytes) */
	virtual void Seek(_DWORD) = 0;

	/*! Return the current playing position (in bytes) */
	virtual _DWORD Position() = 0;

	/*! Return the maximum size of all tracks in bytes. */
	_DWORD MaxTrackSize();

	/*! Return the list of sound buffers */
	List<SoundBuffer>& Tracks();

	/*! Return the name of the driver */
	const Stringz& DriverName() const;

	/*! Return the name of the device */
	Stringz Name();

	/* Return the maximum size of all tracks in hours,
		minutes and seconds */
	void TimeTotal(DateTime::Time&);

	/* Return the current position in hours,
		minutes and seconds */
	void TimePosition(DateTime::Time&);

	/*! Post preparation before Open().
		Normally called by Open() itself. */
	void Prepare();

	// must be public for static callbacks
	List<Stringz> devices;

protected:
	_BOOL	freeze;
	_DWORD	sampleduration;
	_DWORD	channels;
	_DWORD	samplespersec;
	_DWORD	bitspersample;
	_DWORD	volume;
	_LONG	pan;

	_BOOL	loop;
	_BOOL	paused;
	_BOOL	playing;
	_BOOL	prepared;
	_DWORD	maxTrackSize;
	List<SoundBuffer> buffers;
	SoundDeviceTracker *tracker;
	Stringz	drivername;

	_DWORD	deviceIndex;

	SoundDevice();
	virtual SoundBuffer* CreateBuffer();
	virtual void MixBuffers(_BYTE*, _DWORD);
};

inline _DWORD SoundDevice::Channels() const { return channels; }
inline _DWORD SoundDevice::SamplesPerSec() const { return samplespersec; }
inline _DWORD SoundDevice::BitsPerSample() const { return bitspersample; }
inline _BOOL SoundDevice::IsOpen() const { return prepared; }
inline _BOOL SoundDevice::IsPaused() { return paused; }
inline _BOOL SoundDevice::IsPlaying() { return playing; }
inline List<SoundBuffer>& SoundDevice::Tracks() { return (List<SoundBuffer>&)buffers; }
}

#endif
