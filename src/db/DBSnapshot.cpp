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

	(c) 1999-2001 Henri Michelon

$Id: DBSnapshot.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBSnapshot::DBSnapshot(DBDatabase&DB, const Stringz&NAME): DBSet(DB, NAME)
{}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBSnapshot* DBSnapshot::Open(DBDatabase&DB, const Stringz&SQL)
{
	if ((!DB.Connected()) || (!SQL.Len())) return NULL;

	DBSnapshot *table = new DBSnapshot(DB, SQL);
	if (DB.Query(SQL, table->Columns(), table->Records()))
		return table;
	delete table;
	return NULL;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DBSnapshot::Refresh()
{
	VALIDATETHIS;
	Columns().Clear();
	Records().Clear();
	database->Query(name, Columns(), Records());
}

