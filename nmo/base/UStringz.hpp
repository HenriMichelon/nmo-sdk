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
#ifndef _NMO_USTRING_H_
#define _NMO_USTRING_H_

namespace NMO {

/*!
\brief		UNICODE double Zero terminated strings.

			Implementation of a UNICODE (UChar based) UStringz.
			Not all methods are described here (missing construtors, type
			conversions, comparisons and concatenation). 
			Refers to UStringz.hpp for this methods.

\author		(c) 2000-2001 Henri Michelon
\version	$Id: UStringz.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class UStringz: public NMOObject
{
public:
	UStringz(const _CHAR*);
	UStringz(const UChar*);
	UStringz(const _WORD*);
	UStringz(UChar);
	UStringz(const UStringz &);
	UStringz();

	virtual ~UStringz();

	/*! Return a reference to a char.
	  	\param _LONG	(input) 0 based index of the char.
	*/	
	UChar& operator [] (_LONG);

	/*! Return a reference to a char.
	  	\param _DWORD	(input) 0 based index of the char.
	*/	
	UChar& operator [] (_DWORD);

#ifndef __STORMC__
	/*! Return a reference to a char.
	  	\param _LONG	(input) 0 based index of the char.
	*/	
	const UChar& operator [] (_LONG) const;

	/*! Return a reference to a char.
	  	\param _DWORD	(input) 0 based index of the char.
	*/	
	const UChar& operator [] (_DWORD) const;
#endif

	const UStringz &operator = (const UStringz &);
	const UStringz &operator = (const Stringz &);
	const UStringz &operator = (UChar);
	const UStringz &operator = (const UChar *);
	const UStringz &operator = (const _WORD *);

	const UStringz &operator += (const UChar *);
	const UStringz &operator += (const _WORD *);
	const UStringz &operator += (UChar);
	const UStringz &operator += (const UStringz &);

	operator const UChar *() const;
	operator UChar *() const;
	operator Stringz () const;

	/*! Convert all characters to lower case */
//	void LCase();

	/*! Convert all characters to upper case */
//	void UCase();

	/*! Delete all starting spaces, tabs & ctrl characters  */
	UStringz& LTrim();

	/*! Delete all ending spaces, tabs & ctrl characters */
	UStringz& RTrim();

	/*! Delete all starting & ending space, tabs & ctrl characters  */
	UStringz& Trim();

	/*! Return the length of the string */
	_DWORD Len() const;

	/*! Return a sub string of the string
		\param _DWORD	(input) first character
		\param _DWORD	(input) len of the sub string
	*/	
	UStringz Copy(_DWORD, _DWORD) const;

	/*! Return a sub string of the string
		\param _DWORD	(input) number of char from the right
	*/	
	UStringz Right(_DWORD) const;

	/*! Return a sub string of the string
		\param 	_DWORD	(input) number of char from the left
	*/	
	UStringz Left(_DWORD) const;

	/*! Find a character in the string
		\param UChar	(input) char to search for
		\param 	_LONG	(input) starting chraracter position.
					  -1 = search backward from end
		\return -1 if the char is not found.
				0 for the first position
	*/			
	_LONG InStr(const UChar&, _LONG = 0) const ;

	/*! Find a sub string in the string
		\param UStringz	(input) sub string to search for
		\param _DWORD	(input) starting character position
		\return -1 if the sub string is not found
				0 for the first position
	*/			
	_LONG InStr(const UStringz&, _LONG = 0) const ;

	/*! Create a formated string (cf printf for parameters).
		result string is limited to 1000 chars
	*/	
//	UStringz& Format(const UStringz&, ...);

	/*! Split a string into a list of string.
	  	\param	List	(output) substrings.
		\param	UChar	(input) substrings separator.
	*/	
	void Split(List<UStringz>&, UChar = _WORD(0x0020)) const;


	/*! Concatenate a list of string into one string
	  	\param	List	(input) substrings.
		\param	UChar	(input) substrings separator.
	*/	
	void Join(List<UStringz>&, UChar = _WORD(0x0020));

	friend _BOOL operator == (const UStringz &, const UStringz &);
	friend _BOOL operator == (const UStringz &, UChar);

	friend _BOOL operator != (const UStringz &, const UStringz &);
	friend _BOOL operator != (const UStringz &, UChar);

	friend _BOOL operator > (const UStringz &, const UStringz &);
	friend _BOOL operator < (const UStringz &, const UStringz &);
	friend _BOOL operator <= (const UStringz &, const UStringz &);
	friend _BOOL operator >= (const UStringz &, const UStringz &);

	friend UStringz operator + (const UStringz &, const UStringz &);
	friend UStringz operator + (UChar, const UStringz &);
	friend UStringz operator + (const UStringz &, UChar);
	friend UStringz operator + (const UStringz &, const UChar*);
	friend UStringz operator + (const UStringz &, const _WORD*);

	UStringz& From(UChar::CharacterSetName);
	UStringz& To(UChar::CharacterSetName);

	UStringz& FromLatin1();
	UStringz& ToLatin1();

	static _LONG Strcmp(const UChar*, const UChar*);

private:
	UChar	*Str;
	_DWORD	len;

	void Strcat(const UChar*, const UChar*) const;
	void Strcat(const UChar*, const _WORD*) const;
	void Strcpy(const UChar*, const UChar*) const;
	void Strcpy(const UChar*, const _WORD*) const;
	void Strncpy(const UChar*, const UChar*, _DWORD) const;
	_DWORD Strlen(const UChar*) const;	
	_DWORD Strlen(const _WORD*) const;	
};

#include "nmo/base/UStringz.inl"

}

#endif
