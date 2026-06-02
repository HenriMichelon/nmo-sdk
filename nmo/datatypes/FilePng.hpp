/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _DATATYPE_FILEPNG_H_
#define _DATATYPE_FILEPNG_H_

namespace NMO {

/*!
\brief	PNG (Portage Network Graphics) graphics file support
		
		This class is normally never used, since
		it is called by the DatatypePixmap class.

\author	(c) 1999-2001 Henri Michelon 
\version $Id: FilePng.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class FilePng: public FilePixmap
{
public:	
	IPixmap *Load(Stream&, DynModule*);
};


}

#endif

