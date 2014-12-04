/*============================================================================*/
/**  @file       sdl_button.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default button implementation.
 **
 **  Create a default button. This is a basic template for all buttons.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cbutton
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 28 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#include "sdl_info_button.h"

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param type [in] What type of button.
/// @param key  [in] What key.
/// @param text [in] What text.
/// @param icon [in] Which icon.
///
/*============================================================================*/
CinfoButton::CinfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code,
			     const std::string &defaultText)
: CdialogObject( parent, rect, code),
  m_title( parent,
		   Crect( rect.left(), rect.top(), 10, rect.height()),
		   code,
		   (Sfont)CtextFont("info_button"),
		   id,
		   BORDER_NONE,
		   GRAVITY_RIGHT_CENTER,
		   0,
		   FILL_GRADIENT),
  m_data( parent,
		  Crect( rect.left()+10, rect.top(), rect.width()-10, rect.height()),
		  code,
		  defaultText)
{
	m_title.setLabel("");
}

CinfoButton::~CinfoButton()
{
}

void CinfoButton::onPaint( int touch)
{
	m_title.onPaint( 0);
	m_data.onPaint( touch);
}

void CinfoButton::setText( const std::string &text)
{
	m_data.setText( text);
}

std::string CinfoButton::getText()
{
	return m_data.getText();
}

void CinfoButton::setTitleColours( colour infoText,
			            colour background1, colour background2)
{
	(void)infoText;
	(void)background1;
	(void)background2;
}

void CinfoButton::setDataColours( colour infoText,
			            colour background1, colour background2)
{
	m_data.setColours( infoText, background1, background2);
}

void CinfoButton::setTitleWidth( int width)
{
	(void)width;
}

