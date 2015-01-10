
#include "data_t.h"
#include "../pkg/string_simple_kit.h"

#pragma once

namespace ox {
namespace utl {


template <typename character_tn>
struct seplist
{
	static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
	typedef cdata_t cdata_t;
	typedef character_tn character;
	typedef ox::string::string_simple_kit<character> strkit;
	typedef typename strkit::std_string std_string;
	typedef typename strkit::foreach_line<casive> foreach_line;
	typedef typename foreach_line::line_handle line_handle;
	typedef typename strkit::get_line<casive> get_line;

	struct cmp_default
	{
		bool operator()(cdata_t const& a,cdata_t const& b)
		{
			if (a.size!=b.size) return false;
			return 0==strkit::strncmp<casive>(a.begin,b.begin,a.size);
		}
	};
	template <typename comp_tn=cmp_default>
	struct find_handle
	{
		bool operator()(character const* b,character const* e)
		{
			//if (e-b!=data.size) return true;
			//bool finded = 0==strkit::strncmp<casive>(b,data.begin,data.size);
			bool finded = cmp(data,cdata_t(b,e-b));
			if (!finded) return true;
			pos.assign(b,e-b);
			return false;
		}
		cdata_t data;
		cdata_t pos;
		comp_tn cmp;
	};
	template <>
	struct find_handle <int>
	{
		bool operator()(character const* b,character const* e)
		{
			bool finded = cmp_default()(data,cdata_t(b,e-b));
			if (!finded) return true;
			pos.assign(b,e-b);
			return false;
		}
		cdata_t data;
		cdata_t pos;
		int cmp;
	};

	seplist(std_string& buf,character const& sep=',')
		: _m_buffer(buf)
	{
		_m_sep[0] = sep;
		_m_sep[1] = 0;
	}
	character const* sep() const
	{
		return _m_sep;
	}
	character sepchar() const
	{
		return _m_sep[0];
	}
	bool is_empty() const
	{
		return _m_buffer.empty();
	}
	cdata_t data() const
	{
		return cdata_t(_m_buffer.data(),_m_buffer.size());
	}
	cdata_t find(cdata_t const& item) const
	{
		return find<cmp_default>(item,cmp_default());
	}
	template <typename comp_tn>
	cdata_t find(cdata_t const& item,comp_tn comp) const
	{
		find_handle<comp_tn> handle = {item};
		handle.cmp = comp;
		foreach(line_handle(&handle));
		if (handle.pos.is_empty()) return cdata_t();
		cdata_t pos = handle.pos;
		return pos;
	}
	void add_unique(cdata_t const& data)
	{
		cdata_t r = find(data);
		if (r.is_empty()) add(data);;
	}
	void add_unique(character const* data)
	{
		add_unique(cdata_t(data,strkit::length(data)));
	}
	void add(cdata_t const& data)
	{
		if (!_m_buffer.empty()) _m_buffer.append(_m_sep);
		_m_buffer.append(data.begin,data.size);
	}
	void add(character const* data)
	{
		add(cdata_t(data,strkit::length(data)));
	}
	bool erase(char const* item)
	{
		return erase(cdata_t(item,strkit::length(item)));
	}
	bool erase(cdata_t const& item)
	{
		return erase<cmp_default>(item,cmp_default());
	}
	template <typename cond_tn>
	bool erase(cdata_t const& item,cond_tn cond)
	{
		cdata_t pos = find<cond_tn>(item,cond);
		if (pos.is_empty()) return false;
		size_t off = pos.begin - _m_buffer.data();
		if (*pos.end()==_m_sep[0]) pos.size++;
		_m_buffer.erase(off,pos.size);
		if (!_m_buffer.empty() && _m_buffer[_m_buffer.size()-1]==_m_sep[0])
			_m_buffer.erase(--_m_buffer.end());
		return true;
	}
	cdata_t next(cdata_t const& prev) const
	{
		char const* sep [] = {_m_sep};
		assert(prev.is_empty() || prev.begin>=_m_buffer.data() && prev.end()<=buf_end());
		char const* begin = prev.end();
		if (!begin) begin = _m_buffer.data();
		else
		{
			begin = prev.end();
			if (begin<buf_end() && *begin==_m_sep[0]) ++begin;
		}
		get_line get(begin,buf_end());
		get.find.set_seps(sep);
		char const* line_end = 0;
		char const* line = get(&line_end);
		return cdata_t(line,line_end-line);
	}
	char const* buf_end() const {return _m_buffer.data()+_m_buffer.size();}

	void foreach(cdata_t data,line_handle const& handle) const
	{
		foreach_line foreach_item;
		char const* sep [] = {_m_sep};
		foreach_item._m_get_line.find.set_seps(sep);
		foreach_item(data.begin,data.begin+data.size,handle);
	}
	void foreach(line_handle const& handle) const
	{
		foreach(cdata_t(_m_buffer.data(),_m_buffer.size()),handle);
	}

private:
	std_string& _m_buffer;
	character _m_sep[2];
};


template <typename seplist_tn,typename bind_tn>
struct seplist_transform
{
	typedef seplist_tn seplist_t;
	typedef bind_tn bind_t;
	typedef typename seplist_t::character character;
	typedef delegate<bool(std::string& out,bind_t bind,character const* b,character const* e)> transform_handle;

	struct transform_handle_internal
	{
		transform_handle_internal(bind_t value,std::string& out,character const& sep)
			: _m_value(value), _m_list(out,sep)
		{}
		bool operator()(character const* b,character const* e)
		{
			_m_each_out.clear();
			bool is_continue = _m_handle(_m_each_out,_m_value,b,e);
			if (!is_continue) return false;
			_m_list.add(cdata_t(_m_each_out.data(),_m_each_out.size()));
			return true;
		}
		std::string _m_each_out;
		seplist_t _m_list;
		transform_handle _m_handle;
		bind_t _m_value;
	};

	seplist_transform(seplist_t& sepl)
		: _m_seplist (sepl)
	{}

	void operator()(std::string& out,transform_handle const& handle,bind_t value)
	{
		typedef typename seplist_t::foreach_line foreach_line;
		typedef typename foreach_line::line_handle line_handle;
		transform_handle_internal handle_internal(value,out,_m_seplist.sepchar());
		handle_internal._m_handle = handle;
		foreach_line foreach_item;
		char const* sep [] = {_m_seplist.sep()};
		foreach_item._m_get_line.find.set_seps(sep);
		cdata_t buff_data = _m_seplist.data();
		foreach_item(buff_data.begin,buff_data.end(),line_handle(&handle_internal));
	}

	seplist_t& _m_seplist;
};


}}