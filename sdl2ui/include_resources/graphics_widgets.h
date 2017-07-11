/*============================================================================*/
/**  @file       graphics_widgets.h
 **  @ingroup    user_interface
 **  @brief		 To be expired soom
 **
 **  @todo       Remove this file not OO.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdatesDialog
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
#include "money.h"
#include "sdl_graphics.h"
#include "zhongstring.h"
#include "sdl_rect.h"

// Attribute lines
#define DOT_ON            1
#define DOT_OFF           0
// Linestyles
#define STRAIGHT_LINE     0
#define RECTANGLE         1
#define FILLED_RECTANGLE  2
#define SINGLE_COLOUR	  16
#define FAT_BORDER        4096
#define ROUND_RECTANGLE	  8192

#define PSET(x, y, attr) bar((x),(y),(x)+1,(y))

extern void Cursor(int FLAG);

extern int SplitBCD(char **t);

extern int SplitHEX(char **t);

// Hardware registers on system-segment 0xa800
#define SYSTEM_MAP    0xa800
#define CSR1_ENABLE   0x013f

//void TextColor( colour x);

//void BackGround( colour x);

//void SetColors( colour fg, colour bg);

//void PrintAT(int x, int y, int w, int h, keybutton KeyCode,
//		const std::string &text, int FLAGS);

void ColorAT(int x, int y, int w, int h, int colour =0x000000, int radius=0);
void ColorAT( Crect rect, colour color =0x000000, int radius =0);

void networkStatusAT(int x, int y, colour on, colour off, int size, int bits);

void DarkenAT( int x, int y, int w, int h);

void CrossAT(int x, int y, int w, int h, colour col);

//void PrintAT(int x, int y, int w, int h, keybutton code, const Cmoney &d,
//			 colour col1=COLOUR_BLACK, colour col2=COLOUR_MEDIUMGREY);

