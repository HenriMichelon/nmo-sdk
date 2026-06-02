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
#ifndef _NMO_UCHAR_H_
#define _NMO_UCHAR_H_

namespace NMO {

/*!
\brief		UNICODE Character

			This class allow manipulation/convertion of UNICODE characters.
			Character sets names are mapped with the UNICODE name, postfixed
			with the vendor name for vendor specific sets.
			
			Examples:
				USACII
				ISO8859_1
				APPLE_ROMAN

\author		(c) 1998-2001 Henri Michelon
\version	$Id: UChar.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class UChar: public NMOObject
{
public:
	typedef Stringz CharacterSetName;

	UChar();
	UChar(const UChar&);
	UChar(const _WORD);
	UChar(const _INT);
	UChar(const _LONG);
	UChar(const _CHAR);
	virtual ~UChar() {};

	operator _WORD () const;
	operator _INT () const;
	operator _CHAR () const; // To(USASCII)

	const UChar& operator = (const UChar&);
	const UChar& operator = (const _WORD);
	const UChar& operator = (const _INT);
	const UChar& operator = (const _LONG);
	const UChar& operator = (const _CHAR); // From(USASCII)

	friend _BOOL operator > (const UChar &, const UChar &);
	friend _BOOL operator < (const UChar &, const UChar &);
	friend _BOOL operator <= (const UChar &, const UChar &);
	friend _BOOL operator >= (const UChar &, const UChar &);
	friend _BOOL operator == (const UChar &, const UChar &);
	friend _BOOL operator != (const UChar &, const UChar &);

	friend _BOOL operator > (const UChar &, _WORD);
	friend _BOOL operator > (_WORD, const UChar &);
	friend _BOOL operator < (const UChar &, _WORD);
	friend _BOOL operator < (_WORD, const UChar &);
	friend _BOOL operator <= (const UChar &, _WORD);
	friend _BOOL operator <= (_WORD, const UChar &);
	friend _BOOL operator >= (const UChar &, _WORD);
	friend _BOOL operator >= (_WORD, const UChar &);
	friend _BOOL operator == (const UChar &, _WORD);
	friend _BOOL operator == (_WORD, const UChar &);
	friend _BOOL operator != (const UChar &, _WORD);
	friend _BOOL operator != (_WORD, const UChar &);

	_BYTE Low() const;
	_BYTE Hi() const;

	UChar& From(CharacterSetName);
	UChar& To(CharacterSetName);

	UChar& FromLatin1();
	UChar& ToLatin1();

#ifndef _MSC_VER
private:
#endif
	class CharacterSet {
	public:
		Stringz	name;
		_WORD	uni2char[65536];
		_WORD	*char2uni;
		
		CharacterSet(const Stringz &, _DWORD);
		~CharacterSet();
	};
	
	class CharacterSets {
	public:
		CharacterSets();
		List<CharacterSet> sets;
	private:
		void AddSet(ListIterator<Stringz>&);
	};
	
private:
	_WORD _char;
	static CharacterSets mappings;
	friend class UStringz;
	friend class IFontEngine;
};

#include "nmo/base/UChar.inl"

}

#endif

