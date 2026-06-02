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

	(c) 1998-2002 Henri Michelon

$Id: IDisplayX11.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "X11Win.hpp"
#include "IDisplayX11.hpp"

class AbstractX11
{
public:
	GC		gc;
	Window	window;
#ifdef USE_XLIB
	Pixmap		pixmap;
	Drawable	draw;
	XRectangle	clip_mask;
	int			depth;
	int			bitmap_pad;
	Visual		*visual;
	_LONG		clip_x;
	_LONG		clip_y;
	_BOOL		valid;
	unsigned int width;
	unsigned int height;
	unsigned long plane_mask;

	inline AbstractX11():
			clip_x(0),
			clip_y(0),
			valid(FALSE),
			width(0),
			height(0) {};
#else
	XImage	*ximage;

	inline AbstractX11():
			ximage(NULL) {}
#endif // USE_XLIB
};

#define _m(x) ((AbstractX11*)mAbstractX11)->x

// -------------------------------
IDisplayX11 :: IDisplayX11(DisplayType T,
						   IHandle WINDOW):
#ifdef USE_XLIB
	IDisplay(T)
#else
	IDisplayChunky(T)
#endif // USE_XLIB
{
	mAbstractX11 = new AbstractX11;
	_m(window) = IHANDLE(WINDOW);
#ifdef USE_XLIB
	_m(clip_mask).width =
	_m(clip_mask).height = 0;
	_m(draw) = _m(window);
	_m(visual) = DefaultVisual(X11Win::display,
							   X11Win::screen_num);

#endif // USE_XLIB
	// Setting the default colors for the GC
	XGCValues values;
	values.foreground = BlackPixel(X11Win::display, X11Win::screen_num);
	values.background = WhitePixel(X11Win::display, X11Win::screen_num);
	values.graphics_exposures = False;
	_m(gc) = XCreateGC(X11Win::display,
					   _m(window),
					   (GCForeground | GCBackground | GCGraphicsExposures),
					   &values);
}


// -------------------------------
IDisplayX11 :: ~IDisplayX11()
{
	DestroyBuffers();
	XFreeGC(X11Win::display, _m(gc));
	delete (AbstractX11*)mAbstractX11;
}


// -------------------------------
void IDisplayX11 :: Refresh()
{
#ifdef USE_XLIB
	if (_m(draw) == _m(pixmap)) {
		XCopyArea(X11Win::display,
				_m(pixmap),
				_m(window),
				_m(gc),
				0, 0,
				_m(width), _m(height),
				0, 0);
	}
#else
	XPutImage(X11Win::display,
			  _m(window),
			  _m(gc),
			  _m(ximage),
			  0, 0, 0, 0,
			  DispWidth,
			  DispHeight);
	XSync(X11Win::display, False);
#endif // USE_XLIB
}


// -------------------------------
void IDisplayX11 :: Refresh(const IRect&RECT)
{
#ifdef USE_XLIB
	if (_m(draw) == _m(pixmap)) {
		XCopyArea(X11Win::display,
				_m(pixmap),
				_m(window),
				_m(gc),
				RECT.left, RECT.top,
				RECT.width, RECT.height,
				RECT.left, RECT.top);
    }
#else
	IRect r;
	r.left = MAX(RECT.left, 0);
	r.top = MAX(RECT.top, 0);
	_LONG w = MAX(0, MIN(_LONG(RECT.left + RECT.width), _LONG(DispWidth)));
	_LONG h = MAX(0, MIN(_LONG(RECT.top + RECT.height), _LONG(DispHeight)));
	if ((w > r.left) && (h > r.top)) {
		r.width = w - r.left;
		r.height = h - r.top;
		XPutImage(X11Win::display,
				  _m(window),
				  _m(gc),
				  _m(ximage),
				  r.left, r.top,
				  r.left, r.top,
				  r.width, r.height);
	}
#endif // USE_XLIB
}


// -------------------------------
void IDisplayX11::DestroyBuffers()
{
#ifdef USE_XLIB
	if (_m(valid)) {
		if (_m(draw) == _m(pixmap)) {
			XFreePixmap(X11Win::display, _m(pixmap));
		}
		_m(valid) = FALSE;
	}
#else
	if (DrawSurface != NULL) {
		_m(ximage)->data = NULL;
		XDestroyImage(_m(ximage));
		delete []DrawSurface;
		DrawSurface = NULL;
		_m(ximage) = NULL;
	}
#endif // USE_XLIB
}


// -------------------------------
_BOOL IDisplayX11::CreateBuffers()
{
	DestroyBuffers();
	unsigned int	width, height;
	unsigned int	BW, D;
	int				X, Y;
	Window			root;

	// Read the properties of the window
	XGetGeometry(X11Win::display,
				 _m(window),
				 &root,
				 &X, &Y,
				 &width, &height,
				 &BW, &D);

	// Use the root window image color properties
	XImage* wxi = XGetImage(X11Win::display,
							root,
							0, 0,
							width, height,
							0xFFFFFF,
							ZPixmap);
	if (!wxi) { return FALSE; }
	IColorSpace::colorSpace.SetBitsPerPixel(wxi->bits_per_pixel);
#ifdef USE_XLIB
	_m(width) = width;
	_m(height) = height;
	_m(depth) = wxi->depth;
	_m(bitmap_pad) = wxi->bitmap_pad;

	if (Type() == DISPLAY_DOUBLE) {
		// Allocating back buffer pixmap
		_m(pixmap) = XCreatePixmap(X11Win::display,
								_m(window),
								_m(width),
								_m(height),
								wxi->bits_per_pixel);
		_m(draw) = _m(pixmap);
	}
	else {
		_m(draw) = _m(window);
	}
#else
	// Computing & setting the chunky display size properties
	DispWidth = width;
	DispHeight = height;
	DispPitch = width * IColorSpace::colorSpace.BytesPerPixel();

	// Allocating DrawSurface in-memory plane
	DrawSurface = new _BYTE[DispPitch * DispHeight];
	if (!DrawSurface) return FALSE;
	Memory::Fill(DrawSurface, 0xFF, DispPitch * DispHeight);

	// Allocating X Image contaning the in-memory plane
	_m(ximage) = XCreateImage(X11Win::display,
							  DefaultVisual(X11Win::display, X11Win::screen_num),
							  wxi->depth,
							  ZPixmap,
							  0,
							  (char*)DrawSurface,
							  DispWidth, DispHeight,
							  wxi->bitmap_pad,
							  DispPitch);
	if (!_m(ximage)) {
		delete []DrawSurface;
		DrawSurface = NULL;
		return FALSE;
	}
#endif // USE_XLIB

	if (IColorSpace::colorSpace.BitsPerPixel()>8) {
		// Setting the color space properties
		IColorSpace::colorSpace.SetColorMasks(wxi->red_mask,
											  wxi->green_mask,
											  wxi->blue_mask);
	}
	else {
		// Reading current color palette
		XColor 		color;
		_DWORD		ncells = DisplayCells(X11Win::display, X11Win::screen_num);
		IPalette	&pal = IColorSpace::colorSpace.Palette();
		Colormap	cmap = DefaultColormap(X11Win::display, X11Win::screen_num);
		pal.Reset(ncells);
		color.flags = DoRed | DoGreen | DoBlue;
		for (_DWORD idx = 0; idx < ncells; ++idx) {
			color.pixel = idx;
			XQueryColor(X11Win::display, cmap, &color);
			pal.Set(idx,
					color.red >> 8,
					color.green >> 8,
					color.blue >> 8);
		}
	}

#ifdef USE_XLIB
    XSetForeground(X11Win::display,
				  _m(gc),
				  WhitePixel(X11Win::display, X11Win::screen_num));
	XFillRectangle(X11Win::display,
				  _m(draw),
				  _m(gc),
				  0, 0,
				  _m(width), _m(height));
	SetPenColor(PenColor());
	SetTransColor(TransColor());
#endif // USE_XLIB
	XDestroyImage(wxi);
	SetClip(0, 0, width, height);
	Debug(dprintf("IDisplay: %dx%dx%d\n", width, height,
			IColorSpace::colorSpace.BitsPerPixel());)
	return TRUE;
}


#ifdef USE_XLIB
// -------------------------------
void IDisplayX11 :: SetClip(_LONG X, _LONG Y, _DWORD WIDTH, _DWORD HEIGHT)
{
	_m(clip_x) = X;
	_m(clip_y) = Y;
	_m(clip_mask).width = WIDTH;
	_m(clip_mask).height = HEIGHT;
	XSetClipRectangles(X11Win::display,
					   _m(gc),
					   X, Y,
					   &_m(clip_mask),
					   1,
					   Unsorted);
}


// -------------------------------
void IDisplayX11 :: GetClip(_LONG& X, _LONG& Y, _DWORD& WIDTH, _DWORD& HEIGHT)
{
	X = _m(clip_x);
	Y = _m(clip_y);
	WIDTH = _m(clip_mask).width;
	HEIGHT = _m(clip_mask).height;
}


// -------------------------------
void IDisplayX11 :: SetPenColor(const IRGBColor&RGB)
{
	IDisplay::SetPenColor(RGB);
	XColor col;
	col.red = RGB.red << 8;
	col.green = RGB.green << 8;
	col.blue = RGB.blue << 8;
	col.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(X11Win::display,
				DefaultColormap(X11Win::display, X11Win::screen_num),
				&col);
    XSetForeground(X11Win::display,
				  _m(gc),
				  col.pixel);
}


// -------------------------------
void IDisplayX11 :: SetTransColor(const IRGBColor&RGB)
{
	IDisplay::SetTransColor(RGB);
}


// -------------------------------
void IDisplayX11 :: DrawPixel (_LONG X, _LONG Y)
{
	XDrawPoint(X11Win::display,
			   _m(draw),
			   _m(gc),
			   X,
			   Y);
}


// -------------------------------
void IDisplayX11 :: GetPixel (_LONG, _LONG, IRGBColor&)
{
	ASSERTMSG(FALSE, "GetPixel: Unimplemented");
}


// -------------------------------
void IDisplayX11 :: DrawLine (_LONG X1, _LONG Y1, _LONG X2, _LONG Y2)
{
	XDrawLine(X11Win::display,
			  _m(draw),
			  _m(gc),
			  X1,
			  Y1,
			  X2,
			  Y2);
}


// -------------------------------
void IDisplayX11 :: DrawHLine (_LONG X, _LONG Y, _DWORD W)
{
	XDrawLine(X11Win::display,
			  _m(draw),
			  _m(gc),
			  X,
			  Y,
			  X + W,
			  Y);
}


// -------------------------------
void IDisplayX11 :: DrawRect(_LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	XFillRectangle(X11Win::display,
				_m(draw),
				_m(gc),
				X,
				Y,
				W,
				H);
}


// -------------------------------
void IDisplayX11 :: DrawCircle (_LONG X, _LONG Y, _DWORD WIDTH, _BOOL FILL, _DWORD STEP)
{
	if (FILL) {
		XFillArc(X11Win::display,
				_m(draw),
				_m(gc),
				X - WIDTH,
				Y - WIDTH,
				WIDTH * 2,
				WIDTH * 2,
				0,
				360 * 64);
	}
	else  {
		XDrawArc(X11Win::display,
				_m(draw),
				_m(gc),
				X - WIDTH,
				Y - WIDTH,
				WIDTH * 2,
				WIDTH * 2,
				0,
				360 * 64);
	}
}


// -------------------------------
_BYTE * IDisplayX11 :: GetRect(_LONG &X,
							   _LONG &Y,
							   _DWORD&WIDTH,
							   _DWORD&HEIGHT)
{
	_BYTE *result = NULL;
	XImage *image = XGetImage(X11Win::display,
							  _m(draw),
							  X, Y,
							  WIDTH, HEIGHT,
							  AllPlanes,
							  ZPixmap);
	if (image) {
		_DWORD size = image->height * image->bytes_per_line;
		result = new _BYTE[size];
		if (result) {
			WIDTH = image->width;
			HEIGHT = image->height;
			_DWORD real_width = image->bytes_per_line / (image->depth / 8);
			if (WIDTH != real_width) {
				_DWORD dst_pitch = image->width * (image->depth / 8);
				for (_DWORD idx_y = 0; idx_y < HEIGHT; ++idx_y) {
					Memory::Copy(result + idx_y * dst_pitch,
								image->data + idx_y * image->bytes_per_line,
								dst_pitch);
				}
			}
			else {
				Memory::Copy(result,
						 	image->data,
						 	size);
			}
		}
		XDestroyImage(image);
	}
	return result;
}


// -------------------------------
void IDisplayX11 :: DrawNoTransNoClip(const void *PTR,
								    _LONG X,
									_LONG Y,
									_DWORD WIDTH,
									_DWORD HEIGHT)
{
	ASSERT(PTR);
	XImage *image = XCreateImage(X11Win::display,
						        _m(visual),
						  		_m(depth),
						  		ZPixmap,
						  		0,
						  		(char*)PTR,
						  		WIDTH,
						  		HEIGHT,
						  		_m(bitmap_pad),
						  		WIDTH * IColorSpace::colorSpace.BytesPerPixel());
	if (image) {
		XRectangle rect = { 0, 0, _m(width), _m(height) };
		XSetClipRectangles(X11Win::display,
						   _m(gc),
						   0, 0,
						   &rect,
						   1,
						   Unsorted);
		XPutImage(X11Win::display,
				  _m(draw),
				  _m(gc),
				  image,
				  0, 0,
				  X, Y,
				  WIDTH, HEIGHT);
		image->data = 0;
		XDestroyImage(image);
		XSetClipRectangles(X11Win::display,
						   _m(gc),
						   _m(clip_x), _m(clip_y),
						   &_m(clip_mask),
						   1,
						   Unsorted);
	}
}


// -------------------------------
void IDisplayX11 :: DrawTransNoClip(const void *PTR,
								    _LONG X,
									_LONG Y,
									_DWORD WIDTH,
									_DWORD HEIGHT)
{
	ASSERT(PTR);
	XImage *image = XCreateImage(X11Win::display,
						        _m(visual),
						  		_m(depth),
						  		ZPixmap,
						  		0,
						  		(char*)PTR,
						  		WIDTH,
						  		HEIGHT,
						  		_m(bitmap_pad),
						  		WIDTH * IColorSpace::colorSpace.BytesPerPixel());
	if (image) {
		XRectangle rect = { 0, 0, _m(width), _m(height) };
		XSetPlaneMask(X11Win::display,
					_m(gc),
					_m(plane_mask));
		XSetClipRectangles(X11Win::display,
						   _m(gc),
						   0, 0,
						   &rect,
						   1,
						   Unsorted);
		XSetFunction(X11Win::display,
					 _m(gc),
					 GXand);
		XPutImage(X11Win::display,
				  _m(draw),
				  _m(gc),
				  image,
				  0, 0,
				  X, Y,
				  WIDTH, HEIGHT);
		image->data = 0;
		XDestroyImage(image);
		XSetFunction(X11Win::display,
					 _m(gc),
					 GXcopy);
		XSetClipRectangles(X11Win::display,
						   _m(gc),
						   _m(clip_x), _m(clip_y),
						   &_m(clip_mask),
						   1,
						   Unsorted);
	}
}


// -------------------------------
void IDisplayX11 :: DrawNoTransClip(const void *PTR,
								    _LONG X,
									_LONG Y,
									_DWORD WIDTH,
									_DWORD HEIGHT)
{
	ASSERT(PTR);
	XImage *image = XCreateImage(X11Win::display,
						        _m(visual),
						  		_m(depth),
						  		ZPixmap,
						  		0,
						  		(char*)PTR,
						  		WIDTH,
						  		HEIGHT,
						  		_m(bitmap_pad),
						  		WIDTH * IColorSpace::colorSpace.BytesPerPixel());
	if (image) {
		XPutImage(X11Win::display,
				  _m(draw),
				  _m(gc),
				  image,
				  0, 0,
				  X, Y,
				  WIDTH, HEIGHT);
		image->data = 0;
		XDestroyImage(image);
	}
}


// -------------------------------
void IDisplayX11 :: DrawTransClip(const void *PTR,
								    _LONG X,
									_LONG Y,
									_DWORD WIDTH,
									_DWORD HEIGHT)
{
	ASSERT(PTR);
	XImage *image = XCreateImage(X11Win::display,
						        _m(visual),
						  		_m(depth),
						  		ZPixmap,
						  		0,
						  		(char*)PTR,
						  		WIDTH,
						  		HEIGHT,
						  		_m(bitmap_pad),
						  		WIDTH * IColorSpace::colorSpace.BytesPerPixel());
	if (image) {
		XSetFunction(X11Win::display,
					 _m(gc),
					 GXand);
		XPutImage(X11Win::display,
				  _m(draw),
				  _m(gc),
				  image,
				  0, 0,
				  X, Y,
				  WIDTH, HEIGHT);
		image->data = 0;
		XDestroyImage(image);
		XSetFunction(X11Win::display,
					 _m(gc),
					 GXcopy);
	}
}


// -------------------------------
_DWORD IDisplayX11 :: Height() const
{
	return _m(height);
}


// -------------------------------
_DWORD IDisplayX11 :: Width() const
{
	return _m(width);
}


// -------------------------------
void IDisplayX11 :: Clear ()
{
	XFillRectangle(X11Win::display,
				  _m(draw),
				  _m(gc),
				  0, 0,
				  _m(width), _m(height));
}

#endif // USE_XLIB


#endif
