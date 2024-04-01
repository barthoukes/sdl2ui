/*
 * sdl_swype_object_dialog.h
 *
 *  Created on: 18 dec. 2011
 *      Author: mensfort
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
#include <queue>
#include "sdl_swype_dialog.h"

class Cdialog;

/// @brief Scrolling dialog with list of items.
class CswypeObjectDialog : public CswypeDialog
{
public:
	CswypeObjectDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, int span, bool horizontal);
	virtual ~CswypeObjectDialog();

public: // override
	virtual keybutton findButton( const Cpoint &p);
	virtual int getScrollIndex( keybutton sym);
	virtual void onPaintUnit( int unit, const Crect &location) =0;
	CdialogObjectPtr findObject( const Cpoint &p);
};

/* SCROLL_2D_DIALOG_H_ */
