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

*/
#ifndef _NMO_DATE_TIME_H_
#define _NMO_DATE_TIME_H_

namespace NMO {

/*!
\brief		Minimum date & time support

			This set of classes allow easy manipulation of date
			and time (comparisons, string format, ...).
			
			Some methods (comparisons) are not documented here.
			Please refer to the source file.
			
\author		(c) 1999-2001 Henri Michelon
\version	$Id: DateTime.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DateTime: public NMOObject
{
public:

	/*!
		this class represent a date (really ?) composed by
		a day number, a month number and a four digit year number.
	*/
	class Date: public NMOObject
	{
	public:
		/*! TRUE means that the week start on monday, not sunday */
		static _BOOL weekStartMonday;
		//! Day of the week number (0 based)
		_BYTE	dayOfWeek;
		//! Day of the month number (1 based)
		_BYTE	dayOfMonth;
		//! Month number (1 based)
		_BYTE	month;
		//! Four digits year
		_WORD	year;
		
		/*! Create a date.
			\param _BYTE	: day of month
			\param _BYTE	: month
			\param _BYTE	: year
			\param _BYTE	: day of the week
		*/	
		Date(_BYTE j, _BYTE m, _WORD a, _BYTE d = 0):
			dayOfWeek(d), dayOfMonth(j), month(m), year(a) {};
			
		Date(const Date&);
		Date() {};

		/*! return the current date in local time */
		static Date Current();

		/*! Create a formatted date
			 \param Stringz	: format:
			$yy		: 2 digits year
			$yyyy	: 4 digits year
			$m		: 1 or 2 digits month number
			$mm		: 2 digits month number
			$mmm	: short month name (Jan, Feb, ...)
			$mmmm	: long month name (January, February, ...)
			$d		: 1 digit or 2 digits day of month
			$dd		: 2 digits day of month
			$w		: short day of week name (Mon, Thu, ...)
			$ww		: long day of week name (Monday, ...)
		*/
		Stringz Format(const Stringz&) const;
		Stringz& Format(const Stringz&, Stringz&) const;

		const Date& operator = (const Date &);
		friend _BOOL operator == (const Date &, const Date &);
		friend _BOOL operator != (const Date &, const Date &);
		friend _BOOL operator > (const Date &, const Date &);
		friend _BOOL operator < (const Date &, const Date &);
		friend _BOOL operator >= (const Date &, const Date &);
		friend _BOOL operator <= (const Date &, const Date &);
	};


	/*! This class represent a second bases time value. */
	class Time: public NMOObject
	{
	public:
		//! Number of seconds (0 based)
		_BYTE	second;
		//! Number of minutes (0 based)
		_BYTE	minute;
		//! Number of hours (0 based, 24 hours format)
		_BYTE	hour;

		/*! Create a time.
			\param _BYTE	: hours
			\param _BYTE	: minutes
			\param _BYTE	: seconds
		*/	
		Time(_BYTE h, _BYTE m=0, _BYTE s=0): second(s), minute(m), hour(h) {};
		Time(const Time&);
		Time() {};

		/*! Return the current time (in local value) */
		static Time Current();
 
		/*! Create a formatted time
			\param Stringz	: format:
			$S		: 1 or 2 digits seconds
			$SS		: 2 digits seconds
			$M		: 1 or 2 digits minutes
			$MM		: 2 digits minutes
			$H		: 1 or 2 digits hour
			$HH		: 2 digits hour
			$D		: 'PM' or 'AM' if 12 hour format
			\param	_BOOL	: true = 24 hours format
		*/	
		Stringz Format(const Stringz&, _BOOL = TRUE) const;
		Stringz& Format(const Stringz&, Stringz&, _BOOL = TRUE) const;

		const Time& operator = (const Time&);
		friend _BOOL operator == (const Time &, const Time &);
		friend _BOOL operator != (const Time &, const Time &);
		friend _BOOL operator > (const Time &, const Time &);
		friend _BOOL operator < (const Time &, const Time &);
		friend _BOOL operator >= (const Time &, const Time &);
		friend _BOOL operator <= (const Time &, const Time &);
	};


	//! Date part
	Date	date;
	//! Time part
	Time	time;

	/*! Create date+time.*/
	DateTime(Date D, Time T): date(D), time(T) {};
	DateTime(const DateTime&);
	DateTime() {};

	/*! Return the current (in local value) date and time */
	static DateTime Current();

	/*!	Format a date & time.
		cf DateTime::Date::Format and DateTime::Time::Format
		_BOOL	: true = 24 hours format
	*/	
	Stringz Format(const Stringz&, _BOOL = TRUE) const;
	Stringz& Format(const Stringz&, Stringz&, _BOOL = TRUE) const;

	const DateTime& operator = (const DateTime &);
	friend _BOOL operator == (const DateTime &, const DateTime &);
	friend _BOOL operator != (const DateTime &, const DateTime &);
	friend _BOOL operator > (const DateTime &, const DateTime &);
	friend _BOOL operator < (const DateTime &, const DateTime &);
	friend _BOOL operator >= (const DateTime &, const DateTime &);
	friend _BOOL operator <= (const DateTime &, const DateTime &);
};

}

#endif
