/*
 * sdl_label.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */

#include "sdl_label.h"
#include "sdl_text.h"
#include "sdl_background.h"

Clabel::Clabel( Cdialog *parent, const Crect &rect, keybutton key)
: CdialogObject( parent, rect, key)
, m_label( key==KEY_NOCHANGE ? "":Ckeybutton(key).label())
, m_font( CtextFont("alias"))
{
	int len =m_label.length();
	m_enable =!( len==0);
	int w,h;
	//if ( key ==KEY_NOCHANGE)
	//{
	//	m_visible =false;
	//}
	if ( !m_font.local.font)
	{
		return;
	}
	std::string txt =Ckeybutton(key);
	TTF_SizeUTF8( m_font.local.font, txt.c_str(), &w, &h);
	w=(w+7)/8;
	h=(h+4)/8;

	m_rect.setTop( m_rect.bottom()-h);
	m_rect.setHeight(h);
	m_rect.setLeft( m_rect.right()-w);
	m_rect.setWidth( w);
}

void Clabel::onPaint( int touch)
{
	(void)touch;
	if ( Cgraphics::m_defaults.show_labels || m_enable==false || m_visible==false)
	{
		return;
	}
	Cbackground( m_parent, m_rect, m_code, Cgraphics::m_defaults.label_background,
			     4, FILL_GRADIENT, Cgraphics::m_defaults.label_background+0x202020).onPaint(0);
	Ctext txt( m_parent, m_rect, m_code, m_font,
			   m_label, Cgraphics::m_defaults.label_text, GRAVITY_CENTER);
	txt.onPaint( 0);
}

Clabel::~Clabel()
{
}

int Clabel::length()
{
	return m_label.length();
}

void Clabel::setText( const std::string &txt)
{
	m_label=txt;
	m_enable =(m_label.length()) ? true:false;
	int w,h;
	if ( !m_font.local.font)
	{
		return;
	}
	TTF_SizeUTF8( m_font.local.font, txt.c_str(), &w, &h);
	w=(w+7)/8;
	h=(h+4)/8;
	m_rect.setTop( m_rect.bottom()-h);
	m_rect.setHeight(h);
	m_rect.setLeft( m_rect.right()-w);
	m_rect.setWidth( w);
}
