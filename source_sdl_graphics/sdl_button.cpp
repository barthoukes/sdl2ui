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

#include "sdl_button.h"
#include "sdl_button.h"
#include "graphics_widgets.h"
//#include "global.h"
//#include "zhongcan_icon.h"

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
		m_image =icon;
		m_imageEnable =true;
		m_imageAlign =GRAVITY_LEFT;
	}
 	setColours( Cgraphics::m_defaults.button_text,
 				Cgraphics::m_defaults.button_background1,
 				Cgraphics::m_defaults.button_background2);
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
		m_image =icon;
		m_imageEnable =true;
		setGravity( GRAVITY_LEFT_CENTER);
	}
	setColours( Cgraphics::m_defaults.button_text,
			    Cgraphics::m_defaults.button_background1,
    			Cgraphics::m_defaults.button_background2);
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
		m_image =icon;
		m_imageEnable =true;
		setGravity( GRAVITY_LEFT_CENTER);
	}
	setColours( Cgraphics::m_defaults.button_text,
			    Cgraphics::m_defaults.button_background1,
    			Cgraphics::m_defaults.button_background2);
	roundedRectangle(0);
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
	if ( image.length()>0)
	{
		setImage( image, GRAVITY_LEFT);
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
		col2 =m_graphics->brighter( col, 95);
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
			    Cgraphics::m_defaults.calculator_background1,
			    Cgraphics::m_defaults.calculator_background2);
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
			    Cgraphics::m_defaults.calculator_background1,
			    Cgraphics::m_defaults.calculator_background2);
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
, m_label( parent, rect, code)
, m_icon( icon)
, m_imageAlign( GRAVITY_LEFT)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId(id)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_imageEnable( m_icon.length()>0)
, m_alignBottom(false)
, m_textButton(NULL)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
{
	Sfont font( CtextFont("small_button"));
	m_textButton =new Ctext( m_parent, m_rect, code, font, id,
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
	addDefaultIcon();
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
, m_label( parent, rect, code)
, m_icon( icon)
, m_imageAlign( GRAVITY_LEFT)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId( INVALID_TEXT_ID)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_bright)
, m_border2( Cgraphics::m_defaults.line_dark)
, m_imageEnable( m_icon.length()>0)
, m_textButton(NULL)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
{
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	Sfont font( CtextFont("small_button"));

	m_textButton =new Ctext( m_parent, m_rect, code, font, text,
							 Cgraphics::m_defaults.button_text,
			                 GRAVITY_CENTER);
	m_horizontal_margin =2;
	m_vertical_margin =2;
	addDefaultIcon();
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
, m_label( parent, rect, code)
, m_imageAlign( GRAVITY_LEFT_CENTER)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId(id)
, m_useText(false)
, m_border1(Cgraphics::m_defaults.line_dark)
, m_border2(Cgraphics::m_defaults.line_bright)
, m_imageEnable(false)
, m_textButton(NULL)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
{
	m_textButton =new Ctext( m_parent, m_rect, code, font, id,
							 Cgraphics::m_defaults.button_text,
			                 gravity);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	addDefaultIcon();
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
, m_label( parent, rect, code)
, m_imageAlign( GRAVITY_LEFT_CENTER)
, m_text( text)
, m_textColour( Cgraphics::m_defaults.button_text)
, m_border(border)
, m_cross(false)
, m_textId( INVALID_TEXT_ID)
, m_useText(true)
, m_border1( Cgraphics::m_defaults.line_dark)
, m_border2( Cgraphics::m_defaults.line_bright)
, m_imageEnable(false)
, m_textButton(NULL)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(1)
, m_vertical_margin(1)
, m_spacing(0)
, m_shadow(0)
, m_button_shadow(0)
{
	m_textButton =new Ctext( m_parent, m_rect, code, font, text,
							 Cgraphics::m_defaults.button_text, gravity);
	m_noBackground =false;
	if ( border ==BORDER_NONE)
	{
		m_noBackground =true;
	}
	addDefaultIcon();
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
/// @brief Add the appropriate image to a button.
///
/*============================================================================*/
void Cbutton::addDefaultIcon()
{
	if ( m_image.length()!=0)
	{
		m_imageEnable =true;
		m_imageAlign =GRAVITY_CENTER;
		return;
	}
#if 0
	if ( m_border==BORDER_NONE && m_textId==_NO_TEXT)
	{
		m_imageAlign =GRAVITY_CENTER;
		m_imageEnable =true;
		switch ( m_code)
		{
		case KEY_DELETE:
			m_image =ICON_ERASE48;
			break;
		case KEY_CANCEL:
			m_image =ICON_CANCEL48;
			break;
		case KEY_BACK:
			m_image =ICON_BACK48;
			break;
		case KEY_NEXT:
			m_image =ICON_NEXT48;
			break;
		default:
			m_imageEnable =false;
			break;
		}
	}
	if ( m_imageEnable ==true)
	{
		m_noBackground =true;
		return;
	}
	m_imageEnable =true;
	switch (m_textId)
	{
	case _CALCULATOR:
		m_image =ICON_CALC32;
		break;
	case _CHANGELANG:
		m_image =ICON_LANGUAGES32;
		break;
	case _CANCEL:
		m_image =ICON_CANCEL32;
		break;
	case _OK:
		m_image =ICON_ENTER32;
		break;
	case _SHUTDOWN:
		m_image =ICON_SHUTDOWN32;
		break;
	case _LOCK:
		m_image =ICON_SCREENSAVER32;
		break;
	case _EXIT:
		m_image =ICON_LINUX32;
		break;
	case _BACK:
		m_image =ICON_BACK32;
		break;
	case _NEXT:
		m_image =ICON_NEXT32;
		break;
	case _PRIJS_PP:
		m_image =ICON_EURO32;
		break;
	case _PAGE_ORDER:
		m_image =ICON_SITIN32;
		break;
	//case _TABLE_MAP_ORDER:
	//	m_image =ICON_FLOORPLAN;
	//	break;
	case _PRICE_CHANGE:
	case _BILL_OPTION:
		m_image =ICON_EURO32;
		break;
	default:
		//m_imageEnable =false;
		break;
	}
	if ( m_imageEnable ==true)
	{
		return;
	}
#endif
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Cbutton::~Cbutton()
{
	if ( m_textButton)
	{
		delete m_textButton;
		m_textButton =NULL;
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
	paintImage( touch);
	// Draw button text.
	paintText(text, touch);
	// Draw label.
	paintLabel();
	// Set mouse touch.
	m_graphics->setCode( m_rect, m_code);
}

/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void Cbutton::paintBackground( int touch)
{
	m_background.m_rect =m_rect;
	switch ( m_border)
	{
	case BORDER_THIN_BUTTON_SHADOW:
		{
			Cbackground( m_parent,
					     m_background.m_rect+Csize(0,1),
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
		m_graphics->setColour( col1 );
		m_graphics->rectangle( x1,y1, x2,y2, linewidth, radius);
		return;
	}
	m_graphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_graphics->setColour( col1 );
		m_graphics->line( x1+a+radius, y1+a, x2-a-1-radius, y1+a);
		m_graphics->line( x2-a-1, y1+a+1+radius, x2-a-1, y2-a-1-radius);
		m_graphics->setColour( col2 );
		m_graphics->line( x2-a-1-radius, y2-a-1, x1+a+radius, y2-a-1);
		m_graphics->line( x1+a, y2-a-1-radius, x1+a, y1+a+1+radius);
	}
	m_graphics->unlock();
}

/*============================================================================*/
///
/// @brief Paint image next to text.
///
/*============================================================================*/
void Cbutton::paintImage( int touch)
{
	if ( m_imageEnable==false || m_image.empty()==true)
	{
		return;
	}
	int x1 = (m_rect.left() << 3)+8 , y1 =(m_rect.top() << 3);
	int x2 = (m_rect.right() << 3)-8, y2 =(m_rect.bottom() << 3);
    y1+=8;
    y2-=8;
    SDL_Surface *s=m_graphics->findImage( m_image);
	int sizex=32, sizey=32;
	if (m_border ==BORDER_NONE)
	{
		sizex=48;
		sizey=48;
	}
	if (s)
	{
		sizex =gMin( s->w, 8*m_rect.width());
		sizey =gMin( s->h, 8*m_rect.height());
	}
	switch( m_imageAlign)
	{
	case GRAVITY_LEFT:
		y1=(y1+y2)/2-(sizey/2);
		break;

	case GRAVITY_CENTER:
		x1=(x1+x2)/2-(sizex/2);
		y1=(y1+y2)/2-(sizey/2);
		break;

	case GRAVITY_RIGHT:
		x1=x2-sizex;
		y1=(y1+y2)/2-(sizey/2);
		break;

	case GRAVITY_LEFT_CENTER:
		y1=(y1+y2)/2-(sizey/2);
		break;

	default:
		return;
	}
	m_graphics->image( m_image, x1, y1, x1+sizex, y1+sizex);
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
	if ( m_textButton)
	{
		m_textButton->setStyle( m_style);
		m_textButton->setColour( m_textColour);
		m_textButton->setRect( m_rect);
		m_textButton->setShadow( m_shadow);
		if ( m_image.length()>0)
		{
			m_textButton->m_rect.move(6,0);
			m_textButton->m_rect.decrease(6,0);
		}
		m_textButton->setMargin( m_horizontal_margin, m_vertical_margin);
		m_textButton->onPaint( text, touch);
		return;
	}
	// OLD METHOD.
	int x=m_rect.left(), y=m_rect.top();
	int w=m_rect.width(), h=m_rect.height();
	if ( m_imageEnable && m_imageAlign==GRAVITY_LEFT)
	{
		x+=4;
		w-=4;
	}
	Ctext( m_parent, Crect( x+m_horizontal_margin,y+m_vertical_margin,
			 w-2*m_horizontal_margin,h-2*m_vertical_margin),
			 m_code, (Sfont)CtextFont("amounts"), text, m_textColour).onPaint(0);
	m_label.onPaint(0);
}

/*============================================================================*/
///
/// @brief Paint label.
///
/*============================================================================*/
void Cbutton::paintLabel()
{
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
		onPaint( m_text, touch);
		return;
	}
	m_text =Cgraphics::m_defaults.get_translation( m_textId, Cgraphics::m_defaults.country);
	onPaint( m_text, touch);
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
void Cbutton::setText( textId text)
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
std::string Cbutton::getText()
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
void Cbutton::setImage( const std::string &image, Egravity horizontal)
{
	m_image =image;
	if (horizontal!=GRAVITY_NO_CHANGE)
	{
		m_imageAlign =horizontal;
	}
	m_imageEnable =(m_image.size() >0);
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
	m_background.setColours( background1, background2);
	m_noBackground =false;
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
	m_background.setRadius( radius);
}

/// @brief Set style to fill.
void Cbutton::setFillType( EfillType fill)
{
	m_background.setFillStyle( fill);
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
	//roundedRectangle(8);
	setSpacing(1);
	//setLabel("");
	//setShadow(0xff000000);
}
