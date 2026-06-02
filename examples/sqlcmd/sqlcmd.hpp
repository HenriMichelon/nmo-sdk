/*
(c) 1999-2002 Henri Michelon

$Id: sqlcmd.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SQLCMD_APP_H_
#define _SQLCMD_APP_H_

namespace NMO {



class sqlcmd: public ShellApplication
{
public:
	sqlcmd(const UStringz&);

private:
	virtual void OnInit();
	virtual void OnDone();
	virtual void DisplayPrompt();
	virtual void ExtraCompletion(Stringz&, Stringz&, List<Stringz>&);
	virtual void Execute(const Stringz&, System::Args&, System::ExecMode);
};

}

#endif
