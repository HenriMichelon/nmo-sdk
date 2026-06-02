/*                                ---------
                                  [NMO-SDK]
                                  ---------

			(c) 2000-2001 Henri Michelon

$Id: ModODBC.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <sql.h>
using namespace NMO;
#include "ModODBC.hpp"


//=========================================================
ModODBC* ModODBC::Create()
{
	_BOOL	result = TRUE;
	ModODBC *odbc = new ModODBC;
	result = odbc->Load("ODBC32");
	if (result)
	{
		result &= (odbc->SQLAllocConnect = (FSQLAllocConnect)
					odbc->GetMethodAddress("SQLAllocConnect")) != NULL;
		result &= (odbc->SQLAllocEnv = (FSQLAllocEnv)
					odbc->GetMethodAddress("SQLAllocEnv")) != NULL;
		result &= (odbc->SQLBindCol = (FSQLBindCol)
					odbc->GetMethodAddress("SQLBindCol")) != NULL;
		result &= (odbc->SQLBindParameter = (FSQLBindParameter)
					odbc->GetMethodAddress("SQLBindParameter")) != NULL;
		result &= (odbc->SQLAllocStmt = (FSQLAllocStmt)
					odbc->GetMethodAddress("SQLAllocStmt")) != NULL;
		result &= (odbc->SQLColumns = (FSQLColumns)
					odbc->GetMethodAddress("SQLColumns")) != NULL;
		result &= (odbc->SQLConnect = (FSQLConnect)
					odbc->GetMethodAddress("SQLConnect")) != NULL;
		result &= (odbc->SQLDescribeCol = (FSQLDescribeCol)
					odbc->GetMethodAddress("SQLDescribeCol")) != NULL;
		result &= (odbc->SQLDisconnect = (FSQLDisconnect)
					odbc->GetMethodAddress("SQLDisconnect")) != NULL;
		result &= (odbc->SQLError = (FSQLError)
					odbc->GetMethodAddress("SQLError")) != NULL;
		result &= (odbc->SQLExecDirect = (FSQLExecDirect)
					odbc->GetMethodAddress("SQLExecDirect")) != NULL;
		result &= (odbc->SQLExecute = (FSQLExecute)
					odbc->GetMethodAddress("SQLExecute")) != NULL;
		result &= (odbc->SQLFetch = (FSQLFetch)
					odbc->GetMethodAddress("SQLFetch")) != NULL;
		result &= (odbc->SQLFreeConnect = (FSQLFreeConnect)
					odbc->GetMethodAddress("SQLFreeConnect")) != NULL;
		result &= (odbc->SQLFreeEnv = (FSQLFreeEnv)
					odbc->GetMethodAddress("SQLFreeEnv")) != NULL;
		result &= (odbc->SQLFreeStmt = (FSQLFreeStmt)
					odbc->GetMethodAddress("SQLFreeStmt")) != NULL;
		result &= (odbc->SQLNumResultCols = (FSQLNumResultCols)
					odbc->GetMethodAddress("SQLNumResultCols")) != NULL;
		result &= (odbc->SQLPrepare = (FSQLPrepare)
					odbc->GetMethodAddress("SQLPrepare")) != NULL;
		result &= (odbc->SQLSetConnectOption = (FSQLSetConnectOption)
					odbc->GetMethodAddress("SQLSetConnectOption")) != NULL;
		result &= (odbc->SQLTables = (FSQLTables)
					odbc->GetMethodAddress("SQLTables")) != NULL;
	}
	if (!result)
	{
		delete odbc;
		odbc = NULL;
	}
	dprintf("Dynamic module ODBC loaded\n");
	return odbc;
}


#endif