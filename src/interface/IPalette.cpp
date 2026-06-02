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

	(c) 1998-2001 Henri Michelon 

$Id: IPalette.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
using namespace NMO;


//--------------------------------------------------------------
void IPalette::Set(_DWORD i, const IRGBColor&C)
{
	pal[i].red = C.red;
	pal[i].green = C.green;
	pal[i].blue = C.blue;
}



//--------------------------------------------------------------
void IPalette::Set(_DWORD i, _BYTE R, _BYTE G, _BYTE B)
{
	pal[i].red = R;
	pal[i].green = G;
	pal[i].blue = B;
}


//--------------------------------------------------------------
IPalette *IPalette ::Clone()
{
	VALIDATETHIS;
	IPalette *tmp = new IPalette;
	tmp->nbcol = nbcol;
	tmp->pal = pal;
	tmp->clone = TRUE;
	return tmp;
}


//--------------------------------------------------------------
IPalette *IPalette ::Copy()
{
	IPalette *tmp = Clone();
	tmp->pal = new IRGBColor[nbcol];
	Memory::Copy(tmp->pal, pal, sizeof(IRGBColor)*nbcol);
	tmp->clone = FALSE;
	return tmp;
}


//--------------------------------------------------------------
void IPalette :: MapRGB(_BYTE *Tmp, _DWORD Nb)
{
	ASSERT(Tmp);

	if (pal) 
		delete []pal;
	pal = new IRGBColor[Nb];
	ASSERT(pal);
	for (_DWORD i = 0; i < Nb; i++)
	{
		pal[i].red = Tmp[i * 3];
		pal[i].green = Tmp[i * 3 + 1];
		pal[i].blue = Tmp[i * 3 + 2];
	}
	clone = FALSE;
	nbcol = Nb;
}


//--------------------------------------------------------------
IPalette :: IPalette(_DWORD Nb): pal(NULL)
{
	Reset(Nb);
}


//--------------------------------------------------------------
void IPalette :: Clear()
{
	Reset(0);
}


//--------------------------------------------------------------
void IPalette :: Reset(_DWORD Nb)
{
	if (pal) {
		delete []pal;
	}
	if (Nb) {
		nbcol = Nb;
		pal = new IRGBColor[nbcol];
	}
	else {
		pal = NULL;
		nbcol = 0;
	}
	clone = FALSE;
}


//--------------------------------------------------------------
IPalette::IPalette()
{
	nbcol = 0;
	pal = NULL;
	clone = FALSE;
}


//--------------------------------------------------------------
IPalette :: ~IPalette()
{
	if (pal && (!clone))
	{
		delete []pal;
		nbcol = 0;
		pal = NULL;
	}
}


//--------------------------------------------------------------
void IPalette :: LoadRGB(File *F, _DWORD Nb)
{
	ASSERT(F);
	ASSERT(Nb);

	nbcol = Nb;
	_BYTE *Tmp = new _BYTE[Nb * 3];
	ASSERT(Tmp);
	F->Read(Tmp, Nb * 3);
	MapRGB(Tmp, Nb);
	delete []Tmp;
	clone = FALSE;
}


//--------------------------------------------------------------
void IPalette :: LoadRGB(const char *N, _DWORD Nb)
{
	ASSERT(N);
	ASSERT(Nb);
	
	File *F;
	F = File :: Open(N);
	LoadRGB(F, Nb);
	delete F;
}


//--------------------------------------------------------------
const IPalette & IPalette :: operator = (const IPalette &PAL)
{
	nbcol = PAL.nbcol;
	clone = FALSE;
	pal = new IRGBColor[nbcol];
	Memory::Copy(pal, PAL.pal, sizeof(IRGBColor)*nbcol);
	return *this;
}


//--------------------------------------------------------------
IPalette :: IPalette(IPalette&PAL)
{
	nbcol = PAL.nbcol;
	clone = FALSE;
	pal = new IRGBColor[nbcol];
	Memory::Copy(pal, PAL.pal, sizeof(IRGBColor)*nbcol);
}
