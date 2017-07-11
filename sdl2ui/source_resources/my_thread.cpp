/*
 * my_thread.cpp
 *
 *  Created on: 5 jun. 2012
 *      Author: mensfort
 */

#include <pthread.h>
#include "my_thread.h"
#include "timeout.h"

/** @brief Interrupt handler.
 *  @param ptr [in] SELF pointer for the instance.
 */
void *mythread_interrupt(void *ptr)
{
	CmyThread *irq =
			static_cast<CmyThread*> (ptr);
	if (irq != NULL)
	{
		irq->run();
	}
	return NULL;
}

/** Constructor */
CmyLock::CmyLock()
{
    (void) pthread_mutexattr_init( &m_attr);
	pthread_mutexattr_settype( &m_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init( &m_mutex, &m_attr);
}

/** Lock the thread for other threads. */
void CmyLock::lock()
{
	pthread_mutex_lock( &m_mutex);
}

/** Unlock the thread for other threads. */
void CmyLock::unlock()
{
	pthread_mutex_unlock( &m_mutex);
}

/** Constructor new thread. */
CmyThread::CmyThread()
: m_running( false)
, m_thread( 0)
, m_stopThread( false)
{
}

/** Start thread. */
void CmyThread::start()
{
	m_running =true;
	m_stopThread =false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int stack_size =8192*1024;
	pthread_attr_setstacksize(&attr, stack_size);
	pthread_create(&m_thread, &attr, mythread_interrupt, (void*) this);
}

/** Thread function running. */
void CmyThread::run()
{
	startWorking();
	while (m_running && m_stopThread==false)
	{
		work();
	}
	m_running =false;
	stopWorking();
	pthread_exit(0);
}

/** Function to override for a thread. */
void CmyThread::work()
{
	delay(100);
}

/** Destructor. */
CmyLock::~CmyLock()
{
	pthread_mutex_destroy(&m_mutex);
}

/** Stop thread. */
void CmyThread::stop()
{
	if (m_thread)
	{
		m_stopThread = true;
		pthread_join( m_thread, NULL);
		m_thread =(pthread_t)NULL;
	}
}

/** Destructor. */
CmyThread::~CmyThread()
{
	stop();
}
