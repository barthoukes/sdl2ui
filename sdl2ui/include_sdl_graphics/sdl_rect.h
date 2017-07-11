/*============================================================================*/
/**  @file       sdl_rect.h
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

#pragma once

/*------------- Standard includes --------------------------------------------*/

/// Class for point
class Cpoint
{
public:
	Cpoint( int xx, int yy) { x=xx; y=yy; }
	Cpoint( const Cpoint &yy) { x=yy.x; y=yy.y; }
	~Cpoint() {}
	int left() const { return x; }
	int top() const { return y; }
	Cpoint div8() const { return Cpoint( (x+4)/8, (y+4)/8); }
	Cpoint & operator /=(int n);
	Cpoint & operator -=(const Cpoint &v);
	int horizontalDistance( const Cpoint &v) const;
	int verticalDistance( const Cpoint &v) const;
	int distance( const Cpoint b) const
	{
		return (b.x-x)*(b.x-x)+(b.y-y)*(b.y-y);
	}
	void limit( const Cpoint &leftTop, const Cpoint &rightBottom)
	{
		if ( x<leftTop.x)  x=leftTop.x;
		if ( y<leftTop.y)  y=leftTop.y;
		if ( x>rightBottom.x) x=rightBottom.x;
		if ( y>rightBottom.y) y=rightBottom.y;
	}
	void alignMatrix( int width)
	{
		x =x+(width/2);
		y =y+(width/2);
		x =x/width;
		y =y/width;
		x =x*width;
		y =y*width;
	}
public:
	int x;
	int y;
};


/// Class for size of an area
class Csize
{
public:
	Csize( int ww, int hh) { w=ww; h=hh; }
	~Csize() {}
	int width() const { return w; }
	int height() const { return h; }
	int area() const { return w*h; }
	operator Cpoint() { return Cpoint( w,h); }
	void setWidth(int x) { w=x; }
	void setHeight( int y) { h=y; }
	void decrease( int ww, int hh) { w-=ww; h-=hh; }
	void increase( int ww, int hh) { w+=ww; h+=hh; }
private:
	int w;
	int h;
};

/// Class for rectangle.
class Crect
{
public:
	Crect( int xx, int yy, int ww, int hh)
	: m_size(ww, hh)
	, m_origin( xx, yy)
	{}
	Crect( const Crect &y)
	: m_size( y.m_size)
	, m_origin( y.m_origin)
	{
	}
	virtual ~Crect() {}
	int left() const { return m_origin.x; }
	int top() const { return m_origin.y; }
	int bottom() const { return m_origin.y+m_size.height(); }
	int right() const { return m_origin.x+m_size.width(); }
	int height() const { return m_size.height(); }
	int width() const { return m_size.width(); }
	void setTop( int yy) { m_origin.y =yy; }
	void setBottom( int yy) { m_origin.y =yy-m_size.height(); }
	void setLeft( int xx) { m_origin.x =xx; }
	void setRight( int xx) { m_size.setWidth( xx-m_origin.x); }
	int addLeft( int xx) { m_origin.x +=xx; return m_origin.x; }
	int addTop( int yy) { m_origin.y +=yy; return m_origin.y; }
	void setWidth( int ww) { m_size.setWidth(ww); }
	void setHeight( int hh) { m_size.setHeight(hh); }
	void move( int x, int y) { m_origin.x +=x; m_origin.y +=y; }
	void decrease( int w, int h) { m_size.decrease( w,h); }
	void increase( int w, int h) { m_size.increase( w,h); }
	void setSize( const Csize &size) { m_size =size; }
	Crect offset( int x, int y, int w, int h) const
	{ return Crect( left()+x, top()+y, w,h); }
	virtual void setPosition( const Cpoint &point) { m_origin =point; }
	Csize size() const { return m_size; }
	operator Csize() { return m_size; }
	bool inside( const Cpoint &point) const
	{
		return point.x>=left() && point.x<right() && point.y>=top() && point.y<bottom();
	}
	bool collision( const Crect &rect) const
	{
		if ( left()>rect.right() || right()<rect.left() || top()>rect.bottom() || bottom()<rect.top())
		{
			return false;
		}
		return true;
	}
	bool overlap( const Crect &rect) const
	{
		if ( left()>=rect.right() || right()<=rect.left() || top()>=rect.bottom() || bottom()<=rect.top())
		{
			return false;
		}
		return true;
	}
	Cpoint leftBottom()	const { return Cpoint( left(), bottom());	}
	Cpoint leftTop() const { return Cpoint( left(), top()); }
	Cpoint rightTop() const { return Cpoint( right(), top()); }
	Cpoint rightBottom() const { return Cpoint( right(), bottom()); }
	void setLeftTop( const Cpoint &p) { m_origin =p; }
	void setRightTop( const Cpoint &p) { m_origin =Cpoint( p.x-width(), p.y); }
	void setRightBottom( const Cpoint &p) { m_origin =Cpoint( p.x-width(), p.y-height()); }
	void setLeftBottom( const Cpoint &p) { m_origin =Cpoint( p.x, p.y-height()); }
	Cpoint origin() { return m_origin; };

private:
	Csize 	m_size; ///< Width.
	Cpoint	m_origin;  ///< Height.
};

inline Cpoint operator -( const Cpoint &a, const Cpoint &b)
{
	return Cpoint( a.x-b.x, a.y-b.y);
}

inline Cpoint operator +( const Cpoint &a, const Cpoint &b)
{
	return Cpoint( a.x+b.x, a.y+b.y);
}


inline Crect operator +( const Crect &a, const Cpoint &b)
{
	return Crect( a.left()+b.left(), a.top()+b.top(), a.width(), a.height());
}

inline Crect operator *( const Crect &b, int x)
{
	Crect a( b.left()*x, b.top()*x, b.width()*x, b.height()*x);
	return a;
}

inline Cpoint operator /( const Cpoint &p, int y)
{
	return Cpoint( p.left()/y, p.top()/y);
}

inline bool operator ==( const Cpoint &p, const Cpoint &q)
{
	return (p.x==q.x && p.y==q.y);
}

inline bool operator !=( const Cpoint &p, const Cpoint &q)
{
	return (p.x!=q.x || p.y!=q.y);
}

inline Csize operator -( const Csize &a, const Csize &b)
{
	return Csize( a.width()-b.width(), a.height()-b.height());
}

inline Csize operator *( const Csize &b, int x)
{
	Csize a( b.width()*x, b.height()*x);
	return a;
}

inline Cpoint operator *( const Cpoint &b, int x)
{
	Cpoint a( b.x*x, b.y*x);
	return a;
}

inline bool operator ==( const Csize &p, const Csize &q)
{
	return (p.height()==q.height() && p.width()==q.width());
}

inline bool operator !=( const Csize &p, const Csize &q)
{
	return (p.height()!=q.height() || p.width()!=q.width());
}

/* SDL_RECT_H_ */
