/*============================================================================*/
/**  @file       sdl_dialog_object.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Object for dialogs.
 **
 **  We have several types of dialog objects.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogObject
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 16 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include "sdl_dialog_object.h"
#include "sdl_dialog.h"

/** @brief Constructor for dialog object.
 *  @param parent [in] Who is painting me.
 *  @param rect [in] Where am i?
 *  @param code [in] What to react when you press me.
 */
CdialogObject::CdialogObject( Cdialog *parent, const Crect &rect, keybutton code)
: m_rect( rect)
, m_code( code)
, m_dragEnable( false)
, m_visible( true)
, m_graphics( m_mainGraph)
, m_parent( parent)
{
	if ( parent !=NULL && parent->m_graphics !=NULL)
	{
		 m_graphics =parent->m_graphics;
	}
}

/** @brief Paint me.
 *  @param p [in] Different location.
 *  @post I'm painted.
 */
void CdialogObject::onPaint( const Cpoint &p, int touch)
{
	Crect r =m_rect;
	m_rect.setLeft( gMin( p.x, (Cgraphics::m_defaults.width/8)-m_rect.width()));
	m_rect.setTop( gMin( p.y, (Cgraphics::m_defaults.height/8)-m_rect.height()));
	onPaint( touch);
	m_rect =r;
}

/** @brief Destructor. */
CdialogObject::~CdialogObject()
{
}
