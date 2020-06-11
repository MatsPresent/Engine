#pragma once
#include "ThreadPool.h"

template <typename F, typename... Args, typename R>
inline std::future<R> mv::ThreadPool::enqueue(F&& task, Args&&... args)
{
	std::packaged_task<R()>* pt = new std::packaged_task<R()>(std::bind(std::forward<F>(task), std::forward<Args>(args)...));
	std::future<R> fut(pt->get_future());
	auto callback = [pt]() {
		pt->operator()();
		delete pt;
	};

	this->_task_mutex.lock();
	this->_task_queue.push(callback);
	this->_task_mutex.unlock();
	this->_condition.notify_one();
	return fut;
}
