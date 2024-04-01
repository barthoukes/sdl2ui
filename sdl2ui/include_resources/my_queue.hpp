/*============================================================================*/
/**  @file       my_queue.hpp
 **  @ingroup    queue for various purposes. With semaphore and re-entrant
 **  @brief		 Queue for messages
 **  @date       10 feb 2020
 **
 **  Create a message queue for the sql messages.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CsqlMsgQueue
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2020
 ** Houkes Horeca Applications
  **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <queue>
#include <memory>
#include <semaphore.h>
#include "my_thread.hpp"

/*------------- Forward declaractions ----------------------------------------*/

/*------------- Class declaractions ------------------------------------------*/
/// @brief Queue for events.
template<class T>
class CmyQueue : public CmyLock
{
public:
	CmyQueue()
	: m_size(0)
	{
		sem_init(&m_sem, 0, 0);
	}

    virtual ~CmyQueue()
    {
    	lock();
    	while (!m_eventList.empty())
    	{
    		pop();
    		m_size--;
    	}
    	unlock();
    }

	void push( T event)
	{
		lock();
		m_eventList.push(event);
		m_size++;
		sem_post(&m_sem);
		m_lock.unlock();
	}

	T front()
	{
		m_lock.lock();
		T evt =m_eventList.front();
		m_lock.unlock();
		return evt;
	}

	void pop()
	{
		m_lock.lock();
		m_eventList.pop();
		m_size--;
		m_lock.unlock();
	}

	int size()
	{
	    m_lock.lock();
		int retVal = m_size;
	    m_lock.unlock();
	    return retVal;
	}

	bool empty()
	{
	    m_lock.lock();
		bool retVal = !m_size;
	    m_lock.unlock();
	    return retVal;
	}

	void wait()
	{
		sem_wait(&m_sem);
	}

private:
	int m_size;
	std::queue<T> m_eventList; ///< All events in queue.
	CmyLock m_lock; ///< Lock for this queue.
	sem_t m_sem;
};
