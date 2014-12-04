/*
 * sdl_touch.h
 *
 *  Created on: 17 feb. 2013
 *      Author: mensfort
 */

#pragma once

#include "keybutton.h"
#include "sdl_rect.h"

/// @brief Touch library.
class Ctouch
{
public:
	Ctouch();
	virtual ~Ctouch();

	void clear();
	void setSize( const Csize &size);
	void setKey( const Crect &rect, keybutton key);
	keybutton getKey( const Cpoint &size);
	void freeMemory();

private:
	keybutton * m_touchButtons;
	Csize       m_size;
};


/* SDL_TOUCH_H_ */
