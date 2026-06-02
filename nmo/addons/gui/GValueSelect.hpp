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
#ifndef _GUI_VALUESELECT_H_
#define _GUI_VALUESELECT_H_

namespace NMO {

/*!
\brief		super class for selection widgets

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GValueSelect.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GValueSelect: public GPanel
{
public:
	GValueSelect(WidgetType);
	virtual ~GValueSelect() {};
	
	_LONG Min() const;
	_LONG Max() const;
	_LONG Value() const;
	_LONG Step() const;
	
	virtual void SetMin(_LONG);
	virtual void SetMax(_LONG);
	virtual void SetValue(_LONG);
	virtual void SetStep(_LONG);
	virtual void SetRange(_LONG, _LONG, _LONG = 1);

protected:
	_LONG min;
	_LONG max;
	_LONG value;
	_LONG step;

	virtual void EventResize(_BOOL);
	inline virtual void EventRangeChange() {};
	inline virtual void EventValueChange(_LONG) {};
};

}

#endif
