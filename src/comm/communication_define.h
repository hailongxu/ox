
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */


#include <string.h>
#include <string>
#include <assert.h>
#include "../ox/nsab.h"
#include "../utl/data_t.h"
#include "../utl/data_helper.h"
#include "../cxx/delegate.h"
#include "../mos/task_decl.h"
#include "communication_const.h"
#include "communication_public.h"



#pragma once


___namespace2_begin(ox,comm)


struct communication_define
{
	typedef communication_const constant;
	struct connection_struct;
	struct sender_struct;
	struct response_struct;
	struct change_output_t;
	typedef ox::utl::data_t data_t;
	typedef ox::utl::cdata_t cdata_t;

	typedef constant::protocol_enum protocol_enum;
	typedef void* file_handle_t;
	typedef delegate<void(void*)> all_exited_d;
	typedef delegate<void(connection_struct const&,bool)> connection_filter_d;
	typedef delegate<void(constant::response_result_enum)> response_result_d;
	typedef delegate<constant::response_result_enum(response_struct)> response_d;
	typedef delegate<void(sender_struct const&)> send_d;
	typedef delegate<void(cdata_t,size_t)> on_error_d;
	typedef communication_public::send_back_d send_back_d;
	//typedef communication_public::post_task_d post_task_d;

	typedef delegate<bool()> is_data_null_d;
	typedef delegate<void(char*,size_t,char*)> extra_construct_d;
	typedef delegate<void(char*)> extra_destruct_d;
	typedef delegate<void(std::string&)> to_binary_d;
	typedef delegate<void(sender_struct*)> destruct_sender_d;


	template <typename t1,typename t2>
	struct pair
	{
		pair(t1 const& p1,t2 const& p2): first(p1),second(p2) {}
		t1 first;
		t2 second;
	};

	struct cstr_t
	{
		cstr_t() : begin(0), end(0) {}
		cstr_t(char const* b,char const* e) : begin(b), end(e) {}
		cstr_t(char const* b,size_t size) : begin(b), end(b+size) {}
		char const* begin;
		char const* end;
		void set(char const* _begin,char const* _end) {begin=_begin,end=_end;}
		void set(char const* _begin,size_t size) {begin=_begin,end=_begin+size;}
		int size() const {return end-begin;}
		bool is_empty() const {return !begin || !end || begin>=end;}
		void clear() {begin=0,end=0;}
	};

	struct reqid_t
	{
		reqid_t()
			: seq(0),errorid(0)
		{
			name[0] = 0;
		}
		char name[64];
		size_t seq;
		size_t errorid;
	};

	/// [bit0]
	struct dispatchable_flag_t
	{
		dispatchable_flag_t(unsigned short& flag)
			: _m_flag(flag)
		{}
		//void set(bool enabled)
		//{
		//	(_m_flag&=(~1)) | (unsigned short)(enabled);
		//}
		bool is_enabled() const
		{
			return _m_flag & 1;
		}
		void enabled()
		{
			_m_flag |= 1;
		}
		void disabled()
		{
			_m_flag &= (~1);
		}
		unsigned short& _m_flag;
	};

	/// [bit2,bit1]
	struct stream_output_t
	{
		stream_output_t(unsigned short const& flag)
			: _m_flag(*(unsigned short*)&flag)
		{}
		void set(unsigned short number)
		{
			assert(number>=0 && number<=3);
			_m_flag &= (~6);
			_m_flag |= ((number&3)<<1);
		}
		size_t value() const
		{
			return (_m_flag>>1)&3;
		}
		unsigned short& _m_flag;
	};

	/// [bit3]
	struct answer_needed_flag
	{
		answer_needed_flag(unsigned short const& flag)
			: _m_flag(*(unsigned short*)&flag)
		{}
		void set(bool is_needed)
		{
			_m_flag &= (~8);
			_m_flag |= ((is_needed?1:0)<<3);
		}
		bool value() const
		{
			return ((_m_flag>>3)&1)?true:false;
		}
		unsigned short& _m_flag;
	};

	/// [bit4]
	struct response_flag_t
	{
		response_flag_t(unsigned short const& flag)
			: _m_flag(*(unsigned short*)&flag)
		{}
		void prepare_event_enabled(bool enable)
		{
			_m_flag &= (~16);
			_m_flag |= ((enable?1:0)<<4);
		}
		bool is_prepare_event_enabled() const
		{
			return ((_m_flag>>4)&1)?true:false;
		}
		unsigned short& _m_flag;
	};

	/// [bit5]
	struct message_more_flag_t
	{
		message_more_flag_t(unsigned short const& flag)
			: _m_flag(*(unsigned short*)&flag)
		{}
		static size_t const off = 5;
		void set(bool is_more)
		{
			_m_flag &= (~(1<<off));
			_m_flag |= ((is_more?1:0)<<off);
		}
		bool is_message_more() const
		{
			return ((_m_flag>>off)&1)?true:false;
		}
		unsigned short& _m_flag;
	};

	/// [bit6]
	struct event_before_write_flag_t
	{
		event_before_write_flag_t(unsigned short const& flag)
			: _m_flag(*(unsigned short*)&flag)
		{}
		static size_t const off = 6;
		void set(bool is_more)
		{
			_m_flag &= (~(1<<off));
			_m_flag |= ((is_more?1:0)<<off);
		}
		bool is_enabled() const
		{
			return ((_m_flag>>off)&1)?true:false;
		}
		unsigned short& _m_flag;
	};

	struct connection_struct
	{
		cdata_t name;
		size_t file_number;
	};

	struct connection_id
	{
		std::string name;
		size_t file_number;
	};

	struct string_t
	{
		string_t(char* str, size_t size)
			: _m_string(str,size)
		{}
		void set(char const* str)
		{
			assert(!_m_string.is_empty());
			strncpy(_m_string.begin,str,_m_string.size);
		}
		void set(cdata_t const& str)
		{
			set(str,cdata_t());
		}
		template <typename t>
		void set(cdata_t const& str1, t const& o)
		{
			set(str1,cdata_t(&o,sizeof(o)));
		}
		void set(cdata_t const& str1,char const* str2)
		{
			set(str1,cdata_t(str2,str2?strlen(str2):0));
		}
		void set(cdata_t const& str1,cdata_t const& str2)
		{
			assert(!_m_string.is_empty());
			size_t size = str1.size+str2.size;
			if (size==0) return;
			if (size>=_m_string.size) size = _m_string.size;
			strncpy(_m_string.begin,str1.begin,str1.size);
			strncpy(_m_string.begin+str1.size,str2.begin,str2.size);
			_m_string.size = size;
		}
		char const* c_str() const
		{
			return _m_string.begin;
		}
		data_t _m_string;
	};

	struct sender_struct
	{
		typedef communication_define::data_t data_t;
		virtual ~sender_struct(){}
		sender_struct()
		{
			clear();
		}
		virtual void copy_construct(sender_struct& ss) const
		{
			ss = *this;
		}

		string_t name() {return string_t(_m_name,sizeof(_m_name)-1);}
		string_t name() const {return string_t((char*)_m_name,sizeof(_m_name)-1);}
		char _m_name[256];

		size_t file_number;
		protocol_enum protocol;
		cdata_t data;
		unsigned short buffer_size_for_sendrecving;
		unsigned short _m_flag;
		/// size_t timeout_interval;
		response_d completed;
		void const* outofband;
		destruct_sender_d destruct_sender;
		char* extra_param;
		size_t extra_size;
		extra_construct_d extra_construct;
		extra_destruct_d extra_destruct;
		to_binary_d to_binary;
		is_data_null_d is_data_null;

		dispatchable_flag_t dispatchable()
		{
			return dispatchable_flag_t(_m_flag);
		}
		answer_needed_flag is_answer_needed()
		{
			return answer_needed_flag(_m_flag);
		}
		response_flag_t response_flag()
		{
			return response_flag_t(_m_flag);
		}
		event_before_write_flag_t event_before_write_enabled()
		{
			return event_before_write_flag_t(_m_flag);
		}
		void do_to_binary(std::string& out)
		{
			out.assign(data.begin,data.size);
		}
		bool do_is_data_null() const
		{
			return data.is_empty();
		}
		void clear()
		{
			_m_name[0]=0;
			_m_name[sizeof(_m_name)-1]=0;
			file_number = 0; /// up to the system
			protocol = constant::__protocol_tcp;
			extra_param = 0;
			extra_size = 0;
			outofband = 0;
			buffer_size_for_sendrecving = constant::__send_receive_buffer_size_default;
			_m_flag = 8+1; /// need-answer,need dispatchable
			to_binary.assign(this,&sender_struct::do_to_binary);
			is_data_null.assign(this,&sender_struct::do_is_data_null);
		}
	};

	struct response_object
	{
		response_object(void const* _obj=0);
		void const* _m_object;
		size_t seq() const;
		constant::sender_step_enum step() const;
		int errorid() const;
		char const* name() const;
		size_t file_number() const;
		cdata_t data() const;
		void const* outofbind() const;
		void write_back() const;
		size_t extra_size() const;
	};

	struct response_struct
	{
		response_struct()
		{
			seq = 0;
			step = constant::__step_prepare;
			errorid = constant::__error_null;
			name = 0;
			file_number = 0;
			data = 0;
			size = 0;
			buffer_size = 0;
			size_hasbeen_transfered = 0;
			outofband = 0;
			_m_flag = 0;
			result_notify = 0;
		}
		size_t seq;
		constant::sender_step_enum step;
		int errorid;
		char const* name;
		size_t file_number;
		char const* data;
		size_t size;
		size_t buffer_size;
		size_t size_hasbeen_transfered;
		//size_t size_willbe_transfered;
		void const* outofband;
		unsigned short const* _m_flag;
		response_result_d const* result_notify;
		send_back_d* write_back;
		void change_output(unsigned short number) const
		{
			stream_output_t(*_m_flag).set(number);
		}
		void send_back(cdata_t data)
		{
			assert(write_back);
			(*write_back)(this,data);
		}
		bool is_message_more() const
		{
			return message_more_flag_t(*_m_flag).is_message_more();
		}
		response_object response;
	};

} ; ///end of communication_define


___namespace2_end()