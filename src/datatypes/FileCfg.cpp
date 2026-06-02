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

	(c) 1999-2001 Henri Michelon

$Id: FileCfg.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/datatypes/FileTxt.hpp>
#include <nmo/datatypes/FileCfg.hpp>
using namespace NMO;

class FileCfgAbstract {
private:
	friend class FileCfg;

	class CfgValue
	{
	public:
		Stringz name;
		Stringz value;

		CfgValue(const Stringz&N, const Stringz&K):
			name(N), value(K) {};

	};

	friend _BOOL operator == (const CfgValue&A, const CfgValue&B)
	{ return (A.name == B.name); };
	
	class CfgKey
	{
	public:
		NMO::List<CfgValue> values;
		Stringz name;

		CfgKey(const Stringz&N): name(N) {};

	};
	
	_BOOL			modified;
	Stringz			fileName;
	NMO::List<CfgKey>	keys;
	
	CfgKey* Key(const Stringz&);
	CfgValue* Value(CfgKey&, const Stringz&);

	friend _BOOL operator == (const CfgKey&A, const CfgKey&B)
	{ return (A.name == B.name); };
};

#define _m(x) ((FileCfgAbstract*)(this->mFileCfgAbstract))->x

//----------------------------------------------
_BOOL FileCfg::Load(const Stringz&NAME)
{
	_m(fileName) = NAME;
	List<Stringz> list;
	if (FileTxt::Load(NAME, list)) {
		ListIterator<Stringz> lst(list);
		Expand(lst);
		_m(modified) = FALSE;
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------
FileCfg::FileCfg()
{
	mFileCfgAbstract = new FileCfgAbstract;
	_m(modified) = FALSE;
}


//----------------------------------------------
FileCfg::FileCfg(const Stringz &NAME)
{
	mFileCfgAbstract = new FileCfgAbstract;
	Load(NAME);
}


//----------------------------------------------
void FileCfg::Load(Iterator<Stringz>&LIST, const Stringz&NAME)
{
	_m(fileName) = NAME;
	Expand(LIST);
}


//----------------------------------------------
FileCfg::FileCfg(Iterator<Stringz>&LIST, const Stringz&NAME)
{
	Load(LIST, NAME);
}


//----------------------------------------------
FileCfg::~FileCfg()
{
	Save();
	Clear();
	delete (FileCfgAbstract*)mFileCfgAbstract;
}


//----------------------------------------------
void FileCfg::Clear()
{
	_m(keys).Clear();
}


//----------------------------------------------
FileCfgAbstract::CfgKey* FileCfgAbstract::Key(const Stringz&KEY)
{
	ListIterator<CfgKey> list(keys);
	while (!list.End()) {
		CfgKey &key = list.Next();
		if (key.name == KEY) {
			return &key;
		}
	}
	return NULL;
}


//----------------------------------------------
FileCfgAbstract::CfgValue* FileCfgAbstract::Value(FileCfgAbstract::CfgKey&KEY, const Stringz&NAME)
{
//	operator ==
	ListIterator<CfgValue> list(KEY.values);
	while (!list.End()) {
		CfgValue &val = list.Next();
		if (val.name == NAME) {
			return &val;
		}
	}
	return NULL;
}


//----------------------------------------------
void FileCfg::Expand(Iterator<Stringz>&LIST)
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
void FileCfg::Write(const Stringz&KEY, const Stringz&NAME, const Stringz&VAL)
{
	Stringz key(KEY);
	Stringz name(NAME);
	Stringz val(VAL);

	key.Trim();
	name.Trim();
	val.Trim();
	ListIterator<FileCfgAbstract::CfgKey> lstkeys(_m(keys));
	FileCfgAbstract::CfgKey *cfgkey = _m(Key)(key);
	if (!cfgkey)
		cfgkey = &lstkeys[_m(keys).Add(new FileCfgAbstract::CfgKey(key))];
	FileCfgAbstract::CfgValue *cfgval = _m(Value)(*cfgkey, name);
	if (!cfgval)
		cfgkey->values.Add(new FileCfgAbstract::CfgValue(name, val));
	else
		cfgval->value = val;
	_m(modified) = TRUE;
}


//----------------------------------------------
Stringz FileCfg::Read(const Stringz&KEY, const Stringz&NAME, const Stringz&DEF)
{
	FileCfgAbstract::CfgKey *key = _m(Key)(KEY);
	if (key) {
		FileCfgAbstract::CfgValue *val = _m(Value)(*key, NAME);
		if (val) {
			return val->value;
		}
	}
	return DEF;
}


//----------------------------------------------
void FileCfg::Read(const Stringz&KEY, List<Stringz>&LIST)
{
	FileCfgAbstract::CfgKey *key = _m(Key)(KEY);
	if (key) {
		ListIterator<FileCfgAbstract::CfgValue> list(key->values);
		while (!list.End()) {
			FileCfgAbstract::CfgValue &val = list.Next();
			LIST.Add(new Stringz(KEY+'*'+val.name+'='+val.value));
		}
	}
}


//----------------------------------------------
void FileCfg::Read(List<Stringz>&LIST)
{
	ListIterator<FileCfgAbstract::CfgKey> lstkeys(_m(keys));
	while (!lstkeys.End()) {
		FileCfgAbstract::CfgKey &key = lstkeys.Next();
		ListIterator<FileCfgAbstract::CfgValue> lstvalues(key.values);
		while (!lstvalues.End()) {
			FileCfgAbstract::CfgValue &val = lstvalues.Next();
			LIST.Add(new Stringz(key.name+'*'+val.name+'='+val.value));
		}
	}
}


//----------------------------------------------
_BOOL FileCfg::Save()
{
	if (!_m(modified)) { return TRUE; }
	_m(modified) = FALSE;
	if (_m(fileName).Len()) {
		List<Stringz> list;
		Read(list);
		ListIterator<Stringz> lst(list);
		if (list.Count()) {
			return FileTxt::Save(_m(fileName), lst);
		}
	}
	return FALSE;
}


//----------------------------------------------
void FileCfg::Delete(const Stringz&KEY, const Stringz&NAME)
{
	FileCfgAbstract::CfgKey *key = _m(Key)(KEY);
	if (key) {
		FileCfgAbstract::CfgValue *val = _m(Value)(*key, NAME);
		if (val) {
			ListIterator<FileCfgAbstract::CfgValue> list(key->values);
			key->values.Drop(list.IndexOf(*val));
			_m(modified) = TRUE;
		}
	}
}


//----------------------------------------------
void FileCfg::Delete(const Stringz&KEY)
{
	FileCfgAbstract::CfgKey *key = _m(Key)(KEY);
	if (key) {
		ListIterator<FileCfgAbstract::CfgKey> lstkeys(_m(keys));
		_m(keys).Drop(lstkeys.IndexOf(*key));
		_m(modified) = TRUE;
	}
}
