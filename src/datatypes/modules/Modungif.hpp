/* Modungif.hpp generated on Tuesday 30 August 2000, 00:00:31 by genmod
	(c)2000 Henri Michelon
*/
#ifndef _GENMOD_MODModungif
#define _GENMOD_MODModungif

#if defined(HAVE_LIBUNGIF) && ! defined(_WIN32)
extern "C" {
#include <gif_lib.h>
}
#else
#include "../giflib/gif_lib.h"
#endif

namespace NMO {

class Modungif: public DynModule
{
public:
	static Modungif* Create();
	typedef int  (*FGifLastError)(void);
	typedef GifFileType * (*FDGifOpenFileName)(const char *GifFileName);
	typedef GifFileType * (*FDGifOpenFileHandle)(int GifFileHandle);
	//typedef GifFileType * (*FDGifOpen)( void* userPtr, InputFunc readFunc );  /* new one (TVT) */
	typedef int  (*FDGifSlurp)(GifFileType *GifFile);
	typedef int  (*FDGifGetScreenDesc)(GifFileType *GifFile);
	typedef int  (*FDGifGetRecordType)(GifFileType *GifFile, GifRecordType *GifType);
	typedef int  (*FDGifGetImageDesc)(GifFileType *GifFile);
	typedef int  (*FDGifGetLine)(GifFileType *GifFile, GifPixelType *GifLine, int GifLineLen);
	typedef int  (*FDGifGetPixel)(GifFileType *GifFile, GifPixelType GifPixel);

	FGifLastError	GifLastError;
	FDGifOpenFileName	DGifOpenFileName;
	FDGifOpenFileHandle	DGifOpenFileHandle;
	//FDGifOpen	DGifOpen;
	FDGifSlurp	DGifSlurp;
	FDGifGetScreenDesc	DGifGetScreenDesc;
	FDGifGetRecordType	DGifGetRecordType;
	FDGifGetImageDesc	DGifGetImageDesc;
	FDGifGetLine	DGifGetLine;
	FDGifGetPixel	DGifGetPixel;
};

} // namespace NMO

#endif
