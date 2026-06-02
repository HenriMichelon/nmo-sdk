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

$Id: GLayout.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include "nmo/addons/gui/GLayout.hpp"
#include "GResourceVector.hpp"
#include "GLayoutVector.hpp"
#include "GResourcePixmap.hpp"
#include "GLayoutPixmap.hpp"
using namespace NMO;


//----------------------------------------------
GLayout::GLayout()
{
	font = NULL;
	restable = NULL;
}


//----------------------------------------------
GLayout::~GLayout()
{
	if (font != NULL) { 
		delete font; 
		font = NULL;
	}
}


//----------------------------------------------
void GLayout::SetResources(ResourcesTable&RESTABLE)
{
	restable = &RESTABLE;
}


//----------------------------------------------
ResourcesTable* GLayout::Resources() const
{
	return restable; 
}


//----------------------------------------------
GLayout *GLayout::Create(const Stringz&NAME)
{
	GLayout *tmp = NULL;
	if (NAME == "vector") {
		tmp = (GLayout*) new GLayoutVector;
	}
	else if (NAME == "pixmap") {
		tmp = (GLayout*) new GLayoutPixmap;
	}
	if (tmp) {
		if (!tmp->Init()) {
			delete tmp;
			tmp = NULL;
		}
	}
	return tmp;
}


//----------------------------------------------
void GLayout :: SetOption(const Stringz&NAME, const Stringz&VAL)
{
	GLayoutOption *option = NULL;
	ListIterator<GLayoutOption> list(options);
	while (!list.End()) {
		GLayoutOption &opt = list.Next();
		if (opt.name == NAME) {
			option = &opt;
			break;
		}
	}
	if (option == NULL) {
		option = new GLayoutOption(NAME);
		options.Add(option);
	}
	option->value = VAL;
	UpdateOptions();
}


//----------------------------------------------
Stringz GLayout :: Option(const Stringz&NAME)
{
	GLayoutOption *option = NULL;
	ListIterator<GLayoutOption> list(options);
	while (!list.End()) {
		GLayoutOption &opt = list.Next();
		if (opt.name == NAME) {
			option = &opt;
			break;
		}
	}
	if (option == NULL) { return ""; }
	return option->value;
}
