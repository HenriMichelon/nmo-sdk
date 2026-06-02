/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nwww.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1999-2001 Henri Michelon

$Id: SystemPosix.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
---------------------------------------------------------------------*/
#include "nmo/NMO.hpp"
#include <sys/types.h>
#include <sys/wait.h>
using namespace NMO;


//------------------------------------------------------
_BOOL System::Execute(const Stringz &PATH, Args&ARGS, ExecMode MODE, _PID *PID)
{
	_LONG i=0;
	_LONG child=0;
	char** argv = new char*[ARGS.Count()+1];

	ListIterator<Stringz> list(ARGS);
	while (!list.End()) {
		argv[i++] = (char*)list.Next();
	}
	argv[i] = NULL;
	switch (MODE)
	{
	case EXEC_NORMAL:
	case EXEC_BACKGROUND:
#ifdef _BSD
		child = vfork();
#else
		child = fork();
#endif
		break;
	case EXEC_OVERLAY:
		break;
	}
	if (child)
	{
		if (MODE == EXEC_NORMAL)
		{
			int status;
			waitpid(child, &status, 0);
		}
		else if (PID != NULL)
			*PID = child;
	}
	else
		execvp((char*)PATH, argv);
	delete []argv;
	return TRUE;
}
