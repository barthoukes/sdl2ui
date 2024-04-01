/*============================================================================*/
/**  @file       sdl_swype_object.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Swype objecty
 **
 **  Create a default 2D swype object
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CswypeObject
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

/*------------- Standard includes --------------------------------------------*/
#include "sdl_swype_object.h"
#include "sdl_graphics.h"
#include "SDL_video.h"

int CswypeObject::totalMemory =0;
int CswypeObject::totalPictures =0;

/*----------------------------------------------------------------------------*/
CswypeObject::CswypeObject( const Csize &size, int index, const Crect &destination)
: index(index), itemId(0), destination(destination), dialogObject(NULL)
{
	int options =SDL_HWSURFACE;
	Uint32 bmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 rmask = 0x00ff0000;
	Uint32 amask = 0;

	texture =SDL_CreateRGBSurface( options, size.width()*8, size.height()*8,
								   32, //::m_mainGraph->m_renderSurface->format->BitsPerPixel,
								   rmask, gmask, bmask, amask);
    totalMemory ++;
}

/*----------------------------------------------------------------------------*/
CswypeObject::~CswypeObject()
{
	dialogObject = NULL;
	clean();
}

/*----------------------------------------------------------------------------*/
void CswypeObject::clean()
{
	if (texture)
	{
		SDL_FreeSurface(texture);
		totalMemory--;
		texture =NULL;
		return;
	}
}

/*----------------------------------------------------------------------------*/
bool CswypeObject::compareSize(const Csize &size) const
{
	return (texture != NULL
		   && texture->w == size.width()*8
		   && texture->h == size.height()*8);
}

/*----------------------------------------------------------------------------*/
void CswypeObject::onPaint( Cpoint &point)
{
	(void)point;
}

/*----------------------------------------------------------------------------*/
bool CswypeObject::isValid() const
{
	return texture !=NULL;
}

/*----------------------------------------------------------------------------*/
SDL_Surface *CswypeObject::getTexture()
{
	return texture;
}

/*----------------------------------------------------------------------------*/
CdialogObjectPtr CswypeObject::getDialogObject()
{
	return dialogObject;
}
