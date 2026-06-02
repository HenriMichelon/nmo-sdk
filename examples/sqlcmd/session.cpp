/*
(c) 1999-2002 Henri Michelon

$Id: session.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
#include "session.hpp"

using namespace NMO;

Stringz session::driver;
DBDatabase *session::database = NULL;
