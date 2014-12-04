/*============================================================================*/
/**  @file      sdl_label.h
 **  @ingroup   user_interface
 **  @brief		Draw scrollbar.
 **
 **  Create and show scrollbar.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CsdlLabel
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 6 okt 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

#include "sdl_dialog_object.h"
#include "sdl_font.h"

/*------------- Standard includes --------------------------------------------*/
#include <string>

/// @brief  Create and display buttons.
class Clabel : public CdialogObject
{
public:
	Clabel( Cdialog *parent, const Crect &rect, keybutton key);
	virtual ~Clabel();

public:
	void	onPaint( int touch);
	int		length();
	void	setText( const std::string &txt);

private:
	std::string   m_label;
	bool 		  m_enable;
	Sfont		  m_font;
};

