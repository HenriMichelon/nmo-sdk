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
#ifndef _SOUND_BUFFER_H_
#define _SOUND_BUFFER_H_

namespace NMO {
/*!
\brief		A waveform sound buffer to play in a SoundDevice

			Note: when mixing or modifying buffers, always
			query SoundBuffer::Size() instead of using the 
			SoundDevice default buffer size.

\author		(c) 2000-2001 SCS 361
\version	$Id: SoundBuffer.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class SoundBuffer: public SoundObject
{
public:
	/*! Attach a sound buffer to a wave track
		\param	WaveTrack	: wave track to read data from */
	void Attach(WaveTrack&);

	/*! Attach a sound buffer to a wave track
		\param	WaveTrack	: wave track to read data from.
				wave track is destroyed on buffer destruction
	*/
	void Attach(WaveTrack*);

	SoundBuffer();
	virtual ~SoundBuffer();

	/*! Restart buffer queueing 
	*/
	virtual _BOOL Reset(_DWORD = 0) = 0;

	/*! Release all resources allocated by Reset() for queueing */
	virtual void Done() = 0;

	/*! Return TRUE if the end of the track is reached */
	_BOOL End();

	/*! Return the current buffer and proceed the next buffer.
		Use Size() to query the size of this buffer.
		\param _DWORD : size of the buffer to read 
		\return a pointer to the sound buffer
	*/
	virtual _PTR Next(_DWORD) = 0;

	/*! Return the current buffer without proceeding the next buffer.
		Use Size() to query the size of this buffer.
		\param _DWORD : size of the buffer to read 
		\return a pointer to the sound buffer
	*/
	virtual _PTR Current() = 0;

	/*! Set the current read position .
		\param _DWORD	: position in bytes.
	*/
	virtual void Seek(_DWORD) = 0;

	/*! Return the size of the current buffer (after a call to Next())
		or the default buffer size (after a call to Reset()) */
	_DWORD Size() const;

	WaveTrack& Track();

protected:
	WaveTrack	*track;
	_DWORD		size;
	_DWORD		currentsize;

private:
	_BOOL		destroyTrack;
};

inline WaveTrack& SoundBuffer::Track() { return *track; };
}

#endif
