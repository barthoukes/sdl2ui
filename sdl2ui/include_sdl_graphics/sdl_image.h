/*============================================================================*/
/**  @file      sdl_image.h
 **  @ingroup   user_interface
 **  @brief		Draw buttons.
 **
 **  Create and show images.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cimage
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
#include "sdl_rectangle.h"
#include "sdl_label.h"
#include "sdl_background.h"

/// Style for the image
typedef enum
{
	IMAGE_DEFAULT,
	IMAGE_COLOUR,
	IMAGE_INVERSE_COLOUR
} EimageStyle;

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class Cimage : public CdialogObject
{
public:
	Cimage( Cdialog *parent, const Crect &rect, keybutton code, const std::string &icon,
			EborderType border=BORDER_NONE, int margin=0, const std::string &label="");
	void setLabel( const std::string &label);
	virtual ~Cimage();

public:
	void	onPaint( int touch);
	void    close();
	void 	setImage( const std::string &image, Egravity horizontal, int variable_size=-1);
	std::string getImage() { return m_image; }
	void	setBackgroundColour( colour background, int radius=0);
	void	setBorder( EborderType border);
	void 	setCross( bool enable);
	void    load( const std::string &fileName);
	void 	save( const std::string &fileName);
	virtual void onPaint( const Cpoint &p, int touch);
	void    setGravity( Egravity horizontal);
	void    setStyle( EimageStyle style, colour col);
	Egravity gravity() { return m_imageAlign; }

private:
	void 	paintBackground( int touch);
	void	paintBorder( int touch);
	void	paintImage( int touch);

private:
	Egravity			m_imageAlign; ///< Align of image.
	std::string			m_image;	///< Image name.
	std::string			m_press;    ///< Image when pressed.
	Cdialog				*m_parent;	///< Parent dialog for the icon.
public:
	bool				m_noBackground; ///< No background.
private:
	EborderType			m_border;	///< Border.
	colour				m_border1;	///< Border colour 1.
	colour				m_border2;	///< Border colour 2.
	int					m_margin; ///< Margin.
	Clabel				m_label;    ///< Label to use.
	std::string 		m_path; 	///< Path for image.
	Csize				m_size; 	///< Real size
protected:
	sdlTexture			*m_surface; ///< Surface to use if no image.
	Cbackground			m_background; ///< My background.
	EimageStyle			m_imageStyle; ///< What style we want
	colour				m_imageColour; ///< For certain styles
	std::string			m_imagePattern; ///< For certain styles
};

