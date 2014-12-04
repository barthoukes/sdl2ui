/*============================================================================*/
/**  @file       sdl_amount.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Create a default amount box.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Camount
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 28 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#include "sdl_amount.h"
#include "sdl_rectangle.h"

/*============================================================================*/
///
/// @brief Constructor complex version.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Camount::Camount( Cdialog *parent, const Crect &rect, keybutton code, const Sfont &font,
		   const Cmoney &amount, colour text)
: Cbutton( parent, rect, code, (Sfont)CtextFont("amounts"), (std::string)amount,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER)
{
	(void)font;
	roundedRectangle(0);
	m_textColour =text;
}

/*============================================================================*/
///
/// @brief Constructor simple version.
//
/// @param rect [in] Position.
/// @param value [in] Amount to display.
///
/*============================================================================*/
Camount::Camount( Cdialog *parent, const Crect &rect, const Cmoney &amount)
: Cbutton( parent, rect, KEY_NONE, (Sfont)CtextFont("amounts"), (std::string)amount,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER)
{
	roundedRectangle(0);
}

Camount::~Camount() {}

void Camount::setAmount( const Cmoney &amount)
{
	setText( (std::string)amount);
}
