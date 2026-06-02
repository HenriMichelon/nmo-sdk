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

	(c) 1998-2001 Henri Michelon

$Id: GGrid.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//--------------------------------------------------------------------------
GGridCell :: GGridCell (): GPanel(GRIDCELL)
{
	mHborder = 1;
	mVborder = 1;
	redrawOnMouseEvent = TRUE;
	allowFocus = TRUE;
	mSelected = FALSE;
}


//--------------------------------------------------------------------------
_BOOL GGridCell :: Selected() const
{
	return mSelected;
}


//--------------------------------------------------------------------------
GWidget* GGridCell :: EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	GWidget*r = GPanel::EventMouseDown(B, X, Y);
	if (rect.Contains(X, Y)) { Call(GEvent::OnClick); }
	return r;

}


//--------------------------------------------------------------------------
GGrid :: GGrid () : GWidget(GRID)
{
	mColCount = 0;
	mRowCount = 0;
	mDefHeight = 20;
	mDefWidth = 80;
	transparent = TRUE;
    drawBackground = FALSE;
}


//--------------------------------------------------------------------------
void GGrid :: SetDefaultRowsHeight(_DWORD HEIGHT)
{
	mDefHeight = HEIGHT;
}


//--------------------------------------------------------------------------
void GGrid :: SetDefaultColsWidth(_DWORD WIDTH)
{
	mDefWidth = WIDTH;
}

//--------------------------------------------------------------------------
void GGrid :: SetResources(const Stringz&RBG)
{
	Add(mRows, CLIENT, RBG);
	mRows.Connect(GEvent::OnMouseDown,
					this,
					GEventFunction(&GGrid::OnBoxMouseDown));
	AllowFocus(TRUE);
	mRows.InnerBox().SetDrawBackground(FALSE);
}


//--------------------------------------------------------------------------
_DWORD GGrid :: RowCount () const
{
	return mRowCount;
}


//--------------------------------------------------------------------------
_DWORD GGrid :: ColCount () const
{
	return mColCount;
}


//--------------------------------------------------------------------------
void GGrid :: SetRowHeight(_DWORD ROW, _DWORD HEIGHT)
{
	ListIterator<GWidget> lstrows(mRows.Childs());
	lstrows[ROW].SetSize(mDefWidth * mColCount, HEIGHT);
}


//--------------------------------------------------------------------------
void GGrid :: SetColWidth(_DWORD COL, _DWORD WIDTH)
{
	ListIterator<GWidget> lstrows(mRows.Childs());
	while (!lstrows.End()) {
		GWidget &row = lstrows.Next();
		ListIterator<GWidget> lstcol(row.Childs());
		lstcol[COL].SetSize(WIDTH, mDefHeight);
	}
}


//--------------------------------------------------------------------------
void GGrid :: EventResize(_BOOL R)
{
	GWidget::EventResize(R);
}


//--------------------------------------------------------------------------
void GGrid :: OnBoxMouseDown(GWidget&, GEvent*E)
{
	GEventMouse *e = (GEventMouse*)E;
	if (!mRows.InnerBox().Rect().Contains(e->x, e->y)) { return; }
	SelectCell(0, 0);
	ListIterator<GWidget> lstrows(mRows.Childs());
	while (!lstrows.End()) {
		GWidget &row = lstrows.Next();
		ListIterator<GWidget> lstcol(row.Childs());
		while (!lstcol.End()) {
			GGridCell &cell = (GGridCell&)lstcol.Next();
			_BOOL selected = cell.mSelected;
			cell.mSelected = cell.Pushed();
			if (selected != cell.mSelected) {
				cell.Refresh();
				if (cell.mSelected) {
					mSelected.left = mSelected.right = lstcol.Index() - 1;
					mSelected.top = mSelected.bottom = lstrows.Index() - 1;
				}
			}
		}
	}
}


//--------------------------------------------------------------------------
GGridCell* GGrid :: SelectedCell()
{
	if (mSelected.top && mSelected.left) {
		ListIterator<GWidget> lst(mRows.Childs());
		GWidget &row = lst[mSelected.top];
		lst = row.Childs();
		return (GGridCell*)&lst[mSelected.left];
	}
	return NULL;
}


//--------------------------------------------------------------------------
GGridCell& GGrid :: Cell(_DWORD COL, _DWORD ROW)
{
	ListIterator<GWidget> lst(mRows.Childs());
	GWidget &row = lst[ROW];
	lst = row.Childs();
	return (GGridCell&)lst[COL];
}


//--------------------------------------------------------------------------
void GGrid :: SelectCell(_DWORD COL, _DWORD ROW)
{
	if ((!COL) && (!ROW)) {
		mSelected.top = mSelected.bottom = 0;
		mSelected.left = mSelected.right = 0;
	}
}


//--------------------------------------------------------------------------
void GGrid :: DeleteRows (_DWORD COUNT)
{
	_DWORD rowtodelete = MIN(COUNT, mRowCount);
	mRowCount -= rowtodelete;
	for (; rowtodelete > 0; rowtodelete --) {
		mRows.Childs().Drop(mRowCount);
	}
	if (mSelected.bottom > mRowCount) {
		mSelected.bottom = mRowCount;
	}
	if (mSelected.top > mRowCount) {
		SelectCell(0, 0);
	}
}


//--------------------------------------------------------------------------
void GGrid :: DeleteCols (_DWORD COUNT)
{
	_DWORD coltodelete = MAX(COUNT, ColCount());
	mColCount -= coltodelete;
	ListIterator<GWidget> lstrows(mRows.Childs());
	while (!lstrows.End()) {
		GWidget &row = lstrows.Next();
		for (_DWORD idx_col = coltodelete; idx_col > 0; idx_col --) {
			ListIterator<GWidget> lstcol(row.Childs());
			lstcol.Reset(FALSE);
			row.Drop(lstcol.Current());
		}
	}
	if (mSelected.right > mColCount) {
		mSelected.right = mColCount;
	}
	if (mSelected.left > mColCount) {
		SelectCell(0, 0);
	}
}


//--------------------------------------------------------------------------
void GGrid :: AddRows (_DWORD COUNT)
{
	_DWORD colcount = MAX(1, ColCount());
	mColCount = colcount;
	mRowCount += COUNT;
	_DWORD width = mDefWidth * colcount;
	for (_DWORD idx_row = 0; idx_row < COUNT; idx_row++) {
		GWidget *row = new GWidget;
		mRows.Add(row, CORNERTOPLEFT)->SetSize(width, mDefHeight);
		for (_DWORD idx_col = 0; idx_col < colcount; idx_col++)  {
			row->Add(new GGridCell, LEFT)->SetSize(mDefWidth, mDefHeight);
		}
	}
}


//--------------------------------------------------------------------------
void GGrid :: AddCols (_DWORD COUNT)
{
	if (mRowCount == 0) {
		AddRows(1);
		if (COUNT > 0) { COUNT--; }
	}
	mColCount += COUNT;
	_DWORD width = mDefWidth * mColCount;
	ListIterator<GWidget> lstrows(mRows.Childs());
	while (!lstrows.End()) {
		GWidget &row = lstrows.Next();
		row.SetSize(width, mDefHeight);
		for (_DWORD idx_col = 0; idx_col < COUNT; idx_col ++) {
			row.Add(new GGridCell, LEFT)->SetSize(mDefWidth, mDefHeight);
		}
	}
}
