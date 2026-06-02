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
#ifndef _NMO_SYS_CFGSTO_H_
#define _NMO_SYS_CFGSTO_H_

namespace NMO {

/*!
\brief		Configuration storage

			This class is used to store system wide or user 
			configuration values.
			
			Under some OS administrative rights are needed to modify
			system wide values.

\author		(c) 2000-2001 Henri Michelon 
\version	$Id: CfgStorage.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class CfgStorage: public NMOObject
{
public:
	/*! Where to store values */
	typedef enum {
		//! User specific
		USER,
		//! System (all users) wide
		SYSTEM
	} CfgStorageKind;

	CfgStorage();
	virtual ~CfgStorage();

	/*! Open a configuration entry.
		Entry is created if not exists
		\param Stringz			: (input) name of entry
		\param CfgStorageKind	: (input) location of the entry
		\return FALSE on error.
		Possible errors are: can't write to system storage
	*/	
	_BOOL Open(const Stringz&, CfgStorageKind = USER);

	/*! Close the configuratin entry.
		Automatically closed on destruction & Open()
	*/	
	void Close();

	/*! Fill the configuration entry from a collection of string.
		One string per value. Format of the strings:
		KEY*NAME=VALUE
	*/
	void Load(Iterator<Stringz>&);

	/*! Write a named value
		\param String	: (input) Key name
		\param String	: (input) Value name
		\param String	: (input) Value
	*/	
	void Write(const Stringz&, const Stringz&, const Stringz&);

	/*! Read a named value
		\param String	: (input) Key name
		\param String	: (input) Value name
		\param String	: (input) Default value
	*/	
	Stringz Read(const Stringz&, const Stringz&, const Stringz& ="");

	/*! Read all values of a key
		\param String	: (input) Key name
		\param List		: (input) values. Format : KEY*NAME=VALUE
	*/	
	void Read(const Stringz&, List<Stringz>&);

	/*! Read all values of all keys
		\param List	: values. Format : KEY*NAME=VALUE
	*/	
	void Read(List<Stringz>&);

	/*! delete a key
		\param Stringz	: (input) key name
	*/	
	void Delete(const Stringz&);

	/*! delete a value
		\param Stringz	: (input) key name
		\param Stringz	: (input) value name
	*/	
	void Delete(const Stringz&, const Stringz&);

private:
	_PTR mCfgStorageAbstract;	
};

}

#endif
