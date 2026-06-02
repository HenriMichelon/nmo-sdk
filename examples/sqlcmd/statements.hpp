/*
(c) 1999-2002 Henri Michelon

$Id: statements.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _SQLCMD_STMTS_H_
#define _SQLCMD_STMTS_H_

namespace NMO {


class Statement: public ShellCommand
{
public:
	Statement(const Stringz&N, const char*H, const Stringz&A): 
	  ShellCommand(N, H, A) {};
	void Execute(ListIterator<Stringz>&);

private:
	void Display(Array<DBColumn>&, List<DBRecord>&);
};

#define STMT(n, x, h) class x: public Statement{ public: x(const Stringz& N = n, const char*H=h, const Stringz&A=n): Statement(N, H, A) {}; void Execute(ListIterator<Stringz>&); };

STMT("create", StmtCreate, LC_SQL_HELPCREATE);
STMT("select", StmtSelect, LC_SQL_HELPSELECT);
STMT("insert", StmtInsert, LC_SQL_HELPINSERT);
STMT("update", StmtUpdate, LC_SQL_HELPUPDATE);
STMT("drop", StmtDrop, LC_SQL_HELPDROP);
STMT("alter", StmtAlter, LC_SQL_HELPALTER);
STMT("delete", StmtDelete, LC_SQL_HELPDELETE);

}

#endif
