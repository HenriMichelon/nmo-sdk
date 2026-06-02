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

$Id: IDisplay.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;


//--------------------------------------------------------------
IDisplay :: IDisplay(DisplayType T): dt(T)
{
	coltrans = IRGBColor(0, 0, 0);
	colpen = IRGBColor(0, 0, 0);
}


//--------------------------------------------------------------
IDisplay :: ~IDisplay() {}


//--------------------------------------------------------------
_BOOL IDisplay :: NativeDoubleBuffer() const
{
	return FALSE;
}

//--------------------------------------------------------------
void IDisplay :: DrawNoTransNoClip(const void *S, const IRect &R)
{
	VALIDATETHIS;
	ASSERT(S);
	DrawNoTransNoClip(S, R.left, R.top, R.width, R.height);
}


//--------------------------------------------------------------
void IDisplay :: DrawTransNoClip(const void *S, const IRect &R)
{
	VALIDATETHIS;
	ASSERT(S);
	DrawTransNoClip(S, R.left, R.top, R.width, R.height);
}


//--------------------------------------------------------------
void IDisplay :: DrawNoTransClip(const void *S, const IRect &R)
{
	VALIDATETHIS;
	ASSERT(S);
	DrawNoTransClip(S, R.left, R.top, R.width, R.height);
}


//--------------------------------------------------------------
void IDisplay :: DrawTransClip(const void *S, const IRect &R)
{
	VALIDATETHIS;
	ASSERT(S);
	DrawTransClip(S, R.left, R.top, R.width, R.height);
}


//--------------------------------------------------------------
void IDisplay :: Draw(const void *S, _LONG X, _LONG Y, _DWORD W, _DWORD H, _BOOL Trans, _BOOL Clip)
{
	VALIDATETHIS;
	ASSERT(S);
	if (Clip)
		if (Trans)
			DrawTransClip(S, X, Y, W, H);
		else
			DrawNoTransClip(S, X, Y, W, H);
	else
		if (Trans)
			DrawTransNoClip(S, X, Y, W, H);
		else
			DrawNoTransNoClip(S, X, Y, W, H);
}

