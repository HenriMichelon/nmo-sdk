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
#ifndef _NMO_GUI_GRID_H_
#define _NMO_GUI_GRID_H_

namespace NMO {

/*!
\brief		a grid: childs widgets are organized by rows & cols inside
			a ScrollBox widget.

\author		(c) 2001-2002 Henri Michelon 
\version	$Id: GGrid.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GGrid: public GWidget
{
public:
	GGrid();
	virtual ~GGrid() {};

	/*_DWORD SelectedRow() const;
	_DWORD SelectedCol() const;
	GGridRect SelectedRect() const;
	void SelectRow(_DWORD);
	void SelectCol(_DWORD);
	void SelectRect(const GGridRect&);*/

	/*! Return the selected cell, if any
		\return	NULL when no cell is selected
	*/
	GGridCell* SelectedCell();

	/*! Set the selection to a particular cell
		\param	_DWORD	: column index, 1-based from the top
		\param	_DWORD	: row index, 1-based from the left
	*/
	void SelectCell(_DWORD, _DWORD);

	/*! Return the total number of colums */
	_DWORD ColCount() const;

	/*! Return the total number of rows */
	_DWORD RowCount() const;

	/*! Add columns. Add one row if there isn't any.
		\param _DWORD	: number of columns to add
	*/
	void AddCols(_DWORD = 1);

	/*! Add columns. Add one column if there isn't any.
		\param _DWORD	: number of rows to add
	*/
	void AddRows(_DWORD = 1);

	/*! Drop columns from the right of the grid
		\param _DWORD	: number of columns to drop
	*/
	void DeleteCols(_DWORD = 1);

	/*! Drop rows from the bottom of the grid
		\param _DWORD	: number of rows to drop
	*/
	void DeleteRows(_DWORD = 1);

	/*! Set the default rows height used in AddRows() & AddCols()
		\param	_DWORD	: rows height in pixels, including cells border line
	*/
	void SetDefaultRowsHeight(_DWORD);

	/*! Set the default columns width used in AddRows() & AddCols()
		\param	_DWORD	: rows height in pixels, including cells border line
	*/
	void SetDefaultColsWidth(_DWORD);

	/*! Set the height of a row.
		\param	_DWORD	: row index, 1-based from the left
		\param	_DWORD	: row height, in pixels, including cells border line
	*/
	void SetRowHeight(_DWORD, _DWORD);

	/*! Set the width of a colmuns.
		\param	_DWORD	: column index, 1-based from the top
		\param	_DWORD	: column width, in pixels, including cells border line
	*/
	void SetColWidth(_DWORD, _DWORD);

	/*_DWORD FixedColCount() const;
	_DWORD FixedRowCount() const;
	void SetFixedColCount(_DWORD);
	void SetFixedRowCount(_DWORD);*/

	void SetResources(const Stringz&);

	/*! Return a particular cell.
		\param	_DWORD	: column index, 1-based from the top
		\param	_DWORD	: row index, 1-based from the left
	*/
	GGridCell& Cell(_DWORD, _DWORD);

private:
	_DWORD	mRowCount;
	_DWORD	mColCount;
	_DWORD	mDefHeight;
	_DWORD	mDefWidth;
	GGridRect	mSelected;
	GScrollBox	mRows;

	virtual void EventResize(_BOOL);
	virtual void OnBoxMouseDown(GWidget&, GEvent*);
};

}

#endif

