/*                                ---------
                                  [NMO-SDK]
                                  ---------

		Implements a simple encapsulation of a DIB section and a DC.

$Id: SimpleDIB.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
#include "SimpleDIB.hpp"
using namespace NMO;

SimpleDIB :: SimpleDIB()
{
	sizeDIB.cx = sizeDIB.cy = 0;
	m_hbmp = NULL;
	m_pBits = NULL;
	m_pdc = NULL;
}

SimpleDIB :: ~SimpleDIB()
{
	Destroy();
}


void SimpleDIB :: Destroy()
{
    // Select the old bitmap back into the buffer DC.
    if (m_hbmOld)
        ::SelectObject(m_pdc, m_hbmOld);

	// Delete bitmap.
	if (m_hbmp)
	{
		::DeleteObject(m_hbmp);
		m_hbmp = NULL;
	}
}


BOOL SimpleDIB :: CreateDIB(HDC dc, _DWORD cx, _DWORD cy, _BYTE ibitcount )
{
   // Destroy parts of objects if we are recreating it.
   if ((m_pdc != NULL) || (m_hbmp != NULL)) Destroy() ;

   // Save size for drawing later.
   sizeDIB.cx = cx;
   sizeDIB.cy = cy;

   // Fill in the header info.
   int iSize = sizeof(BITMAPINFOHEADER) ;
   Memory::Fill(&BIH, 0, iSize);
   BIH.biSize = iSize;
   BIH.biWidth = cx;
   BIH.biHeight = cy;
   BIH.biPlanes = 1;
   BIH.biBitCount = ibitcount;
   BIH.biCompression = BI_RGB;

   // Create a new DC.
   m_pdc = ::CreateCompatibleDC(NULL);
   if (!m_pdc)
   {
	   //Debug(dprintf("SimpleDIB.CreateDIB: CreateCompatibleDC failed #%d\n", GetLastError());)
	   return FALSE;
   }

   // Create the DIB section.
   m_hbmp = ::CreateDIBSection(m_pdc,
                           	(BITMAPINFO*)&BIH,
                           	DIB_RGB_COLORS,
                           	(void**)&m_pBits,
                           	NULL,
                           	0);
   if ( (!m_hbmp) && (!m_pBits) )
   {
	   Debug(dprintf("SimpleDIB.CreateDIB: CreateDIBSection failed #%d\n", GetLastError());)
	   ASSERT(FALSE);
   }

    // Select the new bitmap into the buffer DC
    m_hbmOld = (HBITMAP) ::SelectObject(m_pdc, m_hbmp);
	if ( (!m_hbmOld) || (m_hbmOld == (HBITMAP)GDI_ERROR) )
   {
	   //Debug(dprintf("SimpleDIB.CreateDIB: SelectObject failed #%d\n", GetLastError());)
	   return FALSE;
   }

	return TRUE;
}

void SimpleDIB :: BlitBits(HDC pdcDest, _LONG x, _LONG y)
{
    ::BitBlt( pdcDest, x, y,
              sizeDIB.cx, sizeDIB.cy,
              m_pdc,
              0, 0,
              SRCCOPY);
}


void SimpleDIB :: BitsToDevice(HDC pdcDest, _LONG x, _LONG y)
{
	BIH.biHeight = -BIH.biHeight;
	::SetDIBitsToDevice(
				pdcDest,	//handle of device context
				x,		//x-coordinate of upper-left corner of dest. rect.
				y,		//y-coordinate of upper-left corner of dest. rect.
				sizeDIB.cx,		//source rectangle width
				sizeDIB.cy,		//source rectangle height
				0,		//x-coordinate of lower-left corner of source rect.
				0,		//y-coordinate of lower-left corner of source rect.
				0,		//first scan line in array
				sizeDIB.cy,		//number of scan lines
				m_pBits,				//address of array with DIB bits
				(LPBITMAPINFO)&BIH,	//address of structure with bitmap info.
				DIB_RGB_COLORS);		//RGB or palette indices
	BIH.biHeight = -BIH.biHeight;
}


void SimpleDIB :: BitsToDevice(HDC pdcDest, _LONG x, _LONG y,
							   _LONG xsrc, _LONG ysrc, _DWORD wsrc, _DWORD hsrc)
{
	BIH.biHeight = -BIH.biHeight;
	::SetDIBitsToDevice(
				pdcDest,	//handle of device context
				x,		//x-coordinate of upper-left corner of dest. rect.
				y,		//y-coordinate of upper-left corner of dest. rect.
				wsrc,		//source rectangle width
				hsrc,		//source rectangle height
				xsrc,		//x-coordinate of lower-left corner of source rect.
				sizeDIB.cy - (hsrc + ysrc),		//y-coordinate of lower-left corner of source rect.
				0,		//first scan line in array
				sizeDIB.cy,		//number of scan lines
				m_pBits,				//address of array with DIB bits
				(LPBITMAPINFO)&BIH,	//address of structure with bitmap info.
				DIB_RGB_COLORS);		//RGB or palette indices
	BIH.biHeight = -BIH.biHeight;
}


BOOL SimpleDIB :: SetPalette(HPALETTE pPal)
{
    // get the colors from the palette
    int iColors = 0;
    if (!::GetObject(pPal, sizeof(iColors), &iColors))
	{
		//Debug(dprintf("SimpleDIB.SetPalette: GetObject failed %d\n", GetLastError());)
		return FALSE;
	}

    PALETTEENTRY* pPE = new PALETTEENTRY[iColors];
    if (!::GetPaletteEntries(pPal, 0, iColors, pPE))
	{
		//Debug(dprintf("SimpleDIB.SetPalette: GetPaletteEntries failed %d\n", GetLastError());)
		return FALSE;
	}

    // Build a table of RGBQUADS
    RGBQUAD* pRGB = new RGBQUAD[iColors];
    for (int i = 0; i < iColors; i++) {
        pRGB[i].rgbRed = pPE[i].peRed;
        pRGB[i].rgbGreen = pPE[i].peGreen;
        pRGB[i].rgbBlue = pPE[i].peBlue;
        pRGB[i].rgbReserved = 0;
    }
    
    if (!::SetDIBColorTable(m_pdc,
                       0, iColors,
                       pRGB))
	{
		//Debug(dprintf("SimpleDIB.SetPalette: SetDIBColorTable failed %d\n", GetLastError());)
		return FALSE;
	}

    delete [] pRGB;
    delete [] pPE;

	return TRUE;
}
#endif