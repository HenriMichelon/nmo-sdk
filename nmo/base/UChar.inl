/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.sourceforge.net/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2000-2001 Henri Michelon

$Id: UChar.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/

//---------------------------------------------------
inline UChar::UChar() {};

//---------------------------------------------------
inline UChar::UChar(const UChar&C) 
{ _char = C._char; };

//---------------------------------------------------
inline UChar::UChar(const _WORD W) 
{ _char = W; };

//---------------------------------------------------
inline UChar::UChar(const _INT W) 
{ _char = _WORD(W); };

//---------------------------------------------------
inline UChar::UChar(const _LONG W) 
{ _char = _WORD(W & 0xFFFF); };

//---------------------------------------------------
inline UChar::UChar(const _CHAR W) 
{ _char = _WORD(W); };

//---------------------------------------------------
inline UChar::operator _WORD () const 
{ return _char; }

//---------------------------------------------------
inline UChar::operator _INT () const 
{ return _INT(_char); }

//---------------------------------------------------
inline UChar::operator _CHAR () const 
{ return _CHAR(_char & 0x7F); }

//---------------------------------------------------
inline const UChar& UChar::operator = (const UChar& C) 
{ _char = C._char; return *this; }

//---------------------------------------------------
inline const UChar& UChar::operator = (const _WORD W)
{ _char = W; return *this; }

//---------------------------------------------------
inline const UChar& UChar::operator = (const _INT W)
{ _char = _WORD(W); return *this; }

//---------------------------------------------------
inline const UChar& UChar::operator = (const _LONG W)
{ _char = _WORD(W & 0xFFFF); return *this; }

//---------------------------------------------------
inline const UChar& UChar::operator = (const _CHAR C)
{ _char = _WORD(C); return *this; }

//---------------------------------------------------
inline _BYTE UChar::Low() const
{ return _BYTE(_char & 0xFF); }

//---------------------------------------------------
inline _BYTE UChar::Hi() const
{ return _BYTE((_char & 0xFF00) >> 8); }

//---------------------------------------------------
inline UChar& UChar::FromLatin1()
{ _char = _WORD(_char & 0xFF); return *this; }

//---------------------------------------------------
inline UChar& UChar::ToLatin1()
{ _char = _WORD(_char & 0xFF); return *this; }

//---------------------------------------------------
inline _BOOL operator > (const UChar &A, const UChar &B)
{ return (A._char > B._char); }

//---------------------------------------------------
inline _BOOL operator < (const UChar &A, const UChar &B)
{ return (A._char < B._char); }

//---------------------------------------------------
inline _BOOL operator <= (const UChar &A, const UChar &B)
{ return (A._char >= B._char); }

//---------------------------------------------------
inline _BOOL operator >= (const UChar &A, const UChar &B)
{ return (A._char <= B._char); }

//---------------------------------------------------
inline _BOOL operator == (const UChar &A, const UChar &B)
{ return (A._char == B._char); }

//---------------------------------------------------
inline _BOOL operator != (const UChar &A, const UChar &B)
{ return (A._char != B._char); }

//---------------------------------------------------
inline _BOOL operator == (const UChar &A, _WORD B)
{ return (A._char == B); }

//---------------------------------------------------
inline _BOOL operator == (_WORD A, const UChar &B)
{ return (A == B._char); }

//---------------------------------------------------
inline _BOOL operator != (const UChar &A, _WORD B)
{ return (A._char != B); }

//---------------------------------------------------
inline _BOOL operator != (_WORD A, const UChar &B)
{ return (A != B._char); }

//---------------------------------------------------
inline _BOOL operator > (const UChar &A, _WORD B)
{ return (A._char > B); }

//---------------------------------------------------
inline _BOOL operator > (_WORD A, const UChar &B)
{ return (A > B._char); }

//---------------------------------------------------
inline _BOOL operator < (const UChar &A, _WORD B)
{ return (A._char < B); }

//---------------------------------------------------
inline _BOOL operator < (_WORD A, const UChar &B)
{ return (A < B._char); }

//---------------------------------------------------
inline _BOOL operator <= (const UChar &A, _WORD B)
{ return (A._char <= B); }

//---------------------------------------------------
inline _BOOL operator <= (_WORD A, const UChar &B)
{ return (A <= B._char); }

//---------------------------------------------------
inline _BOOL operator >= (const UChar &A, _WORD B)
{ return (A._char >= B); }

//---------------------------------------------------
inline _BOOL operator >= (_WORD A, const UChar &B)
{ return (A >= B._char); }
