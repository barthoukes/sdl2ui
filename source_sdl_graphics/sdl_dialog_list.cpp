/*============================================================================*/
/**  @file       sdl_dialog_list.cpp
 **  @ingroup    sdl2ui
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
#include "sdl_dialog_list.h"
#include "sdl_dialog_event.h"
#include "sdl_dialog.h"

static const bool D=false;

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

/** Paint all dialogs */
bool CdialogList::onPaint()
{
	bool retVal =false;
	for ( CdialogBase *p : m_dialogs)
	{
		Cdialog *d = dynamic_cast<Cdialog*>(p);
		if (d && d->isInvalidated() && d->m_myGraphics)
		{
			d->onPaint();
			retVal =true;
		}
	}
	return retVal;
}

/** Paint all dialogs */
void CdialogList::onRender()
{
	for ( CdialogBase *p : m_dialogs)
	{
		Cdialog *d = dynamic_cast<Cdialog*>(p);
		if (d)
		{
			if (d->m_myGraphics)
			{
				d->onRender();
			}
			else
			{
				d->invalidate(false);
				d->onPaint();
			}
		}
	}
}

dialogBaseIterator CdialogList::begin()
{
	return m_dialogs.begin();
}

dialogBaseIterator CdialogList::end()
{
	return m_dialogs.end();
}

Estatus CdialogList::onButton(keymode mod, keybutton sym)
{
	for ( CdialogBase *p : m_dialogs)
	{
		Cdialog *d = dynamic_cast<Cdialog*>(p);
		if (d)
		{
			switch( d->onButton( mod, sym) )
			{
			case DIALOG_EVENT_EXIT:
				return DIALOG_EVENT_EXIT;
			case DIALOG_EVENT_GENERAL:
				return DIALOG_EVENT_GENERAL;
			case DIALOG_EVENT_OPEN:
				continue;
			case DIALOG_EVENT_PROCESSED:
				return DIALOG_EVENT_PROCESSED;
			}
		}
	}
	return DIALOG_EVENT_OPEN;
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
	// CdialogEvent::newDialog %s", interface->getName().c_str());
	if ( interface )
	{
		lock();
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
