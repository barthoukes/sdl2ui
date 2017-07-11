/*============================================================================*/
/**  @file       sdl_image.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default dialog.
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
#include <assert.h>
#include "utf8string.h"
#include "sdl_image.h"

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
		        const std::string &icon, EborderType border, int margin, const std::string &label)
: CdialogObject( parent, rect, code)
, m_imageAlign( GRAVITY_LEFT)
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
, m_imageColour(0)
{
	m_label.setText(label);
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	int sz =8*gMin( rect.width(), rect.height());

	m_image =icon;
	if ( icon.size() ==0)
	{
		setVisible(false);
	}
	size_t pos=m_image.find( "*", 0);

	if (pos!=std::string::npos) // Last character after the dot
	{
		char s[16];
		sz-=2*m_margin;
		sz-=(sz&7);
		sprintf( s, "%d", sz);
		m_image.erase( pos, 1);
		m_image.insert( pos, s);
	}
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
#ifdef USE_SDL2
		SDL_DestroyTexture( m_surface);
#else
		SDL_FreeSurface( m_surface);
#endif
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
#ifdef USE_SDL2
		SDL_Surface *surface =IMG_Load( str.c_str());
		m_surface =NULL;
		if (surface)
		{
			SDL_CreateTextureFromSurface(m_graphics->getRenderer(), surface);
			SDL_FreeSurface(surface);
		}
#else
		m_surface =IMG_Load( str.c_str());
#endif
	}
	else
	{
#ifdef USE_SDL2
		SDL_Surface *surface =IMG_Load( fileName.c_str());
		m_surface =NULL;
		if (surface)
		{
			SDL_CreateTextureFromSurface(m_graphics->getRenderer(), surface);
			SDL_FreeSurface(surface);
		}
#else
		m_surface =IMG_Load( fileName.c_str());
#endif
	}
	if ( m_surface !=NULL)
	{
#ifdef USE_SDL2
		m_size = m_graphics->textureSize( m_surface);
#else
		m_size.setHeight( m_surface->h);
		m_size.setWidth( m_surface->w);
#endif
		//m_rect.setSize( Csize( m_surface->w, m_surface->h));
	}
	else
	{
		m_size.setHeight( 0);
		m_size.setWidth( 0);
		//m_rect.setSize( Csize( 0,0));
	}
}

void Cimage::save( const std::string &fileName)
{
#ifdef USE_SDL2
	assert(0);
	//PixelFormat mask = GetMask(format);
	//SDL_Renderer *m_renderer; ///< Reders a window.
#else
	SDL_SaveBMP( m_surface, fileName.c_str());
#endif
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
		m_background.m_rect =m_rect;
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

	m_graphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_graphics->setColour( col1 );
		m_graphics->line( x1+a, y1+a, x2-a-1, y1+a);
		m_graphics->line( x2-a-1, y1+a+1, x2-a-1, y2-a-1);
		m_graphics->setColour( col2 );
		m_graphics->line( x2-a-1, y2-a-1, x1+a, y2-a-1);
		m_graphics->line( x1+a, y2-a-1, x1+a, y1+a+1);
	}
	m_graphics->unlock();
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
	x1 +=m_margin;
	x2 -=m_margin;
	y1 +=m_margin;
	y2 -=m_margin;
	Csize sz(0,0);

	sdlTexture *bitmap = m_surface ? m_surface:m_graphics->findImage( m_image);
	if ( bitmap == NULL)
	{
		return;
	}
	switch ( m_imageAlign)
	{
	case GRAVITY_RESIZE:
	case GRAVITY_BEHIND_TEXT:
		sz =Csize( x2-x1, y2-y1);
		break;
	default:
#ifdef USE_SDL2
		sz =m_graphics->textureSize(bitmap);
#else
		sz =Csize( bitmap->w, bitmap->h);
#endif
		break;
	}
	if ( sz.width() > x2-x1)
	{
		sz.setWidth(x2-x1);
	}
	if ( sz.height() > y2-y1)
	{
		sz.setHeight(y2-y1);
	}
	// x
	switch( m_imageAlign)
	{
	case GRAVITY_LEFT:
	case GRAVITY_LEFT_CENTER:
	case GRAVITY_LEFT_BOTTOM:
		break;

	case GRAVITY_RIGHT_CENTER:
	case GRAVITY_RIGHT_BOTTOM:
	case GRAVITY_RIGHT:
		x1=x2-sz.width();
		break;

	default:
	case GRAVITY_CENTER:
	case GRAVITY_TOP_CENTER:
	case GRAVITY_BOTTOM_CENTER:
		x1=(x1+x2-sz.width())/2;
		break;
	}
	// y
	switch( m_imageAlign)
	{
	case GRAVITY_LEFT:
	case GRAVITY_RIGHT:
	case GRAVITY_TOP_CENTER:
		break;

	default:
	case GRAVITY_LEFT_CENTER:
	case GRAVITY_RIGHT_CENTER:
	case GRAVITY_CENTER:
		y1=(y1+y2)/2-(sz.height()/2);
		break;

	case GRAVITY_LEFT_BOTTOM:
	case GRAVITY_RIGHT_BOTTOM:
	case GRAVITY_BOTTOM_CENTER:
		y1=y2-sz.height();
		break;
	}
#ifdef USE_SDL2
	m_graphics->renderTexture( bitmap, x1, y1, sz.width(), sz.height());
#else
	m_graphics->renderSurface( bitmap, x1, y1, sz.width(), sz.height());
#endif
}
//	else
//	{
//		switch ( m_imageStyle)
//		{
//		case IMAGE_DEFAULT:
//			m_graphics->image( m_image, x1, y1, x2, y2);
//			//m_graphics->image( m_image, x1, y1, x1+sizex+1, y1+sizex+1);
//			break;
//		case IMAGE_COLOUR:
//			m_graphics->imageColour( m_image, x1, y1, x2, y2, m_imageColour);
//			//m_graphics->imageColour( m_image, x1, y1, x1+sizex+1, y1+sizex+1, m_imageColour);
//			break;
//		case IMAGE_INVERSE_COLOUR:
//			m_graphics->imageInverse( m_image, x1, y1, x2, y2, m_imageColour);
//			//m_graphics->imageInverse( m_image, x1, y1, x1+sizex+1, y1+sizex+1, m_imageColour);
//		}
//	}
//}

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
	m_graphics->setCode( m_rect, m_code);
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
		m_visible=false;
		return;
	}
	m_image =image;
	m_imageAlign =horizontal;
	size_t pos=m_image.find( "*", 0);

	m_visible =true; //(image.size()!=0);
	if (pos!=std::string::npos) // Last character after the dot
	{
		char s[16];
		if ( variable_size<=0)
		{
			variable_size =m_rect.height();
		}
		//variable_size=(variable_size*8-m_margin*2)&0xfff8;
		sprintf( s, "%d", variable_size*8);
		m_image.erase( pos, 1);
		m_image.insert( pos, s);
	}
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
