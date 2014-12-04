/*============================================================================*/
/**  @file       sdl_key_file.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 26 dec 2012
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include "sdl_key_file.h"
#include "sdl_graphics.h"

#define ZHONGCAN_KEYFILE "keys.txt"

CkeyFile::CkeyFile()
: m_file(NULL)
, m_mouseDown(false)
{
	init();
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
void CkeyFile::onEvent(SDL_Event* event)
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
		case SDL_BUTTON_WHEELUP:
			addEvent( "WHEELUP", event->button.x, event->button.y, "");
			break;
		case SDL_BUTTON_WHEELDOWN:
			addEvent( "WHEELDOWN", event->button.x, event->button.y, "");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
