#pragma once

#include <functional>
#include <memory>

namespace tq
{
	typedef long long timespan;
	typedef long long timestamp;
	typedef std::function<timestamp ()> clock;

	struct queue
	{
		typedef std::shared_ptr<queue> ptr;
		typedef std::function<void ()> task;
		typedef std::function<void (const task &task_)> task_decorator;

		virtual bool schedule(const task &task_, timespan defer_by = 0) = 0;
	};

	struct active_queue : queue
	{
		typedef std::shared_ptr<active_queue> ptr;

		virtual void run() = 0;
		virtual void stop() = 0;
	};

	struct passive_queue : queue
	{
		typedef std::shared_ptr<passive_queue> ptr;

		virtual void wait() = 0;
		virtual void executive_ready(timespan execution_cap) = 0;
	};
}
