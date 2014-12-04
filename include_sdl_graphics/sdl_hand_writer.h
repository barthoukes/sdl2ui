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
 **  Copyright (c) Bart Houkes, 23 sep 2013
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include "sdl_image.h"
#include "zinnia.h"

/// @brief  Create and display buttons.
class ChandWriter : public Cimage
{
public:
	ChandWriter( Cdialog *parent, const Crect &rect);
	virtual ~ChandWriter();

public:
	void    clearImage();
	virtual bool onDrag( Cpoint p);
	virtual bool onDragEnd( Cpoint p);
	virtual void onPaint( const Cpoint &p);
	std::string get( size_t n);
	void addPoint( int x, int y);

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
};

