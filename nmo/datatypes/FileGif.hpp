/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _DATATYPE_FILEGIF_H_
#define _DATATYPE_FILEGIF_H_

namespace NMO {

/*!
\brief	Gif (Compuserve) graphics file
		
		This class is normally never used, since
		it is called by the DatatypePixmap class.

\author		(c) 1999-2001 Henri Michelon 
\version	$Id: FileGif.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class FileGif: public FilePixmap
{
public:
	IPixmap *Load(Stream&, DynModule*);
};


}

#endif

