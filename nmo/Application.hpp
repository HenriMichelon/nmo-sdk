/*                             ---------
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
#ifndef _NMO_APP_H_
#define _NMO_APP_H_

namespace NMO {
/*!
\brief	Super class for all kind of applications

		All applications classes inherit from this class.\n
		They must implement at least three methods:\n
			+ SetName() which set the name of the application\n 
			 (for example, in the title bar of a window)\n
			+ Run() which contains the main application loop\n
			+ Terminate() called to exit immediately from the application\

		To create the main instance of your application class, use the
		following statement:

		CreateApplication(MyAppClass, "Name of my application");

\author	(c) 1998-2002 Henri Michelon 
\version $Id: Application.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Application: public NMOObject
{
public:
	//! Create an application with a given name (aka title)
	Application(const UStringz &);

	virtual ~Application() {};

	//! Get command line argument list
	static Args& Argv();

	//! Get application current path
	static const Stringz& Path();

	//! Set application current path
	static void SetPath(const _CHAR*);

	//! Return the name of the application
	const UStringz& Name() const;

	//! Return the current application
	static Application& App();

	//! Set the name of the application
	virtual void SetName(const UStringz&);

	//! Enter the application main loop
	virtual void Run() = 0;

	//! Exit the application main loop
	virtual void Terminate() = 0;

	//! Create event: called before entering the message loop
	virtual void OnCreate() {};

	//! Destroy event: called after exiting the message loop
	virtual void OnDestroy() {};

private:
	static Application	*app;
	static Stringz		path;
	static Args			args;
	UStringz			appName;
};

inline Application& Application::App() { return (Application&)*app; };

}

void nmo_main_init(int, char**, const char*dgo);
int nmo_main_done();
#ifdef __DEBUG
#define NmoCreateApplication(T, N) int main(int argc, char**argv) { nmo_main_init(argc, argv, _nmodebugoutput); try { T app(N); app.Run(); } catch (NMOException e) { e.Display(); } return nmo_main_done(); }
#else
#define NmoCreateApplication(T, N) int main(int argc, char**argv) { nmo_main_init(argc, argv, NULL); try { T app(N); app.Run(); } catch (NMOException e) { e.Display(); } return nmo_main_done(); }
#endif


#endif
