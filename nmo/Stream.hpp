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
#ifndef _NMO_SSTREAM_H_
#define _NMO_SSTREAM_H_

namespace NMO {
/*!
\brief		Stream super class

\author		(c) 2000-2001 Henri Michelon 	
\version	$Id: Stream.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/

class Stream: public NMOObject
{
public:
	/*! Super class for all Stream exceptions */
	class ExcStream: public NMOException { public: ExcStream(const Stringz&); };
	/*! Stream opening and creation error */
	class ExcStreamCreate: public ExcStream { public: ExcStreamCreate(); };
	/*! Stream read error */
	class ExcStreamRead: public ExcStream { public: ExcStreamRead(); };
	/*! Stream write error */
	class ExcStreamWrite: public ExcStream { public: ExcStreamWrite(); };
	
	/*! Mode for Seek() */
	typedef enum
	{
		//! From start
		START,
		//! From end
		END,
		//! From current position
		CURRENT
	} SeekMode;

	virtual ~Stream() {};

	/*! Read bytes from the stream to a memory buffer.
	    can raise ExcStreamRead
		\param _PTR:	buffer address
		\param _DWORD:	number of bytes to read
		\return the number of bytes really read
	*/
	virtual _DWORD Read(_PTR, const _DWORD) = 0;

	/*! Write bytes from another stream to the stram.
	    can raise ExcStreamRead & ExcStreamWrite
		\param	Stream	: input stream
		\param	_DWORD	: number of bytes to write
		\return the number of bytes really written
	*/
	virtual _DWORD Write(Stream&, const _DWORD);

	/*! Write bytes from a memory buffer to the stream
	    can raise ExcStreamWrite
		\param _PTR:	buffer address
		\param _DWORD:	number of bytes to write
		\return the number of bytes really written
	*/
	virtual _DWORD Write(const _PTR, const _DWORD) = 0;

	/*! Return the current position (from the start, 0 based) */
	virtual _DWORD Position() const = 0;

	/*! Set the current position.
		\param	_DWORD : offset in bytes
		\param	SeekMode : offset start
	*/
	virtual void Seek(const _LONG, const SeekMode = START) = 0;

	/*! return TRUE if end of stream reached */
	virtual _BOOL End() const = 0;

	/*! Size of the stream in bytes */
	virtual _DWORD Size() = 0;

	/*! Copy entire the content of a stream into another.
	  	\param Stream	: stream to copy data TO.
	*/	
	void SaveToStream(Stream&);

	/*! Copy entire the content of a stream into another.
	  	\param Stream	: stream to copy data FROM.
	*/	
	void LoadFromStream(Stream&);
};

}

#endif
