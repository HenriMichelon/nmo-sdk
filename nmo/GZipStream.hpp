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
#ifndef _NMO_GZIPSTREAM_H_
#define _NMO_GZIPSTREAM_H_

namespace NMO {
/*!
\brief		Compressed (gziped) Stream

\author		(c) 2000-2001 Henri Michelon
\version	$Id: GZipStream.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/

class GZipStream: public FileStream
{
public:
	//! Compression algorithm stategy. See gzip documentation for details
	typedef enum {
		DEFAULT,
		FILTERED,
		HUFFMANONLY
	} GzipStrategy;
	
	/*! Open a file by is name.
	  	can raise ExcStreamCreate. 
	 \param Stringz		: file name
	 \param FileMode	: file opening mode
	 \param _BYTE		: compression level.
	 						from 0 (no compression) to 9 (max compression).
	 \param GZipStategy : compression algorithm strategy*/
	GZipStream(const Stringz&, 
			   const File::FileMode = File::READ,
			   _BYTE = 6, 
			   GzipStrategy = DEFAULT); 

	/*! Create a stream from an already opened file.
	  	can raise ExcStreamCreate. 
	 \param File		: file object.	
	 \param _BYTE		: compression level.
	 						from 0 (no compression) to 9 (max compression).
	 \param GZipStategy : compression algorithm strategy*/
	GZipStream(File*,
			   _BYTE = 6, 
			   GzipStrategy = DEFAULT);

	virtual ~GZipStream();

	virtual _DWORD Read(_PTR, const _DWORD);
	virtual _DWORD Write(const _PTR, const _DWORD);
	virtual _DWORD Position() const;
	virtual void Seek(const _LONG, const SeekMode = START);
	virtual _BOOL End() const;

private:
	_PTR gzhandle;
	void Open(_BYTE, GzipStrategy);

Debug(public:void Invariant() const;)
};

}

#endif
