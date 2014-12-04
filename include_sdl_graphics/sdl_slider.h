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
 **  Copyright (c) Bart Houkes, 24 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "zhongcan_defines.h"
#include "sdl_graphics.h"
#include "lingual.h"
#include "sdl_dialog_object.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Border type for buttons.
typedef enum
{
	SLIDER_HORIZONTAL,
	SLIDER_VERTICAL,
	SLIDER_VERTICAL_UPDOWN
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
	bool    setRange( double minimum, double maximum);

	void setValue( double y)
	{
		m_value =myLimit( y, m_minimum, m_maximum-m_itemsOnScreen);
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
	int					m_image_y; ///< Position slider.
	int 				m_unitSize;

	double  			m_value; ///< Current y-value.
	double 				m_minimum; ///< Minimum slider value.
	double  			m_maximum; ///< Maximum slider value.
	double  			m_unit; ///< Height of one thing in your list.
	bool				m_offDisabled; ///< temporary disabled until next press.
	int					m_itemsOnScreen; ///< Items fitting on screen.
};

