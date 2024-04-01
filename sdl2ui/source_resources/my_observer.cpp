/*
 * my_observer.cpp
 *
 *  Created on: 1 August 2022
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2022
 ** Houkes Horeca Applications
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include "my_observer.hpp"

/*----------------------------------------------------------------------------*/
ImyObserver::ImyObserver()
{
    m_subject = NULL;
}

/*----------------------------------------------------------------------------*/
ImyObserver::~ImyObserver()
{
	try
	{
		if (m_subject)
		{
			// new char[2]; // Create leak on purpose for tracking.
		}
	}
	catch(...)
	{}
}

/*----------------------------------------------------------------------------*/
void ImyObserver::unregisterMyObserver()
{
	m_subject->unregisterObserver(this);
	m_subject = NULL;
}

/*----------------------------------------------------------------------------*/
void ImyObserver::onMessage(int x)
{
	std::ignore = x;
    // Please override to observe and do something.
}

/*----------------------------------------------------------------------------*/
void ImyObserver::onMessage(int x, int y)
{
    // Please override to observe and do something.
	std::ignore = x;
	std::ignore = y;
}

/*----------------------------------------------------------------------------*/
void ImyObserver::setSubject(Csubject *notifier)
{
    m_subject = notifier;
}

/*----------------------------------------------------------------------------*/
Csubject::Csubject()
: count(0)
, xxx(0)
, yyy(0)
{}

/*----------------------------------------------------------------------------*/
Csubject::~Csubject()
{
	if (m_myObserverPtrList.size())
	{
		printf("Subjects locked\n");
		//throw std::runtime_error("Not all observers cleared!!");
	}
	m_myObserverPtrList.clear();
}


/*----------------------------------------------------------------------------*/
void Csubject::registerObserver( ImyObserver *observer, bool notifyFromHistory)
{
    bool found = false;

    lock();
    for (auto n : m_myObserverPtrList)
    {
        if (n == observer)
        {
            found = true;
            break;
        }
    }
    observer->setSubject(this);
    if (!found)
    {
        m_myObserverPtrList.push_back(observer);
        count++;
    }
    unlock();
    if (notifyFromHistory)
    {
        observer->onMessage(xxx);
    }
}

/*----------------------------------------------------------------------------*/
void Csubject::unregisterObserver( ImyObserver *observer)
{
    lock();

    int i=0;
    while (i<(int)m_myObserverPtrList.size())
    {
        if (m_myObserverPtrList[i] == observer)
        {
            m_myObserverPtrList.erase(m_myObserverPtrList.begin()+i);
            count--;
        }
        else
        {
            i++;
        }
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void Csubject::notify(int x)
{
    lock();
	xxx = x;
    for (auto i : m_myObserverPtrList)
    {
        i->onMessage(x);
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void Csubject::notify(int x, int y)
{
	lock();
    xxx = x;
    yyy = y;
    for (auto i : m_myObserverPtrList)
    {
        i->onMessage(x, y);
    }
    unlock();
}

/*----------------------------------------------------------------------------*/
void Csubject::clearObservers()
{
	m_myObserverPtrList.clear();
}
