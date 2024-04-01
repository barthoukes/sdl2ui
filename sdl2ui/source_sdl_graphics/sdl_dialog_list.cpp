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
#include "sdl_swype_dialog.h"

static const bool D=false;

/*----------------------------------------------------------------------------*/
/// Our system, our world.
//CdialogList g_myWorld;

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** @brief Constructor */
CdialogList::CdialogList()
{
	clear();
}

/*----------------------------------------------------------------------------*/
/** @brief Destructor. */
CdialogList::~CdialogList()
{
	clear();
}

/*----------------------------------------------------------------------------*/
void CdialogList::deleteDialogs()
{
    lock();
    for ( CdialogBase *dialog : m_dialogs1)
    {
        if (dialog) delete dialog;
    }
    unlock();

    clear();
}

/*----------------------------------------------------------------------------*/
void CdialogList::resetPaintedArea()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        CswypeDialog *dialog = dynamic_cast<CswypeDialog*>(base);
        if ( dialog !=NULL)
        {
            dialog->resetPaintedArea();
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        CswypeDialog *swypeDialog =dynamic_cast<CswypeDialog*>(base2.get());
        if ( swypeDialog)
        {
            swypeDialog->resetPaintedArea();
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
bool CdialogList::onInit()
{
    lock();
    bool running = true;
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *dialog = dynamic_cast<Cdialog*>(base);
        if ( dialog && dialog->onInit() ==false)
        {
            running = false;
            break;
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *dialog =dynamic_cast<CswypeDialog*>(base2.get());
        if ( dialog && dialog->onInit() ==false)
        {
            running = false;
            break;
        }
    }
    unlock();
    return running;
}

/*----------------------------------------------------------------------------*/
bool CdialogList::onLoop()
{
    lock();
    bool running = true;
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *dialog = dynamic_cast<Cdialog*>(base);
        if (!dialog || !dialog->isVisible())
        {
        	continue;
        }
        if ( dialog->onLoop() ==false)
        {
            running = false;
            break;
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *dialog =dynamic_cast<CswypeDialog*>(base2.get());
        if (!dialog || !dialog->isVisible())
        {
        	continue;
        }
        if (dialog->onLoop() ==false)
        {
            running = false;
            break;
        }
    }
    unlock();
    return running;
}

/*----------------------------------------------------------------------------*/
void CdialogList::onCleanup()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *dialog = dynamic_cast<Cdialog*>(base);
        if ( dialog)
        {
            dialog->onCleanup();
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *dialog =dynamic_cast<Cdialog*>(base2.get());
        if ( dialog)
        {
            dialog->onCleanup();
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
Estatus CdialogList::onButton(SDLMod mod, keybutton sym, Cdialog *parent, Estatus stat)
{
    lock();
    int idx =(int)m_dialogs1.size()-1;
    while (idx>=0 && stat!=DIALOG_EVENT_PROCESSED && stat!=DIALOG_EVENT_EXIT)
    {
        Cdialog *dialog = dynamic_cast<Cdialog*>(m_dialogs1[idx--]);
        if ( dialog->m_visible ==true)
        {
            stat = dialog->onButton( mod, sym);
            if ( stat==DIALOG_EVENT_EXIT)
            {
                parent->stop(0);
                stat =DIALOG_EVENT_PROCESSED;
            }
        }
    }
    idx =(int)m_dialogs2.size()-1;
    while (idx>=0 && stat!=DIALOG_EVENT_PROCESSED && stat!=DIALOG_EVENT_EXIT)
    {
        Cdialog *dialog = dynamic_cast<Cdialog*>(m_dialogs2[idx--].get());
        if ( dialog->m_visible ==true)
        {
            stat = dialog->onButton( mod, sym);
            if ( stat==DIALOG_EVENT_EXIT)
            {
                parent->stop(0);
                stat =DIALOG_EVENT_PROCESSED;
            }
        }
    }
    unlock();
    return stat;
}

/*----------------------------------------------------------------------------*/
void CdialogList::invalidateSwypeDialogs()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        CswypeDialog *dialog =dynamic_cast<CswypeDialog*>(base);
        if ( dialog !=NULL)
        {
            dialog->invalidate(-1);
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        CswypeDialog *dialog =dynamic_cast<CswypeDialog*>(base2.get());
        if ( dialog !=NULL)
        {
            dialog->invalidate(-1);
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void CdialogList::onPaint()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *t=dynamic_cast<Cdialog*>(base);
        if ( t && t->m_visible)
        {
            t->onPaint();
            t->onPaintButtons();
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *t=dynamic_cast<Cdialog*>(base2.get());
        if ( t && t->m_visible)
        {
            t->onPaint();
            t->onPaintButtons();
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void CdialogList::onClearScreen()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *t=dynamic_cast<Cdialog*>(base);
        if ( t && t->m_visible)
        {
            t->onClearScreen();
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *t=dynamic_cast<Cdialog*>(base2.get());
        if ( t && t->m_visible)
        {
            t->onClearScreen();
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void CdialogList::clearScreenAndPaint()
{
    lock();
    for ( CdialogBase *base : m_dialogs1)
    {
        Cdialog *dialog =dynamic_cast<Cdialog*>(base);
        if (dialog && dialog->m_visible)
        {
            dialog->onClearScreen();
            dialog->onPaint();
            dialog->onPaintButtons();
        }
    }
    for ( CdialogBasePtr base2 : m_dialogs2)
    {
        Cdialog *dialog =dynamic_cast<Cdialog*>(base2.get());
        if (dialog && dialog->m_visible)
        {
            dialog->onClearScreen();
            dialog->onPaint();
            dialog->onPaintButtons();
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
bool CdialogList::onLoopWithSelfDestruct()
{
    bool invalidate = false;
    int n=0;
    while (n<(int)m_dialogs1.size())
    {
        Cdialog *dialog=dynamic_cast<Cdialog*>(m_dialogs1[n]);
        if (dialog && dialog->isVisible() && dialog->onLoop()==false)
        {
            // Close the dialog, not the layers below.
            if ( dialog->m_alive ==false || dialog->m_selfDestruct==true)
            {
                delete dialog;
                removeDialog(dialog);
                invalidate = true;
                break;
            }
        }
        n++;
    }
    n=0;
    while (n<(int)m_dialogs2.size())
    {
        Cdialog *dialog =dynamic_cast<Cdialog*>(m_dialogs2[n].get());
        if (dialog && dialog->isVisible() && dialog->onLoop()==false)
        {
            // Close the dialog, not the layers below.
            if ( dialog->m_alive ==false || dialog->m_selfDestruct==true)
            {
                removeDialog(m_dialogs2[n]);
                invalidate = true;
                break;
            }
        }
        n++;
    }
    return invalidate;
}

/*----------------------------------------------------------------------------*/
Cdialog *CdialogList::findDialog( const Cpoint &point)
{
    lock();
    Cdialog *retval = findDialogInternal(point);
    unlock();

    return retval;
}

/*----------------------------------------------------------------------------*/
Cdialog *CdialogList::findDialogInternal( const Cpoint &point)
{
    Cdialog *retval =NULL;

    int index = m_dialogs1.size()-1;
    while (index>=0 && !retval)
    {
        Cdialog *dialog=dynamic_cast<Cdialog*>(m_dialogs1[index]);
        if ( dialog && dialog->m_visible)
        {
            if ( dialog->inside( point))
            {
                // See if subdialogs inside the found dialog are more suitable.
                Cdialog *retval2 =dialog->findDialogInChild(point);
                retval = retval2 ? retval2:dialog;
            }
            if (point.x==0 && point.y==0)
            {
                retval =dialog;
            }
        }
        index--;
    }
    index =m_dialogs2.size()-1;
    while (index>=0 && !retval)
    {
        Cdialog* dialog =dynamic_cast<Cdialog*>(m_dialogs2[index].get());
        if ( dialog && dialog->m_visible)
        {
            if ( dialog->getRect().inside( point))
            {
                // See if subdialogs inside the found dialog are more suitable.
                Cdialog *retval2 =dialog->findDialogInChild(point);
                retval = retval2 ? retval2:dialog;
            }
            if (point.x==0 && point.y==0)
            {
                retval =dialog;
            }
        }
        index--;
    }

    return retval;
}

/*----------------------------------------------------------------------------*/
/** Clear list without destroy. */
void CdialogList::clear()
{
    lock();
	m_dialogs1.clear();
	m_dialogs2.clear();
    unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Dialog stops by the user.
 *  @param interface [in] Dialog to remove.
 */
void CdialogList::removeDialog( CdialogBase *interface)
{
	lock();
	int size =(int)m_dialogs1.size();
	int n=0;
	while (n<size)
	{
		if ( m_dialogs1[n] ==interface)
		{
			m_dialogs1.erase( m_dialogs1.begin()+n);
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

/*----------------------------------------------------------------------------*/
/** @brief Dialog stops by the user.
 *  @param interface [in] Dialog to remove.
 */
void CdialogList::removeDialog( CdialogBasePtr interface)
{
    lock();
    int size =(int)m_dialogs2.size();
    for ( int n=0; n<size;)
    {
        if ( m_dialogs2[n].get() ==interface.get())
        {
            m_dialogs2.erase( m_dialogs2.begin()+n);
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

/*----------------------------------------------------------------------------*/
/** @brief New dialog created by the user.
 *  @param interface [in] What dialog to add.
 */
void CdialogList::addDialog( CdialogBase *interface)
{
	if ( interface )
	{
		lock();
		int size =(int)m_dialogs1.size();
		for ( int n=0; n<size;n++)
		{
			if ( m_dialogs1[n] ==interface)
			{
				// CdialogList::addDialog  Dialog already found %d %d!!", n, size);
				return;
			}
		}
		m_dialogs1.push_back( interface);
		unlock();
	}
}

/*----------------------------------------------------------------------------*/
/** @brief New dialog created by the user.
 *  @param interface [in] What dialog to add.
 */
void CdialogList::addDialog( CdialogBasePtr interface)
{
    if ( interface.get() )
    {
        lock();
        int size =(int)m_dialogs2.size();
        for ( int n=0; n<size;n++)
        {
            if ( m_dialogs2[n].get() ==interface.get())
            {
                // CdialogList::addDialog  Dialog already found %d %d!!", n, size);
                return;
            }
        }
        m_dialogs2.push_back( interface);
        unlock();
    }
}

/*----------------------------------------------------------------------------*/
/** @brief Calculate last dialog.
 *  @return pointer to dialog.
 */
CdialogBase *CdialogList::firstDialog()
{
	CdialogBase *ret =NULL;

	lock();
	if ( m_dialogs1.size() !=0)
	{
		ret =m_dialogs1[0];
	}
	unlock();
	return ret;
}

/*----------------------------------------------------------------------------*/
/** @brief Calculate last dialog.
 *  @return pointer to dialog.
 */
CdialogBase *CdialogList::lastDialog()
{
	CdialogBase *ret =NULL;

	lock();
	int n=(int)m_dialogs1.size();
	if ( n !=0)
	{
		ret =m_dialogs1[ n-1];
	}
	unlock();
	return ret;
}
