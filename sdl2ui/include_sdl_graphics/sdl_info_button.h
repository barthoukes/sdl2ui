/*============================================================================*/
/**  @file      sdl_info_button.h
 **  @ingroup   user_interface
 **  @brief		Draw info button.
 **
 **  Create an info button with text on the left of the content.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CinfoButton
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
#include <memory>
#include "sdl_button.h"

/// @brief Button with text in left as info.
class CinfoButton : public CdialogObject
{
public:
	CinfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code=KEY_NONE,
			     const std::string &defaultText="",
			     int widthTitle=-1);

	virtual ~CinfoButton();
	void	onPaint( int touch);
	void	setText( int line, const std::string &text);
	void	setText( textId text);
	void 	setNumber( int line, int text);
	void    push_back(int width, const std::string &text, keybutton key);
	void	setFillType( int line, EfillType fill);  //  FILL_UNICOLOURED

	std::string getText(int line);
	void 	setVisible( bool visible);
	void    setTop( int top);

	//void 	setImage( const std::string &image);
	void	setTitleColours( colour infoText,
			            colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void	setDataColours( colour infoText,
			            colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void    setTitleWidth( int width);
	keybutton getKey( const Cpoint &p) const;

public:
	Cbutton	                    m_title;  ///< Title button.
	std::vector<CtextButtonPtr> m_data;   ///< Data inside button.
};

class CverticalInfoButton : public CinfoButton
{
	public:
		CverticalInfoButton( Cdialog *parent,
				     const Crect &rect,
				     textId id,
				     keybutton code=KEY_NONE,
				     const std::string &defaultText="");
};

typedef std::shared_ptr<CinfoButton> CinfoButtonPtr;
typedef std::shared_ptr<CverticalInfoButton> CverticalInfoButtonPtr;

/* INFO_BUTTON */
