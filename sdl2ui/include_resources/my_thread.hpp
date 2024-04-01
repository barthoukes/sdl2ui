/*
 * my_thread.h
 *
 *  Created on: 5 jun. 2012
 *      Author: mensfort
 */

#ifndef MY_THREAD_HPP_
#define MY_THREAD_HPP_

#include <atomic>
#include <mutex>
#include <thread>
#include <memory>

// @brief Class to create a single lock.
class CmyLock
{
public:
	CmyLock();
	virtual ~CmyLock();
	void lock();
	void unlock();

private:
	std::recursive_mutex  m_mutex;
};

// @brief Class to create a single thread.
class CmyThread : public CmyLock
{
public:
	CmyThread();
	virtual ~CmyThread();
	virtual void startWorking();
	virtual void stopWorking();
	virtual void run();
	virtual void work();
	virtual void start();
	virtual void stop();
	virtual bool isStopping();
	virtual bool isRunning();
	virtual void stopFromThread();
	virtual void forceStop();

private:
	bool                          m_running;    ///< Thread is now running.
	std::shared_ptr<std::thread>  m_pThread;     ///< Pointer to thread.
    bool                          m_stopThread; ///< Indicate to stop thread.
};

#endif /* MY_THREAD_HPP_ */
