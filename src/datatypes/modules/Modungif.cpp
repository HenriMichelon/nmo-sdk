/* Modungif.cpp generated on Tuesday 30 August 2000, 00:00:31 by genmod
	(c)2000 Henri Michelon
*/

#include <nmo/NMO.hpp>
#include "Modungif.hpp"
using namespace NMO;

Modungif* Modungif::Create()
{
	_BOOL result;
	Modungif *ungif = new Modungif;
	result = ungif->Load("ungif");
	if (result) {
		result &= (ungif->GifLastError = (FGifLastError)
				ungif->GetMethodAddress("GifLastError")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method GifLastError in dynamic module ungif\n"); })
		result &= (ungif->DGifOpenFileName = (FDGifOpenFileName)
				ungif->GetMethodAddress("DGifOpenFileName")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifOpenFileName in dynamic module ungif\n"); })
		result &= (ungif->DGifOpenFileHandle = (FDGifOpenFileHandle)
				ungif->GetMethodAddress("DGifOpenFileHandle")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifOpenFileHandle in dynamic module ungif\n"); })
		/*result &= (ungif->DGifOpen = (FDGifOpen)
				ungif->GetMethodAddress("DGifOpen")) != NULL;*/
		Debug(if (!result) { dprintf("Can't get address of method DGifOpen in dynamic module ungif\n"); })
		result &= (ungif->DGifSlurp = (FDGifSlurp)
				ungif->GetMethodAddress("DGifSlurp")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifSlurp in dynamic module ungif\n"); })
		result &= (ungif->DGifGetScreenDesc = (FDGifGetScreenDesc)
				ungif->GetMethodAddress("DGifGetScreenDesc")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifGetScreenDesc in dynamic module ungif\n"); })
		result &= (ungif->DGifGetRecordType = (FDGifGetRecordType)
				ungif->GetMethodAddress("DGifGetRecordType")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifGetRecordType in dynamic module ungif\n"); })
		result &= (ungif->DGifGetImageDesc = (FDGifGetImageDesc)
				ungif->GetMethodAddress("DGifGetImageDesc")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifGetImageDesc in dynamic module ungif\n"); })
		result &= (ungif->DGifGetLine = (FDGifGetLine)
				ungif->GetMethodAddress("DGifGetLine")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifGetLine in dynamic module ungif\n"); })
		result &= (ungif->DGifGetPixel = (FDGifGetPixel)
				ungif->GetMethodAddress("DGifGetPixel")) != NULL;
		Debug(if (!result) { dprintf("Can't get address of method DGifGetPixel in dynamic module ungif\n"); })
	}
	if (!result) {
		//dprintf("Error loading dynamic module ungif\n");
		delete ungif;
		return NULL;
	}
	dprintf("Dynamic module ungif loaded\n");
	return ungif;
}
