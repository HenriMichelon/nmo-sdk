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

$Id: DBSet.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBSet::DBSet(DBDatabase&DB, const Stringz&NAME)
{
	database = &DB;
	name = NAME;
	lstRecords = records;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const Stringz& DBSet::Name() const
{
	return name;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBDatabase& DBSet::Database() const
{
	ASSERT(database);
	return *database;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
_DWORD DBSet::ColumnIndex(const Stringz&NAME)
{
	columns.Reset();
	while (!columns.End()) {
		if (columns.Next().name == NAME) {
			return columns.Index()-1;
		}
	} 
	return 0;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
_DWORD DBSet::Column(const Stringz&NAME, DBColumn&COL)
{
	columns.Reset();
	while (!columns.End())
	{
		DBColumn &col = columns.Next();
		if (col.name == NAME)
		{
			COL = col;
			return columns.Index()-1;
		}
	} 
	return 0;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DBValue& DBSet::operator[] (_DWORD IDX)
{
	return lstRecords.Current()[IDX];
}
