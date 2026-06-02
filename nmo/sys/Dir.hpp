/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_DIR_H_
#define _NMO_SYS_DIR_H_

namespace NMO {
/*!
\brief		Directory manipulation

			This class have static only method for basic directory
			manipulation.
			
			For more complex manipulation or directory tree searching,
			consider using DirSearch and DirScan classes.

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: Dir.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Dir
{
public:
	/*! Change current directory. */
	static _BOOL Change(const Stringz&);

	/*! Return the current directory */
	static Stringz Current();

	/*! Create a directory (not recursive)*/
	static _BOOL Create(const Stringz&);

	/*! Create a directory (recursive)*/
	static _BOOL CreateAll(const Stringz&);

	/*! Return TRUE if directory exists */
	static _BOOL Exists(const Stringz&);

	/*! Remove a directory (not recursive) */
	static _BOOL Remove(const Stringz&);

	static Stringz CleanFilePath(const Stringz&);

	/*! Expand the ~ character */
	static void Expand(Stringz&);

	/*! return TRUE if the directory exists and is writeable */
	static _BOOL IsWriteable(const Stringz&);
};

}

#endif 
