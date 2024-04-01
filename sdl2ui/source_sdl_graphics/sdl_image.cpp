/*============================================================================*/
/**  @file       sdl_image_base.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Shared base for images.
 **
 **  Create a default image.
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

/*------------- Standard includes --------------------------------------------*/
#include <ctype.h>
#include <SDL_image.h>

#include "sdl_image.h"
#include "utf8string.hpp"
#include "var_string.hpp"

const std::map<std::string, EimageStyle> Cimage::m_monoImages =
{
    { "abc", IMAGE_BACKGROUND_COLOUR },
    { "address_book", IMAGE_BACKGROUND_COLOUR },
	{ "advertisement", IMAGE_BACKGROUND_COLOUR },
	{ "asset", IMAGE_BACKGROUND_COLOUR },
	{ "back",  IMAGE_BACKGROUND_COLOUR },
    { "bag", IMAGE_BACKGROUND_COLOUR },
    { "button_mobile", IMAGE_BACKGROUND_COLOUR },
    { "cancel", IMAGE_CANCEL_COLOUR },
    { "check", IMAGE_BACKGROUND_COLOUR },
    { "delivery", IMAGE_BACKGROUND_COLOUR },
    { "down", IMAGE_BACKGROUND_COLOUR },
    { "drawer", IMAGE_BACKGROUND_COLOUR },
    { "enter", IMAGE_OK_COLOUR },
    { "eraser", IMAGE_BACKGROUND_COLOUR },
    { "euro", IMAGE_BACKGROUND_COLOUR },
    { "floorplan", IMAGE_BACKGROUND_COLOUR },
    { "graph", IMAGE_BACKGROUND_COLOUR },
    { "height_min", IMAGE_BACKGROUND_COLOUR },
    { "height_plus", IMAGE_BACKGROUND_COLOUR },
    { "icon_123_", IMAGE_BACKGROUND_COLOUR },
	{ "icon_add_part", IMAGE_BACKGROUND_COLOUR },
    { "icon_backspace", IMAGE_BACKGROUND_COLOUR },
    { "icon_calculator", IMAGE_BACKGROUND_COLOUR  },
	{ "icon_cancel", IMAGE_BACKGROUND_COLOUR },
    { "icon_colour", IMAGE_BACKGROUND_COLOUR },
    { "icon_credit", IMAGE_BACKGROUND_COLOUR },
	{ "icon_discount", IMAGE_BACKGROUND_COLOUR },
    { "icon_disk", IMAGE_BACKGROUND_COLOUR },
	{ "icon_eye", IMAGE_BACKGROUND_COLOUR },
	{ "icon_info", IMAGE_BACKGROUND_COLOUR },
	{ "icon_inside", IMAGE_BACKGROUND_COLOUR },
	{ "icon_invoice", IMAGE_BACKGROUND_COLOUR },
    { "icon_key", IMAGE_BACKGROUND_COLOUR },
    { "icon_magnify", IMAGE_BACKGROUND_COLOUR },
    { "icon_manager_key", IMAGE_BACKGROUND_COLOUR },
	{ "icon_menu_copy", IMAGE_BACKGROUND_COLOUR },
	{ "icon_menu_remove", IMAGE_BACKGROUND_COLOUR },
	{ "icon_money_table", IMAGE_BACKGROUND_COLOUR },
	{ "icon_more", IMAGE_BACKGROUND_COLOUR },
    { "icon_move", IMAGE_BACKGROUND_COLOUR },
	{ "icon_person_add", IMAGE_BACKGROUND_COLOUR },
	{ "icon_shop", IMAGE_BACKGROUND_COLOUR },
    { "icon_shop_add", IMAGE_BACKGROUND_COLOUR },
	{ "icon_sort", IMAGE_BACKGROUND_COLOUR },
    { "icon_stamp", IMAGE_BACKGROUND_COLOUR },
    { "icon_statiegeld", IMAGE_BACKGROUND_COLOUR },
    { "icon_takephone", IMAGE_BACKGROUND_COLOUR },
    { "icon_tax", IMAGE_BACKGROUND_COLOUR },
    { "icon_transfer", IMAGE_BACKGROUND_COLOUR },
	{ "icon_translate", IMAGE_BACKGROUND_COLOUR },
	{ "icon_twin", IMAGE_BACKGROUND_COLOUR },
    { "icon_type", IMAGE_BACKGROUND_COLOUR },
	{ "icon_usb", IMAGE_BACKGROUND_COLOUR },
	{ "icon_visible", IMAGE_BACKGROUND_COLOUR },
	{ "icon_waiter_key", IMAGE_BACKGROUND_COLOUR },
	{ "icon_wallet", IMAGE_BACKGROUND_COLOUR },
    { "icon_wok", IMAGE_BACKGROUND_COLOUR },
    { "languages", IMAGE_BACKGROUND_COLOUR },
    { "left", IMAGE_BACKGROUND_COLOUR },
    { "menu", IMAGE_BACKGROUND_COLOUR },
    { "new_menu", IMAGE_BACKGROUND_COLOUR },
    { "printer", IMAGE_BACKGROUND_COLOUR },
    { "right", IMAGE_BACKGROUND_COLOUR },
    { "screensaver", IMAGE_BACKGROUND_COLOUR },
    { "settings", IMAGE_BACKGROUND_COLOUR },
    { "shutdown", IMAGE_BACKGROUND_COLOUR },
    { "sitin", IMAGE_BACKGROUND_COLOUR },
    { "split", IMAGE_BACKGROUND_COLOUR },
    { "trash", IMAGE_BACKGROUND_COLOUR },
    { "uncheck", IMAGE_BACKGROUND_COLOUR },
    { "up", IMAGE_BACKGROUND_COLOUR },
    { "width_min", IMAGE_BACKGROUND_COLOUR },
    { "width_plus", IMAGE_BACKGROUND_COLOUR },
};

/*============================================================================*/
///
/// @brief Constructor.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Cimage::Cimage( Cdialog *parent, const Crect &rect, keybutton code,
		        const std::string &icon, EborderType border, int margin)
: CdialogObject( parent, rect, code)
, m_imageAlign( GRAVITY_LEFT_CENTER)
, m_parent(NULL)
, m_noBackground(false)
, m_border(border)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_margin(margin)
//, m_radius(8)
, m_label( parent, rect, code)
, m_size(0,0)
, m_surface(NULL)
, m_background( parent, rect, KEY_NOCHANGE,
		        Cgraphics::m_defaults.button_background2,
		        8,
			    FILL_CIRCULAR,
			    Cgraphics::m_defaults.button_background2)
, m_imageStyle( IMAGE_DEFAULT)
, m_imageColour(0x006060)
{
    m_label.setVisible(false);
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	setImage(icon, m_imageAlign);
	m_path =Cgraphics::m_defaults.image_path;
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Cimage::~Cimage()
{
	close();
}

void Cimage::setLabel( const std::string &label)
{
	m_label.setText(label);
}

/// @brief Close the image and free memory.
void Cimage::close()
{
	if ( m_surface !=NULL)
	{
		SDL_FreeSurface( m_surface);
		m_surface =NULL;
	}
}

/// Load image surface from disk. For each image.
/// @param fileName [in] Which file to use.
/// @todo Maybe not used.
void Cimage::load( const std::string &fileName)
{
	close();
	if ( fileName.find( "/")==std::string::npos)
	{
		std::string str=Cgraphics::m_defaults.image_path;
		str +=fileName;
		m_surface =IMG_Load( str.c_str());
	}
	else
	{
		m_surface =IMG_Load( fileName.c_str());
	}
	if ( m_surface !=NULL)
	{
		m_size.setHeight( m_surface->h);
		m_size.setWidth( m_surface->w);
	}
	else
	{
		m_size.setHeight( 0);
		m_size.setWidth( 0);
	}
}

void Cimage::save( const std::string &fileName)
{
	SDL_SaveBMP( m_surface, fileName.c_str());
}

/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void Cimage::paintBackground( int touch)
{
	if ( m_noBackground==false || touch>0)
	{
		int margin =m_background.m_borderWidth;
		m_background.m_borderWidth -=touch/8;
		m_background.setRect(m_rect);
		m_background.onPaint( touch);
		m_background.m_borderWidth =margin;
	}
}

/*============================================================================*/
///
/// @brief Paint border of button.
///
/*============================================================================*/
void Cimage::paintBorder( int touch)
{
	(void)touch;
	int linewidth =2;
	colour col1=0,col2=0;

	switch (m_border)
	{
	case BORDER_NONE:
		return;
	case BORDER_THIN:
		col1 =m_border1;
		col2 =m_border1;
		break;
	case BORDER_THIN_SHADOW:
	case BORDER_THIN_BUTTON_SHADOW:
		col1 =m_border1;
		col2 =m_border2;
		break;
	case BORDER_THIN_SELECTED:
		col1 =m_border2;
		col2 =m_border1;
		break;
	case BORDER_THICK:
		linewidth =4;
		col1 =m_border1;
		col2 =m_border1;
		break;
	case BORDER_THICK_SHADOW:
		linewidth =4;
		col1 =m_border1;
		col2 =m_border2;
		break;
	case BORDER_THICK_SELECTED:
		linewidth =4;
		col1 =m_border2;
		col2 =m_border1;
		break;
	case BORDER_ITEM_THICK:
		linewidth =4;
		col1 =m_border2;
		col2 =m_border2;
		break;
	}
	int x1 = (m_rect.left() << 3), y1 = (m_rect.top() << 3);
	int x2 = (m_rect.width() << 3) + x1, y2 = (m_rect.height() << 3) + y1;

	m_pGraphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_pGraphics->setColour( col1 );
		m_pGraphics->line( x1+a, y1+a, x2-a-1, y1+a);
		m_pGraphics->line( x2-a-1, y1+a+1, x2-a-1, y2-a-1);
		m_pGraphics->setColour( col2 );
		m_pGraphics->line( x2-a-1, y2-a-1, x1+a, y2-a-1);
		m_pGraphics->line( x1+a, y2-a-1, x1+a, y1+a+1);
	}
	m_pGraphics->unlock();
}

/*============================================================================*/
///
/// @brief Paint image next to text.
///
/*============================================================================*/
void Cimage::paintImage( int touch)
{
	(void)touch;
	int x1 = (m_rect.left() << 3);
	int y1 = (m_rect.top() << 3);
	int x2 = (m_rect.width() << 3) + x1;
	int y2 = (m_rect.height() << 3) + y1;
	int sizex =0;
	int sizey =0;

	if ( m_size.width()==0)
	{
		SDL_Surface *bitmap =m_pGraphics->findImage( m_image, (m_imageStyle!=IMAGE_DEFAULT));
		if (  bitmap)
		{
			sizex=bitmap->w;
			sizey=bitmap->h;
		}
	}
	else
	{
		sizex=m_size.width()*8-2*m_margin;
		sizey=m_size.height()*8-2*m_margin;
	}
	x1 +=m_margin;
	x2 -=m_margin;
	y1 +=m_margin;
	y2 -=m_margin;
	if ( sizex>x2-x1)
	{
		sizex=x2-x1;
	}
	if ( sizey>y2-y1)
	{
		sizey=y2-y1;
	}
	// x
	switch( m_imageAlign)
	{
	case GRAVITY_LEFT:
    case GRAVITY_LEFT_TOP:
	case GRAVITY_LEFT_CENTER:
	case GRAVITY_LEFT_BOTTOM:
		break;

    case GRAVITY_RIGHT:
    case GRAVITY_RIGHT_TOP:
	case GRAVITY_RIGHT_CENTER:
	case GRAVITY_RIGHT_BOTTOM:
		x1=x2-sizex;
		break;

	default:
	case GRAVITY_CENTER:
	case GRAVITY_TOP_CENTER:
	case GRAVITY_BOTTOM_CENTER:
		x1=(x1+x2)/2-(sizex/2);
		break;
	}
	// y
	switch( m_imageAlign)
	{
	case GRAVITY_LEFT:
	case GRAVITY_RIGHT:
	case GRAVITY_TOP_CENTER:
	case GRAVITY_LEFT_TOP:
	case GRAVITY_RIGHT_TOP:
		break;

	default:
	case GRAVITY_LEFT_CENTER:
	case GRAVITY_RIGHT_CENTER:
	case GRAVITY_CENTER:
		y1=(y1+y2)/2-(sizey/2);
		break;

	case GRAVITY_LEFT_BOTTOM:
	case GRAVITY_RIGHT_BOTTOM:
	case GRAVITY_BOTTOM_CENTER:
		y1=y2-sizey;
		break;
	}
	if ( m_surface)
	{
		m_pGraphics->surface( m_surface, x1,y1,x2-x1,y2-y1);
	}
	else
	{
		switch ( m_imageStyle)
		{
		case IMAGE_CANCEL_COLOUR:
		    m_pGraphics->imageColour( m_image, x1, y1, x2, y2, 0x00d00060);
		    break;
		case IMAGE_OK_COLOUR:
		    m_pGraphics->imageColour( m_image, x1, y1, x2, y2, 0x0000ff06);
		    break;
		case IMAGE_DEFAULT:
			m_pGraphics->image( m_image, x1, y1, x2, y2);
			break;
		case IMAGE_MONO_COLOUR:
			m_pGraphics->imageColour( m_image, x1, y1, x2, y2, m_imageColour);
			break;
		case IMAGE_INVERSE_COLOUR:
			m_pGraphics->imageInverse( m_image, x1, y1, x2, y2, m_imageColour);
			break;
		case IMAGE_BACKGROUND_COLOUR:
            m_pGraphics->imageWithColourChart( m_image, x1, y1, x2, y2);
            //m_pGraphics->imageWithBackgroundColour( m_image, x1, y1, x2, y2);
            break;
		}
	}
}

/*============================================================================*/
///
///  @brief 	Paint the dialog once. Use invalidate() to re-paint soon.
///
///	 @pre		Called when dialog is invalidated.
///
///  @post		Dialog updated. Buttons, drag and background are done automatic.
///
/*============================================================================*/
void Cimage::onPaint( int touch)
{
	if ( !m_visible)
	{
		return;
	}
	// Draw background fill.
	paintBackground( touch);
	// Draw button edge.
	paintBorder( touch);
	// Draw button image.
	paintImage( touch);
	// Draw label
	m_label.onPaint( touch);
	// Set mouse touch.
	m_pGraphics->setKey( m_rect, m_code);
}

/*============================================================================*/
///
/// @brief Set new image
///
/// @param image [in] Image to display.
/// @param horizontal [in] New gravity.
/// @param variable_size [in] If * in the name, the variable size will fill in. -1=rect.height
///
/*============================================================================*/
void Cimage::setImage( const std::string &image, Egravity horizontal, int variable_size)
{
    if ( image.size()==0)
    {
        m_image = image;
        m_visible=false;
        return;
    }
    m_imageAlign =horizontal;

    size_t pos = image.find( "*", 0);
    m_visible= true;

    if (pos != std::string::npos) // Last character after the dot
    {
        std::string icon = image.substr(0, pos);
        if ( Cgraphics::m_defaults.use_mono_images)
        {
            auto iter = m_monoImages.find(icon);
            if ( iter != m_monoImages.end())
            {
                m_imageStyle = iter->second;
            }
        }
        if ( variable_size<=0)
        {
            variable_size = 8*gMin( m_rect.width(), m_rect.height());
        }
        else
        {
            variable_size <<=3;
        }
        variable_size-=2*m_margin;
        variable_size-=(variable_size&7);
        var_string s(icon);
        s << variable_size << ".png";
        m_image = s.string();
    }
    else
    {
        m_image = image;
    }
}

void Cimage::clear()
{
	m_image ="";
}

/*============================================================================*/
///
/// @brief Set gravity.
///
/// @param horizontal [in] New gravity.
///
/*============================================================================*/
void Cimage::setGravity( Egravity horizontal)
{
	m_imageAlign =horizontal;
}

/*============================================================================*/
///
/// @brief Set new colour.
///
/// @param background [in] Background to display.
///
/*============================================================================*/
void Cimage::setBackgroundColour( colour background, int radius)
{
	m_background.setColours( background, background);
	m_background.setRadius( radius);
	m_noBackground =false;
	m_background.setFillStyle( FILL_UNICOLOURED);
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cimage::setBorder( EborderType border)
{
	m_border =border;
}

/// Paint to absolute coordinates.
void Cimage::onPaint( const Cpoint &p, int touch)
{
	Crect r=m_rect;
	m_rect.setLeft( gMin( p.x, (Cgraphics::m_defaults.width/8)-m_rect.width()));
	m_rect.setTop( gMin( p.y, (Cgraphics::m_defaults.height/8)-m_rect.height()));
	onPaint( touch);
	m_rect =r;
}

/** @brief New function to set the style of images.
 *  @param style [in] Whether we want default of floodfill the image
 */
void Cimage::setStyle( EimageStyle style, colour col)
{
	m_imageStyle =style;
	m_imageColour =col;
}

void Cimage::setRect( const Crect &rect)
{
    CdialogObject::setRect(rect);
    m_label.setRect(rect);
}
