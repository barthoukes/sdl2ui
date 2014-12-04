/*============================================================================*/
/**  @file       sdl_image.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 15 sep 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#include "sdl_image.h"
#include "graphics_widgets.h"

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
, m_fill(FILL_UNICOLOURED)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_margin(margin)
, m_radius(0)
, m_label( parent, rect, code)
, m_size(0,0)
, m_backgroundColour( Cgraphics::m_defaults.button_background2)
, m_surface(NULL)
{
	m_label.setText(label);
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	m_image =icon;
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
	SDL_SaveBMP( m_surface, fileName.c_str());
}

/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void Cimage::paintBackground( int touch)
{
	if ( m_fill ==FILL_UNICOLOURED && m_noBackground==false)
	{
		ColorAT( m_rect, m_backgroundColour, m_radius);
	}
}

/*============================================================================*/
///
/// @brief Paint border of button.
///
/*============================================================================*/
void Cimage::paintBorder( int touch)
{
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
	int x1 = (m_rect.left() << 3);
	int y1 = (m_rect.top() << 3);
	int x2 = (m_rect.width() << 3) + x1;
	int y2 = (m_rect.height() << 3) + y1;
	int sizex =0;
	int sizey =0;

	if ( m_size.width()==0)
	{
		SDL_Surface *bitmap =m_graphics->findImage( m_image);
		if (  bitmap)
		{
			sizex=bitmap->w-2*m_margin;
			sizey=bitmap->h-2*m_margin;
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
		m_graphics->surface( m_surface, x1,y1);
	}
	else
	{
		m_graphics->image( m_image, x1, y1, x1+sizex+1, y1+sizex+1);
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
	m_graphics->setCode( m_rect, m_code);
}

/*============================================================================*/
///
/// @brief Set new image
///
/// @param image [in] Image to display.
/// @param horizontal [in] New gravity.
///
/*============================================================================*/
void Cimage::setImage( const std::string &image, Egravity horizontal)
{
	m_image =image;
	m_imageAlign =horizontal;
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
	m_backgroundColour =background;
	m_radius =radius;
	m_noBackground =false;
	m_fill =FILL_UNICOLOURED;
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
