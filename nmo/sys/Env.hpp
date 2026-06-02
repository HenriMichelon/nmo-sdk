/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_ENV_H_
#define _NMO_SYS_ENV_H_

namespace NMO {

/*!
\brief		Environnement variables manipulations

\author		(c) 1999 Henri Michelon
\version	$Id: Env.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Env
{
public:
	/*!	Write (set) or modify an environnment variable
		\param Stringz	: name of the variable
		\param Stringz	: value
	*/
	static void Write(const Stringz&, const Stringz&);

	/*!	Read (get) an environnment variable
		\param Stringz	: name of the variable
		\param Stringz&	: (out) result
		\return FALSE if the variable is not found
	*/
	static _BOOL Read(const Stringz&, Stringz&);

	/*! Delete (unset) an variable
		\param Stringz& : name of the variable
	*/
	static void Delete(const Stringz&);

	/*! Fill the string list with all the variable names */
	static void Environ(List<Stringz> &);

	/*! Expand variable names in the string */
	static void Expand(Stringz&);
};

}

#endif
