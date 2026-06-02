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

$Id: SoundDeviceDsp.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _SOUND_SOUNDDEVICE_DSP_H_
#define _SOUND_SOUNDDEVICE_DSP_H_

namespace NMO {

class SoundDeviceDsp: public SoundDevice, protected ThThread
{
public:
	SoundDeviceDsp();
	virtual ~SoundDeviceDsp();

	virtual _BOOL Open(const Stringz&, _DWORD, _DWORD, _DWORD);
	virtual void Close();
	virtual void Play(_DWORD = 0, _BOOL = FALSE);
	virtual void Stop();
	virtual void Pause();
	virtual void Continue();
	virtual void SetVolume(_DWORD);
	virtual _DWORD GetVolume();
	virtual void SetPan(_LONG);
	virtual _LONG GetPan();
	virtual _DWORD Position();
	virtual _DWORD BufferSize() const;
	virtual void Seek(_DWORD);

private:
	int		dsp;
	_DWORD	WaveDataPlayed;
	_DWORD	WaveBufSize;
	_BYTE	*buffer[2];
	_DWORD	bufsize[2];
	_BYTE	readBuffer;

	void OnStart();
	void QueueWaveData();
	void WriteBuffer(_BYTE*, _DWORD);
};

}

#endif
