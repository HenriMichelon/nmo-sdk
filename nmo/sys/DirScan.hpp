/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_DIRS_SCAN_H_
#define _NMO_SYS_DIRS_SCAN_H_

namespace NMO {

/*!
\brief		Directory tree scanner

			Recursive search in a directory tree.

\author		(c) 1998-2000 Henri Michelon 
\version	$Id: DirScan.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

typedef List<Stringz> DirScanList;

class DirScan: private DirSearch
{
public:
	/*!	Scan a directory tree & fill a list with 
		directories names
		\param DirScanList : list of directories names (out)
		\param char*		: name of the root directory (in)
	*/
	void ScanTree(DirScanList&, const Stringz&) const;

	/*!	Scan a directory tree & fill a list with
		file names
		\param DirScanList : list of files names (out)
		\param DirScanList : list of directories names (in)
		\param DirScanList : list of extensions for files
							selection (in)
	*/
	void GetFileNames(DirScanList&, DirScanList&, DirScanList&) const;
};


}

#endif
