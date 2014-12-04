/*============================================================================*/
/**  @file       sdl_swype_dialog.cpp
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

#include "sdl_swype_dialog.h"
#include "sdl_surface.h"
#include "log_base.h"
#include "zhongcan_conf.h"

static bool D=false;

/** Dialog to scroll objects left-right or up-down */
CswypeDialog::CswypeDialog( Cdialog *parent, Crect rect, keybutton firstKey, bool horizontal)
: Cdialog( parent, "scrollRect", rect)
, m_paintedArea(0,0, rect.width(), 0)
, m_surfacePixels(0,0, rect.width()*8, rect.height()*8)
, m_horizontal(horizontal)
, m_sizes(0)
, m_scroll(0)
, m_speed(0.0f)
, m_firstRow(0)
, m_lastRow(0)
, m_nrFkeys((firstKey==KEY_F1) ? 12:16)
, m_firstKey(firstKey)
, m_endMargin(0)
, m_itemBlocks( 6)
, m_startPosition(0)
, m_endPosition(0)
, m_moveTime(0)
, m_dialogSpeed(0)
, m_repaint(false)
, m_dragEnable(0)
, m_cursor(0)
{
	m_object =new CscrollObject( this, 0, Csize(m_rect.width(), itemBlocks()));
	if ( m_graphics !=NULL && m_graphics !=m_mainGraph)
	{
		delete m_graphics;
	}
	m_graphics =NULL;
	setRect( rect);
}

/** @brief Change or init the rectangle to this size.
 *  @param rect [in] What position and size to use.
 */
void CswypeDialog::setRect( const Crect &rect)
{
	m_rect.setLeft( rect.left());
	m_rect.setTop( rect.top());
	int py =rect.height()*8;
	int px =rect.width()*8;

	if ( m_rect.width() !=rect.width() || m_rect.height() !=rect.height()
	   || m_graphics ==NULL)
	{
		if ( m_graphics && m_graphics !=m_mainGraph)
		{
			delete m_graphics;
			m_graphics =m_mainGraph;
		}
		m_rect =rect;
		if ( m_horizontal)
		{
			int pixelsReserved=3*m_rect.width()*8+itemBlocks()*8-1;
			m_endMargin =itemBlocks()*1*8;
			pixelsReserved =pixelsReserved -(pixelsReserved%itemBlocks());
		   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
		       as expected by OpenGL for textures */
			px *=3;
			px =px-(px%(itemBlocks()*8));
		}
		else
		{
			int pixelsReserved=3*m_rect.height()*8+itemBlocks()*8-1;
			m_endMargin =itemBlocks()*1*8;
			pixelsReserved =pixelsReserved -(pixelsReserved%itemBlocks());
		   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
		       as expected by OpenGL for textures */
			py *=3;
			py =py-(py%(itemBlocks()*8));
		}
	    m_graphics =new Cgraphics( Csize(px, py), false);
	    if( m_graphics == NULL)
	    {
	        if(D)Log.error( "CreateRGBSurface failed: %s", SDL_GetError());
	    }
	    m_graphics->init();
	}
}

CswypeDialog::~CswypeDialog()
{
	if ( m_graphics && m_graphics !=m_mainGraph)
	{
		delete m_graphics;
		m_graphics =m_mainGraph;
	}
	if ( m_object)
	{
		delete m_object;
		m_object =NULL;
	}
}

void CswypeDialog::invalidate()
{
	if(D)Log.write( "CscrollDialog::invalidate");
	if ( m_horizontal)
	{
		m_paintedArea.setWidth(0);
		m_lastRow =m_firstRow;
	}
	else
	{
		m_paintedArea.setHeight(0);
		m_lastRow =m_firstRow;
	}
	if ( m_graphics->height()-m_paintedArea.top()*8 < m_rect.height()*8)
	{
		m_paintedArea.setTop( myMin(12,m_firstRow)*itemBlocks());
	}
	Cdialog::invalidate();
}

void CswypeDialog::onUpdate()
{

}

/** @brief Get which index it is for the object under the mouse */
int CswypeDialog::getScrollIndex( keybutton sym)
{
	if ( sym<m_firstKey || sym>m_firstKey+m_nrFkeys-1)
	{
		return -1;
	}
	int key =(sym-m_firstKey);
	int invisible;
	if ( m_horizontal)
	{
		invisible =m_surfacePixels.left()/(itemBlocks()*8)+m_firstRow;
	}
	else
	{
		invisible =m_surfacePixels.top()/(itemBlocks()*8)+m_firstRow;
	}
	int firstKey =(invisible%m_nrFkeys);
	int offset =(key-firstKey+m_nrFkeys)%m_nrFkeys+invisible;
	return ( offset>=0 && offset<(int)rows()) ? offset:-1;
}

void CswypeDialog::onPaintHorizontal()
{
	while ( true)
	{
		// Check if we can paint?
		if ( m_surfacePixels.right() > m_graphics->width())
		{
			// Reset the graph.
			resetPaintedArea();
		}
		// Check what to paint.
		switch ( visiblePainted())
		{
		case PAINT_LEFT:
			if ( m_paintedArea.left()< m_itemBlocks)
			{
				resetPaintedArea();
			}
			paintLeft();
			m_repaint =true;
			break;

		case PAINT_RIGHT:
			paintRight();
			m_repaint =true;
			break;

		case PAINT_LEFT_OPTIONAL:
			//if ( m_paintedArea.left()< m_itemBlocks)
			//{
			//	resetPaintedArea();
			//}
			//paintLeft();
			return;

		case PAINT_RIGHT_OPTIONAL:
			//paintRight();
			return;

		case PAINT_READY:
			return;
		default:
			break;
		}
	}
}

/// @brief The surface area should be painted, else not done!
Epainted CswypeDialog::visiblePainted()
{
	if ( m_horizontal)
	{
		if ( m_paintedArea.left()*8>m_surfacePixels.left() && m_firstRow>0)
		{
			return PAINT_LEFT;
		}
		if ( m_paintedArea.right()*8<m_surfacePixels.right())
		{
			return PAINT_RIGHT;
		}
		if ( m_paintedArea.right()+itemBlocks() < m_graphics->width()/8)
		{
			return PAINT_RIGHT_OPTIONAL;
		}
		if ( m_paintedArea.left()>=itemBlocks() && m_firstRow>0)
		{
			return PAINT_LEFT_OPTIONAL;
		}
	}
	else
	{
		if ( m_paintedArea.top()*8>m_surfacePixels.top() && m_firstRow>0)
		{
			return PAINT_TOP;
		}
		if ( m_paintedArea.bottom()*8+itemBlocks()*8<=m_surfacePixels.bottom())
		{
			return PAINT_BOTTOM;
		}
		if ( m_paintedArea.bottom()+itemBlocks() < m_graphics->height()/8)
		{
			return PAINT_BOTTOM_OPTIONAL;
		}
		if ( m_paintedArea.top()>=itemBlocks() && m_firstRow>0)
		{
			return PAINT_TOP_OPTIONAL;
		}
	}
	return PAINT_READY;
}

void CswypeDialog::onPaintVertical()
{
	while ( true)
	{
		// Check if we can paint?
		if ( m_surfacePixels.bottom() > m_graphics->height())
		{
			// Reset the graph.
			resetPaintedArea();
		}
		// Check what to paint.
		switch ( visiblePainted())
		{
		case PAINT_TOP:
			paintTop();
			m_repaint =true;
			break;

		case PAINT_BOTTOM:
			paintBottom();
			m_repaint =true;
			break;

		case PAINT_TOP_OPTIONAL:
			paintTop();
			return;

		case PAINT_BOTTOM_OPTIONAL:
			paintBottom();
			return;

		case PAINT_READY:
			return;
		default:
			break;
		}
	}
}

/** @brief Paint a row on top of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintTop()
{
	m_paintedArea.addTop(-itemBlocks());
	m_paintedArea.increase( 0, itemBlocks());
	// Now paint.
	m_firstRow--;
	if(D)Log.write( "paintTop %d at %d", m_firstRow, m_paintedArea.top()*8);
	onPaintRow( m_firstRow, Crect( 0, m_paintedArea.top(), m_rect.width(), itemBlocks()));
	//Log.write( "paintTop [%d %d] - top=%d bottom=%d", m_firstItem, m_lastItem, m_paintedArea.top()*8,m_paintedArea.bottom()*8);
}

/** @brief Paint a column on the left of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintLeft()
{
	if ( m_paintedArea.width()) m_paintedArea.addLeft(-itemBlocks());
	m_paintedArea.increase( itemBlocks(), 0);
	// Now paint.
	m_firstRow--;
	if(D)Log.write( "paintLeft %d at %d", m_firstRow, m_paintedArea.left()*8);
	onPaintRow( m_firstRow, Crect( m_paintedArea.left(), 0, itemBlocks(), m_rect.height()));
}

/** @brief Paint a row on the bottom of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintBottom()
{
	if(D)Log.write( "paintBottom %d at %d", m_lastRow, m_paintedArea.bottom()*8);
	onPaintRow( m_lastRow, Crect( 0, m_paintedArea.bottom(), m_rect.width(), itemBlocks()));
	m_lastRow++;
	// First paint, then
	m_paintedArea.increase( 0,itemBlocks());
	//Log.write( "paintBottom [%d %d] - top=%d bottom=%d", m_firstItem, m_lastItem, m_paintedArea.top()*8, m_paintedArea.bottom()*8);
}

/** @brief Paint a column on the right of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintRight()
{
	if(D)Log.write( "paintRight %d at %d", m_lastRow, m_paintedArea.right()*8);
	onPaintRow( m_lastRow, Crect( m_paintedArea.right(), 0, itemBlocks(), m_rect.height()));
	m_lastRow++;
	// First paint, then
	m_paintedArea.increase( itemBlocks(), 0);
	//Log.write( "paintBottom [%d %d] - top=%d bottom=%d", m_firstItem, m_lastItem, m_paintedArea.top()*8, m_paintedArea.bottom()*8);
}

#if 0 // Inside your inherited code.
void CswypeDialog::onPaintRow( int index, const Cpoint &location)
{
}
#endif

/// @brief Paint all things.
void CswypeDialog::onPaint()
{
	if ( !m_visible)
	{
		return;
	}
	if (m_horizontal)
	{
		int left =(int)(m_scroll)+m_paintedArea.left()*8-m_firstRow*8*itemBlocks();
		if ( left !=m_surfacePixels.left())
		{
			m_surfacePixels.setLeft( left);
			m_repaint =true;
		}
		m_surfacePixels.setSize( m_rect.size()*8);
		onPaintHorizontal();
	}
	else
	{
		int top =(int)(m_scroll)+m_paintedArea.top()*8-m_firstRow*8*itemBlocks();
		if ( top !=m_surfacePixels.top())
		{
			m_surfacePixels.setTop( top);
			m_repaint =true;
		}
		m_surfacePixels.setSize( m_rect.size()*8);
		onPaintVertical();
	}
	if ( m_repaint ==true && m_visible ==true)
	{
		m_mainGraph->copy( m_graphics, m_rect*8, m_surfacePixels);
		if(D)Log.write("copy pixels (%d,%d)-(%d,%d)", m_surfacePixels.left(), m_surfacePixels.top(), m_surfacePixels.width(), m_surfacePixels.height());
		insertDragObject();
		m_mainGraph->update( m_rect.left()*8, m_rect.top()*8, m_rect.width()*8, m_rect.height()*8);
		m_repaint =false;
	}
}

void CswypeDialog::onCleanup()
{

}

/// @brief  Scroll to absolute position.
/// @return True if changed value.
bool CswypeDialog::scrollToPixel( int offset, int time)
{
	if ( time<=0)
	{
		m_moveTime =0;
		int maxScroll =scrollMax();
		offset =myLimit(offset, 0,maxScroll);
		return scrollRelative( (double)offset-m_scroll, false);
	}
	else
	{
		m_startPosition =(int)m_scroll;
		m_endPosition =offset;
		m_timer.setTime( time, 100,3);
		m_moveTime =time;
		return true;
	}
}

/** @brief Calculate the maximum scroll distance.
 *  @return Number of rows in the scrolling dialog.
 */
int CswypeDialog::scrollMax()
{
	m_sizes =rows();
	int mx =m_sizes*itemBlocks()*8+m_endMargin-
			( m_horizontal ? (m_rect.width()*8):(m_rect.height()*8) );
	if ( mx<0) mx=0;
	return mx;
}

/// @brief Calculate the maximum scroll distance.
int CswypeDialog::surfaceMax()
{
	return m_horizontal ? ( m_graphics->width()-m_rect.width()*8-m_itemBlocks*8):
			              ( m_graphics->height()-m_rect.height()*8-m_itemBlocks*8);
}

/// @brief  Scroll a certain distance.
bool CswypeDialog::scrollRelative( double distance, bool flow)
{
	if ( distance >-0.1 && distance<0.1)
	{
		return false;
	}
	int db =(int)m_scroll;
	m_scroll +=distance;
	if ( m_scroll<0)
	{
		m_scroll =0;
		m_speeding.clear();
		flow =false;
	}
	double mx=scrollMax();
	if ( m_scroll >mx)
	{
		m_scroll =mx;
		m_speeding.clear();
		flow =false;
	}
	if ( flow)
	{
		m_speeding.addPosition( m_scroll);
	}
	if ( (int)(m_scroll+0.5) ==db)
	{
		return true;
	}
	m_repaint =true;
	calculateSurfacePosition();
	return true;
}

/// @brief Calculate a new paintedArea.
void CswypeDialog::resetPaintedArea()
{
	m_repaint =false;
	if ( m_horizontal)
	{
		m_paintedArea.setWidth(0);
		//int columnsVisible =m_rect.width()/itemBlocks();
		m_firstRow =(int)m_scroll/(itemBlocks()*8);
		//m_firstRow -=columnsVisible;
		//if ( m_firstRow<0)
		//{
		//	m_firstRow =0;
		//}
		m_lastRow =m_firstRow;
		m_paintedArea.setLeft( myMin( m_rect.width()/itemBlocks(),m_firstRow)*itemBlocks());
		if ( m_paintedArea.left()==0)
		{
			m_paintedArea.setLeft(40);
		}
	}
	else
	{
		m_paintedArea.setHeight(0);
		m_firstRow =(int)m_scroll/(itemBlocks()*8);
		m_lastRow =m_firstRow;
		m_paintedArea.setTop( myMin( m_rect.height()/3/itemBlocks(),m_firstRow)*itemBlocks());
	}
}

/** @brief Make sure the item is visible.
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 */
void CswypeDialog::makeSureRowVisible( int row, int time)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;

	if ( m_horizontal)
	{
		int width =(m_surfacePixels.width()/pixels);
		int margin =width-(width/3);
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			scrollToPixel( row-pixels*(width/3), time);
			//scrollTo( y-m_rect.height()*4, time);
			return;
		}
		if ( row>(int)(m_scroll+0.5)+m_surfacePixels.width()-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
			//scrollTo( y-m_rect.height()*4, time);
		}
	}
	else
	{
		int height =(m_surfacePixels.height()/pixels);
		int margin =height-(height/3);
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			scrollToPixel( row-pixels*(height/3), time);
			//scrollTo( y-m_rect.height()*4, time);
			return;
		}
		if ( row>(int)(m_scroll+0.5)+m_surfacePixels.height()-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
			//scrollTo( y-m_rect.height()*4, time);
		}
	}
}

/** @brief Is the row visible?
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 *  @return TRUE on visible.
 */
bool CswypeDialog::isRowVisible( int row)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;

	if ( m_horizontal)
	{
		int width =(m_surfacePixels.width()/pixels);
		int margin =width-(width/3);
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scroll+0.5)+m_surfacePixels.width()-pixels)
		{
			return false;
		}
	}
	else
	{
		int height =(m_surfacePixels.height()/pixels);
		int margin =height-(height/3);
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scroll+0.5)+m_surfacePixels.height()-pixels)
		{
			return false;
		}
	}
	return true;
}

/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
void CswypeDialog::calculateSurfacePosition()
{
	int maxScroll =scrollMax();
	m_scroll =myLimit( m_scroll, 0, maxScroll);

	if ( m_horizontal)
	{
		int left =(int)(m_scroll+0.5)+m_paintedArea.left()*8-m_firstRow*8*itemBlocks();
		//int top =m_scroll-m_firstItem*8*itemBlocks();

		if ( left<0 || left>surfaceMax())
		{
			resetPaintedArea();
			left =(int)(m_scroll+0.5)+m_paintedArea.left()*8-m_firstRow*8*itemBlocks();
			//top =m_scroll-m_firstItem*8*itemBlocks();
		}
		m_surfacePixels.setLeft( left);
		if(D)Log.write( "scroll =%3.1f, left=%d  first=%d  last=%d",
				m_scroll, m_surfacePixels.left(), m_firstRow, m_lastRow);
	}
	else
	{
		int top =(int)(m_scroll+0.5)+m_paintedArea.top()*8-m_firstRow*8*itemBlocks();
		//int top =m_scroll-m_firstItem*8*itemBlocks();

		if ( top<0 || top>surfaceMax())
		{
			resetPaintedArea();
			top =(int)(m_scroll+0.5)+m_paintedArea.top()*8-m_firstRow*8*itemBlocks();
			//top =m_scroll-m_firstItem*8*itemBlocks();
		}
		m_surfacePixels.setTop(top);
		if(D)Log.write( "scroll =%3.1f, top=%d  first=%d  last=%d",
				m_scroll, m_surfacePixels.top(), m_firstRow, m_lastRow);
	}
}

/// @brief Set index to a certain item in the list.
/// @return True if changed.
bool CswypeDialog::scrollIndex( double row, int time)
{
	bool retVal =scrollToPixel( row*(itemBlocks()*8.0f), time);
	return retVal;
}


/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
size_t CswypeDialog::rows()
{ 
    return 10; 
}


/** @brief Calculate height of a single row/item.
 *  @return Height of a single item/row.
 */
int CswypeDialog::itemBlocks()
{ 
    return m_itemBlocks;
}

/*============================================================================*/
///
/// @brief 		Loop for the dialog.
///
/// @post       Dialog changed.
///
/*============================================================================*/
bool CswypeDialog::onLoop()
{
	if ( m_visible ==false)
	{
		return m_alive;
	}
	//double speed;
	switch ( CdialogEvent::Instance()->getStatus())
	{
	case MOUSE_START_DRAG:
	case MOUSE_START_SCROLL_OR_DRAG:
		m_dialogSpeed =0;
		m_speeding.clear();
		break;

	case MOUSE_RELEASED:
		//speed =m_speeding.getRelative();
		//Log.write( "Scroll speed =%f", speed);
		if ( m_moveTime !=0)
		{
			m_speeding.clear();
		}
		scrollRelative( m_speeding.getSpeed(), false);
		break;

	case MOUSE_SCROLL:
	case MOUSE_DRAG:
	case MOUSE_PRESS:
	case MOUSE_DRAG_DIALOG:
	default:
		break;
	}
	if ( m_moveTime >0)
	{
		int timer =m_timer.elapsed();
		if ( timer>=m_moveTime)
		{
			// Scroll immediately to this position.
			if ( scrollToPixel( m_endPosition, 0)==true)
			{
				onUpdate();
			}
		}
		else
		{
			double offset =((m_endPosition-m_startPosition)*timer/m_moveTime)+m_startPosition;
			int maxY =scrollMax();
			offset =myLimit(offset, 0,maxY);
			double relative =offset-m_scroll;
			if ( relative >-10.0 && relative<10.0)
			{
				if(D)Log.write( "Reach end!");
				if ( scrollToPixel( m_endPosition, 0)==true)
				{
					onUpdate();
				}
				// Finish scrolling.
//				m_moveTime =0;
				//m_dialogSpeed =0;
//				m_speeding.clear();
//				m_timer.stop();
			}
			else
			{
				scrollRelative( relative, false);
				if(D)Log.write("Scroll %d to %d", relative, m_scroll);
			}
		}
	}
	CswypeDialog::onPaint();
	return m_alive;
}

void CswypeDialog::clearSpeed()
{
	m_speed =0;
	m_speeding.clear();
}
/*============================================================================*/
///
/// @brief		Left button pressed for mouse.
///
/// @post       Mouse event handled.
///
/*============================================================================*/
Estatus CswypeDialog::onButton( SDLMod mod, keybutton sym)
{
	(void)mod;
	(void)sym;
	Estatus stat =DIALOG_EVENT_OPEN;
	return stat;
}

/** Decide which key in a scroll dialog. */
keybutton CswypeDialog::findButton( const Cpoint &p)
{
	if ( m_visible ==false)
	{
		return KEY_NONE;
	}
	m_dialogSpeed =0;
	Cpoint q=p/8;
	if ( m_rect.inside( q)==true && m_visible ==true)
	{
		int item =(p.y-m_rect.top()*8+m_scroll)/(8*itemBlocks());
		keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
		Caudio::Instance()->play(AUDIO_CLICK);
		return key;
	}
	return KEY_NONE;
}

/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelUp( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(-2, false);
}

/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelDown( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(+2, false);
}

/// @brief constructor.
Cspeeding::Cspeeding()
: m_measurements(0)
, m_previousTime(0)
{
	m_friction =CFG("scroll_friction");
}

/// @brief Add a new position to the speeding.
void Cspeeding::addPosition( double position)
{
	//if(D)Log.write( "Add position %f", position);
	if ( (int)position ==(int)m_position[0])
	{
		return; // same position.
	}
	int now =Ctimeout::GetTickCount();
	int delta_t=m_previousTime-now;
	if ( delta_t<0) { delta_t =now-m_previousTime; }
	m_previousTime =now;
	if ( delta_t>4000)
	{
		m_measurements =0;
	}
	if ( delta_t<4)
	{
		// Same time, no use to record timestamps.
		m_position[0]=position;
		return;
	}
	int n=( m_measurements>=MEASUREMENTS) ? MEASUREMENTS-1:m_measurements;
	for ( int x=n-1; x>=0; x--)
	{
		m_position[x+1] =m_position[x];
		m_time[x+1] =m_time[x];
	}
	m_position[0] =position;
    m_time[0] =(double)delta_t;
    if ( m_measurements<MEASUREMENTS) m_measurements++;
}

/// @brief Get speed to run now.
double Cspeeding::getSpeed()
{
	if ( m_measurements==0)
	{
		return 0;
	}
	double totalSpeed =0;
	/// Calculate speeds.
	for ( int n=0; n<m_measurements-1; n++)
	{
		m_speed[n] =(m_position[n+1]-m_position[n])/(m_time[n]+0.001);
		totalSpeed -=m_speed[n];
	}
	int now=Ctimeout::GetTickCount();
	int delta_t=m_previousTime-now;
	if ( delta_t<0) { delta_t =now-m_previousTime; }
	if ( delta_t<20 || m_friction>1000)
	{
		return 0;
	}
	//totalSpeed/=(1+m_friction*y);
	return totalSpeed*1000.0f/(1000.0f+(delta_t*m_friction)/10.0f);
}

/** @brief Paint one item in the list again.
 *  @param row [in] What item to paint.
 */
void CswypeDialog::invalidate( int row)
{
	if ( row<0)
	{
		m_repaint =true;
		return;
	}
	if ( m_horizontal)
	{
		int left =m_paintedArea.left()+(row-m_firstRow)*itemBlocks();
		if ( left>=0 && (left+itemBlocks())*8<m_surfacePixels.width())
		{
			// Fits in current dialog.
			Crect rect( left, m_rect.top(), itemBlocks(), m_rect.height());
			onPaintRow( row, rect);
			m_repaint =true;
		}
	}
	else
	{
		int top =m_paintedArea.top()+(row-m_firstRow)*itemBlocks();
		int bottom =(top+itemBlocks())*8;
		if ( top>=0 && bottom<=m_graphics->height()) //m_surfacePixels.height())
		{
			// Fits in current dialog.
			Crect rect( m_rect.left(), top, m_rect.width(), itemBlocks());
			onPaintRow( row, rect);
			m_repaint =true;
		}
	}
}

/** @brief Change cursor to other position.
 *  @param row [in] New cursor position.
 */
void CswypeDialog::setCursor( int row)
{
	if ( row ==m_cursor)
	{
		invalidate( m_cursor);
		makeSureRowVisible( row, 1000);
		return;
	}
	int prev =m_cursor;
	m_cursor =row;
	invalidate( prev);
	invalidate( row);
	makeSureRowVisible( row, 1000);
}

/** @brief Find object for a certain point. Not in sub-dialogs! */
CdialogObject * CswypeDialog::findObject( const Cpoint &p)
{
	Cpoint q=p/8;
	if ( m_rect.inside( q)==false || m_visible ==false)
	{
		return NULL;
	}
	int itemPixelHeight =8*itemBlocks();
	int row =(p.y-m_rect.top()*8+m_scroll)/itemPixelHeight;
	if ( row<0 || row>=(int)rows())
	{
		return NULL;
	}
	m_dragIndex =row;
	m_object->m_rect.setTop( m_rect.top()+(m_dragIndex*itemPixelHeight-m_scroll)/8 );
	//m_object->m_rect.setTop( m_rect.top()+(itemPixelHeight-m_scroll)/8 );
	return m_object;
}

/** Paint on normal background */
void CswypeDialog::onPaintBackground( int row, const Crect &location)
{
	(void)row;
	Cgraphics *previous =m_graphics;
	//m_object->m_parent =NULL;
	Cdialog *parent =m_parent;
	m_graphics =m_mainGraph;
	m_parent =NULL;
	m_object->m_graphics =m_mainGraph;
	m_graphics =m_mainGraph;
	onPaintRow( m_dragIndex, location);
	m_graphics =previous;
	m_parent =parent;
}

/** @brief Scroll to a certain row (or column).
 *  @param row [in] What row for vertical scrolling, what column for horizontal scrolling.
 *  @param time [in] Time to get to that row.
 *  @return If we have a go!
 */
bool CswypeDialog::scrollToRow( int row, int time)
{
	return scrollToPixel( row*itemBlocks()*8, time);
}
