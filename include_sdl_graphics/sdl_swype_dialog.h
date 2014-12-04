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
 **  Copyright (c) Bart Houkes, 15 aug 2013
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#include <queue>
#include "sdl_dialog.h"

#pragma once

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
	CswypeDialog( Cdialog *parent, Crect rect, keybutton firstKey, bool horizontal);
	virtual ~CswypeDialog();
        
	/// Part of the surfacePixels, which is painted.
	/// Inside this part, we are safe to scroll without painting extra things.
    Crect m_paintedArea;
    /// Part of the graphics, which is visible on screen. Especially the offset is correct.
    /// The size is equal to m_rect.size().
    Crect m_surfacePixels;
    // Crect m_rect has where it is inserted on screen.
    bool m_horizontal; ///< Is it horizontal scrolling?
	int m_sizes; ///< Number of elements in the list.
    double m_scroll; ///< Amount of scroll.
    double m_speed; ///< Speed scroll;
	std::queue<IscrollElement*>  m_scrollItems;
    int m_firstRow; ///< First item painted inside the paintedArea.
    int m_lastRow; ///< last item painted in the paintedArea.
    int m_nrFkeys; ///< How many Function keys we use?
    keybutton m_firstKey; ///< First key.
    int m_endMargin; ///< Margin at bottom.
    Cspeeding m_speeding; ///< Calculate speed of scrollbar.
    int m_itemBlocks; ///< Height/Width of 1 item divided by 8 pixels.

public:
    virtual size_t rows();
    virtual int itemBlocks();
    virtual bool scrollToRow( int offset ,int time);
    virtual bool scrollToPixel( int offset ,int time);
    virtual bool scrollIndex( double row, int time);
	virtual keybutton findButton( const Cpoint &p);

	virtual void onUpdate();
	virtual void onPaint();

	//virtual void onMouseClick(keybutton sym, SDLMod mod) =0;
	virtual void onCleanup();
	virtual void onPaintRow( int row, const Crect &location) =0;
	virtual void onPaintBackground( int row, const Crect &location);
	virtual void invalidate();
	virtual int getScrollIndex( keybutton sym);
	virtual bool scrollRelative( double distance, bool flow);
	virtual CdialogObject *findObject( const Cpoint &p);
	virtual void setRect( const Crect &rect);
	virtual bool isRowVisible( int row);

	//virtual Estatus onLButtonDown(int mX, int mY);
	Estatus onButton( SDLMod mod, keybutton sym);
	virtual bool onLoop();
	//virtual Estatus onLButtonUp(int mX, int mY);
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual int getScrollOffset() { return m_scroll; }
	virtual double getScrollIndex() { return m_scroll/(itemBlocks()*8.0f); }
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

protected:
	void clearSpeed();

private:
	virtual void onPaintHorizontal();
	virtual void onPaintVertical();
	Epainted visiblePainted();
	Epainted paintSingleAreaVertical();
	void paintTop();
	void paintRight();
	void paintLeft();
	void paintBottom();
	int surfaceMax();
	void calculateSurfacePosition();
	virtual bool isSwypeDialog( const Cpoint &p) { (void)p; return true; }

protected:
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

