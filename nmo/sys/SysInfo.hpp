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

*/
#ifndef _NMO_SYSINFO_H_
#define _NMO_SYSINFO_H_

namespace NMO {

/*!
\brief			OS & environment informations

\author			(c) 1999-2002 Henri Michelon
\version		$Id: SysInfo.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class SysInfo
{
public:
	
	/*! Get current user name */
	static Stringz Username();

	/*! Get current host name */
	static Stringz Hostname();

	/*! Get current user home directory
		or system/configuration directory
		in a single user OS
	*/
	static Stringz Userhome();

	/*! Get OS Name */
	static Stringz OSName();

	/*! Get OS Release string */
	static Stringz OSRelease();

	/*! Get OS Version string */
	static Stringz OSVersion();

	/*! Get OS Version numbers
		\param _DWORD	: version number
		\param _DWORD	: release number
		\param _DWORD	: build number
	*/
	static void OSVersion(_DWORD&, _DWORD&, _DWORD&);

	/*! Get hardware platform short name */
	static Stringz Hardware();
};

}

#endif
