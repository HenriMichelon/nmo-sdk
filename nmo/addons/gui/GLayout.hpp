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
#ifndef _NMO_GUI_LAYOUT_H_
#define _NMO_GUI_LAYOUT_H_

namespace NMO {

class GWidget;	

/*!
\brief		GUI drawing & resizing layout

\author		(c) 2000-2002 Henri Michelon 
\version	$Id: GLayout.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GLayout: public GObject 
{ 
public:
	GLayout();
	virtual ~GLayout();

	/*! Create a new layout.
	  \param	Stringz	: layout name
	  \return 	NULL on error (unknown layout)
	*/ 
	static GLayout* Create(const Stringz& = "vector");

	/*! Create a resource from a resource description string.
	  \param	Stringz : string that describe the resources of a widget
	  \return 	NEVER return NULL
	*/
	virtual void AddResource(GWidget&, const Stringz& = "") = 0;

	/*! Delete a resource created with AddResource() */
	virtual void DeleteResource(GResource*) = 0;
	
	/* Set a layout specific option
		\param	Stringz	: option name
		\param	Stringz	: value
	*/
	virtual void SetOption(const Stringz&, const Stringz&);
	
	/* Read a layout specific option
		\param	Stringz	: option name
		\return option value
	*/
	Stringz Option(const Stringz&);

	/*! Draw a widget.
	  \param	GWidget	: widget to draw
	  \param	GResource : resources used for drawing this widget
	  \param	IRect : unused
	  \param	_BOOL : TRUE = before drawing childs, FALSE = after
	*/
  	virtual void Draw(GWidget&, GResource&, const IRect&, _BOOL) = 0;


	/*! Resize a widget.
	  \param	GWidget	: widget to draw
	  \param	GResource : resources used for resizing this widget
	  \param	_DWORD	: width
	  \param	_DWORD	: height
	*/
	virtual void Resize(GWidget&, IRect&, GResource&) = 0;


	/*! Return the default font for the layout.
	  \param	IFont	: font to use for the layout
	 */
   	IFont& Font() const;

	/*! Set the resources table to use to this layout */
	virtual void SetResources(ResourcesTable&);

	/*! Return the resources table use by this layout or NULL */
	ResourcesTable *Resources() const;

protected:
	ResourcesTable	*restable;
	IFont	*font;
	virtual _BOOL Init() = 0;
	virtual void UpdateOptions() = 0;
	
private:
	class GLayoutOption
	{
	public:
		Stringz name;
		Stringz value;
		GLayoutOption(const Stringz&);
	};
	
	List<GLayoutOption> options;
};

inline IFont& GLayout::Font() const { return *font; }
inline GLayout::GLayoutOption::GLayoutOption(const Stringz&N): name(N) {};

}

#endif 
