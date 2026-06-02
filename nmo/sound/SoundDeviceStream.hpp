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

$Id: SoundDeviceStream.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _SOUND_SOUNDDEVICE_STREAM_H_
#define _SOUND_SOUNDDEVICE_STREAM_H_

namespace NMO {

class SoundDeviceStream: public SoundDevice
{
public:
	SoundDeviceStream();
	virtual ~SoundDeviceStream();
	void SetStream(Stream&);

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
	_PTR mSoundDeviceStreamAbstract;
	_BOOL QueueWaveData(_BYTE*, _DWORD);
};

}

#endif
