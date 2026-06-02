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
#ifndef _NMO_FILESTREAM_H_
#define _NMO_FILESTREAM_H_

namespace NMO {
/*!
\brief		File based Stream 

\author		(c) 2000-2001 Henri Michelon 	
\version	$Id: FileStream.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/

class FileStream: public Stream
{
public:
	/*! Open a file by is name.
	  	can raise ExcStreamCreate. */
	FileStream(const Stringz&, const File::FileMode = File::READ); 

	/*! Create a stream from an already opened file.
	  	can raise ExcStreamCreate. */
	FileStream(File*); 

	virtual ~FileStream();

	virtual _DWORD Read(_PTR, const _DWORD);
	virtual _DWORD Write(const _PTR, const _DWORD);
	virtual _DWORD Position() const;
	virtual void Seek(const _LONG, const SeekMode = START);
	virtual _BOOL End() const;
	virtual _DWORD Size();

	/*! Return the file object */
	File* FileObject() const;

Debug(public:void Invariant() const;)
private:
	_PTR mFileStreamAbstract;	
};

}

#endif
