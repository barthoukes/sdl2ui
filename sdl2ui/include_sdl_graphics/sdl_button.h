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
#include <memory>
#include <string>
#include "lingual.h"
#include "sdl_text.h"
#include "sdl_label.h"
#include "sdl_image.h"
#include "sdl_graphics.h"
#include "sdl_keybutton.h"
#include "sdl_rectangle.h"
#include "sdl_dialog_object.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class Cbutton : public CdialogObject
{
public:
	Cbutton( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 textId id=INVALID_TEXT_ID,
			 EborderType border=BORDER_THIN_SHADOW,
			 EfillType fill=FILL_UNICOLOURED,
			 const std::string &icon="");
	Cbutton( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const std::string &text,
			 EborderType border=BORDER_THIN_SHADOW,
			 EfillType fill=FILL_UNICOLOURED,
			 const std::string &icon="");
	Cbutton( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const Sfont &font,
			 textId id=INVALID_TEXT_ID,
			 EborderType border=BORDER_THIN_SHADOW,
			 Egravity gravity=GRAVITY_CENTER,
			 int radius=6,
	         EfillType fill=FILL_UNICOLOURED);
	Cbutton( Cdialog *parent,
			 const Crect &rect,
			 keybutton code,
			 const Sfont &font,
			 const std::string &text="",
			 EborderType border=BORDER_THIN_SHADOW,
			 Egravity gravity=GRAVITY_CENTER,
			 int radius=6,
			 EfillType fill=FILL_UNICOLOURED);
	virtual ~Cbutton();

public:
	virtual void onPaint( int touch);
	virtual void onPaint( const std::string &text, int touch);
	void	setText( const std::string &text);
	void 	setTextId( textId id);
	std::string getText() const;
	textId  getTextId() const;
	void 	setInfoText( const std::string &text);
	void 	setImage( const std::string &image, Egravity horizontal=GRAVITY_NO_CHANGE, int size=-1);
	void    noImage() { m_image.clear(); }
	void    setTop( int y) { m_rect.setTop(y); }
	void    setTextColour( colour text);
	void	setColours( colour text, colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void 	setBorderColours( colour b1, colour b2=-1);
	void	setBackgroundColour( colour background, colour background2=COLOUR_DARKGRAY);
	void	setBorder( EborderType border, int radius =0);
	void    setSpacing( int spacing);
	void    setImageGravity( Egravity gravity);
	void    setTextGravity( Egravity text);
	void	setFillType( EfillType fill =FILL_UNICOLOURED);
    void    setFont( const Sfont &font);
	void    enableBackground();
	void    disableBackground();
	void 	roundedRectangle( int radius);
	void    decrease(int width, int height);
	void    setGreyedOut( bool grey);
	void	noLabel() { m_label.setText(""); }
	void    setMargin( int hor, int ver) { m_horizontal_margin =hor; m_vertical_margin=ver; }
	void    setLabel( const std::string &label)
	{
		m_label.setText( label);
	}
	virtual void onPaint( const Cpoint &p, int touch);
	void    alignBottom( bool bottom)
	{
		m_alignBottom =bottom;
	}
	void  setGravity( Egravity gravity)
	{
		if ( m_textButton)
		{
			m_textButton->setGravity( gravity);
		}
	}
	void setShadow( colour col)
	{
		m_shadow =col;
	}
	void setStyle( int style) { m_style =style; }
	void bold() { m_style =TTF_STYLE_BOLD; }
	void setRect( const Crect &rect)
	{
		m_label.setRect(rect); m_background.setRect(rect);
		if ( m_textButton) m_textButton->setRect(rect);
		m_image.setRect(rect);
		CdialogObject::setRect( rect);
	}
    void addLeft( int left);
	void setVisible( bool visible)
	{
		m_label.setVisible(visible); m_background.setVisible(visible);
		if ( m_textButton) m_textButton->setVisible(visible);
		CdialogObject::setVisible(visible);
	}
	void setCursor( int cursor)
	{
		if ( m_textButton) { m_textButton->setCursor( cursor); }
	}
	void setStyle( EimageStyle style, colour col) { m_image.setStyle(style,col); }
	void setCorner( int corner )
	{ m_background.setCorner( corner ); m_background.setFillStyle( FILL_PIE ); }

private:
	void 	paintBackground( int touch);
	void	paintBorder( EborderType border, int spacing, int radius, int touch);
	void 	paintText( const std::string &text, int touch);
	void	paintLabel();
	void	paintCross();
	void    paintImage();
	Sfont   calculateFont( const std::string &text);

public:
	bool				m_noBackground; ///< No background.
	Cbackground			m_background;	///< Square inside.

protected:
	Cimage				m_image;		///< Image.
	Clabel		 		m_label;	///< Label for the button.
	std::string			m_text;		///< Text to show.
	colour		 		m_textColour; ///< Colour text. Almost obsolete.
	EborderType			m_border;	///< Border.
	bool				m_cross;	///< Cross.
	textId				m_textId;	///< Text by id.
	bool				m_useText;	///< Use text or id.
	colour			 	m_border1;	///< Border colour 1.
	colour			 	m_border2;	///< Border colour 2.
	bool				m_alignBottom;	///< Bottom alignment.
	CtextPtr            m_textButton; ///< Text button to use.
	int					m_style;		///< Style for the button.
	int					m_horizontal_margin; ///< Margin.
	int					m_vertical_margin; ///< Margin.
	int                 m_spacing; ///< space between buttons.
	colour				m_shadow; ///< text shadow colour.
	colour				m_button_shadow; ///< shadow button.
	int					m_imageSize; ///< Size of image.
	Egravity			m_imageGravity; ///< Gravity image.
	bool				m_greyedOut; ///< Are we invalid?
};


/// @brief  Button seperate on screen.
class CmenuButton : public Cbutton
{
public:
	CmenuButton( const Crect &rect,
					 keybutton key,
				     textId id,
				     const std::string &icon="");
	CmenuButton( const Crect &rect,
					 keybutton key,
				     const std::string &text,
				     const std::string &icon="");

public:
	~CmenuButton() {}
};

/// @brief Button at bottom of screen.
class CbottomButton : public Cbutton
{
public:
	CbottomButton( Cdialog *parent,
			       const Crect &rect,
				   keybutton key,
				   textId id,
				   const std::string &icon="");
	CbottomButton( Cdialog *parent,
			       const Crect &rect,
				   keybutton key,
				   const std::string &text,
				   const std::string &icon="");
public:
	virtual ~CbottomButton() {}
};

/// @brief Button at bottom of screen.
class CkeyboardButton : public Cbutton
{
public:
	CkeyboardButton( const Crect &rect,
				   keybutton key,
				   textId id,
				   const std::string &icon="");
public:
	virtual ~CkeyboardButton() {}
};

/// Image information.
typedef struct
{
	int x; ///< X-position.
	int y; ///< Y-position.
	int w; ///< Width.
	int h; ///< Height
	keybutton k; ///< Key to press.
	std::string name; ///< Which file.
} Simage;

/// @brief Button at bottom of screen.
class CimageButton : public Cbutton
{
public:
	CimageButton( const Crect &rect,
				   keybutton key,
				   textId id,
				   const std::string &background ="", //ICON_VLOER14,
				   const std::string &image ="");
public:
	virtual ~CimageButton() {}
};

/// @brief Button at bottom of screen.
class CheaderButton : public Cbutton
{
public:
	CheaderButton( const Crect &rect,
			       textId id,
	   	   	       keybutton key =KEY_NONE);
	CheaderButton( const Crect &rect,
			       const std::string &s,
	   	   	       keybutton key =KEY_NONE);
public:
	virtual ~CheaderButton() {}
	virtual void onPaint(int touch) { Cbutton::onPaint(touch); }
};

/// @brief Button for matrix.
class CmatrixButton : public Cbutton
{
public:
	CmatrixButton( Cdialog *parent,
			       const Crect &rect,
			       textId id,
	   	   	       colour text,
	   	   	       colour back1,
	   	   	       colour back2);
	CmatrixButton( Cdialog *parent,
			       const Crect &rect,
			       textId id,
			       keybutton key = KEY_NOCHANGE);
	CmatrixButton( Cdialog *parent,
			       const Crect &rect,
			       const std::string &s,
			       keybutton key);
	CmatrixButton( Cdialog *parent,
			       const Crect &rect,
			       const std::string &s,
			       colour text,
			       colour back1,
			       colour back2);
public:
	virtual ~CmatrixButton() {}
};

/// @brief Button at bottom of screen.
class CcalculatorButton : public Cbutton
{
public:
	CcalculatorButton( const Crect &rect,
			       const std::string &s,
	   	   	       keybutton key);
	CcalculatorButton( const Crect &rect,
			       EtextId id,
	   	   	       keybutton key);
	CcalculatorButton( const Crect &rect,
			       const std::string &s,
	   	   	       keybutton key,
				   const std::string &img);
	CcalculatorButton( Simage *image, int x, int y);
public:
	virtual ~CcalculatorButton() {}
};

/// @brief Text button on screen.
class CtextButton : public Cbutton
{
public:
	CtextButton( Cdialog *parent,
			     const Crect &rect,
			     keybutton key,
			     const std::string &s);
public:
	virtual ~CtextButton() {}
};

/// @brief Text button on screen.
class CfunctionButton : public Cbutton
{
public:
	CfunctionButton( Cdialog *parent,
			     const Crect &rect,
			     keybutton key,
			     textId id,
				 const std::string &image,
			     colour col=-1);
public:
	virtual ~CfunctionButton() {}
};

/// @brief Graphics button.
class CgraphButton : public Cbutton
{
public:
	CgraphButton( Cdialog *parent,
			      const Crect &rect,
			      const std::string &str,
			      bool	rotate);
public:
	virtual ~CgraphButton() {}

};

/// @brief Button at bottom of screen.
class CiconButton: public Cbutton
{
public:
	CiconButton(Cdialog *parent, const Crect &rect, keybutton key,
			const std::string &icon, colour background);
public:
	virtual ~CiconButton() {}
};

typedef std::shared_ptr<Cbutton> CbuttonPtr;
typedef std::shared_ptr<CmenuButton> CmenuButtonPtr;
typedef std::shared_ptr<CtextButton> CtextButtonPtr;
typedef std::shared_ptr<CimageButton> CimageButtonPtr;
typedef std::shared_ptr<CbottomButton> CbottomButtonPtr;
typedef std::shared_ptr<CheaderButton> CheaderButtonPtr;
typedef std::shared_ptr<CmatrixButton> CmatrixButtonPtr;
typedef std::shared_ptr<CfunctionButton> CfunctionButtonPtr;
typedef std::shared_ptr<CkeyboardButton> CkeyboardButtonPtr;
typedef std::shared_ptr<CcalculatorButton> CcalculatorButtonPtr;
