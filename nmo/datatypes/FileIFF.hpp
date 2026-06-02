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
#ifndef _NMO_DATATYPE_FILEIFF_H_
#define _NMO_DATATYPE_FILEIFF_H_

namespace NMO {

/*!
\brief		Generic support for IFF (Interchange File Format) compliants files.

\author		(c) 2000-2001 Henri Michelon 
\version	$Id: FileIFF.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

#define IFFID(ch0, ch1, ch2, ch3)  ((_DWORD)(_BYTE)(ch0) | ((_DWORD)(_BYTE)(ch1) << 8) | ((_DWORD)(_BYTE)(ch2) << 16) | ((_DWORD)(_BYTE)(ch3) << 24 ))

class FileIFF: public NMOObject
{
public:
	//! IFF Chunk/SubChunk ID
	typedef _DWORD ID;

	//! Create a empty IFF file. You must call Open() after.
	FileIFF();
	
	//! Open an IFF file. 
	FileIFF(Stream&);

	virtual ~FileIFF() {};

	//! Open an IFF file.
	void Open(Stream&);

	/*! Find a "main" chunck, generaly a FORM chunk.
		Raise ExcStream on I/O error.
		\param	_CHAR, _CHAR, _CHAR, _CHAR	: four chars describing the file content type
		\param	_CHAR, _CHAR, _CHAR, _CHAR	: four chars describing the chunk name 
		\return total size of the chunk, 0 on error
	*/
	_DWORD FindChunk(_CHAR, _CHAR, _CHAR, _CHAR, 
					_CHAR = 'F', _CHAR = 'O', _CHAR = 'R', _CHAR = 'M');

	/*! Find a subchunk. must be called AFTER FindChunk, or unexpected
		result will occurs. Raise ExcStream on I/O error.
		\param	_CHAR, _CHAR, _CHAR, _CHAR	: four chars describing the chunk name 
		\param	_BOOL	: load complete chunk in memory, or only name & size
		\return a pointer to the sub chunk or NULL on error. Must be freed with FreeSubChunk().
	*/
	_PTR FindSubChunk(_CHAR, _CHAR, _CHAR, _CHAR, _BOOL = TRUE);

	/*! Free a sub chunk block returned by FindSubChunk() */
	static void FreeSubChunk(_PTR);

private:
	_DWORD chunkpos;
	Stream *stream;
};

}

#endif
