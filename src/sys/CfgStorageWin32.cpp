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

						Configuration storage use
						registry under Win32.

						Does not support this datatypes:
							REG_BINARY
							REG_NONE
							REG_LINK
							REG_RESOURCE_LIST
						Since configuration storage use only
						strings on others systems

						Root key is HKEY_xxx\Software\NAME


  TODO: use WCHAR functions under WinNT

	(c) 2000-2001 Henri Michelon

$Id: CfgStorageWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <winreg.h>
#endif

typedef struct {
	HKEY	hkey;
	HKEY	handle;
} CfgStorageAbstract;

#define __m(f) ((CfgStorageAbstract*)(this->mCfgStorageAbstract))->f


typedef struct TInfoKey
{
	DWORD nbSubKey;			// Number of sub keys
	DWORD maxLenSubKeyName;	// Max length of a sub key name
	DWORD nbValues;			// Number of values in the key
	DWORD maxLenValueName;	// Max length of a value name
	DWORD maxLenValue;		// Max length of a value
} TInfoKey;

//----------------------------------------------
CfgStorage::CfgStorage() {
	mCfgStorageAbstract = new CfgStorageAbstract;
	__m(handle) = NULL;
	__m(hkey) = NULL;
}


//----------------------------------------------
CfgStorage::~CfgStorage() {
	Close();
	delete (CfgStorageAbstract*)mCfgStorageAbstract;
}


//----------------------------------------------
_BOOL CfgStorage::Open(const Stringz &NAME, CfgStorage::CfgStorageKind KIND)
{
	Close();
	
	// Select root key
	HKEY rootkey;
	if (KIND == SYSTEM) {
		rootkey = HKEY_LOCAL_MACHINE;
	}
	else {
		rootkey = HKEY_CURRENT_USER;
	}

	// Connect to local registry
	LONG res = RegConnectRegistry (NULL, rootkey, &__m(handle));
	if (res == ERROR_SUCCESS) {
		Stringz keyname = "Software\\" + NAME;
		res = RegCreateKeyEx (__m(handle),
                        (_CHAR*)keyname,
						0,
						NULL,
						REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS,
						NULL,
                        &__m(hkey),
						NULL);
		if (res != ERROR_SUCCESS) {
			Stringz keyname = "\\Software\\" + NAME;
			res = RegCreateKeyEx (__m(handle),
							(_CHAR*)keyname,
							0,
							NULL,
							REG_OPTION_NON_VOLATILE, 
							KEY_ALL_ACCESS,
							NULL,
							&__m(hkey),
							NULL);
		}
		if (res == ERROR_SUCCESS) { return TRUE; }
		RegCloseKey(__m(handle));
	}

	__m(handle) = NULL;
	__m(hkey) = NULL;
	return FALSE;
}


//----------------------------------------------
void CfgStorage::Close()
{
	if (__m(hkey)) {
		RegFlushKey(__m(hkey));
		RegCloseKey(__m(hkey));
		__m(hkey) = NULL;
		if (__m(handle)) {
			RegFlushKey(__m(handle));
			RegCloseKey(__m(handle));
			__m(handle) = NULL;
		}
	}
}


//----------------------------------------------
void CfgStorage::Load(Iterator<Stringz>&LIST)
{
	_LONG pos;
	Stringz line, keyname, valname;

	LIST.Reset();
	while (!LIST.End()) {
		line = LIST.Next();
		pos = line.InStr('*');
		if (pos != -1) {
			keyname = line.Copy(0, pos);
			line = line.Copy(pos+1, line.Len()-pos-1);
			pos = line.InStr('=');
			if (pos != -1) {
				valname = line.Copy(0, pos);
				line = line.Copy(pos+1, line.Len()-pos-1);
				Write(keyname, valname, line);
			}
		}
	}
}


//----------------------------------------------
void CfgStorage::Write(const Stringz&KEY, const Stringz&NAME, const Stringz&VAL)
{
	BOOL	_opened;
	HKEY	_phOpened;
	LONG	res = -1;
	
	if (__m(hkey)) {
		res = RegCreateKeyEx (__m(hkey),
                        (_CHAR*)KEY,
						0,
						NULL,
						REG_OPTION_NON_VOLATILE,
                        KEY_ENUMERATE_SUB_KEYS |
                        KEY_EXECUTE |
                        KEY_QUERY_VALUE |
						KEY_WRITE,
						NULL,
                        &_phOpened,
						NULL);
	}
	_opened = (res == ERROR_SUCCESS);
	
	if (_opened) {
		res =  RegSetValueEx (_phOpened, 
						(_CHAR*)NAME, 
						0,
						REG_SZ,
						(BYTE*)(_CHAR*)VAL, 
						VAL.Len());
		RegCloseKey(_phOpened);
	}
}


//----------------------------------------------
Stringz CfgStorage::Read(const Stringz&KEY, const Stringz&NAME, const Stringz&DEF)
{
	BOOL	_opened;
	HKEY	_phOpened;
	LONG	res = -1;
	
	if (__m(hkey)) {
		res = RegOpenKeyEx (__m(hkey),
                        (_CHAR*)KEY,
						0,
                        KEY_READ,
                        &_phOpened);
	}
	_opened = (res == ERROR_SUCCESS);

	if (_opened) {
		_DWORD type;
		_DWORD size;
		res = RegQueryValueEx (_phOpened, 
						(_CHAR*)NAME, 
						0,
						&type,
						NULL, 
						&size);

		if (res != ERROR_SUCCESS) {
			RegCloseKey(_phOpened);
			return DEF;
		}
		Stringz result(_DWORD(size + 1));

		res = RegQueryValueEx (_phOpened, 
						(_CHAR*)NAME, 
						0,
						&type,
						(_BYTE*)(_CHAR*)result, 
						&size);

		RegCloseKey(_phOpened);

		switch (type) {
		case REG_DWORD:
		//case REG_DWORD_LITTLE_ENDIAN:
			result.Format("%d", LE2LONG(*((_LONG*)(_CHAR*)result)));
			return result;
			break;
		case REG_DWORD_BIG_ENDIAN:
			result.Format("%d", BE2LONG(*((_LONG*)(_CHAR*)result)));
			return result;
			break;
		case REG_EXPAND_SZ:
		case REG_SZ:
			return result;
			break;
		case REG_MULTI_SZ:
			// XXX: return only the first string
			return result;
			break;
		default:
			// others return default value
			break;
		}
	}
	return DEF;
}


//----------------------------------------------
void CfgStorage::Read(const Stringz&KEY, List<Stringz>&LIST)
{
	BOOL	_opened;
	HKEY	_phOpened;
	LONG	res = -1;
	
	if (__m(hkey)) {
		res = RegOpenKeyEx (__m(hkey),
                        (_CHAR*)KEY,
						0,
                        KEY_ENUMERATE_SUB_KEYS |
                        KEY_EXECUTE |
                        KEY_QUERY_VALUE,
                        &_phOpened);
	}
	_opened = (res == ERROR_SUCCESS);

	if (_opened) {
		TInfoKey IK;
		res = RegQueryInfoKey(_phOpened,
							NULL,
							NULL,
							NULL,
							NULL,
							NULL,
							NULL,
							&IK.nbValues,
							&IK.maxLenValueName,
							NULL,
							NULL,
							NULL);
		if (!res) return;

		_CHAR *name = new _CHAR[IK.maxLenValueName+1];

		for (_DWORD index = 0; index < IK.nbValues; index ++) {
			_DWORD size = IK.maxLenValueName+1;
			res = RegEnumValue(_phOpened, 
								index,
								name, 
								&size,
								NULL,
								NULL,
								NULL,
								NULL);
			if (res) {
				// Let Read() handle data convertion
				LIST.Add(new Stringz(KEY + '*' + 
									 name + '=' + 
									 Read(KEY, name)));
			}
		}
		RegCloseKey(_phOpened);
	}
}


//----------------------------------------------
void CfgStorage::Read(List<Stringz>&LIST)
{
	LONG	res = -1;
	
	if (__m(hkey)) {
		TInfoKey IK;
		res = RegQueryInfoKey(__m(hkey),
							NULL,
							NULL,
							NULL,
							&IK.nbSubKey,
							&IK.maxLenSubKeyName,
							NULL,
							NULL,
							NULL,
							NULL,
							NULL,
							NULL);
		if (!res) return;

		_CHAR *name = new _CHAR[IK.maxLenSubKeyName+1];

		for (_DWORD index = 0; index < IK.nbSubKey; index ++) {
			_DWORD size = IK.maxLenSubKeyName+1;
			res = RegEnumKeyEx(__m(hkey), 
								index, 
								name, 
								&size,
								NULL,
								NULL,
								NULL,
								NULL);
			Read(name, LIST);
		}
	}

}


//----------------------------------------------
void CfgStorage::Delete(const Stringz&KEY, const Stringz&NAME)
{
	BOOL	_opened;
	HKEY	_phOpened;
	LONG	res = -1;
	
	if (__m(hkey)) {
		res = RegCreateKeyEx (__m(hkey),
                        (_CHAR*)KEY,
						0,
						NULL,
						REG_OPTION_NON_VOLATILE,
                        KEY_ENUMERATE_SUB_KEYS |
                        KEY_EXECUTE |
                        KEY_QUERY_VALUE |
						KEY_WRITE,
						NULL,
                        &_phOpened,
						NULL);
	}
	_opened = (res == ERROR_SUCCESS);
	
	if (_opened) {
		res =   RegDeleteValue (_phOpened, (_CHAR*)NAME);
		if (res == ERROR_SUCCESS) RegFlushKey(__m(hkey));
		RegCloseKey(_phOpened);
	}
}


//----------------------------------------------
void CfgStorage::Delete(const Stringz&KEY)
{
	LONG	res = -1;
	
	if (__m(hkey)) {
		res =   RegDeleteKey (__m(hkey), (_CHAR*)KEY);
		if (res == ERROR_SUCCESS) RegFlushKey(__m(hkey));
	}
}
#endif
