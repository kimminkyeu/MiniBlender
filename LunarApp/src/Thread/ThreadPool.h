//
// Created by Minkyeu Kim on 9/14/23.
//

#ifndef SCOOP_THREADPOOL_H
#define SCOOP_THREADPOOL_H

#include <functional> // https://modoocode.com/254
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

// NOTE: Referenced from Dear ImGUI Thread pool implementation
// (1) https://github.com/ZenSepiol/Dear-ImGui-App-Framework/tree/main/src/lib/thread_pool
// (2) https://www.youtube.com/watch?v=6re5U82KwbY --> explanation

// https://www.youtube.com/watch?v=meiGRnyRBXM&list=PLF02cl7EH4A6KM-gvdJcnb4HbwixVnS_6&index=18
// (3) https://github.com/valerioformato/RTIAW/blob/master/src/App/Renderer/Renderer.cpp

// NOTE: CppNuts C++ threading
// (4) https://www.youtube.com/watch?v=zUy66Bats5c
//     unique_lock, lock_guard 등 각 객체의 특징을 설명함.

// std::lock_guard
// std::unique_lock
// std::defer_lock

class ThreadPool
{
	friend class ThreadWorker;

public:
	int BusyThreads;

private:
	bool m_ShutdownRequested = false;

	mutable std::mutex m_Mutex; // mutable https://modoocode.com/253
	std::condition_variable m_ConditionVariable;
	std::vector<std::thread> m_Threads; // worker pool
	std::queue<std::function<void()>> m_TaskQueue; // queue of callable objects.

public:
	ThreadPool()
	{
		auto max = std::thread::hardware_concurrency();
		BusyThreads = (int)max;
		m_Threads.resize(max);
		for (size_t i = 0; i < max; ++i)
		{
			m_Threads[i] = std::thread(ThreadWorker(this));
		}
		LOG_INFO("ThreadPool : {0} threads created", max);
	}

	explicit ThreadPool(const int size)
			: BusyThreads(size), m_Threads(size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			m_Threads[i] = std::thread(ThreadWorker(this));
		}
		LOG_INFO("ThreadPool : {0} threads created", size);
	}

	~ThreadPool()
	{
		Shutdown();
	}

	ThreadPool(const ThreadPool&) = delete;

	ThreadPool(ThreadPool&&) = delete;

	ThreadPool& operator=(const ThreadPool&) = delete;

	ThreadPool& operator=(ThreadPool&&) = delete;

	// Waits until threads finish their current task and shutdowns the pool
	void Shutdown()
	{
		{
			// std::lock_gaurd 는 Scope를 벗어나면 자동으로 Unlock 하는 Wrapper이다.(명시적 unlock 불가능)
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_ShutdownRequested = true;
			m_ConditionVariable.notify_all(); // Unblocks all threads currently waiting for *this.
		}

		for (size_t i = 0; i < m_Threads.size(); ++i) {
			if (m_Threads[i].joinable()) {
				m_Threads[i].join();
			}
		}
	}

	// https://www.youtube.com/watch?v=6re5U82KwbY
	template<typename FunctionType, typename... Args>             // "args..." => unpack arguments
	auto AddTask(FunctionType&& f, Args&& ... args) -> std::future<decltype(f(args...))>
	{
		// Create a function with bounded parameters ready to execute
		auto func = std::bind(std::forward<FunctionType>(f), std::forward<Args>(args)...);

		// Encapsulate it into a shared_ptr in order to be able to copy construct / assign
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

		// Wrap the task pointer into a void lambda
		auto wrapper_func = [task_ptr]() { (*task_ptr)(); };

		{
			// std::lock_gaurd 는 Scope를 벗어나면 자동으로 Unlock 하는 Wrapper이다.(명시적 unlock 불가능)
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_TaskQueue.push(wrapper_func);
			// Wake up one thread if its waiting
			m_ConditionVariable.notify_one();
		}

		// Return future from promise
		return task_ptr->get_future();
	}

	// 각각에 lock을 거는 이유는, 각 thread에서 threadpool에 접근하여 taskQueueSize를 얻어갈 수 있기 때문
	size_t GetTaskQueueSize() const
	{
		// Automatically calls lock on mutex
		std::unique_lock<std::mutex> lock(m_Mutex);
		return m_TaskQueue.size();
	}

private:
	class ThreadWorker
	{
	private:
		ThreadPool* m_ThreadPoolPtr;

	public:
		ThreadWorker() = delete;

		explicit ThreadWorker(ThreadPool* pool)
				: m_ThreadPoolPtr(pool)
		{}

		void operator()()
		{
			std::unique_lock<std::mutex> lock(m_ThreadPoolPtr->m_Mutex);
			// Note: 아래 조건 주의할 것. TaskQueue가 빌 때 까지는 비우지 않는 방식임.
			while ( (!m_ThreadPoolPtr->m_ShutdownRequested)
					|| ( m_ThreadPoolPtr->m_ShutdownRequested && !m_ThreadPoolPtr->m_TaskQueue.empty()) )
			{
				m_ThreadPoolPtr->BusyThreads--;
				m_ThreadPoolPtr->m_ConditionVariable.wait(lock, [this]
				{
				  return (this->m_ThreadPoolPtr->m_ShutdownRequested || !this->m_ThreadPoolPtr->m_TaskQueue.empty());
				});

				m_ThreadPoolPtr->BusyThreads++;

				if (!this->m_ThreadPoolPtr->m_TaskQueue.empty())
				{
					auto func = m_ThreadPoolPtr->m_TaskQueue.front();
					m_ThreadPoolPtr->m_TaskQueue.pop();

					lock.unlock();
					func();
					lock.lock();
				}
			}
		}
	};
	// End of Thread Pool Class
};


#endif//SCOOP_THREADPOOL_H
