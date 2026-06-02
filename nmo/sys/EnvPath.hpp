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
#ifndef _NMO_SYS_ENVPATH_H_
#define _NMO_SYS_ENVPATH_H_

namespace NMO {

/*!
\brief		Help query some PATH informations in a portable manner.

			This class allow you to access different PATH information
			(binary path, library path, ...) without dealing on
			how to query this information (environement variable,
			registry, ...) and on this information is encoded 
			(dir/dir/dir:dir/dir:..., dir\dir\dir;dir\dir;...).
			
			Note: the list have AT LEAST one entry

\author		(c) 2000-2002 Henri Michelon
\version	$Id: EnvPath.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class EnvPath: public List<Stringz>
{
public:
	//! Type of PATH information to query
	typedef enum {
		//! Binary, executables programs (aka PATH on un*x)
		PATH_BIN,		
		//! Library path (aka LD_LIBRARY_PATH on un*x)
		PATH_LIB,
		//! Where configuration files are stored
		PATH_CONF,
		//! Where shared files (not libraries) are stored
		PATH_SHARE,
		//! Temporary directories
		PATH_TEMP,
		//! Where third party programs are generally installed (like /usr/local or "C:\Program Files")
		PATH_APPS
	} PathType;

	EnvPath(PathType);
	virtual ~EnvPath() {};

	const Stringz& operator [] (_DWORD) const;
	
#ifdef _MACOS
	static Stringz ExtractPath(short, long);
#endif
};

}

#endif
