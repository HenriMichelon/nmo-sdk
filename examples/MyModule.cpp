// MyModule.cpp : Defines the entry points for the dynamic library.
// $Id: MyModule.cpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
#include <nmo/NMO.hpp>
#include <nmo/SharedObject.hpp>
using namespace NMO;

class MyClass: public SharedObject {
public:
	MyClass(): count(0) {}

	char* MyMethod (char*STR) {
		int i = 0;
		while (STR[i]) STR[i++] += 'A' - 'a';
		return STR;
	}

	int GetMyData() {
		return ++count;
	}

private:
	_LONG count;
};

NmoCreateSharedObject(MyClass, myclass);

NmoExport char* MyMethod(char*STR) { return myclass.MyMethod(STR); }
NmoExport int GetMyData() { return myclass.GetMyData(); }
