
#include "../../ox/mos/task_decl.h"
#include "../../ox/cxx/delegate.h"
#include "../../ox/comm/communication_public.h"


#pragma once


___namespace2_begin(ox,ipc)


/// should be init at first
struct post_task_proxy
{
	typedef ox::comm::communication_public::post_task_d post_task_d;
	enum enum_returnid
	{
		__callee_thread = -1,
		__call_null = 0,
		__caller_thread = 1,
		__called_thread = 2,
	};
	struct item_t
	{
		size_t id;
		post_task_d post_task;
	};
	std::vector<item_t> _m_post_task_vector;

	static post_task_proxy& instance()
	{
		static post_task_proxy _static_post_task_proxy;
		return _static_post_task_proxy;
	}
	/// 0: not called, -1:callee thread, 1:caller thread, 2:called thread
	size_t post_task(size_t pid,ox::mos::thread_task_t* task,size_t threadid=-1)
	{
		if (threadid==-1)
		{
			task->run();
			return __callee_thread;
		}
		std::vector<item_t>::iterator i = find(pid);
		if (i==_m_post_task_vector.end())
			return __call_null;
		post_task_d& post = i->post_task;
		if (post.is_empty())
			return __call_null;
		post(task,threadid);
		return __called_thread;
	}
	void update(post_task_d const& post_task,size_t id)
	{
		std::vector<item_t>::iterator i = find(id);
		if (i!=_m_post_task_vector.end())
		{
			i->post_task = post_task;
			return;
		}
		item_t item = {id,post_task};
		_m_post_task_vector.push_back(item);
	}
	std::vector<item_t>::iterator find(size_t id)
	{
		std::vector<item_t>::iterator i=_m_post_task_vector.begin();
		for (;i!=_m_post_task_vector.end();++i)
			if (i->id==id) return i;
		return _m_post_task_vector.end();
	}
	post_task_d find_post_task(size_t id)
	{
		std::vector<item_t>::iterator i = find(id);
		if (i!=_m_post_task_vector.end())
			return i->post_task;
		return post_task_d();
	}
};


___namespace2_end()
