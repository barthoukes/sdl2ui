/*
 * sdl_rectangle.h
 *
 *  Created on: 18 dec. 2011
 *      Author: mensfort
 */

#pragma once

#include "sdl_background.h"

/// @brief  Border type for buttons.
typedef enum
{
	BORDER_NONE,
	BORDER_THIN,
	BORDER_THIN_SHADOW,
	BORDER_THIN_SELECTED,
	BORDER_THICK,
	BORDER_THICK_SHADOW,
	BORDER_ITEM_THICK,
	BORDER_THICK_SELECTED,
	BORDER_THIN_BUTTON_SHADOW,
} EborderType;


/// @brief Rectangle to display on screen.
class Crectangle : public CdialogObject
{
public:
	Crectangle( Cdialog *parent,
			    const Crect &rect,
			    keybutton code,
			    EborderType border=BORDER_THIN,
			    colour back=COLOUR_BLACK,
			    int corner=0,
			    EfillType fill=FILL_UNICOLOURED,
			    colour col2=COLOUR_DARKGRAY);
	//Crectangle( const std::string &name, const Crect &rect, keybutton code, EborderType border =BORDER_NONE,
	//		    colour back=COLOUR_BLACK);
	virtual ~Crectangle();

	void    setBackground( EfillType pattern,
			               colour background1=COLOUR_BLACK,
			               colour foreground2=COLOUR_WHITE,
			               colour background2=COLOUR_DARKGRAY);
	virtual void onPaint( int touch);
	void	setCode( keybutton key);
	EborderType getBorder() { return m_border; }
	void 	setBorder( EborderType border) { m_border=border; }
	void    setFillStyle( EfillType pattern);
	void 	paintBorder();

private:
	void paintCross();

private:
	Cbackground	m_square;	///< Background square to display.
	EborderType m_border; 	///< What type of border we paint.
	colour		m_border1;  ///< First border colour.
	colour 		m_border2;  ///< Second border colour.
    EfillType   m_fill;     ///< Fill type.
};
