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

				(c) 2000 Henri Michelon

$Id: UStringz.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/

//------------------------------------------------------
inline _DWORD UStringz :: Len() const
{ return len; }


//------------------------------------------------------
inline UChar& UStringz :: operator [] (_DWORD i)
{
	PRE(i <= Len(), "Index out of range");
	return Str[i];
}


//------------------------------------------------------
inline UChar& UStringz :: operator [] (_LONG i)
{
	PRE(i <= (_LONG)Len(), "Index out of range");
	return Str[i];
}


#ifndef __STORMC__
//------------------------------------------------------
inline const UChar& UStringz :: operator [] (_DWORD i) const
{
	PRE(i <= Len(), "Index out of range");
	return Str[i];
}


//------------------------------------------------------
inline const UChar& UStringz :: operator [] (_LONG i) const
{
	PRE(i <= (_LONG)Len(), "Index out of range");
	return Str[i];
}
#endif // __STORMC__

//------------------------------------------------------
inline UStringz :: operator const UChar *() const
{ return (const UChar*) Str; }


//------------------------------------------------------
inline UStringz :: operator UChar *() const
{ return (UChar*) Str; }


//------------------------------------------------------
inline _BOOL operator > (const UStringz &S1, const UStringz &S2)
{ return UStringz::Strcmp(S1.Str, S2.Str) > 0; }


//------------------------------------------------------
inline _BOOL operator > (const UStringz &S1, const UChar *S2)
{ return UStringz::Strcmp(S1, S2) > 0; }


//------------------------------------------------------
inline _BOOL operator > (const UChar *S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) > 0; }


//------------------------------------------------------
inline _BOOL operator < (const UStringz &S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) < 0; }


//------------------------------------------------------
inline _BOOL operator < (const UStringz &S1, const UChar *S2)
{ return UStringz::Strcmp(S1, S2) < 0; }


//------------------------------------------------------
inline _BOOL operator < (const UChar *S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) < 0; }


//------------------------------------------------------
inline _BOOL operator <= (const UStringz &S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) <= 0; }


//------------------------------------------------------
inline _BOOL operator <= (const UStringz &S1, const UChar *S2)
{ return UStringz::Strcmp(S1, S2) <= 0; }


//------------------------------------------------------
inline _BOOL operator <= (const UChar *S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) <= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const UStringz &S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) >= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const UStringz &S1, const UChar *S2)
{ return UStringz::Strcmp(S1, S2) >= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const UChar *S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2) >= 0; }

//------------------------------------------------------
inline _BOOL operator == (const UStringz &S1, const UStringz &S2)
{ return !UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator == (const UStringz &S1, const UChar *S2)
{ return !UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator == (const UStringz &S1, UChar C)
{ return ((S1.Len() == 1) && (_WORD(S1[0L]) == C)); }


//------------------------------------------------------
inline _BOOL operator == (const UChar *S1, const UStringz &S2)
{ return !UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator != (const UStringz &S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator != (const UStringz &S1, const UChar *S2)
{ return UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator != (const UChar *S1, const UStringz &S2)
{ return UStringz::Strcmp(S1, S2); }


//------------------------------------------------------
inline _BOOL operator != (const UStringz &S1, UChar C)
{ return ((S1.Len() > 1) || (_WORD(S1[0L]) != C)); }

