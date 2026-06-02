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
				
$Id: GProgressBar.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//--------------------------------------------------------------------------
GProgressBar::GProgressBar(ProgressBarType T, _DWORD MIN, _DWORD MAX, 
						_DWORD STEP, _DWORD VAL): 
	GValueSelect(PROGRESSBAR),dtype(NONE), type(T)
{
	transparent = TRUE;
	min = MIN;
	max = MAX;
	value = VAL;
	step = STEP;
}


//--------------------------------------------------------------------------
void GProgressBar::SetResources(const Stringz&RBOX)
{
	Add(box, CLIENT, RBOX);
}


//--------------------------------------------------------------------------
void GProgressBar::SetDisplayType(ProgressBarDisplay D)
{
	dtype = D;
	Refresh();
}

//--------------------------------------------------------------------------
void GProgressBar::SetType(ProgressBarType D)
{
	type = D;
	Refresh();
}
