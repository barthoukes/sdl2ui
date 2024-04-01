/*============================================================================*/
/**  @file       sdl_button.cpp
 **  @ingroup    sdl2ui
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
#include "sdl_button.h"
#include "taal_type.h"
#include "lingual.h"

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
CmenuButton::CmenuButton( const Crect &rect,
						  keybutton key,
						  textId id,
						  const std::string &icon)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("button"), id,
		   BORDER_THIN_BUTTON_SHADOW,
		   GRAVITY_CENTER, 8, FILL_PYRAMID)
//					  Cbutton( rect, key, , id, BORDER_THIN, icon.length()? false:true, FILL_PYRAMID, icon)
{
    if ( icon.length())
	{
    	setImage( icon, GRAVITY_LEFT_CENTER);
	}
 	setColours( Cgraphics::m_defaults.menu_button_text,
 				Cgraphics::m_defaults.menu_button_background1,
 				Cgraphics::m_defaults.menu_button_background2);
	m_button_shadow =Cgraphics::m_defaults.button_shadow;
	roundedRectangle(8);
}

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
CmenuButton::CmenuButton( const Crect &rect,
						  keybutton key,
						  const std::string &text,
						  const std::string &icon)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("button"), text,
		   BORDER_THIN_BUTTON_SHADOW,
		   GRAVITY_CENTER, 8, FILL_PYRAMID)
//					  Cbutton( rect, key, , id, BORDER_THIN, icon.length()? false:true, FILL_PYRAMID, icon)
{
    if ( icon.length())
	{
    	setImage( icon, GRAVITY_LEFT_CENTER);
        //m_image.setStyle(IMAGE_COLOUR, Cgraphics::m_defaults.menu_button_text);
	}
 	setColours( Cgraphics::m_defaults.menu_button_text,
 				Cgraphics::m_defaults.menu_button_background1,
 				Cgraphics::m_defaults.menu_button_background2);
	m_button_shadow =Cgraphics::m_defaults.button_shadow;
	roundedRectangle(8);
}

/*============================================================================*/
///
/// @brief Constructor special types.
///
/// @param rect [in] Position.
/// @param type [in] What type of button.
/// @param key  [in] What key.
/// @param text [in] What text.
/// @param icon [in] Which icon.
///
/*============================================================================*/
CbottomButton::CbottomButton( Cdialog *parent,
		                  const Crect &rect,
						  keybutton key,
						  textId id,
						  const std::string &icon)
: Cbutton( parent, rect, key, id, BORDER_THIN, FILL_2COLOURS, icon)
{
    if ( icon.length())
	{
    	setImage( icon, GRAVITY_LEFT_CENTER);
	}
	setColours( Cgraphics::m_defaults.button_text,
			    Cgraphics::m_defaults.button_background1,
    			Cgraphics::m_defaults.button_background2);
    //m_image.setStyle(IMAGE_COLOUR, Cgraphics::m_defaults.button_text);
	roundedRectangle(0);
}

CbottomButton::CbottomButton( Cdialog *parent,
		       const Crect &rect,
			   keybutton key,
			   const std::string &text,
			   const std::string &icon)
: Cbutton( parent, rect, key, text, BORDER_THIN, FILL_2COLOURS, icon)
{
    if ( icon.length())
	{
    	setImage( icon, GRAVITY_LEFT_CENTER);
	}
	setColours( Cgraphics::m_defaults.button_text,
			    Cgraphics::m_defaults.button_background1,
    			Cgraphics::m_defaults.button_background2);
	roundedRectangle(0);
}

/*----------------------------------------------------------------------------*/
void Cbutton::decrease(int width, int height)
{
	m_rect.decrease(width, height);
	m_label.m_rect.addLeft(-width);
	m_label.m_rect.addTop(-height);
}

/*----------------------------------------------------------------------------*/
void Cbutton::setGreyedOut( bool greyed)
{
	m_greyedOut =greyed;
}

/*----------------------------------------------------------------------------*/
void Cbutton::addLeft( int left)
{
	m_rect.addLeft(left);
	m_label.m_rect.addLeft(left);
}

/*----------------------------------------------------------------------------*/
void Cbutton::setSpacing( int spacing)
{
	m_spacing =spacing;
}

/*----------------------------------------------------------------------------*/
void Cbutton::setImageGravity( Egravity gravity)
{
	m_image.setGravity( gravity);
}

/*----------------------------------------------------------------------------*/
void Cbutton::setTextGravity( Egravity text)
{
	if (m_textButton) m_textButton->setGravity( text);
}

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
CkeyboardButton::CkeyboardButton( const Crect &rect,
						  keybutton key,
						  textId id,
						  const std::string &icon)
: Cbutton( NULL, rect, key, id, BORDER_THIN, FILL_2COLOURS, icon)
{
	setColours( Cgraphics::m_defaults.button_text,
			Cgraphics::m_defaults.button_background1,
			Cgraphics::m_defaults.button_background2);
	roundedRectangle(12);
	setGravity( icon.length() ? GRAVITY_LEFT_CENTER:GRAVITY_CENTER);
}

/*============================================================================*/
///
/// @brief Constructor image with circular filled pattern.
///
/// @param parent [in] Who's my dialog?
/// @param rect [in] Position
/// @param key  [in] What key
/// @param icon [in] Which icon
/// @param background [in] Colour background
///
/*============================================================================*/
CiconButton::CiconButton( Cdialog *parent,
		                  const Crect &rect,
						  keybutton key,
						  const std::string &icon,
						  colour background)
: Cbutton( parent, rect, key, INVALID_TEXT_ID, BORDER_NONE, FILL_CIRCULAR)
{
	setColours( 0, Cgraphics::brighter( background, 50), background);
	setBorder( BORDER_NONE, 0);
	roundedRectangle( m_rect.height()*8/2-2);
	//setGravity( GRAVITY_LEFT_CENTER);
	setImage( icon, GRAVITY_CENTER);
	setLabel("");
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
/// @param background [in] Which icon.
///
/*============================================================================*/
CimageButton::CimageButton( const Crect &rect,
						  keybutton key,
						  textId id,
						  const std::string &background,
						  const std::string &image)
: Cbutton( NULL, rect, key, id, BORDER_THIN_BUTTON_SHADOW, FILL_IMAGE)
{
	setColours( Cgraphics::m_defaults.image_button_text);
	m_background.setBackground( (background.length()==0) ?
			 Cgraphics::m_defaults.image_button_background:background);
	roundedRectangle(12);
	if ( image.length())
	{
		setImage( image, GRAVITY_LEFT_CENTER);
	}
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
/// @param background [in] Which icon.
///
/*============================================================================*/
CfunctionButton::CfunctionButton( Cdialog *parent,
	     	 	 	 	 	 	  const Crect &rect,
	     	 	 	 	 	 	  keybutton key,
	     	 	 	 	 	 	  textId id,
								  const std::string &image,
	     	 	 	 	 	 	  colour col)
: Cbutton( NULL, rect, key, id, BORDER_THICK, FILL_PYRAMID)
{
	(void)parent;
	colour col1 =col;
	colour col2 =col;
	if ( col ==-1)
	{
		col1 =Cgraphics::m_defaults.function_button_background1;
		col2 =Cgraphics::m_defaults.function_button_background2;
	}
	else
	{
		col2 =m_pGraphics->brighter( col, 95);
	}
	if ( image.length())
	{
		setImage(image, GRAVITY_RIGHT_CENTER);
	}
	setColours( Cgraphics::m_defaults.function_button_text, col1, col2);
	setSpacing( 2);
	roundedRectangle(3);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
/// @param background [in] Which icon.
///
/*============================================================================*/
CgraphButton::CgraphButton( Cdialog *parent,
	      const Crect &rect,
	      const std::string &str,
	      bool rotate)
: Cbutton( parent, rect, KEY_NOCHANGE, (Sfont)CtextFont("graph"), str,
		   BORDER_NONE, GRAVITY_TOP_CENTER, 0, FILL_UNICOLOURED)
{
	(void)rotate;
	setColours( Cgraphics::m_defaults.graph_text, Cgraphics::m_defaults.graph_background1,
			    Cgraphics::m_defaults.graph_background2);
	if ( m_textButton)
	{
		m_textButton->rotate();
	}
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
///
/*============================================================================*/
CheaderButton::CheaderButton( const Crect &rect,
							  textId id,
			   	   	   	      keybutton key)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("title"), id, BORDER_NONE,
		   GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setImage( "", GRAVITY_CENTER);
	setColours( Cgraphics::m_defaults.header_text,
			    Cgraphics::m_defaults.header_background1,
			    Cgraphics::m_defaults.header_background2);
	roundedRectangle(0);
	setLabel("");
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
///
/*============================================================================*/
CheaderButton::CheaderButton( const Crect &rect,
							  const std::string &s,
			   	   	   	      keybutton key)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("title"), s,
		   BORDER_NONE, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setColours( Cgraphics::m_defaults.header_text,
		    Cgraphics::m_defaults.header_background1,
		    Cgraphics::m_defaults.header_background2);
	roundedRectangle(0);
	setLabel("");
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param parent [in] Where to place the button.
/// @param rect   [in] Position.
/// @param key    [in] What key.
/// @param id     [in] What text.
///
/*============================================================================*/
CmatrixButton::CmatrixButton( Cdialog *parent,
		                      const Crect &rect,
							  textId id,
							  colour text,
					          colour back1,
							  colour back2)
: Cbutton( parent, rect, KEY_NONE, (Sfont)CtextFont("matrix"), id, BORDER_NONE,
		GRAVITY_RIGHT_CENTER, 8, FILL_GRADIENT)
{
	setColours( text, back1, back2);
	setLabel("");
	roundedRectangle(0);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param parent [in] Where to place the button.
/// @param rect   [in] Position.
/// @param s      [in] What text.
/// @param key    [in] What key-code.
///
/*============================================================================*/
CmatrixButton::CmatrixButton( Cdialog *parent,
		                      const Crect &rect,
							  const std::string &s,
			   	   	   	      colour text,
			   	   	   	      colour back1,
			   	   	   	      colour back2)
: Cbutton( parent, rect, KEY_NONE, (Sfont)CtextFont("matrix"), s,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER, 8, FILL_GRADIENT)
{
	setColours( text, back1, back2);
	setLabel("");
	roundedRectangle(0);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param parent [in] Where to place the button.
/// @param rect   [in] Position.
/// @param s      [in] What text.
/// @param key    [in] What key-code.
///
/*============================================================================*/
CmatrixButton::CmatrixButton( Cdialog *parent,
		                      const Crect &rect,
		                      textId id,
			   	   	   	      keybutton key)
: Cbutton( parent, rect, key, (Sfont)CtextFont("matrix"), id,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER, 8, FILL_GRADIENT)
{
	setColours( Cgraphics::m_defaults.matrix_header_text,
			    Cgraphics::m_defaults.matrix_header_background1,
			    Cgraphics::m_defaults.matrix_header_background2);
	setLabel("");
	roundedRectangle(0);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param parent [in] Where to place the button.
/// @param rect   [in] Position.
/// @param s      [in] What text.
/// @param key    [in] What key-code.
///
/*============================================================================*/
CmatrixButton::CmatrixButton( Cdialog *parent,
		                      const Crect &rect,
		                      const std::string &s,
			   	   	   	      keybutton key)
: Cbutton( parent, rect, key, (Sfont)CtextFont("matrix"), s,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER, 8, FILL_GRADIENT)
{
	setColours( Cgraphics::m_defaults.matrix_header_text,
			    Cgraphics::m_defaults.matrix_header_background1,
			    Cgraphics::m_defaults.matrix_header_background2);
	setLabel("");
	roundedRectangle(0);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
///
/*============================================================================*/
CcalculatorButton::CcalculatorButton( const Crect &rect,
		       const std::string &s,
   	   	       keybutton key)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("calculator"), s,
		   BORDER_THICK, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setColours( Cgraphics::m_defaults.calculator_text,
			    Cgraphics::m_defaults.calculator_background2,
			    Cgraphics::m_defaults.calculator_background1);
	setBorderColours( Cgraphics::m_defaults.calculator_border);
	setSpacing(1);
	setLabel("");
	setShadow(0xff000000);
}

CcalculatorButton::CcalculatorButton( const Crect &rect,
		       const std::string &s,
   	   	       keybutton key,
			   const std::string &img)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("calculator"), s,
		   BORDER_THICK, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setImage(img, GRAVITY_CENTER);
	setColours( Cgraphics::m_defaults.calculator_text,
			    Cgraphics::m_defaults.calculator_background2,
			    Cgraphics::m_defaults.calculator_background1);
	setBorderColours( Cgraphics::m_defaults.calculator_border);
	setSpacing(1);
	setLabel("");
	setShadow(0xff000000);
}

CcalculatorButton::CcalculatorButton( const Crect &rect,
		       EtextId id,
   	   	       keybutton key)
: Cbutton( NULL, rect, key, (Sfont)CtextFont("calculator"), id,
		   BORDER_THICK, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setColours( Cgraphics::m_defaults.calculator_text,
			    Cgraphics::m_defaults.calculator_background2,
			    Cgraphics::m_defaults.calculator_background1);
	setBorderColours( Cgraphics::m_defaults.calculator_border);
	setSpacing(1);
	setLabel("");
	setShadow(0xff000000);
}

CcalculatorButton::CcalculatorButton( Simage *image, int x, int y)
: Cbutton( NULL, Crect(x+image->x,y+image->y, image->w, image->h), image->k,
		   (Sfont)CtextFont("calculator"),
		   image->name, BORDER_THICK, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setColours( Cgraphics::m_defaults.calculator_text,
			    Cgraphics::m_defaults.calculator_background2,
			    Cgraphics::m_defaults.calculator_background1);
	setBorderColours( Cgraphics::m_defaults.calculator_border);
	//roundedRectangle(16);
	setSpacing(1);
	setLabel("");
	setShadow(0xff000000);
}

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Cbutton::Cbutton( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          textId id,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: CdialogObject( parent, rect, code)
, m_background( parent, rect, code, Cgraphics::m_defaults.button_background1,
		        0, fill)
, m_image( parent, rect, KEY_NOCHANGE, icon, BORDER_NONE, 3)
, m_label( parent, rect, code)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId(id)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_alignBottom(false)
, m_textButton()
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
, m_imageGravity(GRAVITY_CENTER)
, m_greyedOut(false)
{
	m_image.setGravity( GRAVITY_LEFT);
	Sfont font( CtextFont("small_button"));
	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, code, font, id,
			                 Cgraphics::m_defaults.button_text,
			                 GRAVITY_CENTER);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	if ( id!=INVALID_TEXT_ID)
	{
		m_useText =false;
	}
	m_horizontal_margin =2;
	m_vertical_margin =2;
	roundedRectangle(0);
}

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Cbutton::Cbutton( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const std::string &text,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: CdialogObject( parent, rect, code)
, m_background( parent, rect, code, Cgraphics::m_defaults.button_background2, 0, fill)
, m_image( parent, rect, KEY_NOCHANGE, icon, BORDER_NONE, 3)
, m_label( parent, rect, code)
, m_text( text)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId( INVALID_TEXT_ID)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_textButton()
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
, m_imageGravity(GRAVITY_CENTER)
, m_greyedOut(false)
{
	m_image.setGravity( GRAVITY_LEFT);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	Sfont font( CtextFont("small_button"));

	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, code, font, text,
							 Cgraphics::m_defaults.button_text,
			                 GRAVITY_CENTER);
	m_horizontal_margin =2;
	m_vertical_margin =2;
	roundedRectangle( 0);
}

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Cbutton::Cbutton( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          textId id,
		          EborderType border,
		          Egravity gravity,
		          int radius,
		          EfillType fill)
: CdialogObject( parent, rect, code)
, m_background( parent, rect, code, Cgraphics::m_defaults.button_background2,
		   radius, fill, Cgraphics::m_defaults.button_background2)
, m_image( parent, rect, KEY_NOCHANGE, "", BORDER_NONE, 3)
, m_label( parent, rect, code)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId(id)
, m_useText(false)
, m_border1(Cgraphics::m_defaults.line_dark)
, m_border2(Cgraphics::m_defaults.line_bright)
, m_alignBottom(false)
, m_textButton()
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
, m_imageSize(0)
, m_imageGravity(GRAVITY_CENTER)
, m_greyedOut(false)
{
	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, code, font, id,
                             Cgraphics::m_defaults.button_text,
			                 gravity);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
}

Cbutton::Cbutton( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          const std::string &text,
		          EborderType border,
		          Egravity gravity,
		          int radius,
				  EfillType fill)
: CdialogObject( parent, rect, code)
, m_background( parent, rect, code, Cgraphics::m_defaults.button_background2,
		    radius, fill, Cgraphics::m_defaults.button_background2)
, m_image( parent, rect, KEY_NOCHANGE, "", BORDER_NONE, 3)
, m_label( parent, rect, code)
, m_text( text)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId( INVALID_TEXT_ID)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_dark)
, m_border2( Cgraphics::m_defaults.line_bright)
, m_alignBottom(false)
, m_textButton()
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(1)
, m_vertical_margin(1)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
, m_imageSize(0)
, m_imageGravity(GRAVITY_CENTER)
, m_greyedOut(false)
{
	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, code, font, text,
							 Cgraphics::m_defaults.button_text,
							 gravity);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
}

/*============================================================================*/
///
/// @brief Add the image to a certain point.
///
/// @param x [in] X-coordinate of drag-object.
/// @param y [in] Y-coordinate of drag object.
///
/*============================================================================*/
void Cbutton::onPaint( const Cpoint &p, int touch)
{
	Crect r=m_rect;
	int w=Cgraphics::m_defaults.width/8;
	int h=Cgraphics::m_defaults.height/8;
	m_rect.setLeft( gMin( p.x, w-m_rect.width()));
	m_rect.setTop( gMin( p.y, h-m_rect.height()));
	onPaint( touch);
	m_rect =r;
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Cbutton::~Cbutton()
{
	m_textButton = nullptr;
}

/** @brief Paint the image here.
 */
void Cbutton::paintImage()
{
//	int mywidth=width();
//	int myheight=height();
//	int hspace=(mywidth-m_imageSize)/2;
//	int vspace=(myheight-m_imageSize)/2;
//	int l,t;
//
//	switch (m_image.gravity())
//	{
//	case GRAVITY_LEFT:  		l=left();        t=top(); break;
//	case GRAVITY_LEFT_CENTER:	l=left();        t=top()+vspace; break;
//	case GRAVITY_LEFT_BOTTOM:	l=left();        t=bottom()-m_imageSize; break;
//	case GRAVITY_RIGHT_CENTER:	l=right()-m_imageSize;     t=top()+vspace; break;
//	case GRAVITY_RIGHT_BOTTOM:	l=right()-m_imageSize;     t=bottom()-m_imageSize; break;
//	case GRAVITY_RIGHT:			l=right()-m_imageSize;     t=top(); break;
//	case GRAVITY_TOP_CENTER:	l=left()+hspace; t=top(); break;
//	case GRAVITY_BOTTOM_CENTER:	l=left()+hspace; t=bottom()-m_imageSize; break;
//	default:
//	case GRAVITY_CENTER:		l=left()+hspace; t=top()+vspace; break;
//	}
	m_image.onPaint( Cpoint(left(),top()), 0);
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
void Cbutton::onPaint( const std::string &text, int touch)
{
	if ( !m_visible)
	{
		return;
	}
	// Draw background fill.
	paintBackground( touch);
	// Draw button edge.
	paintBorder( m_border, m_spacing, m_background.getRadius(), touch);
	// Draw button image.
	paintImage();
	// Draw button text.
	paintText(text, touch);
	// Draw label.
	paintLabel();
	// Set mouse touch.
	m_pGraphics->setKey( m_rect, m_greyedOut ? KEY_NONE:m_code);
}

/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void Cbutton::paintBackground( int touch)
{
	m_background.setRect(m_rect);
	switch ( m_border)
	{
	case BORDER_THIN_BUTTON_SHADOW:
		{
			Cbackground( m_pParent,
					     m_background.getRect()+Csize(0,1),
						 KEY_NONE,
						 touch ? COLOUR_BLACK:COLOUR_DARKGRAY,
						 m_background.getRadius(),
						 FILL_UNICOLOURED,
					 	 m_button_shadow).onPaint( touch);
		}
		break;
	default:
		break;
	}
	if ( m_noBackground ==false)
	{
		m_background.setBorderWidth( m_spacing);
		m_background.onPaint( touch);
	}
}

/*============================================================================*/
///
/// @brief Paint border of button.
///
/*============================================================================*/
void Cbutton::paintBorder( EborderType border, int spacing, int radius, int touch)
{
	int linewidth =2;
	colour col1=0,col2=0;
	(void)touch;

	switch ( border)
	{
	case BORDER_NONE:
		return;
	case BORDER_THIN:
		linewidth =1;
		col1 =m_border1;
		col2 =m_border1;
		break;
	case BORDER_THIN_SHADOW:
	case BORDER_THIN_BUTTON_SHADOW:
		linewidth =1;
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
	case BORDER_ITEM_THICK:
		linewidth =4;
		col1 =m_border2;
		col2 =m_border2;
		break;
	case BORDER_THICK_SELECTED:
		linewidth=4;
		col1 =m_border2;
		col2 =m_border1;
		break;
	}
	int x1 = (m_rect.left() << 3)+spacing , y1 = (m_rect.top() << 3)+spacing;
	int x2 = (m_rect.right() << 3)-spacing, y2 = (m_rect.bottom() << 3)-spacing;

	if ( radius>0)
	{
		m_pGraphics->setColour( col1 );
		m_pGraphics->rectangle( x1,y1, x2,y2, linewidth, radius);
		return;
	}
	m_pGraphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_pGraphics->setColour( col1 );
		m_pGraphics->line( x1+a+radius, y1+a, x2-a-1-radius, y1+a);
		m_pGraphics->line( x2-a-1, y1+a+1+radius, x2-a-1, y2-a-1-radius);
		m_pGraphics->setColour( col2 );
		m_pGraphics->line( x2-a-1-radius, y2-a-1, x1+a+radius, y2-a-1);
		m_pGraphics->line( x1+a, y2-a-1-radius, x1+a, y1+a+1+radius);
	}
	m_pGraphics->unlock();
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void Cbutton::paintText( const std::string &text, int touch)
{
	if ( !*text.c_str())
	{
		// Nothing to do
		return;
	}
	if ( m_textButton)
	{
		m_textButton->setStyle( m_style);
		m_textButton->setColour( m_textColour);
		m_textButton->setRect( m_rect);
		m_textButton->setShadow( m_shadow);
		if ( m_image.isVisible())
		{
			switch (m_imageGravity)
			{
			case GRAVITY_LEFT:
            case GRAVITY_LEFT_TOP:
			case GRAVITY_LEFT_CENTER:
			case GRAVITY_LEFT_BOTTOM:
				m_textButton->move( m_imageSize,0);
				m_textButton->decrease( m_imageSize,0);
				break;
			default:
			case GRAVITY_RIGHT_CENTER:
			case GRAVITY_RIGHT_BOTTOM:
			case GRAVITY_CENTER:
			case GRAVITY_RIGHT:
			case GRAVITY_TOP_CENTER:
			case GRAVITY_RIGHT_TOP:
			case GRAVITY_BOTTOM_CENTER:
			case GRAVITY_RESIZE:
			case GRAVITY_BEHIND_TEXT:
				break;
			}
		}
		m_textButton->setMargin( m_horizontal_margin, m_vertical_margin);
		m_textButton->onPaintText( text, touch);
	}
}

/*============================================================================*/
///
/// @brief Paint label.
///
/*============================================================================*/
void Cbutton::paintLabel()
{
	if ( m_label.bottom() !=m_rect.bottom())
	{
		m_label.calculateSize( m_rect);
	}
	m_label.onPaint(0);
}

/*============================================================================*/
///
/// @brief Paint cross.
///
/*============================================================================*/
void Cbutton::paintCross()
{

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
void Cbutton::onPaint( int touch)
{
	if (m_useText)
	{
		onPaint( m_text, m_greyedOut ? 150:touch);
		return;
	}
	m_text =getTranslation( m_textId, Cgraphics::m_defaults.country);
	onPaint( m_text, m_greyedOut ? 150:touch);
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cbutton::setText( const std::string &text)
{
	m_text =text;
	m_useText =true;
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cbutton::setTextId( textId text)
{
	m_textId =text;
	m_useText =false;
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
std::string Cbutton::getText() const
{
	return m_text;
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
textId Cbutton::getTextId() const
{
    return m_textId;
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cbutton::setImage( const std::string &image, Egravity horizontal, int size)
{
    if (size>0)
    {
        m_imageSize = size;
    }
    else
    {
        switch ( m_rect.height())
        {
        case 3: m_imageSize=2; break;
        case 4: m_imageSize=2; break;
        case 5: m_imageSize=4; break;
        case 6:	m_imageSize=4; break;
        case 7: m_imageSize=5; break;
        case 8: m_imageSize=6; break;
        case 9: m_imageSize=7; break;
        case 10:
        default:m_imageSize =8; break;
        }
    }
    m_imageGravity =horizontal;
	m_image.setImage( image, horizontal, m_imageSize);
}

/*============================================================================*/
///
/// @brief Set new colours.
///
/// @param text [in] Text colour.
/// @param background [in] Background colour.
///
/*============================================================================*/
void Cbutton::setColours( colour text, colour background1, colour background2)
{
	if ( m_textButton)
	{
		m_textButton->setColour( text);
	}
	m_textColour =text;
	if (background1>=0 || background2>=0)
	{
		m_background.setColours( background1, background2);
	}
	m_noBackground =false;
}

/*============================================================================*/
///
/// @brief Set new colours.
///
/// @param text [in] Text colour.
///
/*============================================================================*/
void Cbutton::setTextColour( colour text)
{
	if ( m_textButton)
	{
		m_textButton->setColour( text);
	}
	m_textColour =text;
}

/*============================================================================*/
///
/// @brief Set new colours.
///
/// @param text [in] Text colour.
/// @param b1 [in] Background colour.
/// @param b2 [in] Background colour.
///
/*============================================================================*/
void Cbutton::setBorderColours( colour b1, colour b2)
{
	if (b2==-1)
	{
		b2=b1;
	}
	m_border1 =b1;
	m_border2 =b2;
}

/*============================================================================*/
///
/// @brief Set new colour.
///
/// @param background1 [in] Colour at background.
/// @param background2 [in] Colour at background.
///
/*============================================================================*/
void Cbutton::setBackgroundColour( colour background1, colour background2)
{
	m_background.setColours( background1, background2);
	m_noBackground =false;
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cbutton::setBorder( EborderType border, int radius)
{
	m_border =border;
	if ( radius>=0)
	{
		m_background.setRadius( radius);
	}
}

/// @brief Set style to fill.
void Cbutton::setFillType( EfillType fill)
{
	m_background.setFillStyle( fill);
}

/*----------------------------------------------------------------------------*/
void Cbutton::setFont( const Sfont &font)
{
	if ( m_textButton) m_textButton->setFont( font);
}

/*----------------------------------------------------------------------------*/
void Cbutton::enableBackground()
{
	m_noBackground=false;
}

/*----------------------------------------------------------------------------*/
void Cbutton::disableBackground()
{
	m_noBackground =true;
}

/*----------------------------------------------------------------------------*/
void Cbutton::roundedRectangle( int radius)
{
	m_background.setRadius( radius);
}

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param key  [in] What key.
/// @param id   [in] What text.
///
/*============================================================================*/
CtextButton::CtextButton( Cdialog *parent,
		                  const Crect &rect,
		                  keybutton key,
		                  const std::string &s)
: Cbutton( parent, rect, key, (Sfont)CtextFont("edit_string"), s,
		   BORDER_THIN, GRAVITY_CENTER, 8, FILL_PYRAMID)
{
	setColours( Cgraphics::m_defaults.text_button_text,
			    Cgraphics::m_defaults.text_button_background1,
			    Cgraphics::m_defaults.text_button_background1);
	setBorderColours( Cgraphics::m_defaults.text_button_border);
	setSpacing(1);
}
