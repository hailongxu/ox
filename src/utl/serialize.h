#include "../utl/vecbuff.h"
#include "../utl/data_helper.h"
#include "../utl/data_t.h"
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>



#pragma once


namespace ox
{
namespace utl
{

template <typename t1,typename t2>
static void serialize(std::string& all,t1 const& v1,t2 const& v2);
template <typename t1,typename t2>
static void deserialize(std::string const& all,t1& v1,t2& v2);


namespace serl_ns
{


typedef ox::utl::vecbuff_tt<std::string> vecbuff_helper;
typedef std::vector<int> path_t;
typedef ox::utl::vecbuff_t vecbuff_t;
typedef ox::utl::cdata_t cdata_t;



//vecbuff_t* get_vecbuf(std::string& out,path_t const& path)
//{
//	vecbuff_t* vb = &vecbuff_helper::as_vecbuff(out,0);
//	for (size_t i=0;i<path.size();++i)
//	{
//		cdata_t data = vb->data_item(path[i]);
//		return &vecbuff_helper::as_vecbuff(data);
//	}
//	return vb;
//}
//size_t get_vecbuff_offset(std::string& out,path_t const& path)
//{
//	return 0;
//}

template<typename t>
struct serialize_each
{};

template <>
struct serialize_each<char>
{
	typedef serialize_each self;
	typedef char value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
		return ;
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<unsigned char>
{
	typedef serialize_each self;
	typedef unsigned char value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
//template <>
//struct serialize_each<wchar_t>
//{
//	typedef serialize_each self;
//	typedef wchar_t value_type;
//	void add(std::string& out,value_type const& v)
//	{
//		vecbuff_helper help(out,0);
//		help.add_binary(v);
//		return ;
//	}
//	void sub(cdata_t data,value_type& v)
//	{
//		v = *(value_type*)(data.begin);
//	}
//};
//template <>
//struct serialize_each<unsigned wchar_t>
//{
//	typedef serialize_each self;
//	typedef unsigned wchar_t value_type;
//	void add(std::string& out,value_type const& v)
//	{
//		vecbuff_helper help(out,0);
//		help.add_binary(v);
//	}
//	void sub(cdata_t data,value_type& v)
//	{
//		v = *(value_type*)(data.begin);
//	}
//};
template <>
struct serialize_each<short>
{
	typedef serialize_each self;
	typedef short value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<unsigned short>
{
	typedef serialize_each self;
	typedef unsigned short value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<int>
{
	typedef serialize_each self;
	typedef int value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<unsigned int>
{
	typedef serialize_each self;
	typedef unsigned int value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<long>
{
	typedef serialize_each self;
	typedef long value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<unsigned long>
{
	typedef serialize_each self;
	typedef unsigned long value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<float>
{
	typedef serialize_each self;
	typedef float value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};
template <>
struct serialize_each<double>
{
	typedef serialize_each self;
	typedef double value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_binary(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = *(value_type*)(data.begin);
	}
};

template <>
struct serialize_each<char const*>
{
	typedef serialize_each self;
	typedef char const* value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_str1(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = data.begin;
	}
};
template <>
struct serialize_each<unsigned char const*>
{
	typedef serialize_each self;
	typedef char const* value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add_str1(v);
	}
	void sub(cdata_t data,value_type& v)
	{
		v = data.begin;
	}
};
template <typename t,typename mem>
struct serialize_each<std::basic_string<t,mem>>
{
	typedef serialize_each self;
	typedef std::basic_string<t,mem> value_type;
	void add(std::string& out,value_type const& v)
	{
		vecbuff_helper help(out,0);
		help.add(cdata_t((char const*)v.data(),v.size()*sizeof(t)));
	}
	void sub(cdata_t data,value_type& v)
	{
		v.assign((t const*)data.begin,data.size/sizeof(t));
	}
};
template <typename t,typename mem>
struct serialize_each<std::vector<t,mem>>
{
	typedef serialize_each self;
	typedef std::vector<t,mem> value_type;
	void add(std::string& out,value_type const& v)
	{
		std::string buff;
		vecbuff_helper(buff,0).init();
		typedef value_type::const_iterator I;
		for (I i=v.begin();i!=v.end();++i)
			serialize_each<t>().add(buff,*i);
		vecbuff_helper(out,0).add(ox::utl::to_data(buff));
	}
	void sub(cdata_t data,value_type& v)
	{
		vecbuff_t& vecbuf = *vecbuff_t::as_vecbuff(data);
		size_t count = vecbuf.item_count();
		for (size_t i=0;i<count;++i)
		{
			cdata_t item = vecbuf.data_item(i);
			t item_value;
			serialize_each<t>().sub(item,item_value);
			v.push_back(item_value);
		}
	}
};
template <typename t,typename mem>
struct serialize_each<std::list<t,mem>>
{
	typedef serialize_each self;
	typedef std::list<t,mem> value_type;
	void add(std::string& out,value_type const& v)
	{
		std::string buff;
		vecbuff_helper(buff,0).init();
		typedef value_type::const_iterator I;
		for (I i=v.begin();i!=v.end();++i)
			serialize_each<t>().add(buff,*i);
		vecbuff_helper(out,0).add(ox::utl::to_data(buff));
	}
	void sub(cdata_t data,value_type& v)
	{
		vecbuff_t& vecbuf = *vecbuff_t::as_vecbuff(data);
		size_t count = vecbuf.item_count();
		for (size_t i=0;i<count;++i)
		{
			cdata_t item = vecbuf.data_item(i);
			t item_value;
			serialize_each<t>().sub(item,item_value);
			v.push_back(item_value);
		}
	}
};
template <typename t,typename c,typename mem>
struct serialize_each<std::set<t,c,mem>>
{
	typedef serialize_each self;
	typedef std::set<t,c,mem> value_type;
	void add(std::string& out,value_type const& v)
	{
		std::string buff;
		vecbuff_helper(buff,0).init();
		typedef value_type::const_iterator I;
		for (I i=v.begin();i!=v.end();++i)
			serialize_each<t>().add(buff,*i);
		vecbuff_helper(out,0).add(ox::utl::to_data(buff));
	}
	void sub(cdata_t data,value_type& v)
	{
		vecbuff_t& vecbuf = *vecbuff_t::as_vecbuff(data);
		size_t count = vecbuf.item_count();
		for (size_t i=0;i<count;++i)
		{
			cdata_t item = vecbuf.data_item(i);
			t item_value;
			serialize_each<t>().sub(item,item_value);
			v.push_back(item_value);
		}
	}
};
template <typename k,typename v,typename c,typename mem>
struct serialize_each<std::map<k,v,c,mem>>
{
	typedef serialize_each self;
	typedef std::map<k,v,c,mem> value_type;
	void add(std::string& out,value_type const& object)
	{
		std::string buff;
		vecbuff_helper(buff,0).init();
		typedef value_type::const_iterator I;
		for (I i=object.begin();i!=object.end();++i)
		{
			serialize_each<k>().add(buff,i->first);
			serialize_each<v>().add(buff,i->second);
		}
		vecbuff_helper(out,0).add(ox::utl::to_data(buff));
	}
	void sub(cdata_t data,value_type& object)
	{
		vecbuff_t& vecbuf = *vecbuff_t::as_vecbuff(data);
		size_t count = vecbuf.item_count();
		assert(count%2==0);
		for (size_t i=0;i<count;)
		{
			cdata_t kitem = vecbuf.data_item(i++);
			cdata_t vitem = vecbuf.data_item(i++);
			k key;v value;
			serialize_each<k>().sub(kitem,key);
			serialize_each<v>().sub(vitem,value);
			object[key]=value;
		}
	}
};

} /// end of namespace serl


template <typename t>
static void serialize(std::string& all,t const& v)
{
	all.clear();
	vecbuff_helper help(all,0);
	help.init();
	serl_ns::serialize_each<t>().add(all,v);
}

template <typename t>
static void deserialize(std::string const& all,t& v)
{
	deserialize(ox::utl::to_data(all),v);
}
template <typename t>
static void deserialize(serl_ns::cdata_t all,t& v)
{
	vecbuff_t& vecbuf = *vecbuff_t::as_vecbuff(all);
	cdata_t data = vecbuf.data_item(0);
	serl_ns::serialize_each<t>().sub(data,v);
}

template <typename t1,typename t2>
static void serialize(std::string& all,t1 const& v1,t2 const& v2)
{
	serl_ns::vecbuff_helper help(all,0);
	help.init();
	std::string buf;
	serialize(buf,v1);
	help.add(ox::utl::to_data(buf));
	buf.clear();
	serialize(buf,v2);
	help.add(ox::utl::to_data(buf));
}
template <typename t1,typename t2>
static void deserialize(std::string const& all,t1& v1,t2& v2)
{
	deserialize(ox::utl::to_data(all),v1,v2);
}
template <typename t1,typename t2>
static void deserialize(cdata_t all,t1& v1,t2& v2)
{
	serl_ns::vecbuff_t* vecbuf = serl_ns::vecbuff_t::as_vecbuff(all);
	serl_ns::cdata_t data = vecbuf->data_item(0);
	deserialize(data,v1);
	data = vecbuf->data_item(1);
	deserialize(data,v2);
}

} /// end
}
