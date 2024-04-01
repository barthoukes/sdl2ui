/*
 * my_thread.cpp
 *
 *  Created on: 5 jun. 2012
 *      Author: mensfort
 */

#include "my_thread.hpp"
#include "timeout.hpp"

#if __cplusplus <= 199711L
/** @brief Interrupt handler.
 *  @param ptr [in] SELF pointer for the instance.
 */
void *mythread_interrupt(void *ptr)
{
	CmyThread *irq = static_cast<CmyThread*> (ptr);
	if (irq != NULL)
	{
		irq->run();
	}
	return NULL;
}
#endif

/** Constructor */
CmyLock::CmyLock()
{
}

/** Lock the thread for other threads. */
void CmyLock::lock()
{
	m_mutex.lock();
}

/** Unlock the thread for other threads. */
void CmyLock::unlock()
{
	m_mutex.unlock();
}

/** Constructor new thread. */
CmyThread::CmyThread()
: CmyLock()
, m_running( false)
#if __cplusplus <= 199711L
, m_pThread( nullptr)
#else
#endif
, m_stopThread( false)
{
}

/** Start thread. */
void CmyThread::start()
{
	if (!m_running)
	{
    	m_running =true;
        m_stopThread =false;
#if __cplusplus <= 199711L
        m_pThread = std::make_shared<std::thread>(mythread_interrupt, this);
#else
        m_pThread = std::make_shared<std::thread>([this]{ run(); });
#endif
	}
}

void CmyThread::startWorking()
{
}

void CmyThread::stopWorking()
{
}

bool CmyThread::isStopping()
{
	lock();
	bool retVal = m_stopThread;
	unlock();
	return retVal;
}

bool CmyThread::isRunning()
{
	lock();
	bool retVal = m_running && !m_stopThread;
	unlock();
	return retVal;
}

void CmyThread::stopFromThread()
{
	lock();
	m_stopThread = true;
	unlock();
}

/** Thread function running. */
void CmyThread::run()
{
	startWorking();
	while (isRunning())
	{
		work();
	}
	stopWorking();
#if __cplusplus <= 199711L
	pthread_exit(0);
#endif
}

/** Function to override for a thread. */
void CmyThread::work()
{
	delay(100);
}

/** Destructor. */
CmyLock::~CmyLock()
{
}

void CmyThread::forceStop()
{
	// User can override to force a stop.
}

/** Stop thread. */
void CmyThread::stop()
{
	lock();
	if (m_pThread.get() && m_running)
	{
		m_stopThread = true;
		unlock();
		forceStop();
	    m_pThread->join();
	    lock();
	    m_pThread = nullptr;
	    m_running = false;
	}
    unlock();
}

/** Destructor. */
CmyThread::~CmyThread()
{
	stop();
}
