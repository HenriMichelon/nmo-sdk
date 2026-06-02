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

	(c) 1998-2001 Henri Michelon

$Id: Stringz.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/

//------------------------------------------------------
inline _DWORD Stringz :: Len() const
{ return Strlen(Str); }


//------------------------------------------------------
inline _CHAR& Stringz :: operator [] (_DWORD i)
{
	PRE(i <= Len(), "Index out of range");
	return Str[i];
}


//------------------------------------------------------
inline _CHAR& Stringz :: operator [] (_LONG i)
{
	PRE(i <= (_LONG)Len(), "Index out of range");
	return Str[i];
}


#ifndef __STORMC__
//------------------------------------------------------
inline const _CHAR& Stringz :: operator [] (_DWORD i) const
{
	PRE(i <= Len(), "Index out of range");
	return Str[i];
}


//------------------------------------------------------
inline const _CHAR& Stringz :: operator [] (_LONG i) const
{
	PRE(i <= (_LONG)Len(), "Index out of range");
	return Str[i];
}
#endif // __STORMC__

//------------------------------------------------------
inline Stringz :: operator const _CHAR *() const
{ return (const _CHAR *) Str; }


//------------------------------------------------------
inline Stringz :: operator _CHAR *() const
{ return (_CHAR *) Str; }


//------------------------------------------------------
inline Stringz Stringz :: Right(_DWORD L) const
{ return Copy(Len()-L, L); }


//------------------------------------------------------
inline Stringz Stringz :: Left(_DWORD L) const
{ return Copy(0, L); }


//------------------------------------------------------
inline _BOOL operator > (const Stringz &S1, const Stringz &S2)
{ return Stringz::Strcmp(S1.Str, S2.Str) > 0; } 


//------------------------------------------------------
inline _BOOL operator > (const Stringz &S1, const _CHAR *S2)
{ return Stringz::Strcmp(S1.Str, S2) > 0; }


//------------------------------------------------------
inline _BOOL operator > (const _CHAR *S1, const Stringz &S2)
{ return Stringz::Strcmp(S1, S2.Str) > 0; }


//------------------------------------------------------
inline _BOOL operator < (const Stringz &S1, const Stringz &S2)
{ return Stringz::Strcmp(S1.Str, S2.Str) < 0; }


//------------------------------------------------------
inline _BOOL operator < (const Stringz &S1, const _CHAR *S2)
{ return Stringz::Strcmp(S1.Str, S2) < 0; }


//------------------------------------------------------
inline _BOOL operator < (const _CHAR *S1, const Stringz &S2)
{ return Stringz::Strcmp(S1, S2.Str) < 0; }


//------------------------------------------------------
inline _BOOL operator <= (const Stringz &S1, const Stringz &S2)
{ return Stringz::Strcmp(S1.Str, S2.Str) <= 0; }


//------------------------------------------------------
inline _BOOL operator <= (const Stringz &S1, const _CHAR *S2)
{ return Stringz::Strcmp(S1.Str, S2) <= 0; }


//------------------------------------------------------
inline _BOOL operator <= (const _CHAR *S1, const Stringz &S2)
{ return Stringz::Strcmp(S1, S2.Str) <= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const Stringz &S1, const Stringz &S2)
{ return Stringz::Strcmp(S1.Str, S2.Str) >= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const Stringz &S1, const _CHAR *S2)
{ return Stringz::Strcmp(S1.Str, S2) >= 0; }


//------------------------------------------------------
inline _BOOL operator >= (const _CHAR *S1, const Stringz &S2)
{ return Stringz::Strcmp(S1, S2.Str) >= 0; }


//------------------------------------------------------
inline _BOOL operator == (const Stringz &S1, const Stringz &S2)
{ return !Stringz::Strcmp(S1.Str, S2.Str); }


//------------------------------------------------------
inline _BOOL operator == (const Stringz &S1, const _CHAR *S2)
{ return !Stringz::Strcmp(S1.Str, S2); }


//------------------------------------------------------
inline _BOOL operator == (const Stringz &S1, _CHAR C)
{ return ((S1.Len() == 1) && (S1[0L] == C)); }


//------------------------------------------------------
inline _BOOL operator == (const _CHAR *S1, const Stringz &S2)
{ return !Stringz::Strcmp(S1, S2.Str); }


//------------------------------------------------------
inline _BOOL operator != (const Stringz &S1, const Stringz &S2)
{ return Stringz::Strcmp(S1.Str, S2.Str); }


//------------------------------------------------------
inline _BOOL operator != (const Stringz &S1, const _CHAR *S2)
{ return Stringz::Strcmp(S1.Str, S2); }


//------------------------------------------------------
inline _BOOL operator != (const _CHAR *S1, const Stringz &S2)
{ return Stringz::Strcmp(S1, S2.Str); }


//------------------------------------------------------
inline _BOOL operator != (const Stringz &S1, _CHAR C)
{ return ((S1.Len() > 1) || (S1[0L] != C)); }

