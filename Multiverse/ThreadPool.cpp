#include "MultiversePCH.h"
#include "ThreadPool.h"
#include "Multiverse.h"

mv::ThreadPool::ThreadPool(size_type thread_count)
	: _threads{}, _task_queue{}, _condition{}, _task_mutex{}, _shutdown{ false }
{
	this->_threads.reserve(thread_count);
	for (size_type i = 0; i < thread_count; ++i) {
		this->_threads.emplace_back([this]() { this->_task_loop(); });
	}
}


mv::ThreadPool::~ThreadPool()
{
	this->_task_mutex.lock();
	this->_shutdown = true;
	this->_task_mutex.unlock();
	_condition.notify_all();
	for (std::thread& thread : this->_threads) {
		thread.join();
	}
}

void mv::ThreadPool::_task_loop()
{
	while (true) {
		std::unique_lock<std::mutex> lock(this->_task_mutex);
		this->_condition.wait(lock,	[this]() { return this->_shutdown || !this->_task_queue.empty(); });
		if (this->_shutdown && !this->_task_queue.empty())
			return;
		std::function<void()> task = std::move(this->_task_queue.front());
		this->_task_queue.pop();
		lock.unlock();
		task();
	}
}
