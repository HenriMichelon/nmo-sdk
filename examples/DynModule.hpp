/*
$Id: DynModule.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef __APP_H_
#define __APP_H_

class Dynload: public StdioApplication
{
public:
	Dynload(UStringz N): StdioApplication(N) {};

	
private:
	void OnCreate();
};

#endif
