/*                                ---------
                                  [NMO-SDK]
                                  ---------

		Implements a simple encapsulation of a DIB section and a DC.

		(c) 1998 Henri Michelon 

$Id: SimpleDIB.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------------------*/
#ifndef _DIB_SimpleDIB_H_
#define _DIB_SimpleDIB_H_

namespace NMO {

class SimpleDIB
{
	public:
		SIZE	sizeDIB;
		BITMAPINFOHEADER BIH;

		// Construction
		SimpleDIB();
		virtual ~SimpleDIB();
		// Destruction helper.
		virtual void Destroy(); 

		// Create a SimpleDIB object. Create can be called multiple times.
		BOOL CreateDIB(HDC, _DWORD, _DWORD, _BYTE );

		// Draw the DIB onto the destination DC. 
		void BlitBits(HDC pdcDest, _LONG x, _LONG y);
		// Draw the DIB onto the destination DC (up->down)
		void BitsToDevice(HDC pdcDest, _LONG x, _LONG y);
		// Draw the DIB onto the destination DC (up->down)
		void BitsToDevice(HDC pdcDest, _LONG x, _LONG y, 
						_LONG xsrc, _LONG ysrc, _DWORD wsrc, _DWORD hsrc);

		// Set the palette used in the DIB.
		BOOL SetPalette(HPALETTE pPal);

		inline HDC GetDC(void) { return m_pdc; }
		inline _BYTE *GetBits(void) { return m_pBits; }
		inline HBITMAP GetHandle(void) { return m_hbmp; }

	protected:
		HBITMAP	m_hbmOld;
		HDC		m_pdc;
		HBITMAP	m_hbmp;
		_BYTE	*m_pBits;		// Pointer to the bits.

};

}

#endif 
