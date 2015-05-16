

#include <string>
#include "communication_client_interface.h"
#include "communication_define.h"
#include "http_define.h"
#include "http_helper.h"
#include "winop.h"
#include "task_feedback.h"
#include "win_queue_thread_user.h"
#include "http_memory_define.h"



#pragma once



___namespace2_begin(ox,comm)


struct communication_client_side
{
	typedef ox::utl::cdata_t cdata_t;
	typedef abc::communication_client_interface service_t;
	typedef abc::http_helper::reqid_t reqid_t;
	typedef delegate<service_t*()> create_service_d;
	typedef abc::communication_define::constant constant;
	typedef http_helper::http_event_d http_event_d;
	typedef service_t* (__stdcall create_instance_fun)(void);
	typedef communication_public::post_task_d post_task_d;
	typedef abc::task_single<int> task_single;

	typedef win_queue_thread_dll::thread_interface thread_interface;



	communication_client_side() : _m_create_instance(0)
	{}

	bool load_dll(wchar_t const* communication_dll_path)
	{
		return _m_thread_dll.load_dll(communication_dll_path);
	}

	void init()
	{
		_m_create_instance = _m_thread_dll._m_dll.function<create_instance_fun>("create_communication_client");
		assert(_m_create_instance);
		_m_client = _m_create_instance();
		client().init();
	}
	void start()
	{
		_m_thread_default_event = _m_thread_dll.create();
		assert (_m_thread_default_event);
		_m_default_post_task.assign(_m_thread_default_event,&thread_interface::add2);
		_m_thread_default_event->start();
		client().start();
	}
	void stop()
	{
		client().stop();
	}
	reqid_t http_get_send_and_recv_all(char const* url,http_event_d const& http_event,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_get,url,http_event,cdata_t(),data_binded);
	}
	reqid_t http_post_send_and_recv_all(char const* url,http_event_d const& http_event,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_post,url,http_event,cdata_t(),data_binded);
	}
	reqid_t http_get_send_and_recv_all(char const* url,http_event_d const& http_event,cdata_t const& body,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_get,url,http_event,body,data_binded);
	}
	reqid_t http_post_send_and_recv_all(char const* url,http_event_d const& http_event,cdata_t const& body,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_post,url,http_event,body,data_binded);
	}
	reqid_t http_get_send_and_recv_all(char const* url,http_event_d const& http_event,std::string const& body,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_get,url,http_event,cdata_t(body.data(),body.size()),data_binded);
	}
	reqid_t http_post_send_and_recv_all(char const* url,http_event_d const& http_event,std::string const& body,void* data_binded=0)
	{
		return http_send_and_recv_all(http_define::__item_value_post,url,http_event,cdata_t(body.data(),body.size()),data_binded);
	}

	reqid_t http_send_and_recv_all(
		http_define::head_itemid_value_enum methodid,
		char const* url,
		http_event_d const& http_event,
		cdata_t body = cdata_t(),
		void* data_binded = 0)
	{
		abc::http_helper::request_format format;

		assert (methodid==http_define::__item_value_get || methodid==http_define::__item_value_post);
		if (methodid==http_define::__item_value_get)
			format.method().get_selected();
		else if (methodid==http_define::__item_value_post)
			format.method().post_selected();
		else
			assert(false);

		format.uri().set(url);
		format.httpver()._11_selected();
		format.on_http_feedback().assign(http_handle_all);
		//format.on_http_outer_feedback().assign(http_event);
		format.body().set(body.begin,body.size);
		format.set_data_binded(data_binded);
		reqid_t r = http_send_and_recv_all(format);
		return r;
	}

	reqid_t http_send_and_recv_all(http_helper::request_format const& format)
	{
		reqid_t r = http_send_and_recv_all(format.as_http_param());
		return r;
	}

	reqid_t http_send_and_recv_all(http_define::http_user_sender_struct const& request)
	{
		reqid_t r = client().http_send2(request);
		return r;
	}

	void set_default_post_task(post_task_d const& default_post_task)
	{
		_m_default_post_task = default_post_task;
	}

	static constant::response_result_enum http_handle_all(http_helper::response_struct resp)
	{
		if (resp.tcp->step != constant::__step_read)
			return constant::__resp_unfinished;
		//resp.buffer_append(resp.user_data,resp.user_data_size);
		
		bool user_data_is_all_received = resp.user_data_is_all_received;
		if (!user_data_is_all_received) return constant::__resp_unfinished;
		communication_define::response_struct tcp_response = *resp.tcp;
		return constant::__resp_finished_and_close;
	}
private:
	static void do_http_outer_event(http_event_d event,http_helper::response_struct resp)
	{
		if (!event.is_empty()) event(resp);
	}
	static void do_http_outer_event2(http_event_d event,http_helper::response_struct resp,communication_define::response_struct const& tcp_resp)
	{
		resp.tcp = &tcp_resp;
		if (!event.is_empty()) event(resp);
	}

public:
	service_t& client() {return *_m_client;}

private:
	create_instance_fun* _m_create_instance;
	service_t* _m_client;
	post_task_d _m_default_post_task;
	win_queue_thread_dll _m_thread_dll;
	win_queue_thread_interface* _m_thread_default_event;
};


___namespace2_end()