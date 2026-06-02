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
#ifndef _SOUND_WAVETRACK_H_
#define _SOUND_WAVETRACK_H_

namespace NMO {
/*!
\brief		A waveform sound track to play in a SoundDevice

\author		(c) 2000-2001 SCS 361
\version	$Id: WaveTrack.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class WaveTrack: public Stream
{
public:
	/*! Create a wave track from a wave file.
		Note that the memory block with the wave datas
		isn't copied, thus the FileWave & the WaveTrack objects
		uses the same memory buffer
		\param	FileWav	: wav file to attach.
		\param	_BOOL	: loop when playing.
		\param	_DWORD	: Set the number of bytes to ignore before reading
	*/
	WaveTrack(FileWav &, _DWORD = 0, _BOOL = FALSE, _DWORD = 0);

	/*! Create a wave track from a wave file.
		Note that the memory block with the wave data
		isn't copied, thus the FileWave & the WaveTrack object
		use the same memory buffer.
		The FileWav object is destroyed on WaveTrack desctruction.
		\param	FileWav	: wav file to attach.
		\param	_BOOL	: loop when playing.
		\param	_DWORD	: Set the number of bytes to ignore before reading
	*/
	WaveTrack(FileWav *, _DWORD = 0, _BOOL = FALSE, _DWORD = 0);

	virtual ~WaveTrack();

	/* Set the number of bytes to ignore before really starting Read() */
	void SetStart(_DWORD);

	/* Set this to TRUE if you want this track to loop
		when playing */
	void SetLoop(_BOOL, _DWORD = 0);

	/* Return true if we want to loop this track when playing */
	_BOOL Loop() const;

	//! Number of channels
	_DWORD Channels() const;

	//! Number of samples per second
	_DWORD SamplesPerSec() const;

	//! Number of bits per sample
	_DWORD BitsPerSample() const;

	virtual _DWORD Read(_PTR, const _DWORD);
	virtual _DWORD Write(const _PTR, const _DWORD);
	virtual _DWORD Position() const;
	virtual void Seek(const _LONG, const SeekMode = START);
	virtual _BOOL End() const;
	virtual _DWORD Size();

private:
	_PTR mWaveTrackAbstract;	
};

}

#endif
