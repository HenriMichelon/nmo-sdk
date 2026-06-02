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
#ifndef _NMO_WINAPP_H_
#define _NMO_WINAPP_H_

namespace NMO {
/*!
  \brief	Super class for windowed applications classes

  			This class is used to write windows, graphical applications.
			Used in conjonction with the Interface package, you can
			draw graphics and text into the windows.\n
			Consider the GUI addon package for a real GUI system.

\author		(c) 1998-2002 Henri Michelon
\version	$Id: WindowApplication.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class WindowApplication: public Application
{
public:
	/*! Create an application with the given application name (aka title) */
	WindowApplication(const UStringz &N);
	virtual ~WindowApplication();

	virtual void SetName(const UStringz&);
	virtual void Run();
	virtual void Terminate();
	virtual void ProcessNextEvent(IWindow*);
	_BOOL Terminated() const;

	/*! Add a window to the application.\n
		Window is created but not shown
		\param IWindow		: (in) window to add
		\param WindowType	: (in) type of border for window
		\param Stringz		: (int) additional parameter,
								like the display driver to use.\n
								Win32: can be "DIB" (default) or "DirectDraw"
	*/
	_BOOL Add(IWindow&, IWindow::WindowType = IWindow::NORMAL, const Stringz& = "");

	/*! Close all existing (visible or not) windows */
	void CloseAllWindows();

 // public for callbacks
#if defined(_WIN32) || defined(_MACOS) || defined(_QNX)
	_PTR	mWindowApplicationAbstract;
#endif
private:
	_BOOL	mQuit;
	_BOOL	OpenDisplayEnv();

	void CloseDisplayEnv();
	_BOOL Create(IWindow::Link&, IWindow::WindowType, const Stringz&);
};

}

extern void win_main_done();

#undef CreateApplication
#ifdef _WIN32
extern void win_main_init(HINSTANCE);
#define CreateApplication(T, N) int PASCAL WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int) { win_main_init(hInst); try { T app(N); app.Run(); } catch (NMOException e) { e.Display(); } nmo_main_done(); return 0; }
#else
#define CreateApplication(T, N) NmoCreateApplication(T, N)
#endif

#endif
