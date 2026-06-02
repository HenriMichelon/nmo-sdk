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

$Id: SoundBufferRaw.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _SOUND_BUFFERRAW_H_
#define _SOUND_BUFFERRAW_H_

namespace NMO {

class SoundBufferRaw: public SoundBuffer
{
public:
	SoundBufferRaw();
	virtual ~SoundBufferRaw();
	virtual _BOOL Reset(_DWORD = 0);
	virtual _PTR Next(_DWORD);
	virtual void Done();
	virtual _PTR Current();
	virtual void Seek(_DWORD);

private:
	_BYTE		*buffer;
};

}

#endif
