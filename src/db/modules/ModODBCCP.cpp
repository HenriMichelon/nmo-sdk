/*                                ---------
                                  [NMO-SDK]
                                  ---------
				(c) 2000-2001 Henri Michelon

$Id: ModODBCCP.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <odbcinst.h>
using namespace NMO;
#include "ModODBCCP.hpp"


//=========================================================
ModODBCCP* ModODBCCP::Create()
{
	_BOOL  result = TRUE;
	ModODBCCP *odbccp = new ModODBCCP;
   	result = odbccp->Load("ODBCCP32");
	if (result)
	{
		result &= (odbccp->SQLGetInstalledDrivers = (FSQLGetInstalledDrivers)
					odbccp->GetMethodAddress("SQLGetInstalledDrivers")) != NULL;
	}
	if (!result)
	{
		delete odbccp;
		odbccp = NULL;
	}
	dprintf("Dynamic module ODBCCP loaded\n");
	return odbccp;
}


#endif