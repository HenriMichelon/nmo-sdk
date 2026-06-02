/*                                ---------
                                  [NMO-SDK]
                                  ---------

				ODBC32.DLL

				(c) 2000 Henri Michelon

$Id: ModODBC.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------------------*/

#ifndef _NMO_WIN32_MODODBC_H_
#define _NMO_WIN32_MODODBC_H_

class ModODBC: public DynModule
{
public:
	typedef SQLRETURN (SQL_API *FSQLAllocConnect)(SQLHENV,SQLHDBC*);
	typedef SQLRETURN (SQL_API *FSQLAllocEnv)(SQLHENV*);
	typedef SQLRETURN (SQL_API *FSQLAllocStmt)(SQLHDBC,SQLHSTMT*);
	typedef SQLRETURN (SQL_API *FSQLBindCol)(SQLHSTMT, 
								SQLUSMALLINT, SQLSMALLINT, 
								SQLPOINTER, SQLINTEGER, 
	   							SQLINTEGER *);
	typedef SQLRETURN (SQL_API *FSQLBindParameter)(SQLHSTMT,
								SQLUSMALLINT,SQLSMALLINT,SQLSMALLINT,
								SQLSMALLINT,SQLUINTEGER,SQLSMALLINT,
								SQLPOINTER,SQLINTEGER,SQLINTEGER*);
	typedef SQLRETURN (SQL_API *FSQLColumns)(SQLHSTMT,SQLCHAR*,SQLSMALLINT,SQLCHAR*,
							  SQLSMALLINT,SQLCHAR*,SQLSMALLINT,
							  SQLCHAR*,SQLSMALLINT);
	typedef SQLRETURN (SQL_API *FSQLConnect)(SQLHDBC,SQLCHAR*,SQLSMALLINT,SQLCHAR*,
							  SQLSMALLINT,SQLCHAR*,SQLSMALLINT);
	typedef SQLRETURN (SQL_API *FSQLDescribeCol)(SQLHSTMT,SQLUSMALLINT,SQLCHAR*,SQLSMALLINT,
								  SQLSMALLINT*,SQLSMALLINT*,SQLUINTEGER*,
								  SQLSMALLINT*,SQLSMALLINT*);
	typedef SQLRETURN (SQL_API *FSQLDisconnect)(SQLHDBC);
	typedef SQLRETURN (SQL_API *FSQLError)(SQLHENV,SQLHDBC,SQLHSTMT,SQLCHAR*,SQLINTEGER*,
							SQLCHAR*,SQLSMALLINT,SQLSMALLINT*);
	typedef SQLRETURN (SQL_API *FSQLExecDirect)(SQLHSTMT,SQLCHAR*,SQLINTEGER);
	typedef SQLRETURN (SQL_API *FSQLExecute)(SQLHSTMT);
	typedef SQLRETURN (SQL_API *FSQLFetch)(SQLHSTMT);
	typedef SQLRETURN (SQL_API *FSQLFreeConnect)(SQLHDBC);
	typedef SQLRETURN (SQL_API *FSQLFreeEnv)(SQLHENV);
	typedef SQLRETURN (SQL_API *FSQLFreeStmt)(SQLHSTMT,SQLUSMALLINT);
	typedef SQLRETURN (SQL_API *FSQLNumResultCols)(SQLHSTMT,SQLSMALLINT*);
	typedef SQLRETURN (SQL_API *FSQLPrepare)(SQLHSTMT,SQLCHAR*,SQLINTEGER);
	typedef SQLRETURN (SQL_API *FSQLSetConnectOption)(SQLHDBC,SQLUSMALLINT,SQLUINTEGER);
	typedef SQLRETURN (SQL_API *FSQLTables)(SQLHSTMT,SQLCHAR*,SQLSMALLINT,SQLCHAR*,
							 SQLSMALLINT,SQLCHAR*,SQLSMALLINT,
							 SQLCHAR*,SQLSMALLINT);

	FSQLAllocConnect SQLAllocConnect;
	FSQLAllocEnv SQLAllocEnv;
	FSQLAllocStmt SQLAllocStmt;
	FSQLBindCol SQLBindCol;
	FSQLBindParameter SQLBindParameter;
	FSQLColumns SQLColumns;
	FSQLConnect SQLConnect;
	FSQLDescribeCol SQLDescribeCol;
	FSQLDisconnect SQLDisconnect;
	FSQLError SQLError;
	FSQLExecDirect SQLExecDirect;
	FSQLExecute SQLExecute;
	FSQLFetch SQLFetch;
	FSQLFreeConnect SQLFreeConnect;
	FSQLFreeEnv SQLFreeEnv;
	FSQLFreeStmt SQLFreeStmt;
	FSQLNumResultCols SQLNumResultCols;
	FSQLPrepare SQLPrepare;
	FSQLSetConnectOption SQLSetConnectOption;
	FSQLTables SQLTables;

	static ModODBC* Create();
};

#endif

