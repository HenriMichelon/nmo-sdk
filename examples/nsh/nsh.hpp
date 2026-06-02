/*
NMO Shell Addon Example	
(c) 1999-2002 Henri Michelon

$Id: nsh.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NSH_APP_H_
#define _NSH_APP_H_

namespace NMO {


class nsh: public ShellApplication
{
public:
	nsh(const UStringz&);

private:
	_DWORD _dwBackground;

	_BOOL InPath(Stringz&);

	virtual void ExpandCommandLine(System::Args&);
	virtual void DisplayPrompt();
	virtual void ExtraCompletion(Stringz&, Stringz&, List<Stringz>&);
	virtual void Execute(const Stringz&, System::Args&, System::ExecMode);
};

}

#endif
