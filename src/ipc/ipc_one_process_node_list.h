
#include "../utl/data_t.h"
#include "../utl/seplist.h"
#include "../mos/scope_raii.h"



#pragma once


namespace ox
{
namespace ipc
{

	struct ipc_one_process_node_list
	{
		typedef ox::utl::cdata_t cdata_t;
		typedef ox::utl::seplist<char> node_name_list;
		ipc_one_process_node_list()
			: _m_name_list(_m_str_list)
		{}
		cdata_t find(cdata_t const& item) const
		{
			ox::mos::scope_cs lock(_m_cs);
			return _m_name_list.find(item);
		}
		void add(cdata_t const& data)
		{
			ox::mos::scope_cs lock(_m_cs);
			_m_name_list.add_unique(data);
		}
		/// return is ok
		bool merge(cdata_t const& item)
		{
			ox::mos::scope_cs lock(_m_cs);
			cdata_t r = _m_name_list.find(item);
			if (r.is_empty())
				_m_name_list.add_unique(item);
			return r.is_empty();
		}

		ox::mos::critical_section _m_cs;
		std::string _m_str_list;
		node_name_list _m_name_list;
	};

}
}