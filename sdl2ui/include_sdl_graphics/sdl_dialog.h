/*============================================================================*/
/**  @file       sdl_dialog.h
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Create a default dialog. This is a basic template for all dialogs, including message boxes.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cdialog
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
#include <map>
#include <memory>
#include <SDL.h>
#include <pthread.h>

#include "sdl_keybutton.h"
#include "sdl_dialog_event.h"
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "sdl_audio.h"
#include "timeout.h"
#include "sdl_rectangle.h"
#include "sdl_key_file.h"
#include "sdl_dialog_list.h"
#include "sdl_after_glow.h"
#include "sdl_drag_object.h"
#include "sdl_world_interface.h"

/*------------- Standard includes --------------------------------------------*/
class Cdialog;
class Cworld;
typedef std::vector<Cdialog*> dialogList;
typedef dialogList::iterator dialogIterator;
typedef dialogList::const_iterator constDialogIterator;

/// @brief Dialog on screen.
class Cdialog : public CeventInterface, public CdialogBase
{
friend Cworld;

public:
	Cdialog( Cdialog *parent, const std::string &name, const Crect &rect=Crect(0,0,0,0), Iworld *world = NULL);
	virtual ~Cdialog();

	std::shared_ptr<Cgraphics> graphics() { return m_world->graphics(); }

	// From interface CinputEvent
	virtual void notifyTouch( const Cpoint &p);
	std::string getName() { return m_name; }
	virtual Estatus onEvent( const Cevent &event);
	virtual void onStopDrag();
	virtual void onStartDrag( CdialogObject * movingObject);
	virtual CdialogObject *findObject( const Cpoint &p);
	virtual keybutton findButton( const Cpoint &p);
	void scrollDialog( const Cpoint &start, const Cpoint &distance);
	bool isSwypeDialog( const Cpoint &p);
	void dragObject( CdialogObject * movingObject, const Cpoint &to);
	virtual bool isScrollDragDialog( const Cpoint &p);
	virtual bool isHorizontalScrollDialog( const Cpoint &p);
	void insertDragObject();

	virtual bool onInit();
	virtual void onUpdate() =0;
	virtual void onPaint() =0;
	virtual void onCleanup() =0;
	virtual int onExecute( Cdialog *parent);
	virtual void onDrag( const Cpoint &position) { (void)position; }
	virtual void onDragRelease( const Cpoint &position, CdialogObject * movingObject); // 800x600
	virtual bool onLoop();
	virtual void stop(int exitValue);

	virtual void onInputFocus();
	virtual void onInputBlur();
	virtual Estatus onKeyUp(keybutton sym, keymode mod);
	virtual void onMouseFocus();
	virtual void onMouseBlur();
	virtual Estatus onKeyPressed( keybutton key);
	virtual void onMouseMove( const Cpoint &position);
	virtual void onMouseWheel(bool Up, bool Down);
	virtual void onMinimize();
	virtual void onRestore();
	virtual void onResize(int w, int h);
	virtual void onExpose();
	virtual void onExit();
	virtual void onDisplay();
	virtual void exit() { m_running=false; }
	virtual bool isRunning() { return m_running; }
	virtual void onClearScreen();
	virtual void invalidate( bool needs_repaint=true);
	virtual bool isInvalidated();
	virtual void invalidateAll();
	virtual void setBackgroundColour( colour colour);
	virtual void registerObject(CdialogObject *child);
	virtual void unregisterObject(CdialogObject *child);
	virtual void registerChild(Cdialog *child);
	virtual void unregisterChild(Cdialog *child);
	Cpoint getTouchPosition();
	static void objectLock();
	static void objectUnlock();
	void setVisible( bool visible=true);
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual Estatus onButton( keymode mod, keybutton sym) =0;
	virtual int width() { return m_rect.width(); }
	virtual int height() { return m_rect.height(); }
	virtual Crect &getRect() { return m_rect; }
	virtual void touchPress( const Cpoint &p) { (void)p; }
	virtual void touchLong( const Cpoint &point, keybutton sym) { (void)point; (void)sym; }
	virtual void createMyGraph();
	virtual void onPaintingStart( const Cpoint &position);
	virtual void onPaintingMove( const Cpoint &position);
	virtual void onPaintingStop( const Cpoint &position);

protected:
	void onPaintButtons();
	Cpoint dragPoint( const Cpoint &mouse);

	Cdialog       * findDialog( const Cpoint &p);
	void createRandomEvent( SDL_Event *m_event);
	void idiotTest();
	Estatus tryButton( keymode mod, keybutton sym);
	virtual void stopExecute();
	virtual void startExecute();

public:
	//void registerMessageBox();
	//void unregisterMessageBox();
	virtual bool spaceIsLanguage() { return m_spaceIsLanguage; }
	virtual void setRect( const Crect &rect);
	virtual void onRender(); // What should be painted every time?

public:
	Iworld				*m_world;
	bool 				m_alive; 			///< Am I alive.
	bool 				m_allButtons; 		///< Also when button is outside window.
	colour			 	m_backgroundColour;	///< Colour as background.
	int 				m_current_thread;	///< Current thread.
	int 				m_exitValue;
	bool				m_in_main_thread; 	///< Start from main thread or not?
	std::string 		m_name;				///< Name of current thread.
	Cdialog 			*m_parent;
	bool 				m_spaceIsLanguage;
	bool 				m_visible; 			///< Is the dialog visible?

	static pthread_mutex_t	m_objectMutex;
	static pthread_mutexattr_t m_attr;

public:
	static Iworld		*g_defaultWorld;

protected:
	dialogObjectList	m_objects;			///< Objects to display
private:
	static bool			m_useClick;			///< Use the clicking sound
	//bool 				m_prevLock; ///< Was the dialog previously locked?

public:
	CdialogList         m_children;			///< Sub dialogs. Should be handled atomic.
protected:
	bool 				m_invalidate;		///< Should we re-paint the screen.
	bool 				m_isMessageBox;   	///< Am I a messagebox?
	bool 				m_running;			///< Am I still running?
	SDL_Event			m_repeatEvent;		///< What to repeat.
	SDL_Event			m_lastEvent;		///< Last event received.
	Cdialog 			*m_root;			///< Which dialog pressed.
	static bool 		m_objectLocked;     ///< Are we locked?
	Crect 				m_rect;				///< In 1/8th resolution.
	int					m_squares_width; 	///< Width in squares of 8 pixels
	int					m_squares_height; 	///< Height in squares of 8 pixels
	Cpoint				m_dialogOffset;		///< Offset X,Y in pixels to draw.

public:
	CafterGlowList		m_touchList;		///< List for touched objects
	std::shared_ptr<Cgraphics> 	m_graphics;		///< Graphics layer from world.
	std::shared_ptr<Cgraphics>  m_myGraphics; ///< My private Graphics layer.
	bool				m_render_after_paint;///< Paint only when we render.
	bool				m_full_screen;		///< Are we full screen?
	bool 				m_selfDestruct;		///< Can I self-destruct
	CdragObject			m_dragObject;		///< Object to drag
};

/* APPLICATION_DIALOG_H_ */
