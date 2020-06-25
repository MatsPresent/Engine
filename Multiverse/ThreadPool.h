#pragma once
#include "setup.h"

#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <queue>

namespace mv
{
	class Multiverse;

	class ThreadPool
	{
		friend Multiverse;

	private:
		std::vector<std::thread> _threads;
		std::queue<std::function<void()>> _task_queue;
		std::condition_variable _condition;
		std::mutex _task_mutex;
		bool _shutdown;

		ThreadPool(size_type thread_count);
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) noexcept = delete;

	public:
		~ThreadPool();

		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) noexcept = delete;

		template <typename F, typename... Args, typename R = typename std::invoke_result<F, Args...>::type>
		std::future<R> enqueue(F&& task, Args&&... args);

	private:
		void _task_loop();
	};
}

#include "ThreadPool.inl"
