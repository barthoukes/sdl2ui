/*============================================================================*/
/**  @file       sdl_key_file.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Low level graph
 **
 **  Init the key_file, then write the file.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CkeyFile
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
#include "sdl_key_file.h"
#include "sdl_graphics.h"
#include "sdl_dialog_event.h"

#define ZHONGCAN_KEYFILE "keys.txt"

CkeyFile::CkeyFile()
: m_file(NULL)
, m_mouseDown(false)
{
}

CkeyFile::~CkeyFile()
{
	close();
}

void CkeyFile::close()
{
	if (m_file)
	{
		fclose( m_file);
		m_file =NULL;
	}
}

void CkeyFile::init()
{
	int enable =Cgraphics::m_defaults.enable_record_keyfile;
	if ( m_file !=NULL || enable==0)
	{
		return;
	}
	close();
	bool append =(enable==2);
	std::string s=Cgraphics::m_defaults.data_path + ZHONGCAN_KEYFILE;
	m_file =fopen( s.c_str(), append ? "at" : "wt");

}

/// @brief Add button to file.
void CkeyFile::addEvent( const std::string &event, int value1, int value2, const std::string &comment)
{
	char t[128];
	if ( m_file ==NULL)
	{
		return;
	}

	sprintf( t, "  \"%s:%d:%d:%s\",\n", event.c_str(), value1, value2, comment.c_str());
	fputs( t, m_file);
	fflush( m_file);
}

/// @brief Add event to the file.
/// @param event [in] What to save.
void CkeyFile::onEvent(const SDL_Event *event)
{
	std::string str;

	switch (event->type)
	{
	case SDL_KEYDOWN:
		str=(std::string)Ckeybutton( (keybutton)(event->key.keysym.sym));
		if ( !strcmp( str.c_str(), "\"" ))
		{
			str="\\\"";
		}
		addEvent( "KEYDOWN", event->key.keysym.mod, event->key.keysym.sym, str);
		break;

	case SDL_KEYUP:
		str=(std::string)Ckeybutton( (keybutton)(event->key.keysym.sym));
		if ( !strcmp( str.c_str(), "\"" ))
		{
			str="\\\"";
		}
		addEvent( "KEYUP", event->key.keysym.mod, event->key.keysym.sym, str);
		break;

	case SDL_MOUSEMOTION:
		if ( m_mouseDown)
		{
			addEvent( "MOUSEMOTION", event->motion.x, event->motion.y, "");
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			addEvent( "BUTTON_UP", event->motion.x, event->motion.y, "");
			m_mouseDown =false;
			break;
		default:
			break;
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			addEvent( "BUTTON_DOWN", event->button.x, event->button.y, "");
			m_mouseDown =true;
			break;
#ifdef USE_SDL2
#else
		case SDL_BUTTON_WHEELUP:
			addEvent( "WHEELUP", event->button.x, event->button.y, "");
			break;
		case SDL_BUTTON_WHEELDOWN:
			addEvent( "WHEELDOWN", event->button.x, event->button.y, "");
			break;
#endif
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/// @brief Add event to the file.
/// @param event [in] What to save.
void CkeyFile::onEvent(const Cevent &event)
{
	std::string str;

	switch (event.type)
	{
	case EVENT_KEY_PRESS:
		str=(std::string)Ckeybutton(event.button);
		if ( !strcmp( str.c_str(), "\"" ))
		{
			str="\\\"";
		}
		addEvent( "KEYDOWN", event.mod, event.button, str);
		break;

	case EVENT_KEY_RELEASE:
		str=(std::string)Ckeybutton(event.button);
		if ( !strcmp( str.c_str(), "\"" ))
		{
			str="\\\"";
		}
		addEvent( "KEYUP", event.mod, event.button, str);
		break;

	case EVENT_MOUSE_MOVE:
		if ( m_mouseDown)
		{
			addEvent( "MOUSEMOTION", event.point.x, event.point.y, "");
		}
		break;

	case EVENT_TOUCH_RELEASE:
		addEvent( "BUTTON_UP", event.point.x, event.point.y, "");
		m_mouseDown =false;
		break;

	case EVENT_TOUCH_LONG:
		addEvent( "TOUCH_LONG", event.point.x, event.point.y, "");
		break;

	case EVENT_TOUCH_PRESS:
		addEvent( "BUTTON_DOWN", event.point.x, event.point.y, "");
		break;

	case EVENT_WHEEL_UP:
		addEvent( "WHEELUP", event.point.x, event.point.y, "");
		break;

	case EVENT_WHEEL_DOWN:
		addEvent( "WHEELDOWN", event.point.x, event.point.y, "");
		break;

	default:
		break;
	}
}
