/*============================================================================*/
/**  @file      sdl_hand_writer.h
 **  @ingroup   user_interface
 **  @brief		Chinese hand-writing library.
 **
 **  Create and show images.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              ChandWriter
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
#include "sdl_image.h"
#include "zinnia.h"

/// @brief  Create and display buttons.
class ChandWriter : public Cimage
{
public:
	ChandWriter( Cdialog *parent, const Crect &rect, const std::string &model, int distance);
	virtual ~ChandWriter();

public:
	void    clearImage();
	virtual void onPaint( const Cpoint &p);
	std::string get( size_t n);
	void addPoint( int x, int y);
	virtual bool onPaintingStart( const Cpoint &point);
	virtual bool onPaintingMove( const Cpoint &point);
	virtual bool onPaintingStop( const Cpoint &point);

private:
	int 	m_stroke; ///< Index for our stroke.
	int 	m_distance; ///< Distance for current stroke.
	int		m_index;	///< which sub-line inside a stroke.
	bool 	m_started; ///< Is the mouse pressed.
	Cpoint  m_lastPoint; ///< Last point.
	zinnia_recognizer_t *m_recognizer;
	zinnia_character_t 	*m_character;
	zinnia_result_t 	*m_result;
	char 	m_value[4];
	int		m_minimum_distance; ///< Minimum distance between points
};

