/*
(c) 1999-2002 Henri Michelon

$Id: session.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SQLCMD_SESSION_H_
#define _SQLCMD_SESSION_H_

namespace NMO {


class session
{
public:
	static Stringz driver;
	static DBDatabase *database;
};

}

#endif
