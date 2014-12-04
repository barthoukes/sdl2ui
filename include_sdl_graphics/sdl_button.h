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
 **  Copyright (c) Bart Houkes, 10 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "keybutton.h"
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "sdl_rectangle.h"
#include "sdl_text.h"
#include "sdl_label.h"

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
	void	onPaint( int touch);
	void	onPaint( const std::string &text, int touch);
	void	setText( const std::string &text);
	void 	setText( textId id);
	std::string getText();
	void 	setInfoText( const std::string &text);
	void 	setImage( const std::string &image, Egravity horizontal=GRAVITY_NO_CHANGE);
	void	setColours( colour text, colour background1 =COLOUR_BLACK, colour background2=COLOUR_DARKGRAY);
	void 	setBorderColours( colour b1, colour b2=-1);
	void	setBackgroundColour( colour background, colour background2=COLOUR_DARKGRAY);
	void	setBorder( EborderType border, int radius =0);
	void    setSpacing( int spacing) { m_spacing =spacing; }
	void    setImageGravity( Egravity gravity) { m_imageAlign =gravity; }
	void    setTextGravity( Egravity text) { if (m_textButton) m_textButton->setGravity( text); }
	void	setFillType( EfillType fill =FILL_UNICOLOURED);
    void    setFont( const Sfont &font) { if ( m_textButton) m_textButton->setFont( font); }
	void    enableBackground() { m_noBackground=false; }
	void    disableBackground() { m_noBackground =true; }
	void 	roundedRectangle( int radius)
	{
		m_background.setRadius( radius);
	}
	void	noLabel() { m_label.setText(""); }
	void    setMargin( int hor, int ver) { m_horizontal_margin =hor; m_vertical_margin=ver; }
	void    setLabel( const std::string &label)
	{
		m_label.setText( label);
	}
	virtual void onPaint( const Cpoint &p, int touch);
	void 	addDefaultIcon();
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
		CdialogObject::setRect( rect);
	}
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

private:
	void 	paintBackground( int touch);
	void	paintBorder( EborderType border, int spacing, int radius, int touch);
	void	paintImage( int touch);
	void 	paintText( const std::string &text, int touch);
	void	paintLabel();
	void	paintCross();


protected:
	Cbackground			m_background;	///< Square inside.
	Clabel		 		m_label;	///< Label for the button.
	std::string			m_icon;		///< Image for the button.
	Egravity			m_imageAlign; ///< Align of image.
	std::string			m_image;	///< Image name.
	std::string			m_text;		///< Text to show.
	colour		 		m_textColour; ///< Colour text. Almost obsolete.
	bool				m_noBackground; ///< No background.
	EborderType			m_border;	///< Border.
	bool				m_cross;	///< Cross.
	textId				m_textId;	///< Text by id.
	bool				m_useText;	///< Use text or id.
	colour			 	m_border1;	///< Border colour 1.
	colour			 	m_border2;	///< Border colour 2.
	bool				m_imageEnable; ///< Image enabled.
	bool				m_alignBottom;	///< Bottom alignment.
	Ctext				*m_textButton; ///< Text button to use.
	int					m_style;		///< Style for the button.
	int					m_horizontal_margin; ///< Margin.
	int					m_vertical_margin; ///< Margin.
	int                 m_spacing; ///< space between buttons.
	colour				m_shadow; ///< text shadow colour.
	colour				m_button_shadow; ///< shadow button.
};


/// @brief  Button seperate on screen.
class CmenuButton : public Cbutton
{
public:
	CmenuButton( const Crect &rect,
					 keybutton key,
				     textId id,
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
			       keybutton key);
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
