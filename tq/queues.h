#pragma once

#include "queue.h"

namespace tq
{
	struct default_decorator
	{
		void operator ()(queue::task &task_) const
		try
		{
			task_();
		}
		catch (...)
		{
		}
	};
}

extern "C" void tq_create_passive(tq::passive_queue::ptr &queue, const tq::clock &clock,
	const tq::queue::task_decorator &decorator = tq::default_decorator());

extern "C" void tq_create_interactive(tq::queue::ptr &queue,
	const tq::queue::task_decorator &decorator = tq::default_decorator());

extern "C" void tq_create_active(tq::active_queue::ptr &queue, const tq::clock &clock,
	const tq::queue::task_decorator &decorator = tq::default_decorator());
