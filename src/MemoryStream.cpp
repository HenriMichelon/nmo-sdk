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

$Id: MemoryStream.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>


class MemoryStreamAbstract
{
private:
	friend class MemoryStream;
	_BOOL owned;
	_BYTE *address;
	_DWORD fakesize;
	_DWORD size;
	_DWORD pos;
	_DWORD increment;
};

#define _m(x) ((MemoryStreamAbstract*)(this->mMemoryStreamAbstract))->x

//-------------------------------------------------------------------
MemoryStream::MemoryStream() { 
	mMemoryStreamAbstract = new MemoryStreamAbstract;
	SetBlock((_PTR)0, 0); 
}


//-------------------------------------------------------------------
void MemoryStream::SetBlock(_PTR ADDR, _DWORD SIZE)
{
	_m(address) = (_BYTE*)ADDR;
	_m(owned) = FALSE;
	_m(fakesize) = SIZE;
	_m(size) = SIZE;
	_m(pos) = 0;
	_m(increment) = 0;
}


//-------------------------------------------------------------------
void MemoryStream::SetBlock(_DWORD SIZE, _DWORD INCR)
{
	ASSERT(SIZE);
	_m(address) = new _BYTE[SIZE];
	_m(owned) = TRUE;
	_m(fakesize) = 0;
	_m(size)= SIZE;
	_m(pos) = 0;
	_m(increment) = INCR;
}


//-------------------------------------------------------------------
MemoryStream::MemoryStream(_DWORD SIZE, _DWORD INCR)
{
	mMemoryStreamAbstract = new MemoryStreamAbstract;
	SetBlock(SIZE, INCR);
}


//-------------------------------------------------------------------
MemoryStream::MemoryStream(_PTR ADDR, _DWORD SIZE)
{
	mMemoryStreamAbstract = new MemoryStreamAbstract;
	SetBlock(ADDR, SIZE);
}


//-------------------------------------------------------------------
MemoryStream::~MemoryStream()
{
	if (_m(owned)) { 
		delete []_m(address); 
	}
	delete (MemoryStreamAbstract*)mMemoryStreamAbstract;
}


//-------------------------------------------------------------------
_DWORD MemoryStream::Read(_PTR BUFF, const _DWORD SIZE)
{
	_DWORD nb = SIZE;
	if ((nb + _m(pos)) > _m(fakesize)) {
		nb = _m(fakesize) - _m(pos);
	}
	Memory::Copy(BUFF, _m(address) + _m(pos), nb);
	_m(pos) += nb;
	return nb;
}


//-------------------------------------------------------------------
_DWORD MemoryStream::Write(const _PTR BUFF, const _DWORD SIZE)
{
	_DWORD nb = SIZE;
	_LONG rest = (nb + _m(pos)) - _m(size);
	if (rest > 0) {
		if (_m(increment) > 0) {
			_DWORD nblocks = (rest / _m(increment)) + 1;
			_BYTE *tmp = new _BYTE[_m(size) + nblocks * _m(increment)];
			Memory::Copy(tmp, _m(address), _m(size));
			delete []_m(address);
			_m(size) += nblocks * _m(increment);
			_m(address) = tmp;
			_m(fakesize) += (nb + _m(pos)) - _m(fakesize);
		}
		else {
			nb = _m(size) - _m(pos);
		}
	}
	Memory::Copy(_m(address) + _m(pos), BUFF, nb);
	_m(pos) += nb;
	if (_m(pos) > _m(fakesize)) { 
		_m(fakesize) = _m(pos); 
	}
	return nb;
}


//-------------------------------------------------------------------
_DWORD MemoryStream::Position() const
{
	return _m(pos);
}


//-------------------------------------------------------------------
void MemoryStream::Seek(const _LONG POS, const SeekMode MODE)
{
	switch (MODE) {
	case START:
			  _m(pos) = POS;
			  break;
	case END:
			  _m(pos) = _m(fakesize) + POS;
			  break;
	case CURRENT:
			  _m(pos) = _m(pos) + POS;
			  break;
	}
}


//-------------------------------------------------------------------
_BOOL MemoryStream::End() const
{
	return (_m(pos) >= _m(fakesize));
}


//-------------------------------------------------------------------
_DWORD MemoryStream::Size()
{
	return _m(fakesize);
}


//-------------------------------------------------------------------
void MemoryStream::FreeOnDestruction(_BOOL F)
{
	_m(owned) = F;
}


//-------------------------------------------------------------------
_PTR MemoryStream::Address() const 
{
	return (_PTR)_m(address);
}


//-------------------------------------------------------------------
void MemoryStream::Clear()
{
	_m(fakesize) = 0;
	_m(pos) = 0;
}


#ifdef __DEBUG
//-------------------------------------------------------------------
void MemoryStream::Invariant() const 
{
	ASSERT(_m(address) != NULL);
	ASSERT(_m(size) > 0);
	ASSERT(_m(fakesize) > 0);
	ASSERT(_m(fakesize) <= _m(size));
	ASSERT(_m(pos) <= _m(fakesize));
}
#endif
