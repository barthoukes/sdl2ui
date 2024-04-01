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
	Cpoint( int xx, int yy);
	~Cpoint();
	int left() const;
	int top() const;
	Cpoint div8() const;
	Cpoint & operator /=(int n);
	Cpoint & operator -=(const Cpoint &v);
	int horizontalDistance( const Cpoint &v) const;
	int verticalDistance( const Cpoint &v) const;
	int distance( const Cpoint b) const;
	void limit( const Cpoint &leftTop, const Cpoint &rightBottom);
	void alignMatrix( int width);

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
	Crect( int xx, int yy, int ww, int hh);
	Crect( int xx, int yy, int ww, int hh, int index, int numberOfItems);
	Crect( const Cpoint &side, const Csize &size, int margin=0);
	~Crect();
	int left() const;
	int top() const;
	int bottom() const;
	int right() const;
	int height() const;
	int width() const;
	void setTop( int yy);
	void setBottom( int yy);
	void setLeft( int xx);
	void setRight( int xx);
	int addLeft( int xx);
	int addTop( int yy);
	void setWidth( int ww);
	void setHeight( int hh);
	void move( int x, int y);
	void decrease( int w, int h);
	void increase( int w, int h);
	void setSize( const Csize &size);
	Crect offset( int x, int y, int w, int h) const;
    Crect offset( int x, int y) const;
	void setPosition( const Cpoint &point);
	Csize size() const;
	operator Csize() const;
	Crect neighbour( const Csize &size, int margin=0) const;
	bool inside( const Cpoint &point) const;
	bool collision( const Crect &rect) const;
	bool overlap( const Crect &rect) const;
	Cpoint leftBottom()	const;
	Cpoint leftTop() const;
	Cpoint rightTop() const;
	Cpoint rightBottom() const;
	void setLeftTop( const Cpoint &p);
	void setRightTop( const Cpoint &p);
	void setRightBottom( const Cpoint &p);
	void setLeftBottom( const Cpoint &p);
	Cpoint origin() const;

private:
	Csize 	m_size; ///< Width.
	Cpoint	m_origin;  ///< Height
};

Cpoint operator -( const Cpoint &a, const Cpoint &b);

Cpoint operator +( const Cpoint &a, const Cpoint &b);

Crect operator +( const Crect &a, const Cpoint &b);

Crect operator *( const Crect &b, int x);

Cpoint operator /( const Cpoint &p, int y);

bool operator ==( const Cpoint &p, const Cpoint &q);

bool operator !=( const Cpoint &p, const Cpoint &q);

Csize operator -( const Csize &a, const Csize &b);

Csize operator *( const Csize &b, int x);

Cpoint operator *( const Cpoint &b, int x);

bool operator ==( const Csize &p, const Csize &q);

bool operator !=( const Csize &p, const Csize &q);

bool operator ==( const Crect &p, const Crect &q);
/* SDL_RECT_H_ */
