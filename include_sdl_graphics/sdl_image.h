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
 **  Copyright (c) Bart Houkes, 15 sep 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "sdl_rectangle.h"
#include "sdl_label.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class Cimage : public CdialogObject
{
public:
	Cimage( Cdialog *parent, const Crect &rect, keybutton code, const std::string &icon,
			EborderType border=BORDER_NONE, int margin=0, const std::string &label="");
	void setLabel( const std::string &label) { m_label.setText(label); }
	virtual ~Cimage();

public:
	void	onPaint( int touch);
	void    close();
	void 	setImage( const std::string &image, Egravity horizontal);
	std::string getImage() { return m_image; }
	void	setBackgroundColour( colour background, int radius=0);
	void	setBorder( EborderType border);
	void 	setCross( bool enable);
	void    load( const std::string &fileName);
	void 	save( const std::string &fileName);
	virtual void onPaint( const Cpoint &p, int touch);
	void setGravity( Egravity horizontal);

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
	EfillType			m_fill;		///< Fill background.
	colour				m_border1;	///< Border colour 1.
	colour				m_border2;	///< Border colour 2.
	int					m_margin; ///< Margin.
	int					m_radius;	///< Radius background.
	Clabel				m_label;    ///< Label to use.
	std::string 		m_path; 	///< Path for image.
	Csize				m_size; 	///< Real size
protected:
	colour				m_backgroundColour; ///< Background colour.
	SDL_Surface			*m_surface; ///< Surface to use if no image.
};

