/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_DIRSEARCH_H_
#define _NMO_SYS_DIRSEARCH_H_

namespace NMO {

/*!
\brief		Directory structure manipulation

			Typical use:
			\code
			DirSearch dir;
			DirSearch::FindData find;
			if (dir.FindFirstFile(Dir::Current() + PATHSEP + ALLFILES, find)) {
				do {
					// ... do something using find.fileName
				} while (dir.FindNextFile(find));
			}
			dir.FindCloseDir(find);
			\endcode

			\par
			For a recusive searching, use the DirScan class.


\author		(c) 1998-2001 Henri Michelon 
\version	$Id: DirSearch.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------


class DirSearch: public NMOObject
{
public:
	/*! Struct used for directory listing */
	class FindData
	{
	public:
		//! name of the file/directory found
		Stringz	fileName;
		_PTR	abstract;
		
		FindData();
		~FindData();
	};

	/*! find the name of the first directory
		\param Stringz: path to search for
		\param FindData: result
		\return FALSE if there is no directory
	*/
	_BOOL FindFirstDir(const Stringz&, FindData&) const;

	/*! find the name of the next directory
		\return FALSE if there is no more directory
	*/
	_BOOL FindNextDir(FindData&) const;

	/*!	close the search session */
	void FindCloseDir(FindData&) const;

	/*! find the name of the first file
		\param Stringz: path to search for
		\param FindData: result
		\return FALSE is there is no file
	*/
	_BOOL FindFirstFile(const Stringz&, FindData&) const;

	/*!  find the name of the next file
		\return FALSE if there is no more file
	*/
	_BOOL FindNextFile(FindData&) const;

	/*!	close the search session */
	void FindCloseFile(FindData&) const;
};

}

#endif 
