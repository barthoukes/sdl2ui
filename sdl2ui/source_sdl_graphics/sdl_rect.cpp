/*============================================================================*/
/**  @file       sdl_rect.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Rectangle for our system
 **
 **  Simple rectangle, simple size and simple implementation of a point
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cpoint, Csize, Crect
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

/*------------- Standard includes --------------------------------------------*/
#include <stdlib.h>
#include "sdl_rect.h"
#include "sdl_graphics.h"

Cpoint & Cpoint::operator /=(int n)
{
	x /=n;
	y /=n;
	return *this;
}

Cpoint & Cpoint::operator -=(const Cpoint &v)
{
	x -=v.x;
	y -=v.y;
	return *this;
}

int Cpoint::horizontalDistance( const Cpoint &v) const
{
	return abs( v.x-x);
}

int Cpoint::verticalDistance( const Cpoint &v) const
{
	return abs( v.y-y);
}

Crect::Crect( int xx, int yy, int ww, int hh)
: m_size(ww, hh)
, m_origin( xx, yy)
{}

Crect::Crect( int xx, int yy, int ww, int hh, int index, int numberOfItems)
: m_size((index+1)*ww/numberOfItems-index*ww/numberOfItems, hh)
, m_origin(xx+index*ww/numberOfItems, yy)
{
}

Crect::Crect( const Cpoint &side, const Csize &size, int margin)
: m_size(size)
, m_origin(side.x+margin, (side.y-size.height())/2)
{
	if (right() > Cgraphics::m_defaults.width/8)
	{
		setLeft( left()-size.width()-margin);
	}
	if ( top() <0)
	{
		setTop( 0);
	}
	if ( bottom() > Cgraphics::m_defaults.height/8)
	{
		setBottom(Cgraphics::m_defaults.height/8);
	}
}

/*----------------------------------------------------------------------------*/
Crect::~Crect()
{}

/*----------------------------------------------------------------------------*/
int Crect::left() const
{
	return m_origin.x;
}

/*----------------------------------------------------------------------------*/
int Crect::top() const
{
	return m_origin.y;
}

/*----------------------------------------------------------------------------*/
bool operator ==( const Crect &p, const Crect &q)
{
	return p.left() ==q.left() &&
		   p.height() ==q.height() &&
		   p.width() ==q.width() &&
		   p.top() ==q.top();
}

/*----------------------------------------------------------------------------*/
int Crect::bottom() const
{
	return m_origin.y+m_size.height();
}

/*----------------------------------------------------------------------------*/
int Crect::right() const
{
	return m_origin.x+m_size.width();
}

/*----------------------------------------------------------------------------*/
int Crect::height() const
{
	return m_size.height();
}

/*----------------------------------------------------------------------------*/
int Crect::width() const
{
	return m_size.width();
}

/*----------------------------------------------------------------------------*/
void Crect::setTop( int yy)
{
	m_origin.y =yy;
}

/*----------------------------------------------------------------------------*/
void Crect::setBottom( int yy)
{
	m_origin.y =yy-m_size.height();
}

/*----------------------------------------------------------------------------*/
void Crect::setLeft( int xx)
{
	m_origin.x =xx;
}

/*----------------------------------------------------------------------------*/
void Crect::setRight( int xx)
{
	m_size.setWidth( xx-m_origin.x);
}

/*----------------------------------------------------------------------------*/
int Crect::addLeft( int xx)
{
	m_origin.x +=xx;
	return m_origin.x;
}

/*----------------------------------------------------------------------------*/
int Crect::addTop( int yy)
{
	m_origin.y +=yy;
	return m_origin.y;
}

/*----------------------------------------------------------------------------*/
void Crect::setWidth( int ww)
{
	m_size.setWidth(ww);
}

/*----------------------------------------------------------------------------*/
void Crect::setHeight( int hh)
{
	m_size.setHeight(hh);
}

/*----------------------------------------------------------------------------*/
void Crect::move( int x, int y)
{
	m_origin.x +=x;
	m_origin.y +=y;
}

/*----------------------------------------------------------------------------*/
void Crect::decrease( int w, int h)
{
	m_size.decrease( w,h);
}

/*----------------------------------------------------------------------------*/
void Crect::increase( int w, int h)
{
	m_size.increase( w,h);
}

/*----------------------------------------------------------------------------*/
void Crect::setSize( const Csize &size)
{
	m_size =size;
}

/*----------------------------------------------------------------------------*/
Crect Crect::offset( int x, int y, int w, int h) const
{
	return Crect( left()+x, top()+y, w,h);
}

/*----------------------------------------------------------------------------*/
Crect Crect::offset( int x, int y) const
{
    return Crect( left()+x, top()+y, width(),height());
}

/*----------------------------------------------------------------------------*/
void Crect::setPosition( const Cpoint &point)
{
	m_origin =point;
}

/*----------------------------------------------------------------------------*/
Csize Crect::size() const
{
	return m_size;
}

/*----------------------------------------------------------------------------*/
Crect::operator Csize() const
{
	return m_size;
}

/*----------------------------------------------------------------------------*/
bool Crect::inside( const Cpoint &point) const
{
	return point.x>=left() && point.x<right() && point.y>=top() && point.y<bottom();
}

/*----------------------------------------------------------------------------*/
bool Crect::collision( const Crect &rect) const
{
	if ( left()>rect.right() || right()<rect.left() || top()>rect.bottom() || bottom()<rect.top())
	{
		return false;
	}
	return true;
}

/*----------------------------------------------------------------------------*/
bool Crect::overlap( const Crect &rect) const
{
	if ( left()>=rect.right() || right()<=rect.left() || top()>=rect.bottom() || bottom()<=rect.top())
	{
		return false;
	}
	return true;
}

/*----------------------------------------------------------------------------*/
Cpoint Crect::leftBottom()	const
{
	return Cpoint( left(), bottom());
}

/*----------------------------------------------------------------------------*/
Cpoint Crect::leftTop() const
{
	return Cpoint( left(), top());
}

/*----------------------------------------------------------------------------*/
Cpoint Crect::rightTop() const
{
	return Cpoint( right(), top());
}

/*----------------------------------------------------------------------------*/
Cpoint Crect::rightBottom() const
{
	return Cpoint( right(), bottom());
}

/*----------------------------------------------------------------------------*/
void Crect::setLeftTop( const Cpoint &p)
{
	m_origin =p;
}

/*----------------------------------------------------------------------------*/
void Crect::setRightTop( const Cpoint &p)
{
	m_origin =Cpoint( p.x-width(), p.y);
}

/*----------------------------------------------------------------------------*/
void Crect::setRightBottom( const Cpoint &p)
{
	m_origin =Cpoint( p.x-width(), p.y-height());
}

/*----------------------------------------------------------------------------*/
void Crect::setLeftBottom( const Cpoint &p)
{
	m_origin =Cpoint( p.x, p.y-height());
}

/*----------------------------------------------------------------------------*/
Cpoint Crect::origin() const
{
	return m_origin;
};

/*----------------------------------------------------------------------------*/
Cpoint operator -( const Cpoint &a, const Cpoint &b)
{
	return Cpoint( a.x-b.x, a.y-b.y);
}

/*----------------------------------------------------------------------------*/
Cpoint operator +( const Cpoint &a, const Cpoint &b)
{
	return Cpoint( a.x+b.x, a.y+b.y);
}

/*----------------------------------------------------------------------------*/
Crect operator +( const Crect &a, const Cpoint &b)
{
	return Crect( a.left()+b.left(), a.top()+b.top(), a.width(), a.height());
}

/*----------------------------------------------------------------------------*/
Crect operator *( const Crect &b, int x)
{
	Crect a( b.left()*x, b.top()*x, b.width()*x, b.height()*x);
	return a;
}

/*----------------------------------------------------------------------------*/
Cpoint operator /( const Cpoint &p, int y)
{
	return Cpoint( p.left()/y, p.top()/y);
}

/*----------------------------------------------------------------------------*/
bool operator ==( const Cpoint &p, const Cpoint &q)
{
	return (p.x==q.x && p.y==q.y);
}

/*----------------------------------------------------------------------------*/
bool operator !=( const Cpoint &p, const Cpoint &q)
{
	return (p.x!=q.x || p.y!=q.y);
}

/*----------------------------------------------------------------------------*/
Csize operator -( const Csize &a, const Csize &b)
{
	return Csize( a.width()-b.width(), a.height()-b.height());
}

/*----------------------------------------------------------------------------*/
Csize operator *( const Csize &b, int x)
{
	Csize a( b.width()*x, b.height()*x);
	return a;
}

/*----------------------------------------------------------------------------*/
Cpoint operator *( const Cpoint &b, int x)
{
	Cpoint a( b.x*x, b.y*x);
	return a;
}

/*----------------------------------------------------------------------------*/
bool operator ==( const Csize &p, const Csize &q)
{
	return (p.height()==q.height() && p.width()==q.width());
}

/*----------------------------------------------------------------------------*/
bool operator !=( const Csize &p, const Csize &q)
{
	return (p.height()!=q.height() || p.width()!=q.width());
}

/** Find neighbour rectangle with a certain size */
Crect Crect::neighbour( const Csize &size, int margin) const
{
	Crect retVal(right()+margin, (top()+bottom()-size.height())/2, size.width(), size.height());
	if (retVal.right() > Cgraphics::m_defaults.width/8)
	{
		retVal.setLeft( left()-size.width()-margin);
	}
	if ( retVal.top() <0)
	{
		retVal.setTop( 0);
	}
	if ( retVal.bottom() > Cgraphics::m_defaults.height/8)
	{
		retVal.setBottom(Cgraphics::m_defaults.height/8);
	}
	return retVal;
}

/*----------------------------------------------------------------------------*/
Cpoint::Cpoint( int xx, int yy)
{
	x=xx; y=yy;
}

/*----------------------------------------------------------------------------*/
Cpoint::~Cpoint()
{}

/*----------------------------------------------------------------------------*/
int Cpoint::left() const
{
	return x;
}

/*----------------------------------------------------------------------------*/
int Cpoint::top() const
{
	return y;
}

/*----------------------------------------------------------------------------*/
Cpoint Cpoint::div8() const
{
	return Cpoint( (x+4)/8, (y+4)/8);
}

/*----------------------------------------------------------------------------*/
int Cpoint::distance( const Cpoint b) const
{
	return (b.x-x)*(b.x-x)+(b.y-y)*(b.y-y);
}

/*----------------------------------------------------------------------------*/
void Cpoint::limit( const Cpoint &leftTop, const Cpoint &rightBottom)
{
	if ( x<leftTop.x)  x=leftTop.x;
	if ( y<leftTop.y)  y=leftTop.y;
	if ( x>rightBottom.x) x=rightBottom.x;
	if ( y>rightBottom.y) y=rightBottom.y;
}

/*----------------------------------------------------------------------------*/
void Cpoint::alignMatrix( int width)
{
	x =x+(width/2);
	y =y+(width/2);
	x =x/width;
	y =y/width;
	x =x*width;
	y =y*width;
}
