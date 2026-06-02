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

$Id: EnvPathCarbon.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
using namespace NMO;


Stringz EnvPath::ExtractPath(short vrefnum, long dirID)
{
	Stringz 	result;
	Str255		dirname;
	_CHAR		cdirname[260];
	CInfoPBRec 	pbrec;
	pbrec.dirInfo.ioNamePtr = (StringPtr)&dirname;
	pbrec.dirInfo.ioVRefNum = vrefnum;
	pbrec.dirInfo.ioDrParID = dirID;
	pbrec.dirInfo.ioFDirIndex = -1;
	do {
		pbrec.dirInfo.ioDrDirID = pbrec.dirInfo.ioDrParID;
		if (::PBGetCatInfoSync(&pbrec) == noErr) {
			CopyPascalStringToC(dirname, cdirname);
			if (result.Len() > 0) {
				result = Stringz(cdirname) + PATHSEP + result;
			}
			else {
				result = Stringz(cdirname);
			}
		}
	} while (pbrec.dirInfo.ioDrDirID != fsRtDirID);
	return result;
}


EnvPath::EnvPath(PathType TYPE)
{
	OSType folderType;
	switch (TYPE) {
	case PATH_BIN:
	case PATH_SHARE:
		folderType = kSystemFolderType;
		break;
	case PATH_TEMP:
		folderType = kTemporaryFolderType;
		break;
	case PATH_CONF:
		folderType = kPreferencesFolderType;
		break;
	case PATH_LIB:
		folderType = kExtensionFolderType;
		break;
	case PATH_APPS:
	default:
		folderType = kDesktopFolderType;
		break;
	}
	short vrefnum;
	long dirID;
	OSErr err = FindFolder(kOnSystemDisk,
							folderType,
							kCreateFolder,
							&vrefnum,
							&dirID);
	if (err != noErr) {
		FindFolder(kOnSystemDisk,
					kStartupFolderType,
					kCreateFolder,
					&vrefnum,
					&dirID);
	}
	Add(new Stringz(ExtractPath(vrefnum, dirID)));
}
#endif
