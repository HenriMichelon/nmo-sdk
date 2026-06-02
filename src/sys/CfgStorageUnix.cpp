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

$Id: CfgStorageUnix.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

#ifdef _BEOS
	const _CHAR basedir[] = "/boot/etc/";
	const _CHAR extension[]= "rc";
	const _CHAR prefix[]= ".";
#elif defined(_AMIGA)
		const _CHAR extension[]= ".preferences";
		const _CHAR basedir[] = "ENVARC:";
		const _CHAR prefix[]= "";
#elif defined(_MACOS)
	#ifdef __MACH__
		const _CHAR extension[]= "rc";
		const _CHAR basedir[] = "var:db";
		const _CHAR prefix[]= ".";
	#else
		const _CHAR extension[]= " Preferences";
		static _CHAR basedir[260];
		const _CHAR prefix[]= "";
	#endif
#else
	const _CHAR extension[]= "rc";
	const _CHAR basedir[] = "/var/db/";
	const _CHAR prefix[]= ".";
#endif

typedef struct {
	Stringz		path;
	FileCfg		cfg;
} cfgstoabstract;

#define __m(f) ((cfgstoabstract*)(this->mCfgStorageAbstract))->f


//----------------------------------------------
CfgStorage::CfgStorage() {
	mCfgStorageAbstract = new cfgstoabstract;
#if defined(_MACOS) && ! defined(__MACH__)
	short vrefnum;
	long dirID;
	OSErr err = FindFolder(kOnSystemDisk,
							kPreferencesFolderType,
							kCreateFolder,
							&vrefnum,
							&dirID);
	if (err != noErr) {
		FindFolder(kOnSystemDisk,
					kSystemPreferencesFolderType,
					kCreateFolder,
					&vrefnum,
					&dirID);
	}
	if (err != noErr) {
		FindFolder(kOnSystemDisk,
					kSystemFolderType,
					kCreateFolder,
					&vrefnum,
					&dirID);
	}
	Stringz result(EnvPath::ExtractPath(vrefnum, dirID));
	Stringz::Strcpy(basedir, result);	
#endif // _MACOS
}


//----------------------------------------------
CfgStorage::~CfgStorage() {
	delete (cfgstoabstract*)mCfgStorageAbstract;
}


//----------------------------------------------
_BOOL CfgStorage::Open(const Stringz &NAME, CfgStorage::CfgStorageKind KIND)
{
	Stringz dir(_DWORD(256));
	if (KIND == SYSTEM) {
		dir = basedir;
	}
	else {
		dir = SysInfo::Userhome();
	}
	__m(path) = dir;
	if (dir.Len() == 0) {
		dir = Dir::Current();
	}
	Stringz right = __m(path).Right(1);
	if ((right != PATHSEP) && (right != ROOTDIR)) {
		__m(path) += PATHSEP;
	}
	__m(path) = __m(path) + prefix + NAME + extension;
	Dir::Create(dir);
	if (!Dir::IsWriteable(dir)) {
		Debug(dprintf("CfgStorage::Open: not writeable dir %s\n", (_CHAR*)dir);)
		return FALSE;
	}
	__m(cfg).Clear();
	__m(cfg).Load(__m(path));
	return TRUE;
}


//----------------------------------------------
void CfgStorage::Load(Iterator<Stringz>&LIST)
{
	__m(cfg).Load(LIST);
}


//----------------------------------------------
void CfgStorage::Write(const Stringz&KEY, const Stringz&NAME, const Stringz&VAL)
{
	__m(cfg).Write(KEY, NAME, VAL);
	__m(cfg).Save();
}


//----------------------------------------------
Stringz CfgStorage::Read(const Stringz&KEY, const Stringz&NAME, const Stringz&DEF)
{
	return __m(cfg).Read(KEY, NAME, DEF);
}


//----------------------------------------------
void CfgStorage::Read(const Stringz&KEY, List<Stringz>&LIST)
{
	__m(cfg).Read(KEY, LIST);
}


//----------------------------------------------
void CfgStorage::Read(List<Stringz>&LIST)
{
	__m(cfg).Read(LIST);
}


//----------------------------------------------
void CfgStorage::Delete(const Stringz&KEY, const Stringz&NAME)
{
	__m(cfg).Delete(KEY, NAME);
	__m(cfg).Save();
}


//----------------------------------------------
void CfgStorage::Delete(const Stringz&KEY)
{
	__m(cfg).Delete(KEY);
	__m(cfg).Save();
}

