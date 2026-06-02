/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_SYS_PIPE_H_
#define _NMO_SYS_PIPE_H_

namespace NMO {

/*!
\brief		Bidirectional pipes

			Normally used with the File class.

\author		(c) 1999 Henri Michelon
\version	$Id: Pipe.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

#define _FD int

class Pipe: public NMOObject
{
public:
	Pipe();
	virtual ~Pipe();

	/*! Return the input file descriptor */
	const _FD Input() const;

	/*! Return the output file descriptor */
	const _FD Output() const;

private:
	_FD _fds[2];
};

inline const _FD Pipe::Input() const { return _fds[1]; };
inline const _FD Pipe::Output() const { return _fds[0]; };

}

#endif
