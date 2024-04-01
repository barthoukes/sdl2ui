/*
 * my_observer.hpp
 *
 *  Created on: 1 August 2022
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2022
 ** Houkes Horeca Applications
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include <vector>
#include <memory>
#include "my_thread.hpp"

class Csubject;

class ImyObserver
{
public:
	ImyObserver();
    virtual ~ImyObserver();
    virtual void unregisterMyObserver();
    virtual void onMessage(int x);
    virtual void onMessage(int x, int y);
    virtual void setSubject(Csubject *notifier);

private:
    Csubject *m_subject;
};

typedef std::shared_ptr<ImyObserver> ImyObserverPtr;

class Csubject : public CmyLock
{
public:

    Csubject();
    virtual ~Csubject();
    void notify(int x);
    void notify(int x, int y);
    void registerObserver( ImyObserver *observer, bool notifyFromHistory);
    void unregisterObserver( ImyObserver *observer);
    int getNumber() { return xxx; }
    void clearObservers();

protected:
    int count;
    std::vector<ImyObserver*> m_myObserverPtrList;
    int xxx;
    int yyy;
};

typedef std::shared_ptr<Csubject> CsubjectPtr;
