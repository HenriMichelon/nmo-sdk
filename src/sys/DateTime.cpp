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

$Id: DateTime.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/LC/LC.hpp>
using namespace NMO;
#ifndef _MSC_VER
# include <time.h>
#endif

_BOOL NMO::DateTime::Date::weekStartMonday = TRUE;


//---------------------------------------------------------------
NMO::DateTime::Date NMO::DateTime::Date::Current()
{
	time_t ctime;
	::time(&ctime);
	tm *ctm = localtime(&ctime);
	ctm->tm_wday--;
	if (ctm->tm_wday == -1)
		ctm->tm_wday = 6;
	return Date(ctm->tm_mday, ctm->tm_mon+1, 
		ctm->tm_year+1900, ctm->tm_wday);
}


//---------------------------------------------------------------
NMO::DateTime::Time NMO::DateTime::Time::Current()
{
	time_t ctime;
	::time(&ctime);
	tm *ctm = localtime(&ctime);
	return Time(ctm->tm_hour, ctm->tm_min, ctm->tm_sec);
}


//---------------------------------------------------------------
NMO::DateTime NMO::DateTime::Current()
{
	return NMO::DateTime(Date::Current(), Time::Current());
}


//---------------------------------------------------------------
NMO::DateTime::Date::Date(const NMO::DateTime::Date&D)
{
	dayOfMonth = D.dayOfMonth;
	dayOfWeek = D.dayOfWeek;
	month = D.month;
	year = D.year;
}


//---------------------------------------------------------------
NMO::DateTime::Time::Time(const NMO::DateTime::Time&T)
{
	second = T.second;
	minute = T.minute;
	hour = T.hour;
}


//---------------------------------------------------------------
NMO::DateTime::DateTime(const NMO::DateTime&D)
{
	date = D.date;
	time = D.time;
}


//---------------------------------------------------------------
Stringz NMO::DateTime::Date::Format(const Stringz&FMT) const
{
	Stringz res;
	return Format(FMT, res);
}


//---------------------------------------------------------------
Stringz& NMO::DateTime::Date::Format(const Stringz&FMT, Stringz &res) const
{
	Stringz codes("ymdw");
	_DWORD i=0;
	Stringz temp(_DWORD(60));
	Stringz code(_DWORD(60));
	while (i<FMT.Len())	
	{
		if (FMT[i] == '$')
		{
			i++;
			temp = "";
			code = "";
			while ((i<FMT.Len()) && (codes.InStr(FMT[i]) >= 0))
				code += FMT[i++];
			if (code == "yy")
				res += temp.Format("%02d", year);
			else if (code == "yyyy")
				res += temp.Format("%04d", year);
			else if (code == "m")
				res += temp.Format("%d", month);
			else if (code == "mm")
				res += temp.Format("%02d", month);
			else if (code == "mmm")
				res += temp.Format("%s", LC_NMO_SMONTH[month-1]);
			else if (code == "mmmm")
				res += temp.Format("%s", LC_NMO_LMONTH[month-1]);
			else if (code == "d")
				res += temp.Format("%d", dayOfMonth);
			else if (code == "dd")
				res += temp.Format("%02d", dayOfMonth);
			else if ((code == "w") && (weekStartMonday))
				res += temp.Format("%s", LC_NMO_SDAYOFWEEK[dayOfWeek-1]);
			else if ((code == "ww") && (weekStartMonday))
				res += temp.Format("%s", LC_NMO_LDAYOFWEEK[dayOfWeek-1]);
			else
				res += '$' + code;
		}
		else
			res += FMT[i++];
	}
	return res;
}


//---------------------------------------------------------------
Stringz NMO::DateTime::Time::Format(const Stringz&FMT, _BOOL FK) const
{
	Stringz res;
	return Format(FMT, res, FK);
}


//---------------------------------------------------------------
Stringz& NMO::DateTime::Time::Format(const Stringz&FMT, Stringz &res, _BOOL FK) const
{
	Stringz codes("SMHD");
	_DWORD i=0;
	Stringz temp(_DWORD(60));
	Stringz code(_DWORD(60));
	while (i<FMT.Len())	
	{
		if (FMT[i] == '$')
		{
			temp = "";
			code = "";
			i++;
			while ((i<FMT.Len()) && (codes.InStr(FMT[i]) >= 0))
				code += FMT[i++];
			if (code == "S")
				res += temp.Format("%d", second);
			else if (code == "SS")
				res += temp.Format("%02d", second);
			else if (code == "M")
				res += temp.Format("%d", minute);
			else if (code == "MM")
				res += temp.Format("%02d", minute);
			else if ((code == "H") && FK)
				res += temp.Format("%d", hour);
			else if ((code == "HH") && FK)
				res += temp.Format("%02d", hour);
			else if ((code == "H") && (!FK))
				res += temp.Format("%d", hour%12);
			else if ((code == "HH") && (!FK))
				res += temp.Format("%02d", hour%12);
			else if ((code == "D") && (!FK))
			{
				if (hour < 12)
					res += "AM";
				else
					res += "PM";
			}
			else
				res += '$' + code;
		}
		else
			res += FMT[i++];
	}
	return res;
}


//---------------------------------------------------------------
Stringz NMO::DateTime::Format(const Stringz&FMT, _BOOL FK) const
{
	return date.Format(time.Format(FMT, FK));
}


//---------------------------------------------------------------
Stringz& NMO::DateTime::Format(const Stringz&FMT, Stringz&res, _BOOL FK) const
{
	return date.Format(time.Format(FMT, res, FK), res);
}

namespace NMO {


//---------------------------------------------------------------
const NMO::DateTime::Date& NMO::DateTime::Date::operator = (const NMO::DateTime::Date &D)
{
	dayOfMonth = D.dayOfMonth;
	dayOfWeek = D.dayOfWeek;
	month = D.month;
	year = D.year;
	return *this;
}


//---------------------------------------------------------------
const NMO::DateTime::Time& NMO::DateTime::Time::operator = (const NMO::DateTime::Time&T)
{
	second = T.second;
	minute = T.minute;
	hour = T.hour;
	return *this;
}


//---------------------------------------------------------------
const NMO::DateTime& NMO::DateTime::operator = (const NMO::DateTime &D)
{
	date = D.date;
	time = D.time;
	return *this;
}


//---------------------------------------------------------------
_BOOL operator == (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	return ((D1.dayOfMonth == D2.dayOfMonth) &&
			(D1.month == D2.month) &&
			(D1.year == D2.year));
}


//---------------------------------------------------------------
_BOOL operator == (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	return ((T1.second == T2.second) &&
			(T1.minute == T2.minute) &&
			(T1.hour == T2.hour));
}


//---------------------------------------------------------------
_BOOL operator == (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	return ((D1.date == D2.date) &&
			(D1.time == D2.time));
}


//---------------------------------------------------------------
_BOOL operator != (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	return (!(D1 == D2));
}


//---------------------------------------------------------------
_BOOL operator != (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	return (!(T1 == T2));
}


//---------------------------------------------------------------
_BOOL operator != (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	return (!(D1 == D2));
}


//---------------------------------------------------------------
_BOOL operator > (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	if (D1.year > D2.year)
		return TRUE;
	else if (D1.year == D2.year)
	{
		if (D1.month > D2.month)
			return TRUE;
		else if (D1.month == D2.month)
			return (D1.dayOfMonth > D2.dayOfMonth);
	}
	return FALSE;
}


//---------------------------------------------------------------
_BOOL operator > (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	if (T1.hour > T2.hour)
		return TRUE;
	else if (T1.hour == T2.hour)
	{
		if (T1.minute > T2.minute)
			return TRUE;
		else if (T1.minute == T2.minute)
			return (T1.second == T2.second);
	}
	return FALSE;
}


//---------------------------------------------------------------
_BOOL operator > (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	if (D1.date > D2.date)
		return TRUE;
	else if (D1.date == D2.date)
		return (D1.time > D2.time);
	return FALSE;
}


//---------------------------------------------------------------
_BOOL operator < (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	return (!(D1>D2));
}


//---------------------------------------------------------------
_BOOL operator < (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	return (!(T1>T2));
}


//---------------------------------------------------------------
_BOOL operator < (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	return (!(D1>D2));
}


//---------------------------------------------------------------
_BOOL operator >= (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	return ((D1>D2) || (D1==D2));
}


//---------------------------------------------------------------
_BOOL operator >= (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	return ((T1>T2) || (T1==T2));
}


//---------------------------------------------------------------
_BOOL operator >= (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	return ((D1>D2) || (D1==D2));
}


//---------------------------------------------------------------
_BOOL operator <= (const NMO::DateTime::Date &D1, const NMO::DateTime::Date &D2)
{
	return ((D1<D2) || (D1==D2));
}


//---------------------------------------------------------------
_BOOL operator <= (const NMO::DateTime::Time &T1, const NMO::DateTime::Time &T2)
{
	return ((T1<T2) || (T1==T2));
}


//---------------------------------------------------------------
_BOOL operator <= (const NMO::DateTime &D1, const NMO::DateTime &D2)
{
	return ((D1<D2) || (D1==D2));
}

} // namespace NMO
