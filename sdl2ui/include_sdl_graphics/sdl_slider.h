/*============================================================================*/
/**  @file      sdl_slider.h
 **  @ingroup   user_interface
 **  @brief		Draw scrollbar.
 **
 **  Create and show scrollbar.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cslider
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
#include <string>
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Border type for buttons.
typedef enum
{
	SLIDER_HORIZONTAL,
	SLIDER_VERTICAL,
	SLIDER_VERTICAL_UPDOWN,
	SLIDER_HORIZONTAL_BAR,
	SLIDER_VERTICAL_BAR
} EsliderType;

/// @brief  Create and display buttons.
class Cslider : public CdialogObject
{
public:
	Cslider( const Crect &rect, int unitSize, keybutton button, EsliderType type=SLIDER_VERTICAL_UPDOWN);
	virtual ~Cslider();

public:
	void	onPaint( int touch);
	virtual void onPaint( const Cpoint &p, int touch) { (void)p; onPaint(touch); }
	void 	setImage( const std::string &image);
	void	setColours( colour scrollbar, colour background);
	void	paintVertical();
	void    paintHorizontal();
	void    paintHorizontalBar();
	bool    setRange( double minimum, double maximum);

	virtual void setValue( double y)
	{
		m_value =gLimit( y, m_minimum, m_maximum-m_itemsOnScreen);
	}
	double getValue() { return m_value; }
	int getInt() { return (int)(m_value+0.5); }
	virtual bool onDragStart( Cpoint p);
	virtual bool onDrag( Cpoint p);
	virtual bool onDragEnd( Cpoint p);
	virtual bool wheelUp();
	virtual bool wheelDown();

private:
	void 	paintBackground();
	void	paintBorder();
	void	paintImage();

private:
	EsliderType			m_sliderType;	///< Type for the slider.
	std::string			m_icon;		///< Image for the button.
	std::string			m_image;	///< Image name.
	std::string			m_text;		///< Text to show.
	colour				m_scrollbarColour; ///< Colour text.
	colour				m_backgroundColour; ///< Background colour.
	bool				m_noBackground; ///< No background.
	bool				m_imageEnable; ///< Image enabled.
	int					m_image_x; ///< Position slider.
	int					m_image_y; ///< Position slider.
	int 				m_unitSize;

protected:
	double  			m_value; ///< Current y-value.
	double 				m_minimum; ///< Minimum slider value.
	double  			m_maximum; ///< Maximum slider value.

private:
	double  			m_unit; ///< Height of one thing in your list.

	bool				m_offDisabled; ///< temporary disabled until next press.
	int					m_itemsOnScreen; ///< Items fitting on screen.
};

