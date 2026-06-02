/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_DATATYPE_FILETXT_H_
#define _NMO_DATATYPE_FILETXT_H_

namespace NMO {

/*!

\brief		Text files support



			This class is intented to provide a simpler

			text file loading/saving methods than

			File::Readln() and File::Writeln().



\author		(c) 1999-2000 Henri Michelon 

\version	$Id: FileTxt.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/

//--------------------------------------------------------------------------



class FileTxt: public NMOObject
{
public:
	/*! Read a text file ans store each line in a list node
		\param Stringz	(input) file name
		\param List		(output) list to fill
		\param _BOOL	(input) filter empty lines & trim lines
		\return FALSE on read error
	*/	
	static _BOOL Load(const Stringz&, List<Stringz>&, _BOOL = FALSE);

	/*! Write a list of string into a file,
	  one line per list node.
	  \param Stringz	(input) filer name
	  \param Iterator	(input) string list
	*/ 
	static _BOOL Save(const Stringz&, Iterator<Stringz>&);
};


}

#endif
