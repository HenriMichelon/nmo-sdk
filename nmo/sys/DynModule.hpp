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
#ifndef _NMO_DYNLOAD_H_
#define _NMO_DYNLOAD_H_

namespace NMO {

/*!
\brief		Dynamic module loading

			Example:
			\code
			_PTR MyClass::OpenUsefulFunction()
			{
				DynModule *mod = DynModule::Create("ODBC32");
				if (mod) {
					return mod->GetMethodAddress("SQLConnect");
				}
				return NULL;
			}
			\endcode


\author		(c) 2000-2002 Henri Michelon 
\version	$Id: DynModule.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class DynModule: public NMOObject
{
public:
	virtual ~DynModule();

	/*! Load a module in memory.
		\param Stringz	: file name, without extension 
						(a path can be specified)
		return NULL on error
	*/
	static DynModule* Create(const Stringz&);

	/*! Return the address of a function by is name
		\param Stringz	: function name
		\return NULL on error
	*/
	_PTR GetMethodAddress(const Stringz&);


protected:
	DynModule();
	_BOOL Load(const Stringz&);

private:
	_PTR	mDynModuleAbstract;	
};


}

#endif
