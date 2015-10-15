
#include "../utl/data_t.h"
#include "../comm/communication_public.h"


#pragma once


___namespace2_begin(ox,ipc)


	struct ipc_sender_back
	{
		typedef ox::utl::cdata_t cdata_t;
		typedef ox::comm::communication_public::send_back_d send_back_d;

		send_back_d _m_send_back;
		void const* _m_response;

		void set(send_back_d const& send_back,void const* _response) {_m_send_back=send_back,_m_response=_response;}
		void operator()(char const* data) const {operator()(cdata_t(data,strlen(data)));}
		void operator()(char const* data,size_t size) const {operator()(cdata_t(data,size));}
		void operator()(ox::utl::cdata_t const& data) const
		{
			if (!_m_send_back.is_empty()) _m_send_back(_m_response,data);
		}
	};

___namespace2_end()