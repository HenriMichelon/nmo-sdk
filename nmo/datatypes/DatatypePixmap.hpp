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
#ifndef _DATATYPE_DATATYPEPIXMAP_H_
#define _DATATYPE_DATATYPEPIXMAP_H_

namespace NMO {

/*!
\brief		Manipulate files containing pixmaps.

			Type is determined by the filename extension.

\author		(c) 1999-2001 Henri Michelon 
\version	$Id: DatatypePixmap.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DatatypePixmap: public NMOObject
{
public:
	/*! Load a pixmap from a file.
		\param Stringz	: file name
		\return NULL on read error or unknown type.
	*/	 
	IPixmap *Load(const Stringz&);

	/*! Load a pixmap from a stream.
		\param Stringz	: file extension (file type: png, gif, ...)
		\param Stream	: stream to read data from
		\return NULL on read error or unknown type.
	*/	 
	IPixmap *Load(const Stringz&, Stream&);

	/*! Create a loader for pixmaps
		\param ResourcesTable	: optional resources file to use
	*/
	DatatypePixmap(ResourcesTable * = NULL);

	/*! Set the optional resource table to use
	*/
	void SetResources(ResourcesTable&);

	/*! Return the associated resource table.
		Can be NULL.
	*/
	ResourcesTable* Resources() const;

	virtual ~DatatypePixmap();

private:
	_PTR mDatatypePixmapAbstract;	
};

}

#endif
