/*
 * scroll_dialog.h
 *
 *  Created on: 18 dec. 2011
 *      Author: mensfort
 */

#include <queue>
#include "sdl_dialog.h"
#include "sdl_swype_dialog.h"

#pragma once

/// @brief Scrolling dialog with list of items.
class C2DswypeDialog : public CswypeDialog
{
public:
	C2DswypeDialog( Cdialog *parent, Crect rect, keybutton firstKey, int span, bool horizontal);
	virtual ~C2DswypeDialog();

public: // override
	virtual void setCursor( int index);
	virtual keybutton findButton( const Cpoint &p);
	virtual int getScrollIndex( keybutton sym);
	virtual void onPaintRow( int row, const Crect &location);
	virtual void onPaintCell( int index, const Crect &location) =0;
	void onPaintBackground( int index, const Crect &location);
	void invalidateIndex( int index);
	CdialogObject * findObject( const Cpoint &p);
	int getSpan() { return m_span; }
	virtual size_t size();
	virtual size_t rows();
	virtual size_t maxPosition() { return size(); }

public: // New functions.
	virtual void makeSureIndexVisible( int index, int time);
	virtual bool scrollToIndex( int index, int time);
	void setSpan( int x) { m_span =x; }

private:
	int			m_span; ///< The number of items horizontal/vertical in a vertical/horizontal scroll dialog.

public:
	int			m_realIndex;
};

/** Scroll object, not really usefull for other things than drag &drop.
 */
class C2DscrollObject : public CdialogObject
{
public:
	C2DscrollObject(Cdialog *parent, int index, const Csize &size)
    : CdialogObject(parent,Crect(0,0,size.width(),size.height()), KEY_NONE)
    , m_index(index) { m_dragEnable =true; }
	~C2DscrollObject() {}

	void setIndex( int index) { m_index=index; }
	virtual void onPaint( int touch) { (void)touch; }
	virtual void onPaint( const Cpoint &p, int touch)
	{
		(void)touch;
		if ( !m_visible)
		{
			return;
		}
		C2DswypeDialog *dlg=dynamic_cast<C2DswypeDialog*>( m_parent);
		if ( dlg && dlg->m_visible==true)
		{
			Crect r( p.left(), p.top(), dlg->width()/dlg->getSpan(), dlg->itemBlocks());
			dlg->onPaintBackground( m_index, r);
		}
	}

private:
	int m_index; ///< Which object to paint.
};

/* SCROLL_2D_DIALOG_H_ */

