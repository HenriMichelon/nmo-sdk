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
#ifndef _NMO_MEMORYSTREAM_H_
#define _NMO_MEMORYSTREAM_H_

namespace NMO {

/*!
\brief		Memory based Stream 

\author		(c) 2000-2001 Henri Michelon 	
\version	$Id: MemoryStream.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/

class MemoryStream: public Stream
{
public:
	/*! Create a new memory stream 
	  	can raise ExcStreamCreate.
		\param _DWORD	: size
	 	\param _DWORD	: increment size by ...
	*/	
	MemoryStream(_DWORD, _DWORD = 10*1024); 

	/*! Create a stream from existing memory block.
	  	can raise ExcStreamCreate. 
		The memory block isn't freed on Stream destruction.
		\param _PTR		: memory block address
		\param _DWORD	: size
	*/	
	MemoryStream(_PTR, _DWORD); 

	virtual ~MemoryStream();

	virtual _DWORD Read(_PTR, const _DWORD);
	virtual _DWORD Write(const _PTR, const _DWORD);
	virtual _DWORD Position() const;
	virtual void Seek(const _LONG, const SeekMode = START);
	virtual _BOOL End() const;
	virtual _DWORD Size();
	virtual void FreeOnDestruction(_BOOL);

	/*! Return the associated memory block address */
	_PTR Address() const;

	/*! Set the size of the stream to zero */
	void Clear();

protected:
	MemoryStream();
	void SetBlock(_PTR, _DWORD);
	void SetBlock(_DWORD, _DWORD);

Debug(public: void Invariant() const;)
private:
	_PTR mMemoryStreamAbstract;	
};

}

#endif
