#pragma once

#include <queue>
#include <tq/queue.h>

namespace tq
{
	namespace mocks
	{
		struct task
		{
			std::pair<timestamp /*at*/, unsigned int /*order*/> priority;
			tq::queue::task task_fn;
		};

		struct clock
		{
			std::shared_ptr<timestamp> time;

			bool operator ()() const;
		};

		class queue : public tq::queue
		{
		public:
			queue();

			size_t get_task_count() const;
			const tq::mocks::task *get_next() const;
			clock get_clock() const;

			bool process_one();
			void process_all();
			void process_existing();
			void process_advance_time(timespan by);

		private:
			virtual bool schedule(const queue::task &task_, timespan defer_by);

		private:
			unsigned int _order;
			std::shared_ptr<timestamp> _time;
			std::priority_queue<tq::mocks::task> _tasks;
		};



		inline bool operator <(const task &lhs, const task &rhs)
		{	return lhs.priority < rhs.priority;	}


		inline queue::queue()
			: _time(new timestamp(1))
		{	}

		inline size_t queue::get_task_count() const
		{	return _tasks.size();	}

		inline const tq::mocks::task *queue::get_next() const
		{	return !_tasks.empty() ? &_tasks.top() : 0;	}

		inline clock queue::get_clock() const
		{
			clock c = { _time };

			return c;
		}

		inline bool queue::process_one()
		{
			if (!_tasks.empty() && _tasks.top().priority.first < *_time)
			{
				tq::mocks::task t = _tasks.top();
				_tasks.pop();
				t.task_fn();
				return true;
			}
			return false;
		}

		void queue::process_all()
		{
			while (process_one())
			{	}
		}

		void queue::process_existing()
		{
			std::priority_queue<tq::mocks::task> tasks;

			tasks.swap(_tasks);
			if (!tasks.empty() && tasks.top().priority.first < *_time)
			{
				tq::mocks::task t = tasks.top();
				tasks.pop();
				t.task_fn();
			}
			while (!tasks.empty())
			{
				_tasks.push(tasks.top());
				tasks.pop();
			}
		}

		void queue::process_advance_time(timespan /*by*/)
		{
		}

		bool queue::schedule(const queue::task &task_, timespan defer_by)
		{
			tq::mocks::task t = {
				std::make_pair(defer_by ? *_time + defer_by : 0, _order++),
				task_
			};

			_tasks.push(t);
			return true;
		}
	}
}
