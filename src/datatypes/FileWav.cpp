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

  TODO: manage word aligned boundary in subchunks

	(c) 2000-2002 SCS 361

$Id: FileWav.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Stream.hpp>
#include <nmo/MemoryStream.hpp>
#include <nmo/datatypes/FileIFF.hpp>
#include <nmo/datatypes/FileWav.hpp>
using namespace NMO;

class FileWavAbstract
{
private:
	friend class FileWav;
	Stream		*stream;
	_BOOL		frommemory;
	_BOOL		destroystream;
	_DWORD 		channels;
	_DWORD 		samplespersec;
	_DWORD 		bitspersample;
	_DWORD		headersize;
	_PTR   		data;

	void Open(Stream &, _BOOL);
	void Create(Stream&, _DWORD, _DWORD, _DWORD, _DWORD);
	void Create(Stream&, _DWORD, _DWORD, _DWORD, _DWORD&, _DWORD&);
};

#define _m(x) ((FileWavAbstract*)(this->mFileWavAbstract))->x


typedef struct {
	FileIFF::ID chunkID;
	_DWORD	chunkSize;
	_INT	wFormatTag;
	_WORD	wChannels;
	_DWORD	dwSamplesPerSec;
	_DWORD	dwAvgBytesPerSec;
	_WORD	wBlockAlign;
	_WORD	wBitsPerSample;
	/* Note: there may be additional fields here, depending upon wFormatTag. */
} FormatChunk;


typedef struct {
	FileIFF::ID chunkID;
	_DWORD	chunkSize;
} DataChunk;


typedef struct {
	FileIFF::ID chunkID;
	_DWORD	chunkSize;
	FileIFF::ID wavID;
} MainChunk;


//////////////////////////////////////////////////////////////////////
void FileWavAbstract::Open(Stream &STREAM, _BOOL LOADDATA)
{
	data = NULL;
	stream = NULL;
	frommemory = LOADDATA;
	FileIFF iff(STREAM);
	_DWORD size = iff.FindChunk('W', 'A', 'V', 'E', 'R', 'I', 'F', 'F');
	if (size) {
		FormatChunk *fmt = (FormatChunk*)iff.FindSubChunk('f', 'm', 't', ' ');
		if (fmt) {
			data = iff.FindSubChunk('d', 'a', 't', 'a', LOADDATA);
			DataChunk *dat = (DataChunk*)data;
			if (dat) { 
				if (LE2INT(fmt->wFormatTag) == 1) {
					channels = LE2WORD(fmt->wChannels);
					samplespersec = LE2DWORD(fmt->dwSamplesPerSec);
					bitspersample = LE2WORD(fmt->wBitsPerSample);
					if (frommemory) {
						stream = new MemoryStream(((_BYTE*)dat) + sizeof(DataChunk), 
									LE2LONG(dat->chunkSize) - sizeof(DataChunk));
						destroystream = TRUE;
						headersize = 0;
					}
					else {
						stream = &STREAM;
						headersize = stream->Position();
					}
					FileIFF::FreeSubChunk(fmt);
					return;
				}
				FileIFF::FreeSubChunk(data);
			}
			FileIFF::FreeSubChunk(fmt);
		}
	}
	throw FileWav::ExcWavFormat();
}


//////////////////////////////////////////////////////////////////////
_DWORD FileWav::HeaderSize()
{
	return _m(headersize);
}


//////////////////////////////////////////////////////////////////////
FileWav::ExcWavFormat::ExcWavFormat(): ExcStream("WavFormat") {};

//////////////////////////////////////////////////////////////////////
void FileWavAbstract::Create(Stream &STREAM, _DWORD NC, _DWORD NS, _DWORD NBPS, _DWORD SIZE)
{
	data = NULL;
	stream = NULL;
	destroystream = FALSE;
	MainChunk mainchunk;
	mainchunk.chunkID = IFFID('R', 'I', 'F', 'F');
	mainchunk.chunkID = DWORD2LE(mainchunk.chunkID);
	mainchunk.chunkSize = SIZE +
						sizeof(MainChunk) +
						sizeof(FormatChunk) +
						sizeof(DataChunk) - 8;
	mainchunk.chunkSize = DWORD2LE(mainchunk.chunkSize);
	mainchunk.wavID = IFFID('W', 'A', 'V', 'E');
	mainchunk.wavID = DWORD2LE(mainchunk.wavID);
	STREAM.Write(&mainchunk, sizeof(MainChunk));
	
	FormatChunk fmtchunk;
	fmtchunk.chunkID = IFFID('f', 'm', 't', ' ');
	fmtchunk.chunkID = DWORD2LE(fmtchunk.chunkID);
	fmtchunk.chunkSize = sizeof(FormatChunk) - 8;
	fmtchunk.chunkSize = DWORD2LE(fmtchunk.chunkSize);
	fmtchunk.wFormatTag = 1;
	fmtchunk.wFormatTag = INT2LE(fmtchunk.wFormatTag);
	fmtchunk.wChannels = (_WORD)NC;
	fmtchunk.wChannels = WORD2LE(fmtchunk.wChannels);
	fmtchunk.dwSamplesPerSec = DWORD2LE(NS);
	fmtchunk.wBlockAlign = (_WORD)(NC * NBPS) / 8;
	fmtchunk.dwAvgBytesPerSec = (fmtchunk.wBlockAlign * NS);
	fmtchunk.wBlockAlign = WORD2LE(fmtchunk.wBlockAlign);
	fmtchunk.dwAvgBytesPerSec = DWORD2LE(fmtchunk.dwAvgBytesPerSec);
	fmtchunk.wBitsPerSample = (_WORD)NBPS;
	fmtchunk.wBitsPerSample = WORD2LE(fmtchunk.wBitsPerSample);
	STREAM.Write(&fmtchunk, sizeof(FormatChunk));

	DataChunk datachunk;
	datachunk.chunkID = IFFID('d', 'a', 't', 'a');
	datachunk.chunkID = DWORD2LE(datachunk.chunkID);
	datachunk.chunkSize = SIZE - 8;
	datachunk.chunkSize = DWORD2LE(datachunk.chunkSize);
	STREAM.Write(&datachunk, sizeof(DataChunk));
}


//////////////////////////////////////////////////////////////////////
void FileWavAbstract::Create(Stream &STREAM, _DWORD NC, _DWORD NS, _DWORD NBPS, _DWORD &SIZE1, _DWORD &SIZE2)
{
	data = NULL;
	stream = NULL;
	destroystream = FALSE;
	MainChunk mainchunk;
	mainchunk.chunkID = IFFID('R', 'I', 'F', 'F');
	mainchunk.chunkID = DWORD2LE(mainchunk.chunkID);
	mainchunk.chunkSize = sizeof(MainChunk) +
						sizeof(FormatChunk) +
						sizeof(DataChunk);
	SIZE1 = STREAM.Position() + sizeof(mainchunk.chunkID);
	mainchunk.chunkSize = DWORD2LE(mainchunk.chunkSize);
	mainchunk.wavID = IFFID('W', 'A', 'V', 'E');
	mainchunk.wavID = DWORD2LE(mainchunk.wavID);
	STREAM.Write(&mainchunk, sizeof(MainChunk));
	
	FormatChunk fmtchunk;
	fmtchunk.chunkID = IFFID('f', 'm', 't', ' ');
	fmtchunk.chunkID = DWORD2LE(fmtchunk.chunkID);
	fmtchunk.chunkSize = sizeof(FormatChunk) - 8;
	fmtchunk.chunkSize = DWORD2LE(fmtchunk.chunkSize);
	fmtchunk.wFormatTag = 1;
	fmtchunk.wFormatTag = INT2LE(fmtchunk.wFormatTag);
	fmtchunk.wChannels = (_WORD)NC;
	fmtchunk.wChannels = WORD2LE(fmtchunk.wChannels);
	fmtchunk.dwSamplesPerSec = DWORD2LE(NS);
	fmtchunk.wBlockAlign = (_WORD)(NC * NBPS) / 8;
	fmtchunk.dwAvgBytesPerSec = (fmtchunk.wBlockAlign * NS);
	fmtchunk.wBlockAlign = WORD2LE(fmtchunk.wBlockAlign);
	fmtchunk.dwAvgBytesPerSec = DWORD2LE(fmtchunk.dwAvgBytesPerSec);
	fmtchunk.wBitsPerSample = (_WORD)NBPS;
	fmtchunk.wBitsPerSample = WORD2LE(fmtchunk.wBitsPerSample);
	STREAM.Write(&fmtchunk, sizeof(FormatChunk));

	DataChunk datachunk;
	datachunk.chunkID = IFFID('d', 'a', 't', 'a');
	datachunk.chunkID = DWORD2LE(datachunk.chunkID);
	datachunk.chunkSize = 0;
	datachunk.chunkSize = DWORD2LE(datachunk.chunkSize);
	SIZE2 = STREAM.Position() + sizeof(datachunk.chunkID);
	STREAM.Write(&datachunk, sizeof(DataChunk));
}


//////////////////////////////////////////////////////////////////////
FileWav::FileWav(const Stringz&WaveName, _BOOL LOADATA)
{
	mFileWavAbstract = new FileWavAbstract;
	FileStream *file = new FileStream(WaveName);
	_m(destroystream) = TRUE;
	_m(Open(*file, LOADATA));
	if (LOADATA) { delete file; }
}


//////////////////////////////////////////////////////////////////////
FileWav::FileWav(Stream &STREAM, _BOOL LOADATA)
{
	mFileWavAbstract = new FileWavAbstract;
	_m(destroystream) = FALSE;
	_m(Open(STREAM, LOADATA));
}


//////////////////////////////////////////////////////////////////////
FileWav::FileWav(Stream&STREAM, _DWORD NC, _DWORD NS, _DWORD NBPS, _DWORD S)
{
	mFileWavAbstract = new FileWavAbstract;
	_m(Create(STREAM, NC, NS, NBPS, S));
}


//////////////////////////////////////////////////////////////////////
FileWav::FileWav(Stream&STREAM, _DWORD NC, _DWORD NS, _DWORD NBPS, _DWORD &S1, _DWORD &S2)
{
	mFileWavAbstract = new FileWavAbstract;
	_m(Create(STREAM, NC, NS, NBPS, S1, S2));
}


//////////////////////////////////////////////////////////////////////
FileWav::~FileWav()
{
	if (_m(data)) { FileIFF::FreeSubChunk(_m(data)); }
	if (_m(destroystream) && _m(stream)) { delete _m(stream); }
	delete (FileWavAbstract*)mFileWavAbstract;
}


//////////////////////////////////////////////////////////////////////
_BOOL FileWav::Convert(_DWORD NC, _DWORD , _DWORD )
{	
	if (!_m(stream)) { return FALSE; }
	if ((_m(channels) == 1) && (NC == 2)) {
		_m(channels) = 2;
		MemoryStream *oldstream = (MemoryStream *)_m(stream);
		MemoryStream *newstream = new MemoryStream(oldstream->Size()*2);
		if (!newstream) { return FALSE; }
		oldstream->Seek(0);
		if (_m(bitspersample) == 8) {
			_BYTE tmp;
			while (!oldstream->End()) {
				oldstream->Read(&tmp, sizeof(_BYTE));
				newstream->Write(&tmp, sizeof(_BYTE));
				newstream->Write(&tmp, sizeof(_BYTE));
			}
		}
		else if (_m(bitspersample) == 16) {
			_INT tmp;
			while (!oldstream->End()) {
				oldstream->Read(&tmp, sizeof(_INT));
				newstream->Write(&tmp, sizeof(_INT));
				newstream->Write(&tmp, sizeof(_INT));
			}
		}
		if (_m(destroystream)) { delete oldstream; }
		if (_m(data)) { FileIFF::FreeSubChunk(_m(data)); }
		_m(data) = NULL;
		_m(stream) = newstream;
		_m(frommemory) = TRUE;
		_m(destroystream) = TRUE;
		_m(stream)->Seek(0);
		return TRUE;
	}
	return FALSE;
}

_DWORD FileWav::Channels() const { return _m(channels); }
_DWORD FileWav::SamplesPerSec() const { return _m(samplespersec); }
_DWORD FileWav::BitsPerSample() const { return _m(bitspersample); }
_BOOL FileWav::FromMemory() const { return _m(frommemory); }
Stream& FileWav::DataStream() { return (Stream&)*_m(stream); }

