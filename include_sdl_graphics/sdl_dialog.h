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
 **  Copyright (c) Bart Houkes, 28 jan 2011
 ** 
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#pragma once

#include <SDL/SDL.h>
#include <pthread.h>

#include "sdl_dialog_event.h"
//#include "zhongcan_defines.h"
//#include "lingual.h"
//#include "message_handler.h"
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "sdl_audio.h"
#include "timeout.h"
#include "sdl_rectangle.h"
#include "sdl_key_file.h"
#include "sdl_dialog_list.h"
#include "sdl_after_glow.h"

/// @brief Dialog on screen.
class Cdialog : public CeventInterface, public CdialogBase
{
public:
	Cdialog( Cdialog *parent, const std::string &name, const Crect &rect=Crect(0,0,0,0));
	virtual ~Cdialog();

	// From interface CinputEvent
	virtual void notifyTouch( const Cpoint &p);
	std::string getName() { return m_name; }
	virtual Estatus onEvent( Cevent &event);
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
	//virtual void onMouseClick(keybutton sym, SDLMod mod) =0;
	virtual void onCleanup() =0;
	virtual int onExecute( Cdialog *parent);
	virtual void onDrag( const Cpoint &position) { (void)position; }
	virtual void onDragRelease( const Cpoint &position, CdialogObject * movingObject); // 800x600
	virtual bool onLoop();
	virtual void stop(int exitValue);

	virtual void onInputFocus();
	virtual void onInputBlur();
	virtual Estatus onKeyUp(keybutton sym, SDLMod mod);
	virtual void onMouseFocus();
	virtual void onMouseBlur();
	virtual Estatus onKeyPressed( keybutton key);
	virtual void onMouseMove( const Cpoint &position);
	virtual void onMouseWheel(bool Up, bool Down);
	//virtual Estatus onLButtonDown(int mX, int mY);
	//virtual Estatus onLButtonUp(int mX, int mY);
	//virtual Estatus onRButtonDown(int mX, int mY);
	//virtual Estatus onRButtonUp(int mX, int mY);
	//virtual Estatus onMButtonDown(int mX, int mY);
	//virtual Estatus onMButtonUp(int mX, int mY);
	virtual void onMinimize();
	virtual void onRestore();
	virtual void onResize(int w, int h);
	virtual void onExpose();
	virtual void onExit();
	virtual void onDisplay();
	virtual void exit() { m_running=false; }
	virtual void onClearScreen();
	static void invalidate();
	virtual void invalidateAll();
	virtual void setBackgroundColour( colour colour);
	virtual void registerObject(CdialogObject *child);
	virtual void unregisterObject(CdialogObject *child);
	virtual void registerChild(Cdialog *child);
	virtual void unregisterChild(Cdialog *child);
	static void registerMessageBox(Cdialog *child);
	static void unregisterMessageBox(Cdialog *child);
	Cpoint getTouchPosition();
	static void objectLock();
	static void objectUnlock();
	void setVisible( bool visible=true);
	virtual void wheelDown( int mx, int my);
	virtual void wheelUp( int mx, int my);
	virtual Estatus onButton( SDLMod mod, keybutton sym);
	virtual int width() { return m_rect.width(); }
	virtual int height() { return m_rect.height(); }
	virtual Crect &getRect() { return m_rect; }
	virtual void touchPress( const Cpoint &p) { (void)p; }

protected:
	void onPaintButtons();
	Cpoint dragPoint( const Cpoint &mouse);

	Cdialog       * findDialog( const Cpoint &p);
	//CdialogObject * findObject( const Cpoint &p);

	void createRandomEvent( SDL_Event *m_event);
	void idiotTest();
	Estatus tryButton( SDLMod mod, keybutton sym);
	virtual void stopExecute();
	virtual void startExecute();

public:
	void registerMessageBox();
	void unregisterMessageBox();
	virtual void paintAll();
	virtual bool spaceIsLanguage() { return m_spaceIsLanguage; }
	virtual void setRect( const Crect &rect);

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

	static pthread_mutex_t	m_objectMutex;
	static pthread_mutexattr_t m_attr;

private:
	static int 			m_init;

protected:
	dialogObjectList	m_objects;			///< Objects to display.
private:
	static bool			m_useClick;			///< Use the clicking sound.
	//bool 				m_prevLock; ///< Was the dialog previously locked?

protected:
	static CkeyFile		m_keyFile;			///< File to repeat key pressed.

protected:
	CdialogList         m_children;			///< Sub dialogs. Should be handled atomic.
	static bool 		m_invalidate;		///< Should we re-paint the screen.
	bool 				m_isMessageBox;   	///< Am I a messagebox?
	static language 	m_language;			///< Which language on screen.
	bool 				m_running;			///< Am I still running?
	SDL_Event			m_repeatEvent;		///< What to repeat.
	SDL_Event			m_lastEvent;		///< Last event received.
	Cdialog 			*m_root;			///< Which dialog pressed.
	//CdialogObject		*m_dragObject;		///< object to drag.
	static Cpoint		m_dragPoint;		///< point to drag.
	static CdialogObject *m_dragObject;		///< Object to drag.
	static Cpoint		m_dragOffset;		///< Drag offset in 800x600 res.
	static Cpoint		m_dragStart;		///< start of drag object.
	static bool 		m_objectLocked;        ///< Are we locked?
	Crect 				m_rect;	///< In 100x75 resolution.

public:
	CafterGlowList		m_touchList;		///< List for touched objects
	Cgraphics			*m_graphics;		///< Graphics layer.
};

/* APPLICATION_DIALOG_H_ */
