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
#ifndef _DATATYPE_FILECFG_H_
#define _DATATYPE_FILECFG_H_

namespace NMO {

/*!
\brief		Configuration/resource files

			This class allow the manipulation of a "configuration"
			or "resources" text file.

			There is one entry per line. Entry format is the following:

					KEY*NAME=VALUE

			This is only a file format, for system or per user 
			configuration storage, consider using the CfgStorage class.

\author		(c) 1999-2001 Henri Michelon 
\version	$Id: FileCfg.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class FileCfg: public NMOObject
{
public:
	/*! Create a configuration file from a collection of string.
		One string per value. 
		Format of the strings: KEY*NAME=VALUE
		\param Iterator		(input) list of strings.
		\param Stringz		(input) file name if needed.
	*/	
	FileCfg(Iterator<Stringz>&, const Stringz& ="");

	/*! Open and load a configuration file or
		create an empty configuration file if the file
		does not exists.
		\param Stringz	(input) file name
	*/	
	FileCfg(const Stringz&);

	/*! Create an empty configuration file in memory */
	FileCfg();
	
	virtual ~FileCfg();

	/*! Erase all keys in memory */
	void Clear();

	/*! Save modifications to the file */
	_BOOL Save();

	/*! Create a configuration file from a collection of string.
		One string per value. 
		Format of the strings: KEY*NAME=VALUE
		\param Iterator		(input) list of strings.
		\param Stringz		(input) file name if needed.
	*/	
	void Load(Iterator<Stringz>&, const Stringz& ="");

	/*! Open and load a configuration file.
		\param Stringz	(input) file name
	*/	
	_BOOL Load(const Stringz&);

	/*! Set a named value.
		\param String	(input) Key name
		\param String	(input) Value name
		\param String	(input) Value
	*/	
	void Write(const Stringz&, const Stringz&, const Stringz&);

	/*! Read a named value
		\param String	Key name
		\param String	Value name
		\param String	Default value
		\return the value of the key or the default value 
				if the key does not exists
	*/	
	Stringz Read(const Stringz&, const Stringz&, const Stringz& ="");

	/*! Read all values of a key
		\param String	(input) Key name
		\param List		(output) values. Format : KEY*NAME=VALUE
	*/	
	void Read(const Stringz&, List<Stringz>&);

	/*! Read all values of all keys
		\param List	(output) values. Format : KEY*NAME=VALUE
	*/	
	void Read(List<Stringz>&);

	/*! Delete a key
		\param Stringz	(input) key name
	*/	
	void Delete(const Stringz&);

	/*! Delete a value
		\param Stringz	(input) key name
		\param Stringz	(input) value name
	*/	
	void Delete(const Stringz&, const Stringz&);

private:
	_PTR mFileCfgAbstract;
	void Expand(Iterator<Stringz>&);
};

}

#endif
