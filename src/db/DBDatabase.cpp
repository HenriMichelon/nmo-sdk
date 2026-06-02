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

	(c) 1999-2002 Henri Michelon

$Id: DBDatabase.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;

#ifdef _WIN32
# include <windows.h>
# include "DBDatabaseODBC.hpp"
#endif

#ifdef HAVE_POSTGRES
# include "DBDatabasePostgres.hpp"
#endif

#ifdef HAVE_MYSQL
# include "DBDatabaseMySQL.hpp"
#endif

NMO::List<Stringz> DBDatabase::names;
_LONG DBDatabase::nInst = 0;


//---------------------------------------------------------
DBDatabase::ExcLoadingDriver::ExcLoadingDriver(): NMOException("DBDatabase::LoadingDriver") {};


//---------------------------------------------------------
DBDatabase::DBDatabase() : connected(FALSE), autoCommit(TRUE) {};


//---------------------------------------------------------
DBDatabase::~DBDatabase()
{
	nInst--;
	if (!nInst) names.Clear();
}


//---------------------------------------------------------
NMO::List<Stringz>& DBDatabase::Drivers()
{
	if (!nInst)
	{
#ifdef _WIN32
		names.Add(new Stringz(ODBCDRIVERNAME));
#endif
#ifdef HAVE_POSTGRES
		names.Add(new Stringz(POSTGRESDRIVERNAME));
#endif
#ifdef HAVE_MYSQL
		names.Add(new Stringz(MYSQLDRIVERNAME));
#endif
	}
	nInst++;
	return (List<Stringz>&)names;
}
	
//---------------------------------------------------------
DBDatabase* DBDatabase::Create(const Stringz&D)
{
	Stringz drv = D;
	drv.UCase();
	drv.Trim();
	try {
#ifdef _WIN32
		if (drv == (_CHAR*)"ODBC")
			return (DBDatabase*) new DBDatabaseODBC;
#endif
#ifdef HAVE_POSTGRES
		if (drv == (_CHAR*)"POSTGRES")
			return (DBDatabase*) new DBDatabasePostgres;
#endif
#ifdef HAVE_MYSQL
		if (drv == (_CHAR*)"MYSQL")
			return (DBDatabase*) new DBDatabaseMySQL;
#endif
	}
	catch (ExcLoadingDriver) {
	}
	return NULL;
}

