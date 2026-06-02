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

	Include file for the core sdk classes

	(c) 1998-2001 Henri Michelon 

$Id: NMO.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_NMO_H_
#define _NMO_NMO_H_

#include <nmo/Base.hpp>
#include <nmo/Sys.hpp>

#include <nmo/Stream.hpp>
#include <nmo/FileStream.hpp>
#include <nmo/GZipStream.hpp>
#include <nmo/MemoryStream.hpp>
#include <nmo/ResourcesTable.hpp>

#include <nmo/Datatypes.hpp>
#include <nmo/sys/CfgStorage.hpp>
#include <nmo/Application.hpp>

using namespace NMO;

// The following includes are usefuls for speeding up
// compilation when using precompiled header
#ifdef _MSC_VER
	#include <nmo/Interface.hpp>
	#include <nmo/DB.hpp>
	#include <nmo/Net.hpp>
	#include <nmo/Sound.hpp>
	#include <nmo/Thread.hpp>
	#include <nmo/addons/Digest.hpp>
	#include <nmo/addons/Shell.hpp>
	#include <nmo/addons/GUI.hpp>
#endif

#endif // _NMO_NMO_H_
