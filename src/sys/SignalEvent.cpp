/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1999-2001 Henri Michelon

$Id: SignalEvent.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
----------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#ifndef _MSC_VER
# include <signal.h>
#endif

SignalEvent* SignalEvent::_sig_handler = NULL;

void nmo_signal_handler(int SIG)
{
	SignalEvent::Signal sig = SignalEvent::NONE;
	switch (SIG)
	{
	case SIGINT:
		sig = SignalEvent::INTERRUPT;
		break;
	case SIGABRT:
		sig = SignalEvent::ABORT;
		break;
	case SIGSEGV:
		sig = SignalEvent::SEGVIOLATION;
		break;
	case SIGTERM:
		sig = SignalEvent::TERMINATE;
		break;
	case SIGILL:
		sig = SignalEvent::ILLEGALINSTR;
		break;
	}
	if (SignalEvent::_sig_handler != NULL)
		SignalEvent::_sig_handler->OnSignal(sig);
}

const int SIGEQUIV[] = {0, SIGABRT, SIGINT, SIGSEGV, SIGILL, SIGTERM };


//-------------------------------------
void SignalEvent::Unredirect(Signal SIG)
{
	signal(SIGEQUIV[SIG], SIG_DFL);
}


//-------------------------------------
void SignalEvent::Redirect(Signal SIG)
{
#ifdef sgi
	signal(SIGEQUIV[SIG], (void (*)())nmo_signal_handler);
#else
	signal(SIGEQUIV[SIG], nmo_signal_handler);
#endif
	_sig_handler = this;
}


//-------------------------------------
void SignalEvent::Ignore(Signal SIG)
{
	signal(SIGEQUIV[SIG], SIG_IGN);
}


