/*
 * sdl_json_object.cpp
 *
 *  Created on: 18 feb. 2014
 *      Author: mensfort
 */

#include <ctype.h>
#include "sdl_json_object.h"

/** @brief constructor. CdialogObject just for the paint function.
 *  @param parent [in] Dialog to use this object
 *  @param description [in] What description
 *  Following objects are possible:<br>
 *  <li>image 'rect'=[left,top,width,height], 'code'='<key>', 'icon'='picture.png', 'border'='<border>', 'margin'=N
 *  <li>button 'rect'=[left,top,width,height], 'code'='<key>', 'font'='<font>', 'gravity'='<gravity>', 'id'='<text_id>', 'radius'=N, 'fill'='<fill>', 'label'='<label>'
 *  <li>buttombutton 'rect'=[left,top,width,height], 'code'='<key>', 'text'='<text>'
 *  <li>calculatorbutton 'rect'=[left,top,width,height], 'code'='<key>', 'text'='<text>'
 *  <li>background 'rect'=[left,top,width,height], 'radius'=N, 'fill'='<fill>, 'colour1'='<colour>', 'colour2'='<colour', 'icon'='<background_image>', 'border'=P
 *  <li>text 'rect'=[left,top,width,height],
 *
 */
CjsonObject::CjsonObject( Cdialog *parent, const std::string &description)
: CdialogObject( parent, Crect(0,0,0,0), KEY_NONE)
, m_reader( m_features)
, m_index(0)
, m_lastName("search48.png")
, m_lastCode(KEY_NONE)
, m_lastRect(0,0,6,6)
, m_buttonBorder( BORDER_THIN)
, m_lastBorder(BORDER_NONE)
, m_lastGravity(GRAVITY_CENTER)
, m_buttonGravity(GRAVITY_CENTER)
, m_imageGravity(GRAVITY_CENTER)
, m_lastMargin(0)
, m_lastLabel("")
, m_lastType("image")
, m_lastColour1(0)
, m_lastColour2(0)
, m_lastRadius(0)
, m_lastFill( FILL_GRADIENT)
, m_lastButtonFill( FILL_GRADIENT)
, m_lastBorderWidth( 0)
, m_lastBackgroundRadius( 0)
, m_lastBackgroundBorderWidth( 2)
, m_lastButtonRadius( 4)
{
	m_lastFont =CtextFont("calculator");
	add( description);
}

/** @brief Add object ot the JSON dialog object list
 *
 *  @param description [in] Json string containing button information
 */
void CjsonObject::add( const std::string &description)
{
	bool parsingSuccessful = m_reader.parse( description, m_root);
	if ( !parsingSuccessful )
	{
		// "Failed to parse %s", m_reader.getFormatedErrorMessages().c_str() );
		return;
    }
	if ( m_root.isArray())
	{
		for ( int n=0; n<(int)m_root.size(); n++)
		{
			decode( m_parent, m_root[n]);
		}
	}
	else
	{
		decode( m_parent, m_root);
	}
}

/** @brief Decode the object
 *  @param parent [in] Dialog to use it
 *  @param value [out] Initial value
 */
void CjsonObject::decode( Cdialog *parent, const Json::Value &value)
{
	Json::Value v=value["type"];
	if ( v.isString())
	{
		m_lastType =v.asString();
	}
	if ( m_lastType.compare("image")==0)
	{
		decodeImage( parent, value);
	}
	else if ( m_lastType.compare("button")==0)
	{
		decodeButton( parent, value);
	}
	else if ( m_lastType.compare("bottombutton")==0)
	{
		decodeBottomButton( parent, value);
	}
	else if ( m_lastType.compare("calculatorbutton")==0)
	{
		decodeCalculatorButton( value);
	}
	else if ( m_lastType.compare("header")==0)
	{
		decodeHeaderButton( value);
	}
	else if ( m_lastType.compare("headerbutton")==0)
	{
		decodeHeaderButton( value);
	}
	else if ( m_lastType.compare("background")==0)
	{
		decodeBackground( parent, value);
	}
	else if ( m_lastType.compare("text")==0)
	{
		decodeText( parent, value);
	}
	//else "CjsonObject::CjsonObject Wrong type!!");
}

void CjsonObject::decodeImage( Cdialog *parent, const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);
	std::string icon =decodeIcon( root);
	EborderType border =decodeBorder( root, m_lastBorder);
	int margin =decodeMargin( root);
	m_lastLabel ="";
	std::string label =decodeLabel( root);
	Cimage *img =new Cimage( parent, rect, code, icon, border, margin, label);
	Json::Value v=root["gravity"];
	if ( v.isString())
	{
		m_imageGravity =decodeGravity( root, m_imageGravity);
	}
	img->setGravity( m_imageGravity);
	m_object.push_back( img);
}

int CjsonObject::decodeBorderWidth( const Json::Value &root, int &borderwidth)
{
	Json::Value v=root["border"];
	if ( v.isInt())
	{
		borderwidth =v.asInt();
	}
	return borderwidth;
}

std::string CjsonObject::decodeIcon( const Json::Value &root)
{
	std::string icon ="";
	Json::Value v=root["icon"];
	if ( v.isString())
	{
		icon =v.asString();
	}
	else
	{
		v=root["image"];
		if ( v.isString())
		{
			icon =v.asString();
		}
	}
	return icon;
}

int CjsonObject::decodeMargin( const Json::Value &root)
{
	int margin =m_lastMargin;
	Json::Value v=root["margin"];
	if ( v.isIntegral())
	{
		margin =v.asInt();
		m_lastMargin =margin;
	}
	return m_lastMargin;
}

std::string CjsonObject::decodeLabel( const Json::Value &root)
{
	std::string label =m_lastLabel;
	Json::Value v=root["label"];
	if ( v.isString())
	{
		label =v.asString();
		m_lastLabel =label;
	}
	return m_lastLabel;
}

/** @brief Decode the button for the object
 *  @param root [in] What root
 */
keybutton CjsonObject::decodeKeybutton( const Json::Value &root)
{
	Json::Value v=root["code"];
	if ( v.isString())
	{
		m_lastCode =Ckeybutton(v.asString());
	}
	else
	{
		if ( m_lastCode !=KEY_NONE)
		{
			m_lastCode++;
		}
	}
	return (keybutton)m_lastCode;
}

Crect CjsonObject::decodeRect( const Json::Value &root)
{
	Json::Value v=root["rect"];
	if ( v.isArray() && v.size()==4)
	{
		int left =v[(unsigned int)0].asInt();
		int top =v[1].asInt();
		int width =v[2].asInt();
		int height =v[3].asInt();
		m_lastRect =Crect( left, top, width, height);
	}
	else
	{
		m_lastRect.setLeft( m_lastRect.right());
	}
	return m_lastRect;
}

colour CjsonObject::decodeColour1( const Json::Value &root)
{
	Json::Value v=root["colour"];
	if ( !v.isString())
	{
		v =root["colour1"];
	}
	if ( v.isString())
	{
		std::string s =v.asString();
		if (s.size()>1 && toupper(s[1]) =='X')
		{
			m_lastColour1 =strtol(s.c_str()+2, NULL, 16);
		}
		else
		{
			m_lastColour1 =atoi( s.c_str());
		}
	}
	return m_lastColour1;
}

colour CjsonObject::decodeColour2( const Json::Value &root)
{
	Json::Value v=root["colour2"];
	if ( v.isString())
	{
		std::string s =v.asString();
		if (s.size()>1 && toupper(s[1]) =='X')
		{
			m_lastColour2 =strtol(s.c_str()+2, NULL, 16);
		}
		else
		{
			m_lastColour2 =atoi( s.c_str());
		}
	}

	return m_lastColour2;
}

int CjsonObject::decodeRadius( const Json::Value &root, int &lastValue)
{
	Json::Value v=root["radius"];
	if ( v.isInt())
	{
		lastValue =v.asInt();
	}
	return lastValue;
}

EfillType CjsonObject::decodeFillType( const Json::Value &root, EfillType &fill)
{
	Json::Value v=root["fill"];
	if ( v.isString())
	{
		std::string s=v.asString();
		if ( s.compare("none")==0)
		{
			fill =FILL_NONE;
		}
		else if ( s.compare("solid")==0)
		{
			fill =FILL_UNICOLOURED;
		}
		else if ( s.compare("2colours")==0)
		{
			fill =FILL_2COLOURS;
		}
		else if ( s.compare("gradient")==0)
		{
			fill =FILL_GRADIENT;
		}
		else if ( s.compare("pyramid")==0)
		{
			fill =FILL_PYRAMID;
		}
		else if ( s.compare("image")==0)
		{
			fill =FILL_IMAGE;
		}
	}
	return fill;
}

EborderType CjsonObject::decodeBorder( const Json::Value &root, EborderType &border)
{
	Json::Value v=root["border"];
	if ( v.isString())
	{
		std::string  border =v.asString();
		if ( v.compare( "none")==0)
		{
			border =BORDER_NONE;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
		else if ( v.compare( "thin_shadow"))
		{
			border =BORDER_THIN_SHADOW;
		}
		else if ( v.compare( "thin_selected"))
		{
			border =BORDER_THIN_SELECTED;
		}
		else if ( v.compare( "thin_button_shadow"))
		{
			border =BORDER_THIN_BUTTON_SHADOW;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
		else if ( v.compare( "thin"))
		{
			border =BORDER_THIN;
		}
	}
	return border;
}

void CjsonObject::decodeText( Cdialog *parent, const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);
	Json::Value v;
	Sfont font =decodeFont( root, "button");

	std::string fontName ="button";
	v =root["font"];
	if ( v.isString())
	{
		fontName =v.asString();
	}

	std::string txt;
	v =root["text"];
	if ( v.isString())
	{
		txt =v.asString();
	}

	colour colour;
	v =root["text_colour"];
	if ( v.isString())
	{
		std::string sc =v.asString();
		colour =Cgraphics::m_defaults.find_colour(sc);
	}
	else
	{
		 colour =Cgraphics::m_defaults.item_text;
	}
	Egravity gravity =decodeGravity( root, m_buttonGravity);
	int cursor =-1;
	int cursorColour =0;
	Ctext *txtObj =new Ctext( parent, rect, code, (Sfont)CtextFont(fontName.c_str()),
			                  txt, colour, gravity, cursor, cursorColour);
	m_object.push_back( txtObj);
}

/** @brief Find font in the string
 *  @param root [in] Where to look
 *  @param name [in] Default value
 *  @return Font definition
 */
Sfont CjsonObject::decodeFont( const Json::Value &root, const std::string &name)
{
	std::string fontName =name;
	Json::Value v =root["font"];
	if ( v.isString())
	{
		fontName =v.asString();
	}
	return (Sfont)CtextFont( fontName.c_str());
}

/** @brief Find gravity in the string
 *  @param value [in] Where to look
 *  @param gravity [in] Default value
 *  @return Gravity found or default value
 */
Egravity CjsonObject::decodeGravity( const Json::Value &value, Egravity &gravity)
{
	Json::Value v=value["gravity"];
	if ( v.isString())
	{
		std::string s=v.asString();
		if ( s.compare("left")==0)
		{
			gravity =GRAVITY_LEFT_CENTER;
		}
		else if ( s.compare("left_top")==0)
		{
			gravity =GRAVITY_LEFT;
		}
		else if ( s.compare("top_center")==0)
		{
			gravity =GRAVITY_TOP_CENTER;
		}
		else if ( s.compare("right_center")==0)
		{
			gravity =GRAVITY_RIGHT_CENTER;
		}
		else if ( s.compare("right_bottom")==0)
		{
			gravity =GRAVITY_RIGHT_BOTTOM;
		}
		else if ( s.compare("right_top")==0)
		{
			gravity =GRAVITY_RIGHT;
		}
		else if ( s.compare("center")==0)
		{
			gravity =GRAVITY_CENTER;
		}
		else if ( s.compare("resize"))
		{
			gravity =GRAVITY_RESIZE;
		}
		// "CjsonObject::decodeGravity wrong %s!!", s.c_str());
	}
	return gravity;
}

void CjsonObject::decodeButton( Cdialog *parent, const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);

	EborderType border =decodeBorder( root, m_buttonBorder); // default =BORDER_THIN
 	Sfont font =decodeFont( root, "button");

	Egravity gravity=decodeGravity( root, m_buttonGravity);
	textId id=decodeTextId( root);

	int radius =decodeRadius( root, m_lastButtonRadius); // default =6

	EfillType fill =decodeFillType( root, m_lastButtonFill); // default =FILL_UNICOLOURED

	Cbutton *btn;
	if ( id !=INVALID_TEXT_ID && id>=0)
	{
		btn =new Cbutton( parent, rect, code, font, id, border, gravity, radius, fill);
	}
	else
	{
		std::string txt;
		Json::Value v =root["text"];
		if ( v.isString())
		{
			txt =v.asString();
		}
		btn =new Cbutton( parent, rect, code, font, txt, border, gravity, radius, fill);
	}
	Json::Value v =root["label"];
	if ( v.isString())
	{
		std::string sc =v.asString();
		btn->setLabel( sc);
	}

	m_object.push_back( btn);
}

/**
 *  @brief Decode a header button on top of the screen
 *  @param root [in] Text input
 */
void CjsonObject::decodeHeaderButton( const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);

	Egravity gravity=decodeGravity( root, m_buttonGravity);
	textId id=decodeTextId( root);

	CheaderButton *btn;
	if ( id !=INVALID_TEXT_ID && id>=0)
	{
		btn =new CheaderButton( rect, id, code);
	}
	else
	{
		std::string txt;
		Json::Value v =root["text"];
		if ( v.isString())
		{
			txt =v.asString();
		}
		btn =new CheaderButton( rect, txt, code);
	}
	m_object.push_back( btn);
}

void CjsonObject::decodeBottomButton( Cdialog *parent, const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);

	CbottomButton *btn;
	textId id=decodeTextId( root);
	std::string icon =decodeIcon( root);

	if ( id !=INVALID_TEXT_ID && id>=0)
	{
		btn =new CbottomButton( parent, rect, code, id, icon);
	}
	else
	{
		std::string txt;
		Json::Value v =root["text"];
		if ( v.isString())
		{
			txt =v.asString();
		}
		btn =new CbottomButton( parent, rect, code, txt, icon);
	}
	if ( btn !=NULL)
	{
		m_object.push_back( btn);
	}
}

/** @brief Decode the background object
 *
 * @param  value
 * @return
 */
textId CjsonObject::decodeTextId( const Json::Value &value)
{
	textId retVal =INVALID_TEXT_ID;
	Json::Value v=value["id"];
	if ( v.isString())
	{
		std::string s=v.asString();
		retVal =Cgraphics::m_defaults.find_text_id(s); // findTextId( s);
	}
	return retVal;
}

/** @brief Decode the background object
 *
 *  @param root [in] Json object to decode
 */
void CjsonObject::decodeCalculatorButton( const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);

	CcalculatorButton *btn;
	std::string txt;
	Json::Value v =root["text"];
	if ( v.isString())
	{
		txt =v.asString();
	}

	btn =new CcalculatorButton( rect, txt, code);
	if ( btn !=NULL)
	{
		m_object.push_back( btn);
	}
}

/** @brief Decode the background object
 *
 *  @param parent [in] What dialog
 *  @param root [in] Json object to decode
 */
void CjsonObject::decodeBackground( Cdialog *parent, const Json::Value &root)
{
	Crect rect =decodeRect( root);
	keybutton code =decodeKeybutton( root);
	colour colour1 =decodeColour1( root);
	colour colour2 =decodeColour2( root);
	int radius =decodeRadius( root, m_lastBackgroundRadius);
	EfillType fill =decodeFillType( root, m_lastBackgroundFill);
	Cbackground *back =new Cbackground( parent, rect, code, colour1, radius, fill, colour2);
	if ( fill ==FILL_IMAGE)
	{
		std::string image =decodeIcon( root);
		back->setBackground( image);
	}
	int border =decodeBorderWidth( root, m_lastBackgroundBorderWidth);
	if ( border!=0)
	{
		back->setBorderWidth( border);
	}
	m_object.push_back( back);
}

CjsonObject::~CjsonObject()
{
	for ( int n=0; n<(int)m_object.size(); n++)
	{
		delete m_object[n];
	}
	m_object.clear();
}

void CjsonObject::onPaint( int touch)
{
	for ( int n=0; n<(int)m_object.size(); n++)
	{
		m_object[n]->onPaint( touch);
	}
}

/** @brief Paint me.
 *  @param p [in] Different location.
 *  @post I'm painted.
 */
void CjsonObject::onPaint( const Cpoint &p, int touch)
{
	CdialogObject *c;
	for ( int n=0; n<(int)m_object.size(); n++)
	{
		c =m_object[n];
		c->onPaint( c->m_rect.origin()+p, touch);
	}
}

CdialogObject *CjsonObject::find( keybutton key)
{
	CdialogObject *c;
	for ( int n=0; n<(int)m_object.size(); n++)
	{
		c =m_object[n];
		if ( c->m_code ==key)
		{
			return c;
		}
	}
	return NULL;
}

CdialogObject *CjsonObject::operator[]( int index)
{
	if ( (int)m_object.size()>index && index>=0)
	{
		return m_object[ index];
	}
	return NULL;
}

CjsonObject::operator Cbutton*() const
{
	return dynamic_cast<Cbutton*>(m_object[m_index]);
}

CjsonObject::operator Cimage*() const
{
	return dynamic_cast<Cimage*>(m_object[m_index]);
}
