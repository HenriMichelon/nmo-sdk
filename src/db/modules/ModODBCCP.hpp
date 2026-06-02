/*                                ---------
                                  [NMO-SDK]
                                  ---------
				ODBCCP32.DLL

				(c) 2000 Henri Michelon

$Id: ModODBCCP.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------------*/
#ifndef _NMO_WIN32_ODBCCP_H_
#define _NMO_WIN32_ODBCCP_H_

class ModODBCCP: public DynModule
{
public:
	typedef BOOL (INSTAPI *FSQLGetInstalledDrivers)(LPSTR,WORD,WORD*);

	FSQLGetInstalledDrivers SQLGetInstalledDrivers;

	static ModODBCCP* Create();
};

#endif
