/*
 * sdl_2Dswype_dialog.h
 *
 *  Created on: 18 dec. 2011
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2015
 ** Houkes Horeca Applications
 **
 ** This file is part of the SDL2UI Library.  This library is free
 ** software; you can redistribute it and/or modify it under the
 ** terms of the GNU General Public License as published by the
 ** Free Software Foundation; either version 3, or (at your option)
 ** any later version.

 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.

 ** Under Section 7 of GPL version 3, you are granted additional
 ** permissions described in the GCC Runtime Library Exception, version
 ** 3.1, as published by the Free Software Foundation.

 ** You should have received a copy of the GNU General Public License and
 ** a copy of the GCC Runtime Library Exception along with this program;
 ** see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 ** <http://www.gnu.org/licenses/>
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <queue>
#include "sdl_dialog.h"
#include "sdl_swype_dialog.h"

/// @brief Scrolling dialog with list of items.
class C2DswypeDialog : public CswypeDialog
{
public:
	C2DswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, int span, bool horizontal);
	virtual ~C2DswypeDialog();

public: // override
	virtual void onPaint();
	virtual void setCursor( int index);
	virtual keybutton findButton( const Cpoint &p);
	virtual int getScrollIndex( keybutton sym);
	virtual void onPaintUnit( int unit, const Crect &location) =0;
	void onPaintBackground( int index, const Crect &location);
	void invalidateIndex( int index);
	CdialogObjectPtr findObject( const Cpoint &p);
	int getSpan() { return m_span; }
	virtual size_t size();
	virtual size_t rows();
	virtual size_t maxPosition() { return size(); }
	virtual void calculateItemRect();
	void setSpan( int x);
	
public: // New functions.
	virtual void makeSureIndexVisible( int index, int time);
	virtual bool scrollToIndex( int index, int time);

private:
	virtual void calculateSurfacePosition();
	void paintVertical( SDL_Rect &rct, int offx, int offy);
    void paintHorizontal( SDL_Rect &rct, int offx, int offy);

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
		C2DswypeDialog *dlg=dynamic_cast<C2DswypeDialog*>( m_pParent);
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

