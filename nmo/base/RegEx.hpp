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
#ifndef _NMO_REGEX_H_
#define _NMO_REGEX_H_

namespace NMO {

/*!
\brief		Regular expression support

			This class is an encapsulation of the POSIX RegEx functions.
			The implementation is done using the POSIX functions if avaible. 
			If not, the GNU (glibc) RegEx implementation of this functions is used.

\author		(c) 2000-2001 Henri Michelon 

$Id: RegEx.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class RegEx: public NMOObject
{
public:

	/*!
	  \brief	Regular expression evaluation error
	*/  
	class RegExError: public NMOException { public: RegExError(); };
	
	/*! Flags for regular expression compilation */
	typedef enum {
		/*! No flags */
		NONE,
		/*! Ignore case */
		ICASE,	
		/*! No substring matches (ignore ()) */
		NOSUBS
	} RegExFlags;

	RegEx();
	RegEx(const Stringz&, _DWORD = ICASE);
	virtual ~RegEx();

	/*! Compile a regular expression
		\param Stringz		(input) regular expression
		\param _DWORD		(input) flags for compilation
		\return FALSE on error
	*/	
	_BOOL Compile(const Stringz&, _DWORD = ICASE);

	/*! Execute a (previouly compiled) regular expression
	   	on a string.
		\param Stringz		(input) target string
		\return TRUE on success
	*/	
	_BOOL Execute(const Stringz&);

	/*! Execute a (previouly compiled) regular expression
	   on a string
		\param Stringz		(input) target string
		\param List		(output) execution result (substrings).
					note: the first element contain the substring 
					that match the entire regex
		\return TRUE on success
	*/	
	_BOOL Execute(const Stringz&, List<Stringz>&);

	/*! Return the last error string */
	const Stringz Error() const;

	/*! Compile & execute a regular expression
		\param Stringz	(input) target string
		\Stringz		(input) regular expression
		\return TRUE on success
	*/
	static _BOOL Eval(const Stringz&, const Stringz&, _DWORD = NONE);

	/*! Compile & execute a regular expression
		\param Stringz		(input) target string
		\param Stringz		(input) regular expression
		\param List		(output) execution result (substrings)
					note: the first element contain the substring 
					that match the entire regex
		\return TRUE on success
	*/	
	static _BOOL Eval(const Stringz&, const Stringz&, List<Stringz>&, _DWORD = NONE);

private:
	_PTR	mRegExAbstract;
};

}

#endif
