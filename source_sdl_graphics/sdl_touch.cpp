/*
 * sdl_touch.cpp
 *
 *  Created on: 17 feb. 2013
 *      Author: mensfort
 */

#include "sdl_touch.h"
#include "log_base.h"

/// @brief Constructor
Ctouch::Ctouch()
: m_touchButtons(NULL)
, m_size( 0,0)
{
}

//. @brief Destructor
Ctouch::~Ctouch()
{
	freeMemory();
}

/// @brief Free memory for touch controller.
void Ctouch::freeMemory()
{
	if ( m_touchButtons)
	{
		delete [] m_touchButtons;
		m_touchButtons =NULL;
	}
	m_size =Csize(0,0);
}

/// @brief Clear all area.
void Ctouch::clear()
{
	setKey( Crect(0,0, m_size.width(), m_size.height()), KEY_NONE);
}

/// @brief Set key for item.
void Ctouch::setKey( const Crect &rect, keybutton key)
{
	//Log.write( "x,y, w,h, code =%d,%d  %d,%d  %d", rect.left(), rect.top(), rect.width(), rect.height(), (int)key);
	if ( key ==KEY_NOCHANGE)
	{
		/// Nothing to change...
		return;
	}
	int l=rect.left();
	int r=rect.right();
	int t=rect.top();
	int b=rect.bottom();
	if ( l<0) l=0;
	if ( r>m_size.width()) r=m_size.width();
	if ( t<0) t=0;
	if ( b>m_size.height()) b=m_size.height();

	for ( int y=t; y<b; y++)
	{
		keybutton *ptr =m_touchButtons+m_size.width()*y;
		for ( int x=l; x<r; x++)
		{
			ptr[x] =key;
		}
	}
}

/// @brief Get a key from point on screen.
keybutton Ctouch::getKey( const Cpoint &size)
{
	if (  size.x<0 || size.y<0
       || size.x>=m_size.width() || size.y>=m_size.height())
	{
		return KEY_NONE;
	}
	return m_touchButtons[ m_size.width()*size.y+size.x];
}

/// @brief Change or set the size.
void Ctouch::setSize( const Csize &size)
{
	freeMemory();
	m_size =size;
	m_touchButtons =new keybutton[ size.area()];
}
