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
#ifdef USE_SDL2
#include "SDL_render.h"
#include "SDL_video.h"
#else
#endif
#include "sdl_dialog.h"

/// @brief Structure to hold each texture to paint. We have a queue of this.
class CswypeObject
{
public:
	CswypeObject( sdlTexture *texture, int index, const Crect &destination)
	: item(NULL), texture(texture), index(index), itemId(0), destination(destination){}
	virtual ~CswypeObject()
	{
		clean();
	}
	void clean()
	{
#ifdef USE_SDL2
#else
		if (texture) SDL_FreeSurface(texture);
		texture =NULL;
#endif
	}
	void onPaint( Cpoint &point);

public:
	CdialogObject 		*item; ///< Reference to object (if needed)
	sdlTexture	 		*texture; ///< Painted area
	int			  		index; ///< Index in the list
	int					itemId; ///< Item to use.
	Crect				destination; ///< Where to paint the object should be multiplied by 8.
};

/// @brief Element in the list to scroll.
class IscrollElement
{
	virtual void onPaint( int x, int y) =0;
	virtual void onPaint() =0;
	virtual ~IscrollElement() {}
};

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

/// @brief Decide what to paint.
typedef enum
{
	PAINT_BOTTOM,			///< Bottom not finished.
	PAINT_TOP,				///< Top not painted.
	PAINT_BOTTOM_OPTIONAL, 	///< Can paint more on bottom for future scroll.
	PAINT_TOP_OPTIONAL,		///< can paint more on top for future scroll.
	PAINT_RIGHT,			///< Bottom not finished.
	PAINT_LEFT,				///< Top not painted.
	PAINT_RIGHT_OPTIONAL, 	///< Can paint more on bottom for future scroll.
	PAINT_LEFT_OPTIONAL,	///< can paint more on top for future scroll.
	PAINT_READY,			///< All possible items painted.
} Epainted;

/// @brief Scrolling dialog with list of items.
class CswypeDialog : public Cdialog
{
public:
	CswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, bool horizontal);
	virtual ~CswypeDialog();
        
#ifdef USE_SDL2
	SDL_Renderer	*m_itemRenderer;    ///< Renderer for items.
#endif        
	Crect m_itemRect; ///< Size for an item in the list
	int	m_listSize; ///< Size for the list

    // Crect m_rect has where it is inserted on screen.
    bool m_horizontal; ///< Is it horizontal scrolling?
	int m_sizes; ///< Number of elements in the list.
    double m_scroll; ///< Amount of scroll.
    double m_speed; ///< Speed scroll;
    std::vector<CswypeObject*> m_swypeObjects;
    int m_validBuffers; ///< Increasing amount of object buffers
    int m_visibleBuffers; ///< Number of visible object buffers
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
    int 		m_itemBlocks; ///< Height/Width of 1 item divided by 8 pixels.
	Ctimeout 	m_timer; ///< Time to run.
	int			m_startPosition; ///< Place where we come from.
	int			m_endPosition; ///< Place to go to.
	int			m_moveTime; ///< time to move from start to end.
	double		m_dialogSpeed; ///< Speed dialog.
	bool		m_repaint; ///< Paint again.
	bool		m_dragEnable; ///< Can move the button around.
	int			m_dragIndex; ///< Index of object to drag.
	CdialogObject *m_object; ///< Need a pointer to any object.
public:
	int			m_cursor; ///< Selected customer.

public:
    virtual size_t rows();
    virtual void clean();
    virtual int itemBlocks();
    virtual bool scrollToRow( int offset ,int time);
    virtual bool scrollToPixel( int offset ,int time);
    virtual bool scrollIndex( double row, int time);
	virtual keybutton findButton( const Cpoint &p);
	virtual void setItemBlocks( int rowHeight);

	virtual void onUpdate() {}
	virtual void onPaint();
	virtual void onCleanup();
	virtual void onPaintUnit( int unit, const Crect &location) =0;
	virtual void onPaintBackground( int row, const Crect &location);
	virtual void invalidate();
	virtual int getScrollIndex( keybutton sym);
	virtual bool scrollRelative( double distance, bool flow);
	virtual CdialogObject *findObject( const Cpoint &p);
	virtual void setRect( const Crect &rect);
	virtual bool isRowVisible( int row);

	Estatus onButton( keymode mod, keybutton sym);
	virtual bool onLoop();
	virtual void topStop();
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual int getScrollOffset() { return (int)m_scroll; }
	virtual double getScrollIndex() { return m_scroll/(double)(itemBlocks()*8); }
    virtual int scrollMax();
	virtual void resetPaintedArea();
	void makeSureRowVisible( int row, int time =2000);
	virtual void invalidate( int row);
	virtual void setCursor( int index);
	void enableDrag( bool drag) { m_dragEnable=drag; }
	virtual bool isScrollDragDialog( const Cpoint &p) { (void)p; return m_dragEnable; }
	virtual bool isHorizontalScrollDialog( const Cpoint &p) { (void)p; return m_horizontal; }
	int getDragIndex() { return m_dragIndex; }
    void repaint() { m_repaint=true; CswypeDialog::onPaint(); }
	virtual void calculateItemRect();
	void setMargin( double margin) { m_endMargin =(int)( margin*8.0f*itemBlocks()); }
	CswypeObject *insertAtBegin();
    CswypeObject *insertAtEnd();
	
protected:
	void clearSpeed();
	virtual void onPaintSurfaceHorizontal( bool optional);
	virtual void onPaintSurfaceVertical( bool optional);
	virtual void renderCopy( sdlTexture *surface, SDL_Rect *rect);
	sdlTexture *createSurface();
	sdlTexture *createSurface( int w, int h);

private:
	virtual Epainted visiblePainted();
	Epainted paintSingleAreaVertical();
	virtual void paintSurfaceTop();
	virtual bool paintSurfaceRight();
	virtual bool paintSurfaceLeft();
	virtual void paintSurfaceBottom();
	virtual int surfaceMax();
	virtual void calculateSurfacePosition();
	virtual bool isSwypeDialog( const Cpoint &p) { (void)p; return true; }
};


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
		CswypeDialog *dlg=dynamic_cast<CswypeDialog*>( m_parent);
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

