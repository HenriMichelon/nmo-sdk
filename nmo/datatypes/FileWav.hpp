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
#ifndef _NMO_DATATYPE_FILEWAV_H_
#define _NMO_DATATYPE_FILEWAV_H_

namespace NMO {

/*!
\brief		RIFF wave file.

			Only the fmt & data sub chunks are supported.
			Only uncompressed, little endian waves files are supported.

\author		(c) 2000-2002 SCS 361
\version	$Id: FileWav.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class FileWav: public NMOObject
{
public:
	/*! \brief Wav file format error.
			This mean that the file is not a RIFF WAV file
			or one subchunk is missing or bad */
	class ExcWavFormat: Stream::ExcStream { public: ExcWavFormat(); };

	/*! Open a WAV file.
		Raise ExcWavFormat on file error, ExcStream on I/O error.
		\param Stringz	: path & name of the file
		\param _BOOL	: load wav data in memory on open
	*/
	FileWav(const Stringz&, _BOOL = TRUE);

	/*! Open & load in memory a WAV file.
		Raise ExcWavFormat on file error, ExcStream on I/O error.
		\param Stream : stream to load the file from
		\param _BOOL	: load wav data in memory on open
	*/
	FileWav(Stream&, _BOOL = TRUE);

	/*! Create a new WAV file
		Raise ExcWavFormat on file error, ExcStream on I/O error.
		\param Stream : destination stream
		\param	_DWORD	: number of channels.
		\param	_DWORD	: number of samples per second.
		\param	_DWORD	: number of bits per sample.
		\param	_DWORD	: data size in bytes.
	*/
	FileWav(Stream&, _DWORD, _DWORD, _DWORD, _DWORD);

	/*! Create a new WAV file with an unknown size
		Raise ExcWavFormat on file error, ExcStream on I/O error.
		\param Stream : destination stream
		\param	_DWORD	: number of channels.
		\param	_DWORD	: number of samples per second.
		\param	_DWORD	: number of bits per sample.
		\param	_DWORD& : offset 1 to write the data size in bytes (add to a _DWORD in LE format - 8)
		\param	_DWORD& : offset 2 to write the data size in bytes (add to a _DWORD in LE format - 8)
	*/
	FileWav(Stream&, _DWORD, _DWORD, _DWORD, _DWORD&, _DWORD &);

	virtual ~FileWav();

	//! Number of channels
	_DWORD Channels() const;

	//! Number of samples per second
	_DWORD SamplesPerSec() const;

	//! Number of bits per sample
	_DWORD BitsPerSample() const;
	
	//! Return the attached stream
	Stream& DataStream();

	//! Size of the file header (depend on the file format)
	_DWORD HeaderSize();
	
	//! Return TRUE if data is read from memory or directly from the source stream
	_BOOL FromMemory() const;

	/*! Convert the samples to the desired format.
		Currently only converting mono to stereo is implemented
		\param	_DWORD	: number of channels
		\param	_DWORD	: samples per sec
		\param	_DWORD	: bits per sample
	*/
	_BOOL Convert(_DWORD, _DWORD, _DWORD);

private:
	_PTR mFileWavAbstract;
};

}

#endif
