/*============================================================================*/
/**  @file       sdl_swype_dialog.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Swype dialog 1D list
 **
 **  Create a default swype list 1 dimensional.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CswypeDialog
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

#include <memory>
#include "sdl_rect.h"

class SDL_Surface;
class CdialogObject;
typedef std::shared_ptr<CdialogObject> CdialogObjectPtr;

#pragma once
/*------------- Standard includes --------------------------------------------*/
/// @brief Structure to hold each texture to paint. We have a queue of this.
class CswypeObject
{
public:
	/**
	 * @param size [in] Initial size of the object to paint
	 * @param index [in] What index in an array
	 * @param destination [in] Where to draw the image in the end
	 */
	CswypeObject( const Csize &size, int index, const Crect &destination);
	virtual ~CswypeObject();
	void clean();
	void onPaint( Cpoint &point);
	bool isValid() const;
	SDL_Surface *getTexture();
	CdialogObjectPtr getDialogObject();
	bool compareSize(const Csize &size) const;

private:
	CswypeObject(const CswypeObject& o);
	CswypeObject& operator=(const CswypeObject& o);

public:
	static int		totalMemory; ///< Find memory leaks
	static int		totalPictures;

	int			  	index; ///< Index in the list
	int				itemId; ///< Item to use.
	Crect			destination; ///< Where to paint the object should be multiplied by 8.

private:
	CdialogObjectPtr dialogObject; ///< Reference to object (if needed)
	SDL_Surface   	*texture; ///< Painted area
};

typedef std::shared_ptr<CswypeObject> CswypeObjectPtr;
