/*============================================================================*/
/**  @file      sdl_button.h
 **  @ingroup   user_interface
 **  @brief		Draw buttons.
 **
 **  Create and show buttons.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cbutton
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 10 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "sdl_button.h"

/// @brief Button with text in left as info.
class CinfoButton : public CdialogObject
{
public:
	CinfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code=KEY_NONE,
			     const std::string &defaultText="");

	virtual ~CinfoButton();
	void	onPaint( int touch);
	void	setText( const std::string &text);
	std::string getText();

	//void 	setImage( const std::string &image);
	void	setTitleColours( colour infoText,
			            colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void	setDataColours( colour infoText,
			            colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void    setTitleWidth( int width);
    //void    setFont( const Sfont &font);
	//void 	roundedRectangle( int radius);
	//void	noLabel();

public:
	Cbutton		m_title;	///< Title button.
	CtextButton	m_data;		///< Data inside button.
};

/* INFO_BUTTON */
