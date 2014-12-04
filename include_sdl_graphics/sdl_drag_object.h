/*============================================================================*/
/**  @file       sdl_dialog_event.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Default dialog.
 **
 **  Create an event for the dialogs.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cevent
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 28 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/

#ifndef SDL_DRAGG_OBJECT_H_
#define SDL_DRAGG_OBJECT_H_

#ifdef USE_SDL2

#include "sdl_rect.h"
#include "SDL_rect.h"
#include "SDL_render.h"

class CdialogObject;

/// @brief Object to use for dragging around the screen
class CdragObject
{
public:
	//CdragObject( SDL_Texture *reference, const Cpoint &origin);
	CdragObject();
	virtual ~CdragObject();

	void dragStart( CdialogObject *object, const Cpoint &origin);
	void clear();
	void dragMove( const Cpoint &position);
	void dragEnd( const Cpoint &position);
	void setVisible( bool visible);
	void onPaint();
	bool isVisible() { return m_visible; }
	Cpoint dragPoint( const Csize &graph, const Cpoint &mouse);

private:
	CdialogObject 	*m_relatedObject;	///< Relation to an object
	//SDL_Texture		*m_relatedTexture;  ///< Texture relation
	bool			m_visible;			///< Is the item visible
	Cpoint			m_origin; 			///< Position to start from
	SDL_Rect		m_position;			///< Position current
	SDL_Texture		*m_texture;			///< Object itself to paint
	SDL_Renderer    *m_renderer;		///< Where to render
	Cpoint			m_dragOffset;		///< Point within area we touched
	Cpoint			m_dragStart;		///< start of drag object.
};

#endif
#endif /* SDL_DRAG_OBJECT_H_ */
