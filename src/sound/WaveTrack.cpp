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

$Id: WaveTrack.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#include <nmo/NMO.hpp>
#include <nmo/Sound.hpp>
#include <nmo/sound/WaveTrack.hpp>
using namespace NMO;

class WaveTrackAbstract
{
private:
	friend class WaveTrack;
	
	FileWav	*wav;
	_DWORD	loopsize;
	_DWORD	loopstart;
	_LONG	startoffset;
	_LONG	startignored;
	_BOOL	loop;
	_BOOL	destroystream;
	_DWORD	startpos;
#ifdef _BIGENDIAN
	_BOOL	ontheflyconvert;
#endif

	void Convert();
};

#define _m(x) ((WaveTrackAbstract*)this->mWaveTrackAbstract)->x



//---------------------------------------------------------
WaveTrack::WaveTrack(FileWav &WAV, _DWORD STARTPOS, _BOOL LOOP, _DWORD LOOPSTART)
{
	mWaveTrackAbstract = new WaveTrackAbstract;
	_m(startignored) = 0;
	_m(wav) = &WAV;
	_m(destroystream) = FALSE;
	_m(Convert());
	_m(startoffset) = STARTPOS; 
	SetLoop(LOOP, LOOPSTART);
}


//---------------------------------------------------------
WaveTrack::WaveTrack(FileWav *WAV, _DWORD STARTPOS, _BOOL LOOP, _DWORD LOOPSTART)
{
	mWaveTrackAbstract = new WaveTrackAbstract;
	_m(startignored) = 0;
	_m(wav) = WAV;
	_m(destroystream) = TRUE;
	_m(Convert());
	_m(startoffset) = STARTPOS; 
	SetLoop(LOOP, LOOPSTART);
}


//---------------------------------------------------------
WaveTrack::~WaveTrack()
{
	if (_m(wav) && _m(destroystream)) { delete _m(wav); }
	delete (WaveTrackAbstract*)mWaveTrackAbstract;
}


//---------------------------------------------------------
void WaveTrackAbstract::Convert()
{
	ASSERT(wav);
	startpos = wav->HeaderSize();
#ifdef _BIGENDIAN
	if (wav->FromMemory() && (wav->BitsPerSample() == 16)) {
		MemoryStream &stream = (MemoryStream &)wav->DataStream();
		_INT *block = (_INT*)stream.Address();
		_DWORD size = stream.Size();
		while (size) {
			block[0] = _INT(LE2INT(block[0]));
			block++;
			size -= sizeof(_INT);
		}
	}
	ontheflyconvert = (!wav->FromMemory()) && (wav->BitsPerSample() == 16);
#endif
}


//---------------------------------------------------------
void WaveTrack::SetLoop(_BOOL LOOP, _DWORD LOOPSTART) 
{ 
	_m(loop) = LOOP; 
	_m(loopstart) = LOOPSTART;
	_m(loopsize) = (_m(wav)->DataStream().Size() - _m(loopstart));
}


//---------------------------------------------------------
void WaveTrack::Seek(const _LONG S, const SeekMode M) 
{ 
	if (M != (SeekMode) File::START) { return; }
	Stream &stream = _m(wav)->DataStream();
	_DWORD pos = S;
	if (_m(loop)) {
		if (S >= _m(startoffset)) {
			if (pos >= _m(loopstart)) {
				pos = (pos % _m(loopsize)) + _m(loopstart);
			}
			stream.Seek(pos + _m(startpos), Stream::START);
		}
		else {
			stream.Seek(_m(startpos), Stream::START);
		}
	}
	else {
		if (pos >= _m(loopsize)) {
			_m(startignored) = _m(loopsize);
			stream.Seek(0, Stream::END);
			return;
		}
		if (S >= _m(startoffset)) {
			stream.Seek(pos + _m(startpos) - _m(startoffset), Stream::START);
		}
		else {
			stream.Seek(_m(startpos), Stream::START);
		}
	}
	_m(startignored) = S;
}


//---------------------------------------------------------
_DWORD WaveTrack::Read(_PTR BUFFER, const _DWORD SIZE)
{
	Stream &stream = _m(wav)->DataStream();
	if ((!_m(loop)) && stream.End()) {
		return 0;
	}

	_DWORD msize = SIZE;
	if (_m(startignored) < _m(startoffset)) {
		_m(startignored) += msize;
		if (_m(startignored) > _m(startoffset)) {
			msize = _m(startignored) - _m(startoffset);
		}
		else {
			return 0;
		}
	}

	_DWORD res = stream.Read(BUFFER, msize);
	if (_m(loop)) {
		while (res < SIZE) {
			stream.Seek(_m(loopstart) + _m(startpos), Stream::START);
			_DWORD nres = stream.Read((_PTR)((_BYTE*)BUFFER + res),
									msize - res);
			if (nres > 0) {
				res += nres;
			}
			else {
				break;
			}
		}
	}
	_m(startignored) += res;

#ifdef _BIGENDIAN
	if (_m(ontheflyconvert)) {
		_INT *block = (_INT*)BUFFER;
		while (msize) {
			block[0] = _INT(LE2INT(block[0]));
			block++;
			msize -= sizeof(_INT);
		}
	}
#endif
	return res;
}


//---------------------------------------------------------
_DWORD WaveTrack::Write(const _PTR BUFFER, const _DWORD SIZE)
{
	return 0;
}


//---------------------------------------------------------
_BOOL WaveTrack::Loop() const { return _m(loop); }
_DWORD WaveTrack::Channels() const { return _m(wav)->Channels(); }
_DWORD WaveTrack::SamplesPerSec() const { return _m(wav)->SamplesPerSec(); }
_DWORD WaveTrack::BitsPerSample() const { return _m(wav)->BitsPerSample(); }
_DWORD WaveTrack::Position() const { return _m(wav)->DataStream().Position(); }
_BOOL WaveTrack::End() const { return _m(wav)->DataStream().End(); }
_DWORD WaveTrack::Size() { return _m(wav)->DataStream().Size(); }

