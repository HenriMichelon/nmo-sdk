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
#ifndef _NMO_STRING_H_
#define _NMO_STRING_H_

namespace NMO {

/*!
\brief		US-ASCII Zero terminated strings.

			Classic implementation of a US-ASCII, one byte per char string.
			Not all methods are described here (missing construtors, type
			conversions, comparisons and concatenation). Refers to Stringz.hpp
			for this methods.

\author		(c) 1998-2001 Henri Michelon
\version	$Id: Stringz.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Stringz: public NMOObject
{
public:
	Stringz(_CHAR*);
	Stringz(const _CHAR*);
	Stringz(_CHAR);
	Stringz(const Stringz &);
	Stringz();
	Stringz(_DWORD);
#ifdef _MANAGED
	Stringz(String*);
#endif

	virtual ~Stringz();

	/*! Return a reference to a char.
	  	\param _LONG	(input) 0 based index of the char.
	*/	
	_CHAR& operator [] (_LONG);

	/*! Return a reference to a char.
	  	\param _DWORD	(input) 0 based index of the char.
	*/	
	_CHAR& operator [] (_DWORD);

#ifndef __STORMC__
	/*! Return a reference to a char.
	  	\param _LONG	(input) 0 based index of the char.
	*/	
	const _CHAR& operator [] (_LONG) const;

	/*! Return a reference to a char.
	  	\param _DWORD	(input) 0 based index of the char.
	*/	
	const _CHAR& operator [] (_DWORD) const;
#endif

	const Stringz &operator = (const Stringz &);
	const Stringz &operator = (_CHAR);
	const Stringz &operator = (_LONG);
	const Stringz &operator = (_DWORD);
	const Stringz &operator = (const _CHAR *);

	const Stringz &operator += (const _CHAR *);
	const Stringz &operator += (_CHAR);
	const Stringz &operator += (const Stringz &);

	operator const _CHAR *() const;
	operator _CHAR *() const;
	operator _LONG() const;
	operator _FLOAT() const;
	operator _DOUBLE() const;
	operator _LDOUBLE() const;
	operator _DWORD() const;

	/*! Convert all characters to lower case */
	void LCase();

	/*! Convert all characters to upper case */
	void UCase();

	/*! Delete all starting spaces, tabs & ctrl characters  */
	Stringz& LTrim();

	/*! Delete all ending spaces, tabs & ctrl characters */
	Stringz& RTrim();

	/*! Delete all starting & ending space, tabs & ctrl characters  */
	Stringz& Trim();

	/*! Return the length of the string */
	_DWORD Len() const;

	/*! Return a sub string of the string
		\param _DWORD	(input) first character
		\param _DWORD	(input) len of the sub string
	*/	
	Stringz Copy(_DWORD, _DWORD) const;

	/*! Return a sub string of the string
		\param _DWORD	(input) number of char from the right
	*/	
	Stringz Right(_DWORD) const;

	/*! Return a sub string of the string
		\param 	_DWORD	(input) number of char from the left
	*/	
	Stringz Left(_DWORD) const;

	/*! Find a character in the string
		\param _CHAR	(input) char to search for
		\param 	_LONG	(input) starting chraracter position.
					  -1 = search backward from end
		\return -1 if the char is not found.
				0 for the first position
	*/			
	_LONG InStr(_CHAR, _LONG = 0) const ;

	/*! Find a sub string in the string
		\param Stringz	(input) sub string to search for
		\param _DWORD	(input) starting character position
		\return -1 if the sub string is not found
				0 for the first position
	*/			
	_LONG InStr(const Stringz&, _LONG = 0) const ;

	/*! Create a formated string (cf printf for parameters).
		result string is limited to 1000 chars
	*/	
	Stringz& Format(const _CHAR *, ...);

	/*! Split a string into a list of string.
	  	\param	List	(output) substrings.
		\param	_CHAR	(input) substrings separator.
	*/	
	void Split(List<Stringz>&, _CHAR = ' ') const;


	/*! Concatenate a list of string into one string
	  	\param	List	(input) substrings.
		\param	_CHAR	(input) substrings separator.
	*/	
	void Join(List<Stringz>&, _CHAR = ' ');


	/*! substitute one character per another
	  	\param	_CHAR	: character to replace
	  	\param	_CHAR	: new character 
	*/
	void Sub(_CHAR, _CHAR);	


	/*! substitute one substring per another
	  	\param	Stringz	: string to replace
	  	\param	Stringz	: new string
	*/
	void Sub(const Stringz&, const Stringz&);


	friend _BOOL operator == (const Stringz &, const Stringz &);
	friend _BOOL operator == (const Stringz &, const _CHAR *);
	friend _BOOL operator == (const _CHAR *, const Stringz &);
	friend _BOOL operator == (const Stringz &, _LONG);
	friend _BOOL operator == (const Stringz &, _CHAR);
	friend _BOOL operator == (_LONG, const Stringz &);

	friend _BOOL operator != (const Stringz &, const Stringz &);
	friend _BOOL operator != (const Stringz &, const _CHAR *);
	friend _BOOL operator != (const _CHAR *, const Stringz &);
	friend _BOOL operator != (_LONG, const Stringz &);
	friend _BOOL operator != (const Stringz &, _LONG);
	friend _BOOL operator != (const Stringz &, _CHAR);

	friend _BOOL operator > (const Stringz &, const Stringz &);
	friend _BOOL operator > (const Stringz &, const _CHAR *);
	friend _BOOL operator > (const _CHAR *, const Stringz &);

	friend _BOOL operator < (const Stringz &, const Stringz &);
	friend _BOOL operator < (const Stringz &, const _CHAR *);
	friend _BOOL operator < (const _CHAR *, const Stringz &);

	friend _BOOL operator <= (const Stringz &, const Stringz &);
	friend _BOOL operator <= (const Stringz &, const _CHAR *);
	friend _BOOL operator <= (const _CHAR *, const Stringz &);

	friend _BOOL operator >= (const Stringz &, const Stringz &);
	friend _BOOL operator >= (const Stringz &, const _CHAR *);
	friend _BOOL operator >= (const _CHAR *, const Stringz &);

	friend Stringz operator + (const Stringz &, const Stringz &);
	friend Stringz operator + (const Stringz &, const _CHAR *);
	friend Stringz operator + (const _CHAR *, const Stringz &);
	friend Stringz operator + (_CHAR, const Stringz &);
	friend Stringz operator + (const Stringz &, _CHAR);
	friend Stringz operator + (const Stringz &, _LONG);


	static void Strcpy(const _CHAR*, const _CHAR*);
	static void Strncpy(const _CHAR*, const _CHAR*, _DWORD);
	static void Strcat(_CHAR*, const _CHAR*);
	static _DWORD Strlen(const _CHAR*);
	static _LONG Strcmp(const _CHAR*, const _CHAR*);
	static _LONG Strncmp(const _CHAR*, const _CHAR*, _DWORD);
	
private:
	_CHAR 	*Str;
	_DWORD	bufsize;
	static _CHAR* emptystring;
};

#include "nmo/base/Stringz.inl"

}

#endif
