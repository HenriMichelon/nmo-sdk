/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _DATATYPE_FILEPIXMAP_H_
#define _DATATYPE_FILEPIXMAP_H_

namespace NMO {

/*!
\brief		Graphics files support by file name extension

\author		(c) 2000-2001 Henri Michelon 
\version	$Id: FilePixmap.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class FilePixmap: public NMOObject
{
public:
	/*! Load a pixmap from a stream
		\param	Stream		: file name
		\param	DynModule	: module containing extra calls to load the file
		\return NULL on read error
	*/
	virtual IPixmap *Load(Stream&, DynModule*) = 0;
};


}

#endif
