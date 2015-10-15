

#include <string>
#include <vector>
#include <map>
#include "scope_raii.h"
#include "atomic_number.h"
#include "communication_const.h"
#include "dns.h"


#pragma once


namespace abc
{
	struct dns_pipe_t
	{
		typedef dns_pipe_t self;
		typedef dns_t::ipaddresses_t ipaddresses_t;
		typedef dns_t::result_t result_t;
		typedef std::map<std::string,ipaddresses_t> router_t;
		typedef communication_const constant;
		typedef dns_t dns_t;

		dns_t as_dns()
		{
			dns_t dns;
			dns.safe_find.assign(this,&self::safe_find);
			dns.find.assign(this,&self::find);
			return dns;
		}
		result_t safe_find(char const* name)
		{
			ox::mos::scope_cs lock(cs);
			return find(name);
		}
		result_t find(char const* name)
		{
			typedef router_t::iterator I;
			I i = _m_router.find(name);
			if (i != _m_router.end())
			{
				if (!i->second.empty())
					return result_t(0,&i->second,i->first.c_str());
			}
			else
			{
				std::pair<I,bool> r = _m_router.insert(router_t::value_type(name,ipaddresses_t()));
				if (!r.second)
					return result_t(constant::__error_map_insert_error,(ipaddresses_t*)(0),0);
				i = r.first;
			}
			size_t error_code = insert(name,i->second);
			return result_t(error_code,&i->second,i->first.c_str());
		}

		/// the cache is net order
		size_t insert(char const* host_name,ipaddresses_t& ipaddress)
		{
			size_t ip = seq_next();
			ipaddress.push_back(ip);
			return 0;
		}
		static size_t seq_next()
		{
			static ox::mos::atomic_number<long> _static_seq(1);
			return _static_seq.add(1);
		}
		router_t _m_router;
		ox::mos::critical_section cs;
	}; /// end of dns
} ///end of namespace abc