

#include "win_queue_thread_with_timer.h"


#pragma once


namespace ox
{
	struct timer_service
	{
		typedef timer_service self;
		typedef win_queue_thread_with_timer timer_thread;
		typedef timer_thread::timer_event_d timer_event_d;
		typedef std::vector<std::string> users_t;

		static timer_service& instance()
		{
			static timer_service _static_timer_service;
			return _static_timer_service;
		}
		void start()
		{
			//_m_count += 1;
			_m_thread.start();
		}
		void stop()
		{
			_m_thread.add(task_single<int>::make(this,&self::do_stop));
		}
		void wait()
		{
			_m_thread.join();
		}
		size_t add_timer_by_milli_second(LONGLONG period_milli_seconds,timer_event_d const& timer_event,void* binded)
		{
			return _m_thread.add_timer_milli(period_milli_seconds,timer_event,binded);
		}
		void erase_timer(size_t timerid)
		{
			return _m_thread.erase_timer(timerid);
		}
		void add_user(char const* user_name)
		{
			_m_thread.add(ox::task_single<int>::make(this,&self::do_add_user,user_name));
		}
		void remove_user(char const* user_name)
		{
			_m_thread.add(ox::task_single<int>::make(this,&self::do_remove_user,user_name));
		}
	private:
		void do_add_user(std::string const& user_name)
		{
			_m_users.push_back(user_name);
		}
		void do_remove_user(std::string const& user_name)
		{
			users_t::iterator i=_m_users.begin();
			while (i!=_m_users.end())
			{
				std::string const& user = *i;
				if (0==user.compare(user_name)) break;
			}
			if (i==_m_users.end())
			{
				assert ("not found user");
				return;
			}
			_m_users.erase(i);
		}
		void do_stop()
		{
			//_m_count -= 1;
			//if (_m_count.value()>0) return;
			if (!_m_users.empty())
			{
				assert (false && "some one user the timer service");
				return;
			}
			_m_thread.stop_next();
		}
		timer_service(){}
		timer_thread _m_thread;
		users_t _m_users;
	};
}