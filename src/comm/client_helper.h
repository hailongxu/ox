
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

#include "windows.h"
#include <string>
#include <map>
#include <set>
#include <list>
//#include <new.h>
//#define  WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include "../ox/nsab.h"
#include "../mos/atomic_number.h"
#include "../mos/win_queue_thread_with_timer.h"
#include "../mos/win_queue_multi_thread.h"
#include "iocp.h"
#include "client_file_handler.h"
#include "communication_const.h"
#include "communication_helper.h"
#include "communication_define.h"
#include "file_service_operation.h"


#pragma once


___namespace2_begin(ox,comm)


struct client_helper;
template <typename helper_tn>
struct communication_client;
template <typename communication>
struct client_group;
template <typename group_tn>
struct client_multi_group;

struct client_helper
{
	typedef communication_client<client_helper> client_t;
	typedef ox::mos::win_queue_thread thread_t;
	typedef ox::mos::win_queue_thread_with_timer dispatch_thread_t;
	typedef ox::mos::win_queue_multi_threads threads_t;
	typedef ox::mos::task_single<void> task_single;
	typedef ox::mos::thread_task_t thread_task_t;
	typedef dispatch_thread_t::task_timer_t timer_t;
	typedef file_handle_operation file_handle_operation;
	typedef client_connect_t client_connect_t;
	typedef server_accept_t server_accept_t;
	typedef client_connect_t::connection_info connection_info;
	typedef client_group<client_t> group_t;
	typedef client_multi_group<group_t> multi_group_t;
	typedef communication_helper::simple_allocator allocator_t;
	typedef communication_define::cdata_t cdata_t;
	typedef communication_define::reqid_t reqid_t;
	typedef communication_define::dispatchable_flag_t dispatchable_flag_t;
	typedef communication_define::stream_output_t stream_output_t;
	typedef communication_define::answer_needed_flag answer_needed_flag;
	typedef communication_define::response_flag_t response_flag_t;
	typedef communication_define::message_more_flag_t message_more_flag_t;
	typedef communication_define::event_before_write_flag_t event_before_write_flag_t;
	typedef communication_define::connection_struct connection_struct;
	typedef communication_define::connection_filter_d connection_filter_d;
	typedef communication_define::response_result_d response_result_d;
	typedef communication_define::response_struct response_struct;
	typedef communication_define::response_d response_d;
	typedef communication_define::on_error_d on_error_d;
	typedef communication_define::file_handle_t file_handle_t;
	typedef communication_define::send_d send_d;
	typedef communication_const constant;
	typedef constant::response_result_enum response_result_enum;
	
	typedef constant::sender_step_enum sender_step_enum;
	typedef constant::protocol_enum protocol_enum;
	typedef constant::file_dispatchable_enum file_dispatchable_enum;
	typedef constant::file_used_enum file_used_enum;
	typedef constant::file_life_enum file_life_enum;
	typedef constant::actionid_enum actionid_enum;

	typedef communication_public::post_task_d post_task_d;
	typedef communication_define::all_exited_d all_exited_d;
	typedef communication_define::string_t string_t;
	typedef communication_define::extra_destruct_d extra_destruct_d;
	typedef communication_define::sender_struct sender_struct;
	struct file_list_context;
	struct connection_pool_t;
	struct fileinfo_t;
	struct filedata_t;
	struct iodata_t;
	typedef delegate<void(iodata_t*)> system_complete_d;
	typedef delegate<void(fileinfo_t*,size_t)> file_error_d;
	typedef delegate<void(iodata_t*)> iodata_error_d;
	typedef delegate<void(client_t*,connection_struct const&)> connection_create_d;
	typedef delegate<bool(client_t*,connection_struct const&,connection_filter_d const&)> connection_created_successfully_d;
	typedef delegate<void(filedata_t*)> connection_usable_d;
	typedef delegate<void(filedata_t*)> dispatch_d;
	typedef delegate<void(filedata_t*,size_t)> stream_recv_d;

public:
	struct option_filemax
	{
		option_filemax()
		{
			line_max = 2;
			protocol = constant::__protocol_tcp;
		}
		option_filemax(size_t line_max)
		{
			this->line_max = line_max;
			protocol = constant::__protocol_tcp;
		}
		std::string name;
		size_t line_max;
		protocol_enum protocol;
		//response_d completed;
	};
	struct option_event_connection_created_successfully
	{
		connection_created_successfully_d on_connection_created_successfully;
	};

	struct iodata_base_t
	{
		typedef OVERLAPPED overlapped_t;
		overlapped_t _m_overlapped;
		size_t _m_errorid;
		size_t _m_bytes_transfered;
		size_t _m_bytes_hasbeen_transfered;
		actionid_enum _m_actionid;
		iodata_base_t()
		{
			construct(constant::__file_error);
		}
		size_t actionid() const
		{
			return _m_actionid;
		}
		static size_t exit_actionid()
		{
			return constant::__file_exit;
		}
		overlapped_t& overlapped()
		{
			return _m_overlapped;
		}
		void clear_overlapped()
		{
			memset(&_m_overlapped,0,sizeof(_m_overlapped));
		}
		void set_error_code(size_t error_code)
		{
			_m_errorid = error_code;
		}
		size_t error_code() const
		{
			return _m_errorid;
		}
		void set_bytes_transfered(size_t bytes_transfered)
		{
			_m_bytes_transfered = bytes_transfered;
			_m_bytes_hasbeen_transfered += _m_bytes_transfered;
		}
		void destroy_self()
		{
			switch (_m_actionid)
			{
			case constant::__file_read:
			case constant::__file_write:
			case constant::__file_error:
				static_cast<rw_iodata_t*>(this)->destroy_self(); break;
			case constant::__file_connect:
				static_cast<conn_iodata_t*>(this)->destroy_self(); break;
			case constant::__file_task:
				static_cast<task_iodata_t*>(this)->destroy_self(); break;
			default:
				assert(false && "iodata type is illegal");
			}
		}
		void construct (actionid_enum actionid)
		{
			memset(&_m_overlapped,0,sizeof(_m_overlapped));
			_m_errorid = 0;
			_m_bytes_transfered = 0;
			_m_bytes_hasbeen_transfered = 0;
			_m_actionid = actionid;
		}
	};
	struct iodata_t : iodata_base_t
	{
		typedef iodata_t self;
		typedef iodata_base_t base;
		size_t seq() const
		{
			return _m_seq;
		}
		size_t _m_seq;
		sender_step_enum _m_step;
		protocol_enum _m_protocol;
		size_t _m_file_number;
		file_handle_t _m_file_handle;
		std::string _m_name;
		filedata_t* _m_filedata; /// weak
		fileinfo_t* _m_fileinfo;
	//protected:
		static size_t seq_next()
		{
			static ox::mos::atomic_number<long> _static_seq(0);
			return ++_static_seq;
		}
	protected:
		void construct (actionid_enum actionid)
		{
			base::construct(actionid);
			_m_seq = 0;
			_m_step = constant::__step_prepare;
			_m_protocol = constant::__protocol_tcp;
			_m_file_number = 0;
			_m_file_handle = file_handle_operation::invalid_file_handle();
			_m_filedata = 0;
			_m_fileinfo = 0;
		}
	};
	struct rw_iodata_t : iodata_t
	{
		typedef rw_iodata_t self;
		typedef iodata_t base;
		typedef response_d response_d;
		self()
		{
			construct(constant::__file_read);
		}
		void destroy_self()
		{
			if (!_m_extra_destruct.is_empty())
				_m_extra_destruct(extra_data());
			assert(_m_fileinfo);
			_m_fileinfo->destroy(this);
		}
		/// for send
		void assign_data_for_send(char const* buf,size_t size)
		{
			_m_buffer.assign(buf,size);
			memset(_m_wsabuf,0,sizeof(_m_wsabuf));
			_m_bytes_willbe_transfered = size;
			_m_bytes_transfered = 0;
			_m_bytes_hasbeen_transfered = 0;
		}
		/// for send
		void assign_data_for_send(char const* szbuf)
		{
			assign_data_for_send(szbuf,strlen(szbuf));
		}
		void assign_data_for_send()
		{
			memset(_m_wsabuf,0,sizeof(_m_wsabuf));
			_m_bytes_willbe_transfered = _m_buffer.size();
			_m_bytes_transfered = 0;
			_m_bytes_hasbeen_transfered = 0;
		}
		/// set for recv
		void init_data_for_recv()
		{
			_m_bytes_hasbeen_transfered = 0;
			assign_data_for_recv();
		}
		void assign_data_for_recv()
		{
			_m_buffer.resize(_m_buffersize_for_sendrecving);
			_m_bytes_transfered = 0;
			memset(_m_wsabuf,0,sizeof(_m_wsabuf));
		}
		size_t buffer_size() const
		{
			return _m_buffer.size();
		}
		dispatchable_flag_t dispatchable()
		{
			return dispatchable_flag_t(_m_flag);
		}
		stream_output_t output()
		{
			return stream_output_t(_m_flag);
		}
		answer_needed_flag is_answer_needed()
		{
			return answer_needed_flag(_m_flag);
		}
		response_flag_t response_flag()
		{
			return response_flag_t(_m_flag);
		}
		message_more_flag_t message_more()
		{
			return message_more_flag_t(_m_flag);
		}
		event_before_write_flag_t event_before_write()
		{
			return event_before_write_flag_t(_m_flag);
		}
		
		size_t _m_bytes_willbe_transfered;
		response_result_enum _m_response_return;
		response_d _m_complete;
		system_complete_d _m_on_system_complete;
		response_result_d _m_result_notify;
		system_complete_d* _m_request_complete;
		char _m_wsabuf[file_handle_operation::__wsa_buf_size];
		unsigned short _m_buffersize_for_sendrecving;
		unsigned short _m_flag;
		std::string _m_buffer;
		void const* _m_outofband;
		extra_destruct_d _m_extra_destruct;
		size_t _m_extra_size;
		char* extra_data() const {return (char*)(this+1);}
	private:
		void construct(actionid_enum actionid)
		{
			base::construct(actionid);
			memset(&_m_wsabuf,0,sizeof(_m_wsabuf));
			_m_bytes_willbe_transfered = 0;
			_m_outofband = 0;
			_m_extra_size = 0;
			_m_buffersize_for_sendrecving = constant::__send_receive_buffer_size_default;
			_m_flag = 1; /// dispatched enabled
			_m_response_return = constant::__resp_unfinished;
			_m_outofband = 0;
			_m_request_complete = 0;
			_m_result_notify.assign(this,&self::response_notify);
		}
		void response_notify(constant::response_result_enum resp_return)
		{
			assert(_m_request_complete);
			_m_response_return = resp_return;
			(*_m_request_complete)(this);
		}
	};

	struct conn_iodata_t : iodata_t
	{
		typedef conn_iodata_t self;
		typedef iodata_t base;
		typedef response_d response_d;
		conn_iodata_t()
		{
			construct();
		}
		void destroy_self()
		{
			assert(_m_fileinfo);
			_m_fileinfo->destroy(this);
		}
		group_t* _m_group;
		file_dispatchable_enum _m_dispatchable;
		system_complete_d _m_on_system_complete;
	private:
		void construct()
		{
			base::construct(constant::__file_connect);
			_m_group = 0;
			_m_dispatchable = constant::__dispatchable;
		}
	};

	struct task_iodata_t : iodata_base_t
	{
		task_iodata_t()
		{
			iodata_base_t::construct(constant::__file_task);
			_m_task = 0;
			_m_allocator = 0;
		}
		void destroy_self()
		{
			assert(_m_allocator);
			_m_allocator->_delete(this);
		}
		ox::mos::thread_task_t* _m_task;
		allocator_t* _m_allocator;
	};



	typedef std::list<iodata_t*> request_list_t;
	typedef std::list<filedata_t*> file_list_t;
	typedef std::map<size_t,filedata_t> random_file_list_t;

	struct list_helper
	{
		static void find_iodata(request_list_t::iterator& pos,request_list_t& list,size_t seq)
		{
			pos = list.end();
			typedef request_list_t::iterator I;
			for (I i=list.begin();i!=list.end();++i)
			{
				iodata_t* iodata = *i;
				if (iodata->seq()==seq) pos=i;
			}
		}
		static iodata_t* find_iodata(request_list_t const& list,size_t seq)
		{
			request_list_t& reqlist = *const_cast<request_list_t*>(&list);
			request_list_t::iterator i;
			find_iodata(i,reqlist,seq);
			return i==reqlist.end()?0:*i;
		}
		static iodata_t* move_iodata(request_list_t& from,request_list_t& to_tail,iodata_t* iodata)
		{
			typedef request_list_t::iterator I;
			iodata_t* iod = remove_iodata(from,iodata->seq());
			assert(iod==iodata);
			to_tail.push_back(iodata);
			return iod;
		}
		static iodata_t* move_head_to_tail(request_list_t& head_gotten,request_list_t& tail_added)
		{
			if (head_gotten.empty())
				return 0;
			iodata_t* iodata = head_gotten.front();
			head_gotten.pop_front();
			tail_added.push_back(iodata);
			return iodata;
		}
		static void move_list_to_list_tail(request_list_t& list,request_list_t& tail_added)
		{
			typedef request_list_t::iterator I;
			tail_added.insert(tail_added.end(),list.begin(),list.end());
			list.clear();
		}
		static iodata_t* remove_iodata(request_list_t& reqlist,size_t seq)
		{
			request_list_t::iterator i;
			find_iodata(i,reqlist,seq);
			if (i==reqlist.end()) return 0;
			iodata_t* iodata = *i;
			reqlist.erase(i);
			return iodata;
		}
		static bool erase_iodata(request_list_t& reqlist,size_t seq)
		{
			iodata_t* iodata = remove_iodata(reqlist,seq);
			if (iodata) iodata->destroy_self();
			return iodata!=0;
		}

		static void move_file(file_list_t& from,file_list_t& to,filedata_t* fd)
		{
			typedef file_list_t::iterator I;
			if (!fd)
			{
				assert(fd);
				return;
			}
			for (I i=from.begin();i!=from.end();++i)
			{
				if (*i==fd)
				{
#ifdef _DEBUG
					/// we should not find the fd in idle file
					for (I j=to.begin();j!=to.end();++j)
					{
						if (*j==fd)
						{
							assert(false && "we should not find the fd in to-file, it is not the rule");
							break;
						}
					}
#endif
					from.erase(i);
					to.push_back(fd);
					return;
				}
			}
		}

		static void move_file(file_list_t& from,file_list_t& to,file_list_t::iterator ifrom)
		{
			filedata_t* fd = *ifrom;
			from.erase(ifrom);
			to.push_back(fd);
		}

		static filedata_t* move_file(file_list_t& from,file_list_t& to,size_t file_number)
		{
			typedef file_list_t::iterator I;
			for (I i=from.begin();i!=from.end();++i)
			{
				filedata_t* fd = *i;
				if (fd->_m_file_number==file_number)
				{
//#ifdef _DEBUG
//						/// we should not find the fd in idle file
//						for (I j=to.begin();j!=to.end();++j)
//						{
//							if (*j==fd)
//							{
//								assert(false && "we should not find the fd in to-file, it is not the rule");
//								break;
//							}
//						}
//#endif
					to.push_back(fd);
					from.erase(i);
					return fd;
				}
			}
			return 0;
		}
		static file_list_t::iterator find_fid(file_list_t& list,size_t file_number)
		{
			typedef file_list_t::iterator I;
			for (I i=list.begin();i!=list.end();++i)
			{
				if ((*i)->file_number()==file_number)
					return i;
			}
			return list.end();
		}

		static filedata_t* remove_filedata(file_list_t& list,size_t file_number)
		{
			typedef file_list_t::iterator I;
			I i = find_fid(list,file_number);
			if (i==list.end()) return 0;
			filedata_t* fd = *i;
			list.erase(i);
			return fd;
		}
		typedef delegate<void(filedata_t*)> filedata_action;
		typedef delegate<void(iodata_t*)> iodata_action;
		static void do_each(request_list_t& list,delegate<void(iodata_t*)> action)
		{
			if (action.is_empty()) return;
			typedef request_list_t::iterator I;
			for (I i=list.begin();i!=list.end();++i)
				action(*i);
		}
		static void do_each(file_list_t& list,delegate<void(filedata_t*)> action)
		{
			if (action.is_empty()) return;
			typedef file_list_t::iterator I;
			for (I i=list.begin();i!=list.end();++i)
				action(*i);
		}
		static void erase_all(request_list_t& list)
		{
			do_each(list,iodata_action(&destroy_iodata));
			list.clear();
		}
		static void destroy_iodata(iodata_t* iodata)
		{
			iodata->destroy_self();
		}
	};

	struct filedata_t
	{
		typedef filedata_t self;
		self()
		{
			file_handle = file_handle_operation::invalid_file_handle();
			fileinfo = 0;
			life = constant::__file_unknown;
			used = constant::__file_idle;
		}
		void destroy_self()
		{
			fileinfo->destroy(this);
		}
		bool is_connected() const
		{
			return file_handle!=file_handle_operation::invalid_file_handle();
		}

		size_t file_number() const
		{
			return _m_file_number;
			return size_t(file_handle);
		}

		/// memory
		template <typename t>
		t* make()
		{
			return fileinfo->make<t>();
		}
		template <typename t>
		t* make(char id)
		{
			return fileinfo->make<t>(id);
		}
		template <typename t>
		void destroy(t* p)
		{
			fileinfo->destroy(p);
		}
		void dispatch()
		{
			if (!_m_dispatch.is_empty())
				_m_dispatch(this);
		}

		fileinfo_t* fileinfo;
		file_handle_t file_handle;
		size_t _m_file_number;
		file_life_enum life;
		file_used_enum used;
		request_list_t _m_wait;
		dispatch_d _m_dispatch;
	};

	struct fileinfo_t
	{
		typedef fileinfo_t self;
		typedef request_list_t request_list_t;
		typedef file_list_t file_list_t;
		cdata_t name;
		group_t* group;
		size_t file_max_count;
		file_list_t file_creating;
		file_list_t file_suspend;
		file_list_t file_idle;
		file_list_t file_busy;
		/// request_list_t creating; ????
		request_list_t wait;
		request_list_t running;
		random_file_list_t allfilelist;
		connection_pool_t* connection_pool;
		
		static size_t file_number_next()
		{
			static ox::mos::atomic_number<long> _static_seq(0);
			return ++_static_seq;
		}

		fileinfo_t()
		{
			group = 0;
			file_max_count = 2;
			connection_pool = 0;
		}

		~fileinfo_t()
		{
			clear();
		}

		bool can_add_file() const
		{
			return file_idle.empty() && file_count()<this->file_max_count;
		}

		size_t file_count() const
		{
			return file_creating.size()+file_suspend.size()+file_idle.size()+file_busy.size();
		}

		bool is_null() const
		{
			return file_max_count==0 && wait.empty() && running.empty() && allfilelist.empty(); 
		}

		void clear()
		{
			typedef random_file_list_t::iterator I;
			for (I i=allfilelist.begin();i!=allfilelist.end();++i)
			{
				list_helper::erase_all(i->second._m_wait);
			}
			list_helper::erase_all(wait);
			list_helper::erase_all(running); /// the running should not be using anymore
		}

		template <typename close_tn>
		void close_all_file(close_tn close)
		{
			typedef random_file_list_t::iterator I;
			for (I i=allfilelist.begin();i!=allfilelist.end();++i)
			{
				filedata_t& fd = i->second;
				close(fd.file_handle);
			}
		}

		filedata_t* find_filedata(size_t file_number)
		{
			typedef random_file_list_t::iterator I;
			I i = allfilelist.find(file_number);
			if (i==allfilelist.end())
				return 0;
			return &i->second;
		}
		filedata_t* erase_filedata(size_t file_number)
		{
			typedef random_file_list_t::iterator I;
			I i = allfilelist.find(file_number);
			if (i==allfilelist.end())
				return 0;
			filedata_t* fd = &i->second;
			allfilelist.erase(i);
			return fd;
		}
		bool remove_filedata_from_busy(size_t file_number)
		{
			return list_helper::remove_filedata(file_busy,file_number);
		}
		bool remove_filedata_from_idle(size_t file_number)
		{
			return list_helper::remove_filedata(file_idle,file_number);
		}
		filedata_t* add_filedata(size_t file_number)
		{
			filedata_t* fd = find_filedata(file_number);
			if (!fd)
			{
				fd = &(allfilelist[file_number]=filedata_t());
				fd->_m_file_number = file_number;
				fd->fileinfo = this;
			}
			///file_creating.push_back(fd);
			return fd;
		}
		void add_idle(filedata_t* fd)
		{
			list_helper::move_file(file_creating,file_idle,fd);
		}
		void add_idle_from_suspend(size_t file_number)
		{
			filedata_t* fd = list_helper::move_file(file_suspend,file_idle,file_number);
		}
		void add_suspend(filedata_t* fd)
		{
			list_helper::move_file(file_creating,file_suspend,fd);
		}
		void return_filedata(filedata_t* fd)
		{
			list_helper::move_file(file_busy,file_idle,fd);
		}
		void move_suspend_to_idle(filedata_t* fd)
		{
			list_helper::move_file(file_suspend,file_idle,fd);
		}
		void move_idle_to_busy(filedata_t* fd)
		{
			list_helper::move_file(file_idle,file_busy,fd);
		}
		void push_to_busy(filedata_t* fd)
		{
			assert(fd);
			file_list_t& busylist = file_busy;
			busylist.push_back(fd);
		}
		bool remove_file_from_creating(filedata_t* fd)
		{
			file_list_t& filelist = file_creating;
			filedata_t* fd2 = list_helper::remove_filedata(filelist,fd->_m_file_number);
			return fd2!=0;
		}
		bool remove_file_from_suspend(filedata_t* fd)
		{
			file_list_t& filelist = file_suspend;
			filedata_t* fd2 = list_helper::remove_filedata(filelist,fd->_m_file_number);
			return fd2!=0;
		}
		iodata_t* add_iodata(iodata_t* iodata)
		{
			if (iodata->_m_file_number==0)
				return add_iodata_normal(iodata);
			return add_iodata_direct(iodata);
		}
		iodata_t* add_iodata_normal(iodata_t* iodata)
		{
			assert(iodata);
			assert(iodata->_m_fileinfo==this);
			assert(iodata->_m_filedata==0);
			iodata->_m_fileinfo = this;
			wait.push_back(iodata);
			assert(wait.back()==iodata);
			return iodata;
		}
		iodata_t* add_iodata_direct(iodata_t* iodata)
		{
			assert(iodata);
			assert(iodata->_m_fileinfo==this);
			
			filedata_t* fd = find_filedata(iodata->_m_file_number);
			if (!fd)
			{
				iodata->_m_errorid = constant::__error_invalid_file_number;
				return 0;
			}
			iodata->_m_filedata = fd;
			fd->_m_wait.push_back(iodata);
			return iodata;
		}
		void add_iodata_into_running(iodata_t* iodata)
		{
			assert(iodata);
			assert(iodata->_m_fileinfo==this);
			assert(iodata->_m_filedata);
			iodata->_m_fileinfo = this;
			running.push_back(iodata);
			assert(running.back()==iodata);
		}
		bool erase_iodata_from_runing(iodata_t* iodata)
		{
			if (erase_iodata_from_runing(iodata->seq()))
				return true;
			iodata->destroy_self();
			return false;
		}
		bool erase_iodata_from_wait(size_t seq)
		{
			return list_helper::erase_iodata(wait,seq);
		}
		bool erase_iodata_from_runing(size_t seq)
		{
			return list_helper::erase_iodata(running,seq);
		}
		void move_wait_to_running_tail()
		{
			list_helper::move_list_to_list_tail(wait,running);
		}
		void move_list_to_running(request_list_t& reqlist)
		{
			list_helper::move_list_to_list_tail(reqlist,running);
		}
		iodata_t* move_wait_head_to_runing_tail()
		{
			return list_helper::move_head_to_tail(wait,running);
		}
		iodata_t* find_iodata_from_running(size_t seq) const
		{
			return list_helper::find_iodata(running,seq);
		}
		
		/// memory
		template <typename t>
		t* make()
		{
			return connection_pool->make<t>();
		}
		template <typename t>
		t* make(char id)
		{
			return connection_pool->make<t>(id);
		}
		template <typename t>
		void destroy(t* p)
		{
			connection_pool->destroy(p);
		}
	}; /// end of file info

	struct connection_pool_t
	{
	private:
		typedef std::map<std::string,fileinfo_t*> map_pool_t;
		typedef delegate<group_t*(char const*)> get_group_d;
	public:
		struct dot
		{
			typedef map_pool_t::iterator I;
			typedef map_pool_t::reference reference;
			dot(map_pool_t* p)
			{
				pool = p;
				if (pool) i = pool->begin();
			}
			bool is_empty() const
			{
				return pool==0 || i==pool->end();
			}
			I i;
			map_pool_t* pool;
			fileinfo_t* operator()()
			{
				if (pool->end()!=i)
					return i->second;
				return 0;
			}
			reference value()
			{
				return *i;
			}
			fileinfo_t* next()
			{
				assert(pool);
				if (i==pool->end())
					return 0;
				++i;
				if (i==pool->end())
					return 0;
				return i->second;
			}
		};
		struct make_line_number
		{
			make_line_number()
			{
				_m_file_number = 0;
			}
			size_t operator()()
			{
				return ++_m_file_number;
			}
			size_t _m_file_number;
		};
		connection_pool_t(allocator_t& allocator)
			: _m_allocator(allocator)
		{}
		~connection_pool_t()
		{
			clear();
		}
		fileinfo_t* find(std::string const& name)
		{
			typedef map_pool_t::iterator I;
			I i = _m_pool.find(name);
			if (i!=_m_pool.end())
				return i->second;
			return 0;
		}
		fileinfo_t* find(cdata_t const& name)
		{
			return find(ox::utl::to_string(name));
		}
		fileinfo_t* add(std::string const& name,size_t& error_code,size_t file_max)
		{
			typedef std::pair<fileinfo_t*,size_t> result_t;
			typedef map_pool_t::iterator I;
			error_code = 0;
			std::string const& key = name;
			I i = _m_pool.find(key);
			if (i!=_m_pool.end())
			{
				fileinfo_t* fi = i->second;
				if (file_max!=-1 && fi->file_max_count!=file_max) fi->file_max_count=file_max;
				return fi;
			}
			fileinfo_t* fi = make<fileinfo_t>();
			if (!fi)
			{
				assert(false && "new fileinfo is error");
				error_code = constant::__error_no_enough_memory;
				return 0;
			}
			fi->connection_pool = this;
			fi->group = _m_get_group(name.c_str());
			if (!fi->group)
			{
				assert(false && "can not find group name");
				destroy(fi);
				error_code = constant::__error_invalid_group_name;
				return 0;
			}
			typedef map_pool_t::value_type value_type;
			std::pair<I,bool> r = _m_pool.insert(value_type(key,fi));
			if (!r.second)
			{
				destroy(fi);
				error_code = constant::__error_map_insert_error;
				return 0;
			}
			i = r.first; /// get the insert position
			fi->name = ox::utl::to_data(i->first);
			fi->file_max_count = file_max==-1?2:file_max;
			return fi;
		}
		void erase(std::string const& name)
		{
			typedef map_pool_t::iterator I;
			I i = _m_pool.find(name);
			if (i==_m_pool.end()) return;
			fileinfo_t* fi = i->second;
			_m_allocator._delete(fi);
			_m_pool.erase(name);
		}
		dot begin()
		{
			return dot(&_m_pool);
		}

		void clear()
		{
			typedef map_pool_t::iterator I;
			for (I i=_m_pool.begin();i!=_m_pool.end();++i)
				destroy(i->second);
			_m_pool.clear();
		}

		/// memory
		template <typename t>
		t* make()
		{
			t* p = make1((t*)(0));
			return p;
		}
		template <typename t>
		t* make(char actionid,size_t extra_size=0)
		{
			assert(actionid==constant::__file_read || actionid==constant::__file_write);
			t* p = make2((t*)(0),actionid_enum(actionid),extra_size);
			return p;
		}
		conn_iodata_t* make1(conn_iodata_t*)
		{
			conn_iodata_t* obj = _m_allocator._new<conn_iodata_t>();
			if (obj)
			{
				obj->_m_seq = iodata_t::seq_next();
				obj->_m_actionid = constant::__file_connect;
			}
			return obj;
		}
		rw_iodata_t* make2(rw_iodata_t*,actionid_enum id,size_t extra_size)
		{
			rw_iodata_t* obj = (rw_iodata_t*)(_m_allocator._new(sizeof(rw_iodata_t)+extra_size));
			if (obj)
			{
				new (obj) rw_iodata_t;//_m_allocator._new<rw_iodata_t>();
				obj->_m_seq = iodata_t::seq_next();
				obj->_m_actionid = id;
				obj->_m_extra_size = extra_size;
			}
			return obj;
		}
		filedata_t* make1(filedata_t*)
		{
			filedata_t* obj = _m_allocator._new<filedata_t>();
			return obj;
		}
		fileinfo_t* make1(fileinfo_t*)
		{
			fileinfo_t* obj = _m_allocator._new<fileinfo_t>();
			return obj;
		}
		template <typename t>
		void destroy(t* p)
		{
			_m_allocator._delete(p);
		}
		get_group_d& get_group()
		{
			return _m_get_group;
		}
	private:
		map_pool_t _m_pool;
		make_line_number _m_file_number_maker;
		allocator_t& _m_allocator;
		get_group_d _m_get_group;
	};

	struct file_handler_trait
	{
		typedef thread_t thread_t;
		typedef task_single task_single;
		typedef constant constant;
		typedef file_handle_operation file_handle_operation;
		typedef fileinfo_t fileinfo_t;
		typedef filedata_t filedata_t;
		typedef iodata_t iodata_t;
		typedef rw_iodata_t rw_iodata_t;
		typedef conn_iodata_t conn_iodata_t;
		typedef request_list_t request_list_t;
		typedef system_complete_d system_complete_d;
		typedef on_error_d on_error_d;
		typedef file_error_d file_error_d;
		typedef iodata_error_d iodata_error_d;
		typedef response_struct response_struct;
		typedef response_result_enum response_result_enum;
		typedef post_task_d post_task_d;
		typedef stream_recv_d stream_recv_d;
	};
	struct iocp_trait
	{
		typedef iodata_base_t iodata_base_t;
		typedef task_iodata_t task_iodata_t;
		typedef iodata_t iodata_t;
		typedef filedata_t filedata_t;
	};
	typedef iocp_tt<iocp_trait> iocp_t;
	typedef client_file_handler<file_handler_trait> file_event_handler_t;
}; /// end of namespace client-helper

template <typename communication>
struct client_connection
{
	typedef communication_define::reqid_t reqid_t;
	typedef client_helper::sender_struct sender_struct;

	client_connection(size_t errorid = 0)
		: _m_communication(0)
		, _m_errorid(errorid)
		, _m_file_number(0)
	{}
	client_connection(std::string const& name, size_t file_number, communication* comm)
		: _m_communication(comm)
		, _m_name(name)
		, _m_file_number(file_number)
		, _m_errorid(0)
	{}
	bool is_null() const
	{
		return _m_communication == 0;
	}
	size_t file_number() const
	{
		return _m_file_number;
	}
	std::string const& name() const
	{
		return _m_name;
	}
	reqid_t send(sender_struct& sdata)
	{
		assert(_m_communication);
		strcpy(sdata._m_name,_m_name.c_str());
		sdata.file_number = _m_file_number;
		return _m_communication->send(sdata);
	}
	void close()
	{
		assert(_m_communication);
		_m_communication->close_file(_m_file_number,_m_name);
	}
private:
	std::string _m_name;
	size_t _m_file_number;
	communication* _m_communication;
	size_t _m_errorid;
};


template <typename communication>
struct client_group
{
	static size_t const __send_output_max = 1;
	static size_t const __recv_output_max = 4;
	typedef client_group self;
	typedef communication_define::cdata_t cdata_t;
	typedef communication_define::reqid_t reqid_t;
	typedef communication_define::stream_output_t stream_output_t;
	typedef communication_define::message_more_flag_t message_more_flag_t;
	typedef communication_define::send_d send_d;
	typedef communication_define::send_back_d send_back_d;
	typedef client_helper::sender_struct sender_struct;
	typedef client_helper::connection_usable_d connection_usable_d;
	typedef client_helper::connection_created_successfully_d connection_created_successfully_d;
	typedef client_helper::system_complete_d system_complete_d;
	typedef client_helper::response_d response_d;
	typedef client_helper::rw_iodata_t rw_iodata_t;

	self() :_m_communication(0)
	{
		_m_flag = 0; /// "0" is the first output number
		memset(_m_recv_param,0,sizeof(_m_recv_param));
		memset(_m_send_param,0,sizeof(_m_send_param));
		//_m_send.assign(this,&self::send);
		//_m_send_data.assign(this,&self::send_data);
	}
	//self(char const* name)
	//{
	//	_m_name = name;
	//}
	//reqid_t send(sender_struct& ss)
	//{
	//	return _m_communication->send(ss,this);
	//}
	//void close(); /// close all the connections started with name

	void send(communication_define::sender_struct const& send_struct)
	{
		_m_communication->send(send_struct);

		//sender_struct* ss = get_send(0);
		//ss = _m_communication->copy_sender_struct(*ss);
		//assert (ss);
		//ss->file_number = file_number;
		//ss->data = data;
		////ss->name().set(_m_name.c_str());
		//_m_communication->send(*ss);
	}

	std::string _m_name;
	communication* _m_communication;
	connection_usable_d on_connection_usable;
	sender_struct* _m_recv_param[__recv_output_max];
	sender_struct* _m_send_param[__send_output_max];
	system_complete_d on_write_complete_for_independence;
	system_complete_d on_write_complete;
	system_complete_d on_write_for_bottom_half;
	system_complete_d on_read_for_bottom_half;
	unsigned short _m_flag;
	send_back_d _m_send_back;

	send_back_d& send_back() {return _m_send_back;}

	stream_output_t output_first()
	{
		return stream_output_t(_m_flag);
	}
	message_more_flag_t message_more()
	{
		return message_more_flag_t(_m_flag);
	}
	void set_recv(sender_struct* ss,size_t index)
	{
		assert (index<__recv_output_max);
		if (index>=__recv_output_max) return;
		_m_recv_param[index] = ss;
	}
	sender_struct* get_recv(size_t index)
	{
		assert (index<__recv_output_max);
		if (index>=__recv_output_max) return 0;
		return _m_recv_param[index];
	}
	void set_send(sender_struct* ss,size_t index)
	{
		assert (index<__send_output_max);
		if (index>=__send_output_max) return;
		_m_send_param[index] = ss;
	}
	sender_struct* get_send(size_t index)
	{
		assert (index<__send_output_max);
		if (index>=__send_output_max) return 0;
		return _m_send_param[index];
	}
	template <typename deleator>
	void delete_params(deleator& del)
	{
		for (size_t i=0;i<__send_output_max;++i)
		{
			sender_struct* ss = _m_send_param[i];
			_m_send_param[i] = 0;
			if (ss==0) continue;
			if (!ss->destruct_sender.is_empty())
				ss->destruct_sender(ss);
			del._delete((char*)(ss));
		}
		for (size_t i=0;i<__recv_output_max;++i)
		{
			sender_struct* ss = _m_recv_param[i];
			_m_recv_param[i] = 0;
			if (ss==0) continue;
			if (!ss->destruct_sender.is_empty())
				ss->destruct_sender(ss);
			del._delete((char*)(ss));
		}
	}
	//response_d const& on_response()
	//{
	//	assert(_m_recv_param);
	//	return _m_recv_param->completed;
	//}
	//connection_return_d on_connection_return;
};

template <typename group_tn>
struct client_multi_group
{
	typedef group_tn group_t;
	typedef std::map<std::string,group_t*> groups_t;
	typedef client_helper::allocator_t allocator_t;


	client_multi_group(allocator_t& allocator)
		: _m_allocator(allocator)
	{}

	~client_multi_group()
	{
		clear();
	}

	group_t* add(std::string const& name)
	{
		typedef groups_t::iterator I;
		I i = _m_groups.find(name);
		if (i!=_m_groups.end())
			return i->second;
		group_t* g = _m_allocator._new<group_t>();
		if (!g) return 0;
		_m_groups[name] = g;
		return g;
	}

	group_t* find(std::string const& name)
	{
		typedef groups_t::iterator I;
		group_t* group = 0;
		I i = _m_groups.find(name);
		if (i==_m_groups.end())
			return 0;
		return i->second;
	}
	group_t* find(char const* name,size_t len)
	{
		typedef groups_t::iterator I;
		group_t* group = 0;
		I i = _m_groups.find(std::string(name,len));
		if (i==_m_groups.end())
			return 0;
		return i->second;
	}
	//void erase(char const* name)
	//{
	//	group_t* group = find(name);
	//	if (!group)
	//	{
	//		assert(false && "not find group name");
	//		return;
	//	}
	//	group->
	//	_m_groups.erase(name);
	//}
	void clear()
	{
		typedef groups_t::iterator I;
		for (I i=_m_groups.begin();i!=_m_groups.end();++i)
		{
			group_t* g = i->second;
			if (!g) continue;
			g->delete_params(_m_allocator);
			_m_allocator._delete(g);
		}
		_m_groups.clear();
	}
	groups_t _m_groups;
	allocator_t& _m_allocator;
};


___namespace2_end()