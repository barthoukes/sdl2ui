/*
 * my_thread.h
 *
 *  Created on: 5 jun. 2012
 *      Author: mensfort
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
#include "i386-linux-gnu/bits/pthreadtypes.h"
#include "pthread.h"


// @brief Class to create a single lock.
class CmyLock
{
public:
	CmyLock();
	virtual ~CmyLock();
	void lock();
	void unlock();

private:
	pthread_mutex_t		m_mutex;
	pthread_mutexattr_t  m_attr; ///< Mutex attributes.
};

// @brief Class to create a single thread.
class CmyThread : public CmyLock
{
public:
	CmyThread();
	virtual ~CmyThread();
	virtual void startWorking() {}
	virtual void stopWorking() {}
	virtual void run();
	virtual void work();
	virtual void start();
	virtual void stop();
	bool isStopping() { return m_stopThread; }
	bool isRunning() { return m_running && !m_stopThread; }

private:
	bool 				m_running; 		///< Thread is now running.
	pthread_t  			m_thread;		///< Pointer to thread.
	bool				m_stopThread; 	///< Indicate to stop thread.
};

/* End MY_THREAD_H_ */
