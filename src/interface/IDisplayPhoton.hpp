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

	(c) 2000-2002 Henri Michelon

$Id: IDisplayPhoton.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_INTERFACE_DISPPHOTON_H_
#define _NMO_INTERFACE_DISPPHOTON_H_


namespace NMO {

class IDisplayPhoton : public IDisplayChunky
{
public:
	IDisplayPhoton(DisplayType, PtWidget_t*);
	virtual ~IDisplayPhoton();
	
	void Refresh();
	void Refresh(const IRect&);

	// IDisplayPhoton specific
	void RealRefresh(const IRect&);
	_BOOL CreateBuffers();
	void DestroyBuffers();			

private:
	PhImage_t	phimage;
	PtWidget_t	*window;
	static _DWORD nInst;
};

}

#endif 
