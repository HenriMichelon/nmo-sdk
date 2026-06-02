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

$Id: DBType.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;

_DWORD DBType::DisplayLen(_LONG TYPE, _DWORD LEN)
{
	switch(TYPE)
	{
	case BYTE:
		return 3;
	case BOOL:
		return 1;
	case SHORT:
		return 4;
	case INT:
		return 6;
	case LONG:
	case DECIMAL:
	case FLOAT:
	case DOUBLE:
		return 10;
	case STRING:
		return LEN;
	case DATETIME:
	case TIME:
	case DATE:
		return 20;
	case DATA:
		return 5;
	default:
		return 0;
	}
}

Stringz DBType::Format(_LONG TYPE, _DWORD LEN)
{
	Stringz r;
	switch(TYPE)
	{
	case BYTE:
		r = "BYTE";
		break;
	case BOOL:
		r = "BOOL";
		break;
	case SHORT:
		r = "SHORT";
		break;
	case INT:
		r = "INT";
		break;
	case LONG:
		r = "LONG";
		break;
	case DECIMAL:
		r = "DECIMAL";
		break;
	case FLOAT:
		r = "FLOAT";
		break;
	case DOUBLE:
		r = "DOUBLE";
		break;
	case STRING:
		r.Format("STRING(%d)", LEN);
		break;
	case DATETIME:
		r = "DATETIME";
		break;
	case TIME:
		r = "TIME";
		break;
	case DATE:
		r = "DATE";
		break;
	case DATA:
		r.Format("DATA(%d)", LEN);
		break;
	default:
		break;
	}
	return Stringz(r);
}
