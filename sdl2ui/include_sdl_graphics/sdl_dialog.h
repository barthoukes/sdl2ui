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
#include <SDL.h>
#include <memory>
#include <atomic>
#include <pthread.h>

#include "sdl_rectangle.h"
#include "sdl_after_glow.h"
#include "sdl_drag_object.h"
#include "sdl_dialog_list.h"
#include "sdl_dialog_event.h"

class CdialogObject;
typedef std::shared_ptr<CdialogObject> CdialogObjectPtr;
class Cgraphics;
class Cdialog;
typedef std::shared_ptr<Cdialog> CdialogPtr;

/// @brief Dialog on screen.
class Cdialog : public CeventInterface, public CdialogBase, protected CmyLock
{
public:
	Cdialog( Cdialog *parent, const std::string &name, const Crect &rect=Crect(0,0,0,0));
	virtual ~Cdialog();

	// From interface CinputEvent
	virtual void notifyTouch( const Cpoint &p);
	std::string getName() { return m_name; }
	virtual Estatus onEvent( const Cevent &event);
	virtual void onStopDrag();
	virtual void onStartDrag( CdialogObjectPtr movingObject);
	virtual CdialogObjectPtr findObject( const Cpoint &p);
	virtual keybutton findButton( const Cpoint &p);
	void scrollDialog( const Cpoint &start, const Cpoint &distance);
	bool isSwypeDialog( const Cpoint &p);
	void dragObject( CdialogObjectPtr movingObject, const Cpoint &to);
	virtual bool isScrollDragDialog( const Cpoint &p);
	virtual bool isHorizontalScrollDialog( const Cpoint &p);
	void insertDragObject();
	virtual void darken( int bottomMargin);
	virtual bool onInit();
	virtual void onPaint() =0;
	virtual void onCleanup() =0;
	virtual Estatus onButton( SDLMod mod, keybutton sym) =0;
	virtual void onUpdate() {}
	virtual int onExecute( Cdialog *parent);
	virtual void onDrag( const Cpoint &position) { (void)position; }
	virtual void onDragRelease( const Cpoint &position, CdialogObjectPtr movingObject); // 800x600
	virtual bool onLoop();
	virtual void stop(int exitValue);
    virtual void onBarcode( const std::string &barcode);
    virtual void makeValid();

	virtual void onInputFocus();
	virtual void onInputBlur();
	virtual Estatus onKeyUp(keybutton sym, SDLMod mod);
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
	virtual void exitDialog() { m_running=false; }
	virtual void onClearScreen();
	static void invalidate();
	virtual void invalidateAll();
	virtual void setBackgroundColour( colour colour);
	virtual void registerObject(CdialogObjectPtr child);
	virtual void unregisterObject(CdialogObjectPtr child);
	virtual void registerChild(Cdialog *child);
	virtual void registerChild(CdialogPtr child);
	virtual void unregisterChild(Cdialog *child);
	virtual void unregisterChild(CdialogPtr child);
	virtual void registerMessageBox(Cdialog *child);
	virtual void unregisterMessageBox(Cdialog *child);
	static Cpoint getTouchPosition();
	void setVisible( bool visible=true);
	virtual bool isVisible() const;
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual int width() const;
	virtual int height() const;
	virtual Crect getRect() const;
	virtual void touchPress( const Cpoint &p) { (void)p; }
	virtual void touchLong( const Cpoint &point, keybutton sym) { (void)point; (void)sym; }
	void paintCoordinates( bool update);

	virtual void onPaintingStart( const Cpoint &position);
	virtual void onPaintingMove( const Cpoint &position);
	virtual void onPaintingStop( const Cpoint &position);
	virtual void onPaintButtons();
	virtual bool inside(const Cpoint &point) const;

protected:
	Cpoint dragPoint( const Cpoint &mouse);

	Cdialog * findDialog( const Cpoint &p);
	void createRandomEvent( SDL_Event *m_event);
	void idiotTest();
	Estatus tryButton( SDLMod mod, keybutton sym);
	virtual void stopExecute();
	virtual void startExecute();
	virtual void lockEvent();
	virtual void unlockEvent();

public:
	//void registerMessageBox();
	//void unregisterMessageBox();
	virtual void paintAll();
	virtual void paintAllDark();
	virtual bool spaceIsLanguage() { return m_spaceIsLanguage; }
	virtual void setRect( const Crect &rect);
	virtual bool isChild() const;

private:
	Cdialog *findDialogInMbx( const Cpoint &point);
	Cdialog *findDialogInChild( const Cpoint &point);
	void setChild();

public:
	bool 				m_alive; ///< Am I alive.
	bool 				m_allButtons; ///< Also when button is outside window.
	colour			 	m_backgroundColour;	///< Colour as background.
	int 				m_current_thread;	///< Current thread.
	int 				m_exitValue;
	static int 			m_main_thread;		///< Main thread, thread.
	bool				m_in_main_thread; ///< Start from main thread or not?
	std::string 		m_name;				///< Name of current thread.
	Cdialog 			*m_parent;
	bool 				m_spaceIsLanguage;
	bool 				m_visible; ///< Is the dialog visible?
	bool				m_isChild;

private:
	static int 			m_init;

protected:
	dialogObjectList	m_objects;			///< Objects to display
private:
	static bool			m_useClick;			///< Use the clicking sound
	static CdialogList  m_messageBox;
	static std::atomic<bool> m_invalidate;  ///< Should we re-paint the screen.

public:
	static CkeyFile		m_keyFile;			///< File to repeat key pressed.

protected:
	CdialogList         m_children;	    	///< Sub dialogs. Should be handled atomic.
	bool 				m_isMessageBox;   	///< Am I a messagebox?
	bool 				m_running;			///< Am I still running?
	SDL_Event			m_repeatEvent;		///< What to repeat.
	SDL_Event			m_lastEvent;		///< Last event received.
	Cdialog 			*m_root;			///< Which dialog pressed.
	static Cpoint		m_dragPoint;		///< point to drag.
	static CdragObject 	m_dragObject;		///< Object to drag.
	Crect 				m_rect;             ///< In 100x75 resolution.
	int					m_squares_width; 	///< Width in squares of 8 pixels
	int					m_squares_height; 	///< Height in squares of 8 pixels
	Cpoint				m_dialogOffset;		///< Offset X,Y in pixels to draw.

public:
	CafterGlowList		m_touchList;		///< List for touched objects
	Cgraphics			*m_graphics;		///< Graphics layer. Allocated here.
	bool				m_isOwner;			///< Do I own the graph?
	bool m_selfDestruct;

	friend class CdialogList;
    friend class CmessageBox;
};

/* APPLICATION_DIALOG_H_ */
