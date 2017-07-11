/*============================================================================*/
/**  @file       sdl_world.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Create world, with message-boxes and dialogs
 **
 **  Create a default image.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cworld
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2015, 2017
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

#include <assert.h>
#include <pthread.h>
#include "sdl_world.h"
#include "sdl_dialog.h"
#include "sdl_dialog_list.h"
#include "sdl_label.h"

int Cworld::m_init = 0;
pthread_t Cworld::m_main_thread = 0;
CkeyFile Cworld::m_key_file;


Cworld::Cworld(std::shared_ptr<Cgraphics> mainGraph)
: Iworld(mainGraph)
, m_active_dialog(NULL)
, m_main_graph(mainGraph)
, m_drag_point(0,0)
, m_invalidate(false)
{
}

void Cworld::init()
{
	if (!m_init)
	{
		m_main_thread =pthread_self();
		m_key_file.init();
	}
	m_init++;
}

void Cworld::addDialog( Cdialog *dialog)
{
	if (dialog)
	{
		m_dialogs.addDialog(dialog);
	}

}

void Cworld::checkInMainThread()
{
	assert( pthread_self()==m_main_thread);
}

Cworld::~Cworld()
{
	clean();
}

void Cworld::clean()
{
	m_dialogs.clear();
	m_message_box.clear();
	if (--m_init == 0)
	{
		m_lock.lock(); // Children+Messageboxes should not change.
		CdialogEvent::Instance()->stop();
		//CdialogBase *a;
		m_lock.unlock(); // Children+Messageboxes should not change end.
		CdialogEvent::Instance()->KillInstance();
	}
}

void Cworld::invalidate()
{
	m_invalidate = true;
}

/** Paint all, during an invalidate or at every render action */
void Cworld::paintAll()
{
	m_message_box.onPaint();
	m_main_graph->update();

	if (m_active_dialog)
	{
		m_active_dialog->onClearScreen();
		m_active_dialog->onPaint();
		m_active_dialog->onPaintButtons();
		m_active_dialog->m_children.onPaint();
		if ( Cgraphics::m_defaults.debug_coordinates)
		{
			char s[24];
			Cpoint g(CdialogEvent::Instance()->m_debugPosition);
			sprintf(s, " %d %d", g.x, g.y);
			Clabel p(m_active_dialog, Crect(0,0,15,3), KEY_NOCHANGE);
			std::string tt(s);
			p.setText(tt);
			p.onPaint(0);
		}
	}
	m_invalidate =false;
	onRender();
}

/** Render all dialogs to the world graphic, which is the background in the end
 *  Some dialogs may not have their own Cgraphics, so we have to paint them again */
void Cworld::onRender()
{
	// First render all the dialogs
	m_lock.lock();
	if (m_active_dialog == NULL)
	{
		m_lock.unlock();
		return;
	}

	// Update main graph to window
	m_active_dialog->onRender();
	m_main_graph->update();

	// Update message boxes
	m_message_box.onRender();

	// render the drag object
	// render the graphic to the real screen
	m_main_graph->update();
	m_lock.unlock();
}

std::shared_ptr<Cgraphics> Cworld::graphics()
{
	return m_main_graph;
}

void Cworld::setActiveDialog(Cdialog *dialog)
{
	m_lock.lock();
	m_active_dialog = dialog;
	m_lock.unlock();
}

/*============================================================================*/
///
/// @brief 		Add a message box to our system.
///
/// @post       Message box added, but not ready to display.
///
/*============================================================================*/
void Cworld::registerMessageBox(Cdialog *child)
{
	m_lock.lock();
	m_message_box.addDialog( child);
	m_lock.unlock();
	invalidate();
}

/*============================================================================*/
///
/// @brief 		Remove a message box to our system.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
void Cworld::unregisterMessageBox(Cdialog *child)
{
	m_lock.lock();
	m_message_box.removeDialog( child);
	m_lock.unlock();
	invalidate();
}

void Cworld::invalidateAll()
{
	m_invalidate = true;
}

void Cworld::notifyInvalidate()
{
	m_invalidate = true;
}

/** Touch all dialogs irregularly for updates */
bool Cworld::onLoop()
{
	bool no_action =true;
	if ( Cgraphics::m_defaults.external_loop )
	{
		Cgraphics::m_defaults.external_loop();
	}

	for ( auto a : m_message_box)
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if (!t)
		{
			continue;
		}
		if (t->onLoop()==false)
		{
			// Close the dialog, not the layers below.
			if ( t->m_alive ==false || t->m_selfDestruct==true)
			{
				//Cdialog::onExecute  delete the message box  t->m_name
				invalidate();
				unregisterMessageBox(t);
				delete t;
				break;
			}
		}
	}
	for ( auto a : m_active_dialog->m_children )
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if (t==NULL)
		{
			continue;
		}
		if ( t->onLoop()==false)
		{
			m_active_dialog->stop(0);
			break;
		}
	}
	if ( m_active_dialog->isRunning() ==false)
	{
		return false;
	}
	EpollStatus status;
	int n=10;
	int cnt3=0;
	while (  m_active_dialog->isRunning() ==true
		  && (status=CdialogEvent::Instance()->pollEvent(m_active_dialog) )!=POLL_EMPTY)
	{
		if ( !--n) { paintAll(); n=10; }
		no_action =false;
		if ( m_active_dialog->isRunning() ==false)
		{
			break;
		}
		for ( auto a : m_message_box )
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if (!t)
			{
				continue;
			}
			if (t->onLoop()==false)
			{
				// Close the dialog, not the layers below.
				if ( t->m_alive ==false || t->m_selfDestruct==true)
				{
					//Cdialog::onExecute  delete the message box  t->m_name
					invalidate();
					unregisterMessageBox(t);
					delete t;
					break;
				}
			}
			cnt3++;
		}
		if ( status==POLL_TESTING)
		{
			break;
		}
	}
	return no_action;
}

void Cworld::onCleanup()
{
	m_lock.lock();
//	for ( CdialogBase *a : m_active_dialog->m_children)
//	{
//		Cdialog *t=dynamic_cast<Cdialog*>(a);
//		if (t) t->onCleanup();
//	}
//	m_active_dialog->onCleanup();
	m_lock.unlock();
}

Estatus Cworld::tryButton(keymode mod, keybutton sym)
{
	m_lock.lock();
	switch ( m_message_box.onButton(mod,sym) )
	{
	case DIALOG_EVENT_EXIT:
		m_active_dialog->stop(0);
		return DIALOG_EVENT_PROCESSED;
	case DIALOG_EVENT_GENERAL:
	case DIALOG_EVENT_OPEN:
		switch ( m_dialogs.onButton(mod,sym) )
		{
		case DIALOG_EVENT_EXIT:
			m_active_dialog->stop(0);
			return DIALOG_EVENT_PROCESSED;
		case DIALOG_EVENT_GENERAL:
		case DIALOG_EVENT_OPEN:
			break;
		case DIALOG_EVENT_PROCESSED:
			return DIALOG_EVENT_PROCESSED;
		}
		break;
	case DIALOG_EVENT_PROCESSED:
		return DIALOG_EVENT_PROCESSED;
	}
	switch ( m_active_dialog->onButton( mod,sym))
	{
	case DIALOG_EVENT_EXIT:
		m_active_dialog->stop(0);
		return DIALOG_EVENT_PROCESSED;
	case DIALOG_EVENT_GENERAL:
		return DIALOG_EVENT_GENERAL;
	case DIALOG_EVENT_OPEN:
		return DIALOG_EVENT_OPEN;
	case DIALOG_EVENT_PROCESSED:
		return DIALOG_EVENT_PROCESSED;
	}
	return DIALOG_EVENT_PROCESSED;
}

Cdialog *Cworld::findDialog( const Cpoint &p)
{
	Cdialog *retVal =NULL;
	m_lock.lock();
	if ( m_message_box.size()>0 )
	{
		dialogBaseIterator a = m_message_box.end();
		for (;;)
		{
			if ( a==m_message_box.begin())
			{
				break;
			}
			a--;
			Cdialog *t=dynamic_cast<Cdialog*>(*a);
			if ( t==NULL)
			{
				continue;
			}
			if (  t->m_visible ==false)
			{
				continue;
			}
			if ( t->getRect().inside( p))
			{
				retVal =t;
				break;
			}
			if (p.x==0 && p.y==0)
			{
				retVal =t;
				break;
			}
		}
	}
	m_lock.unlock();
	return retVal;
}
