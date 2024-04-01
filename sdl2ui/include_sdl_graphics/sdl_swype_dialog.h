/*============================================================================*/
/**  @file       sdl_swype_dialog.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Swype dialog 1D list
 **
 **  Create a default swype list 1 dimensional.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CswypeDialog
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
#include <memory>
#include "sdl_types.h"
#include "sdl_dialog.h"
#include "my_thread.hpp"
#include "sdl_swype_object.h"

/// @brief Element in the list to scroll.
class IscrollElement
{
	virtual void onPaint( int x, int y) =0;
	virtual void onPaint() =0;
	virtual ~IscrollElement() {}
};

class CdialogObject;
typedef std::shared_ptr<CdialogObject> CdialogObjectPtr;

#define MEASUREMENTS  	10

/// @brief Speeding class.
class Cspeeding
{
public:
	Cspeeding();
	virtual ~Cspeeding() {}
	void addPosition( double position);
	double getSpeed();
	void clear() { m_measurements=0; }

private:
	int			m_measurements;
	double		m_position[MEASUREMENTS];
	double		m_speed[MEASUREMENTS];
	double		m_time[MEASUREMENTS];
	double		m_friction;
	int			m_previousTime;
};

/// @brief Scrolling dialog with list of items.
class CswypeDialog : public Cdialog
{
public:
	CswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, bool horizontal);
	virtual ~CswypeDialog();
        
#ifdef USE_SDL2
	SDL_Renderer	*m_itemRenderer;    ///< Renderer for items.
#endif        
protected:
	Crect m_itemRect; ///< Size for an item in the list
	int	m_listSize; ///< Size for the list

    // Crect m_rect has where it is inserted on screen.
	int m_sizes; ///< Number of elements in the list.
    double m_scrollPosition; ///< Amount of scroll.
    double m_speed; ///< Speed scroll;
    int m_firstUnitPainted; ///< First row painted inside the (paintedArea/textures).
    int m_lastUnitPainted; ///< last row painted in the (paintedArea/textures).
    int m_firstVisibleUnit; ///< First item/row on display
    int m_lastVisibleUnit; ///< Last item/row on display
    int m_visibleSize; ///< What is visible?
    int m_firstOptionalUnit; ///< First item/row just not on display
    int m_lastOptionalUnit; ///< Last item/row just not on display
    int m_nrFkeys; ///< How many Function keys we use?
    keybutton m_firstKey; ///< First key.
    int m_endMargin; ///< Margin at bottom.
    Cspeeding m_speeding; ///< Calculate speed of scrollbar.

protected:
    bool        m_horizontal; ///< Is it horizontal scrolling?
    int 		m_itemBlocks; ///< Height/Width of 1 item divided by 8 pixels.
	Ctimeout 	m_timer; ///< Time to run.
	int			m_startPosition; ///< Place where we come from.
	int			m_endPosition; ///< Place to go to.
	int			m_moveTime; ///< time to move from start to end.
	double		m_dialogSpeed; ///< Speed dialog.
	bool		m_repaint; ///< Paint again.
	bool		m_dragEnable; ///< Can move the button around.
	int			m_dragIndex; ///< Index of object to drag.
	CdialogObjectPtr m_object; ///< Need a pointer to any object.

public:
	int			m_cursor; ///< Selected customer.

    virtual void clean();
    virtual bool onLoop();
    virtual void resetPaintedArea();
    virtual bool scrollRelative( double distance, bool flow);
    virtual void setCursor( int index);
    virtual void setFirstKey( keybutton key);
    virtual void setRect( const Crect &rect);
    virtual void topStop();

    virtual void enableDrag( bool drag);
    virtual keybutton findButton( const Cpoint &p);
    virtual CdialogObjectPtr findObject( const Cpoint &p);
    virtual int getDragIndex();
    virtual int getScrollIndex( keybutton sym);
    virtual int getScrollOffset();
    virtual void invalidate( int row);
    virtual void invalidate();
    virtual bool isRowVisible( int row);
    virtual bool isScrollDragDialog( const Cpoint &pb);
    virtual bool isHorizontalScrollDialog( const Cpoint &p);
    virtual int itemBlocks();
    virtual void makeSureRowVisible( int row, int time =2000);
    virtual void onCleanup();
    virtual void onPaintBackground( int row, const Crect &location);
    virtual void onPaint();
    virtual void repaint();
    virtual size_t rows();
    virtual bool scrollToPixel( int offset ,int time);

protected:
    virtual void popFirst();
    virtual bool scrollToRow( int offset ,int time);
    virtual bool scrollIndex( double row, int time);
	virtual void setItemBlocks( int rowHeight);
	virtual void onUpdate() {}
	virtual void onPaintUnit( int unit, const Crect &location) =0;
	Estatus onButton( SDLMod mod, keybutton sym);
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual double getScrollIndex();
    virtual int scrollMax();
	virtual void calculateItemRect();
	void setMargin( double margin);
	CswypeObjectPtr insertAtBegin(const Crect &rect);
    CswypeObjectPtr insertAtEnd(const Crect &rect);
	bool hasReachedEndPosition();
	void setDirection(bool horizontal);
	
protected:
	void clearSpeed();
	virtual void onPaintSurfaceHorizontal( bool optional);
	virtual void onPaintSurfaceVertical( bool optional);
	virtual void renderCopy( SDL_Surface *surface, SDL_Rect *rect);

private:
	virtual Epainted visiblePainted();
	Epainted paintSingleAreaVertical();
	virtual void paintSurfaceTop();
	virtual void paintSurfaceRight();
	virtual void paintSurfaceLeft();
	virtual void paintSurfaceBottom();
	virtual int surfaceMax();
	virtual void calculateSurfacePosition();
	virtual bool isSwypeDialog( const Cpoint &p) { (void)p; return true; }
	void onPaintHorizontal( SDL_Rect &rct);
    void onPaintVertical( SDL_Rect &rct);


protected:
    std::vector<CswypeObjectPtr> m_swypeObjects;
    std::vector<CswypeObjectPtr> m_spares;

private:
    int  m_lowPrioCount;
};

typedef std::shared_ptr<CswypeDialog> CswypeDialogPtr;

/** Scroll object, not really usefull for other things than drag &drop.
 */
class CscrollObject : public CdialogObject
{
public:
	CscrollObject(Cdialog *parent, int index, const Csize &size)
    : CdialogObject(parent,Crect(0,0,size.width(),size.height()), KEY_NONE)
    , m_index(index) { m_dragEnable =true; }
	~CscrollObject() {}

	void setIndex( int index) { m_index=index; }
	virtual void onPaint( int touch) { (void)touch; }
	virtual void onPaint( const Cpoint &p, int touch)
	{
		(void)touch;
		CswypeDialog *dlg=dynamic_cast<CswypeDialog*>( m_pParent);
		if ( dlg)
		{
			Crect r( p.left(), p.top(), dlg->width(), dlg->itemBlocks());
			dlg->onPaintBackground( m_index, r);
		}
	}

private:
	int m_index; ///< Which object to paint.
};

/* SCROLL_DIALOG_H_ */

