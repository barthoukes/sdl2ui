/*============================================================================*/
/**  @file       sdl_dialog_list.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Keep any list of dialogs.
 **
 **  We have 2 standard lists, one for message boxes, another for our world.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogList
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 17 nov 2013
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include "sdl_dialog_list.h"
#include "sdl_dialog_event.h"

static const bool D=false;

/// Our system, our world.
CdialogList g_myWorld;

/// Our message boxes.
CdialogList g_messageBox;

/** @brief Constructor */
CdialogList::CdialogList()
{
	clear();
}

/** @brief Destructor. */
CdialogList::~CdialogList()
{
	clear();
}

/** Clear list without destroy. */
void CdialogList::clear()
{
	m_dialogs.clear();
}

/** @brief Dialog stops by the user.
 *  @param interface [in] Dialog to remove.
 */
void CdialogList::removeDialog( CdialogBase *interface)
{
	lock();
	//m_repeatTimer.stop();
	//m_mousePressedLeft =false;
	//m_mouseStatus =MOUSE_RELEASED;
	int size =(int)m_dialogs.size();
	for ( int n=0; n<size;)
	{
		if ( m_dialogs[n] ==interface)
		{
			m_dialogs.erase( m_dialogs.begin()+n);
			size--;
			break;
		}
		else
		{
			n++;
		}
	}
	unlock();
}

/** @brief New dialog created by the user.
 *  @param interface [in] What dialog to add.
 */
void CdialogList::addDialog( CdialogBase *interface)
{
	lock();
	// CdialogEvent::newDialog %s", interface->getName().c_str());

	int size =(int)m_dialogs.size();
	for ( int n=0; n<size;n++)
	{
		if ( m_dialogs[n] ==interface)
		{
			// CdialogList::addDialog  Dialog already found %d %d!!", n, size);
			return;
		}
	}
	m_interface =interface;
	m_dialogs.push_back( interface);
	unlock();
}

/** @brief Calculate last dialog.
 *  @return pointer to dialog.
 */
CdialogBase *CdialogList::firstDialog()
{
	CdialogBase *ret =NULL;

	lock();
	if ( m_dialogs.size() !=0)
	{
		ret =m_dialogs[0];
	}
	unlock();
	// first dialog =%s", ret? ret->getName().c_str():"NULL");
	return ret;
}

/** @brief Calculate last dialog.
 *  @return pointer to dialog.
 */
CdialogBase *CdialogList::lastDialog()
{
	CdialogBase *ret =NULL;

	lock();
	int n=(int)m_dialogs.size();
	if ( n !=0)
	{
		ret =m_dialogs[ n-1];
	}
	unlock();
	// last dialog =%s", ret? ret->getName().c_str():"NULL");
	return ret;
}

/** @brief Calculate next dialog.
 *  @param current [in] Start from current.
 *  @return next dialog.
 */
CdialogBase *CdialogList::nextDialog( CdialogBase *current)
{
	CdialogBase *ret =NULL;

	lock();
	int n=(int)m_dialogs.size();
	for ( int x=0; x<n-1; x++)
	{
		if ( m_dialogs[x]==current)
		{
			ret =m_dialogs[x+1];
			break;
		}
	}
	unlock();
	// next dialog =ret->getName()
	return ret;
}

/** @brief Calculate previous dialog.
 *  @param current [in] Start from current.
 *  @return previous dialog.
 */
CdialogBase *CdialogList::previousDialog( CdialogBase *current)
{
	CdialogBase *ret =NULL;

	lock();
	int n=(int)m_dialogs.size();
	for ( int x=1; x<n; x++)
	{
		if ( m_dialogs[x]==current)
		{
			ret =m_dialogs[x-1];
			break;
		}
	}
	unlock();
	return ret;
}
