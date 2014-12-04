/*
 * sdl_rect.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */

#include <stdlib.h>
#include "sdl_rect.h"

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

int Cpoint::horizontalDistance( const Cpoint &v)
{
	return abs( v.x-x);
}

int Cpoint::verticalDistance( const Cpoint &v)
{
	return abs( v.y-y);
}


