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

$Id: DBValue.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/DB.hpp>
using namespace NMO;


Stringz DBValue::Format(_LONG TYPE, const Stringz&FMT)
{
	Stringz s;
	switch (TYPE)
	{
	case DBType::BYTE:
		s.Format("%d", vByte);
		break;
	case DBType::SHORT:
		s.Format("%d", vShort);
		break;
	case DBType::INT:
		s.Format("%d", vInt);
		break;
	case DBType::LONG:
		s.Format("%d", vLong);
		break;
	case DBType::DECIMAL:
		s.Format("%d", _LONG(vDecimal));
		break;
	case DBType::BOOL:
		s.Format("%d", vBool);
		break;
	case DBType::FLOAT:
		s.Format("%f", vFloat);
		break;
	case DBType::DOUBLE:
		s.Format("%f", vDouble);
		break;
	case DBType::STRING:
		return vString;
	case DBType::DATETIME:
		{
			Stringz fmt(FMT);
			if (!fmt.Len()) fmt = "$dd/$mm/$yyyy $HH:$MM:$SS";
			s = vDateTime.Format(fmt);
		}
		break;
	case DBType::TIME:
		{
			Stringz fmt(FMT);
			if (!fmt.Len()) fmt = "$HH:$MM:$SS";
			s = vDateTime.Format(fmt);
		}
		break;
	case DBType::DATE:
		{
			Stringz fmt(FMT);
			if (!fmt.Len()) fmt = "$dd/$mm/$yyyy";
			s = vDateTime.Format(fmt);
		}
		break;
	case DBType::DATA:		
		s = ("(...)");
		break;
	default:
		dprintf("Unknow DBType\n");
		ASSERT(FALSE);
	}
	return s;
}
