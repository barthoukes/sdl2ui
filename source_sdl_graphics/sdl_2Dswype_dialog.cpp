/*============================================================================*/
/**  @file       sdl_2Dswype_dialog.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Swype dialog with list.
 **
 **  Create a 2D list.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              C2DswypeDialog
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 28 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/


#include "sdl_2Dswype_dialog.h"
#include "sdl_surface.h"
//#include "zhongcan_conf.h"

//static bool D=false;

/** Dialog constructor to scroll objects left-right or up-down
 *  @param parent [in] Parent dialog.
 *  @param rect [in] Rectangle to use.
 *  @param firstKey [in] Key to use.
 *  @param span [in] Number of keys horizontal.
 *  @param horizontal [in] True for horizontal, false for vertical swype dialog.
 */
C2DswypeDialog::C2DswypeDialog( Cdialog *parent, Crect rect, keybutton firstKey, int span,
		bool horizontal)
: CswypeDialog( parent, rect, firstKey, horizontal)
, m_span( span)
, m_realIndex(0)
{
	m_nrFkeys =span*(1+ rect.height()/itemBlocks());
	if ( m_object)
	{
		delete m_object;
		m_object =NULL;
	}
	m_object =new C2DscrollObject( this, 0, Csize(rect.width()/span, itemBlocks()));
}

/** @brief Destructor */
C2DswypeDialog::~C2DswypeDialog()
{
}

/** @brief Get which index it is for the object under the mouse.
 *  @param sym [in] Which button found on screen.
 *  @return Item index found in the list.
 */
int C2DswypeDialog::getScrollIndex( keybutton sym)
{
	if ( sym<m_firstKey || sym>m_firstKey+m_nrFkeys-1)
	{
		return -1;
	}
	int key =(sym-m_firstKey);
	int invisible;
	if ( m_horizontal)
	{
		invisible =m_scroll/(itemBlocks()*8);
		//invisible =m_surfacePixels.left()/(itemBlocks()*8)+m_firstRow;
	}
	else
	{
		invisible =m_scroll/(itemBlocks()*8);
		//invisible =m_surfacePixels.top()/(itemBlocks()*8)+m_firstRow;
	}
	invisible *=m_span;
	int firstKey =(invisible%m_nrFkeys);
	int offset =(key-firstKey+m_nrFkeys)%m_nrFkeys+invisible;
	return ( offset>=0 && offset<(int)maxPosition()) ? offset:-1;
}

/** @brief Calculate how many cells we have.
 *  @return Number of cells.
 */
size_t C2DswypeDialog::size()
{
	return 10;
}

/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
size_t C2DswypeDialog::rows()
{
	return (int)((size()+m_span-1)/m_span);
}

/** @brief Make sure the item is visible.
 *  @param index [in] Which item we want to see.
 *  @param time [in] Time in milliseconds to arrive at location.
 */
void C2DswypeDialog::makeSureIndexVisible( int index, int time)
{
	makeSureRowVisible( index/m_span, time);
}

/** Decide which key in a scroll dialog.
 * 	@param p [in] What position on screen.
 * 	@return What key is found on screen.
 */
keybutton C2DswypeDialog::findButton( const Cpoint &p)
{
	m_dialogSpeed =0;
	Cpoint q=p/8;
	if ( m_rect.inside( q)==true && m_visible ==true)
	{
		if ( m_horizontal)
		{
			int column =(p.x-m_rect.left()*8+m_scroll)/(8*itemBlocks());
			int row =(q.y-m_rect.top())/(m_rect.height()/m_span);
			int item =m_span*column+row;
			if ( item >=(int)maxPosition())
			{
				return KEY_NONE;
			}
			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
			Caudio::Instance()->click();
			return key;
		}
		else
		{
			int row =(p.y-m_rect.top()*8+m_scroll)/(8*itemBlocks());
			int column =(q.x-m_rect.left())/(m_rect.width()/m_span);
			int item =m_span*row+column;
			if ( item >=(int)size())
			{
				return KEY_NONE;
			}
			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
			Caudio::Instance()->click();
			return key;
		}
	}
	return KEY_NONE;
}

/** @brief Change cursor to other position.
 *  @param index [in] What index to set.
 */
void C2DswypeDialog::setCursor( int index)
{
	CswypeDialog::setCursor( index/m_span);
	m_realIndex =index;
	invalidate();
}

/** @brief Paint a row from a scrolling dialog. Paint all cells.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporaray surface.
 */
void C2DswypeDialog::onPaintRow( int row, const Crect &location)
{
	if ( m_horizontal)
	{
		// The horizontal span and vertical scrolling.
		int index =row*m_span;
		for ( int p =0; p<m_span; p++)
		{
			int top =location.height()*p/m_span;
			int bottom =location.height()*(p+1)/m_span;
			onPaintCell( index+p, Crect( location.left(), top, location.width(), bottom-top) );
		}
	}
	else
	{
		// The horizontal span and vertical scrolling.
		int index =row*m_span;
		for ( int p =0; p<m_span; p++)
		{
			int left =location.width()*p/m_span;
			int right =location.width()*(p+1)/m_span;
			onPaintCell( index+p, Crect(left, location.top(), right-left, location.height()) );
		}
	}
}

/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @param p [in] Find which object is on that location.
 *  @return Pointer to object. Not re-entrant, please use once.
 */
CdialogObject * C2DswypeDialog::findObject( const Cpoint &p)
{
	if ( m_horizontal)
	{
		Cpoint q=p/8;
		if ( m_rect.inside( q)==false)
		{
			return NULL;
		}
		int itemPixelWidth =8*itemBlocks();
		int column =(p.x-m_rect.left()*8+m_scroll)/itemPixelWidth;
		int row =(q.y-m_rect.top())/(m_rect.height()/m_span);
		int item =row+column*m_span;
		if ( item>=(int)size())
		{
			return NULL;
		}
		m_dragIndex =item;
		m_object->m_rect.setLeft( m_rect.left()+((column*itemPixelWidth)-m_scroll)/8);
		m_object->m_rect.setTop( m_rect.top()+row*m_rect.height()/m_span);
		m_object->m_rect.setHeight( m_rect.top()+(row+1)*m_rect.height()/m_span);
		m_object->m_rect.setWidth( itemBlocks());
		return m_object;
	}
	else
	{
		Cpoint q=p/8;
		if ( m_rect.inside( q)==false)
		{
			return NULL;
		}
		int itemPixelHeight =8*itemBlocks();
		int row =(p.y-m_rect.top()*8+m_scroll)/itemPixelHeight;
		int column =(q.x-m_rect.left())/(m_rect.width()/m_span);
		int item =row*m_span+column;
		if ( item>=(int)size())
		{
			return NULL;
		}
		m_dragIndex =item;
		m_object->m_rect.setTop( m_rect.top()+((row*itemPixelHeight)-m_scroll)/8);
		m_object->m_rect.setLeft( m_rect.left()+column*m_rect.width()/m_span);
		m_object->m_rect.setRight( m_rect.left()+(column+1)*m_rect.width()/m_span);
		m_object->m_rect.setHeight( itemBlocks());
		return m_object;
	}
}

/** Paint again one of the items.
 *  @param index [in] What to paint again.
 */
void C2DswypeDialog::invalidateIndex( int index)
{
	if ( index<0 || index>=(int)size())
	{
		return;
	}
	if ( m_horizontal)
	{
		int col =index/m_span;
		int row =index%m_span;
		int left =m_paintedArea.left()+(col-m_firstRow)*itemBlocks();
		if ( left>=0 && (left+itemBlocks())*8<=m_graphics->width())
		{
			// Fits in current dialog.
			int top =m_rect.height()*row/m_span;
			int bottom =m_rect.height()*(row+1)/m_span;

			Crect rect( left, top, itemBlocks(), bottom-top);
			onPaintCell( index, rect);
			m_repaint =true;
		}
	}
	else
	{
		int row =index/m_span;
		int col =index%m_span;
		int top =m_paintedArea.top()+(row-m_firstRow)*itemBlocks();
		int bottom =(top+itemBlocks())*8;
		if ( top>=0 && bottom<=m_graphics->height()) //m_surfacePixels.height())
		{
			// Fits in current dialog.
			int left =m_rect.width()*col/m_span;
			int right =m_rect.width()*(col+1)/m_span;
			Crect rect( left, top, right-left, itemBlocks());
			onPaintRow( row, rect);
			m_repaint =true;
		}
	}
}

/** Paint on normal background.
 *  @param index [in] Which element to paint.
 *  @param location [in] Where to paint this one.
 */
void C2DswypeDialog::onPaintBackground( int index, const Crect &location)
{
	(void)index;
	Cgraphics *previous =m_graphics;
	//m_object->m_parent =NULL;
	Cdialog *parent =m_parent;
	m_graphics =m_mainGraph;
	m_parent =NULL;
	m_object->m_graphics =m_mainGraph;
	m_graphics =m_mainGraph;

	onPaintCell( m_dragIndex, location);
	m_graphics =previous;
	m_parent =parent;
}

/** @brief make sure this is the top/left item to show.
 *  @param index [in] which one to show.
 *  @param time [in] how much time left.
 */
bool C2DswypeDialog::scrollToIndex( int index, int time)
{
	return CswypeDialog::scrollToRow( index/m_span, time);
}
