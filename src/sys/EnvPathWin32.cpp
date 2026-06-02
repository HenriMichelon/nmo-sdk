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

	(c) 2000-2001 Henri Michelon

$Id: EnvPathWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;


EnvPath::EnvPath(PathType TYPE)
{
	switch (TYPE) {
	case PATH_BIN:
		{
			Stringz path;
			if (Env::Read("PATH", path)) {
				path.Split(*this, ';');
			}
		}
		break;
	case PATH_TEMP:
		{
			Stringz path;
			if (Env::Read("TEMP", path)) {
				path.Split(*this, ';');
			}
			else if (Env::Read("TMP", path)) {
				path.Split(*this, ';');
			}
		}
		break;
	case PATH_SHARE:
		{
			_CHAR path[MAX_PATH];
			if (GetWindowsDirectoryA(path, MAX_PATH)) {
				Add(new Stringz(path));
			}
		}
		break;
	case PATH_CONF:
		{
			Add(new Stringz(Dir::Current()));
			_CHAR path[MAX_PATH];
			if (GetWindowsDirectoryA(path, MAX_PATH)) {
				Add(new Stringz(path));
			}
		}
		break;
	case PATH_LIB:
		{
			_CHAR path[MAX_PATH];
			if (GetSystemDirectoryA(path, MAX_PATH)) {
				Add(new Stringz(path));
			}
		}
		break;
	case PATH_APPS:
	default:
		{
			_CHAR path[MAX_PATH];
			if (GetWindowsDirectoryA(path, MAX_PATH)) {
				Stringz mypath(path);
				mypath = mypath.Left(3) + "Program Files";
				Add(new Stringz(mypath));  // Maybe in the registry ??
			}
		}
		break;
	}
}
#endif
