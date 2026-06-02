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
#ifndef _NMO_SYS_FILE_H_
#define _NMO_SYS_FILE_H_

namespace NMO {

/*!
\brief		basic disk I/O and file name manipulation

\author		(c) 1998-2001 Henri Michelon 	
\version	$Id: File.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/

class File: public NMOObject
{
public:

	/*! File opening mode */
	typedef enum
	{
		NONE,
		//! Read only
		READ,
		//! Write only
		WRITE,
		//! Read write, positioned on start
		READWRITE,
		//! Read write, positioned on end
		APPEND,
		//! Creation, write only
		CREATE
	} FileMode;

	/*! Mode for Seek() */
	typedef enum
	{
		//! From start
		START,
		//! From end
		END,
		//! From current position
		CURRENT
	} SeekMode;

	/*! File informations */
	typedef struct
	{
		//!	Size in bytes
		_DWORD		size;
		//! Last modification date/time
		DateTime	modifDate;
		//! Creation date/time
		DateTime	creatDate;
		//! TRUE if file is a directory
		_BOOL		isDirectory;
		//! TRUE id read access is granted to user
		_BOOL		readAccess;
		//! TRUE id write access is granted to user
		_BOOL		writeAccess;
		//! TRUE id execution is granted to user
		_BOOL		execAccess;
#ifdef USERSECURITY
		//! User name on multi user systems
		Stringz		user;
		//! group name on multi user systems
		Stringz		group;
#endif
	} FileStats;

	/*! Open a file by is name */
	static File *Open(const Stringz&, const FileMode = READ);

	/*! Duplicate a file object */
	static File *Open(const File *, const Stringz&);

	virtual ~File();

	/*! Read bytes from the file to a buffer
		\param _PTR:	buffer address
		\param _LONG:	number of bytes
		\return the number of bytes really read, -1 on error
	*/
	virtual _LONG Read(const _PTR, const _LONG);

	/*! Write bytes from a buffer to the file
		\param _PTR:	buffer address
		\param _LONG:	number of bytes
		\return the number of bytes really written, -1 on error
	*/
	virtual _LONG Write(const _PTR, const _LONG);

	/*! Read file until LF (10).
		CR (13) chars are ignored*/
	virtual _BOOL Readln(Stringz&S);

	/*! write a formatted string into the file */
	virtual void Writeln(char*, ...);

	/*! Return the current position (from the start, 0 based) */
	_LONG Position() const;

	/*! Set the current position.
		\param	_LONG	: offset in bytes, 0 based
		\param	SeekMode : offset start
		\return FALSE on error
	*/
	_BOOL Seek(const _LONG, const SeekMode = START);

	/*! is end of file reached ? */
	_BOOL Eof() const;

	/*! Return file descriptor */
	_PTR Handle() const;

	/*! Read statistics about the currently open file.
		\return FALSE on error
	*/
	_BOOL Stats(FileStats&);

	/*! return the file name and extension */
	Stringz FileName();

	/*! return the real file path (without name) */
	Stringz FilePath();

	/*! return the current mode of the file */
	FileMode Mode() const;

	/*! extract the file extension (without dot) */
	static Stringz FileExt(const Stringz&);

	/*! extract the file name and extension */
	static Stringz FileName(const Stringz&);

	/*! extract the real file path (without name) */
	static Stringz FilePath(const Stringz&);

	/*! Read statistics about a file.
		\param	Stringz	: file name
		\param	FileStat: stats
		\return FALSE on error
	*/
	static _BOOL Stats(const Stringz&, FileStats&);

	/*! return TRUE if the file exists
		\param Stringe	: file name
	*/
	static _BOOL Exists(const Stringz&);

	/*! Set the date (modif & last access) of a file
		\param Stringz	: file name
			\param DateTime :	Modification date&time
	*/
	static _BOOL SetDateTime(const Stringz&, const DateTime&);

	/*! Remove (delete, unlink) a file.
		\param Stringz: file name
	*/
	static _BOOL Remove(const Stringz&);

	/*! Copy a file
		\param Stringz: source file name
		\param Stringz: destination file name
		\return FALSE on error
	*/
	static _BOOL CopyFile(const Stringz&, const Stringz&);

	void Close();

protected:
	_PTR		handle;
	Stringz		name;
	FileMode	mode;

	static _PTR Mode(FileMode);
	inline File(): handle(NULL) {};
	inline File(_PTR H, Stringz N): handle(H), name(N) {};
};

inline _PTR File::Handle() const { return handle; };

}

#endif
