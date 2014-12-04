/*
 * sdl_amount.h
 *
 *  Created on: 28 juy 2012
 *      Author: mensfort
 */

#pragma once

#include <string>
#include "sdl_dialog_object.h"
#include "sdl_graphics.h"
#include "sdl_button.h"
#include "money.h"

/// @brief Button to display.
class Camount : public Cbutton
{
public:
	Camount( Cdialog *parent, const Crect &rect, keybutton code, const Sfont &font,
		   const Cmoney &amount, colour text=0);
	Camount( Cdialog *parent, const Crect &rect, const Cmoney &value);
	virtual ~Camount();
	void	setAmount( const Cmoney &amount);
};
