


#include "../ox/nsab.h"
#include "../utl/shbuffer.h"

#pragma once


___namespace2_begin(ox,str)



struct format
{
	int get_format_length(char const* format_str,va_list argptr)
	{
		/// the size needed NOT include the last null char
		return ::_vscprintf(format_str,argptr);
	}
	int get_format_length(wchar_t const* format_str,va_list argptr)
	{
		/// the size needed NOT include the last null char
		return ::_vscwprintf(format_str,argptr);
	}
	int vsprintf(char* buff,size_t size,char const* format_str,va_list arglist)
	{
		return ::vsprintf_s(buff,size,format_str,arglist);
	}
	int vsprintf(wchar_t* buff,size_t size,wchar_t const* format_str,va_list arglist)
	{
		return ::vswprintf_s(buff,size,format_str,arglist);
	}
	template <typename char_tn>
	ox::utl::cdata_tt<char_tn> resize(std::basic_string<char_tn>& str_out,size_t size)
	{
		assert(size);
		if (size==0) return ox::utl::cdata_tt<char_tn>();
		str_out.resize(size-1);
		return ox::utl::cdata_tt<char_tn>(str_out.data(),str_out.size()+1);
	}
	template <typename char_tn,size_t size_tc>
	ox::utl::cdata_tt<char_tn> resize(ox::utl::good_buffer<char_tn[size_tc]>& str_out,size_t size)
	{
		assert(size);
		/// plus one char as last null
		return str_out.allocate(size);
	}
	template <typename char_tn>
	ox::utl::cdata_tt<char_tn> get_data(std::basic_string<char_tn> const& str_out) const
	{
		/// the last null char reserved we can used too
		return ox::utl::cdata_tt<char_tn>(str_out.data(),str_out.size()+1);
	}
	template <typename char_tn,size_t size_tn>
	ox::utl::cdata_tt<char_tn> get_data(ox::utl::good_buffer<char_tn[size_tn]> const& str_out) const
	{
		return str_out.data();
	}
	template <typename char_tn,typename str_tn>
	size_t get(str_tn& str_out,const char_tn *format_str, va_list arglist)
	{
		int size = 0;
		do {
			size = get_format_length(format_str,arglist);
			ox::utl::cdata_tt<char_tn> data = resize(str_out,size+1);
			if (data.is_empty()) break;
			int size_used = vsprintf((char_tn*)data.begin,data.size,format_str,arglist);
			
			assert (size==size_used);
			size = size_used;
		} while (0);
		return size;
	}
	template <typename t,typename char_new_tn>
	struct rebind_char;
	template <typename char_tn,typename char_new_tn>
	struct rebind_char <std::basic_string<char_tn>,char_new_tn>
	{
		typedef std::basic_string<char_new_tn> type;
	};
	template <typename char_tn,size_t size_tc,typename char_new_tn>
	struct rebind_char <ox::utl::good_buffer<char_tn[size_tc]>,char_new_tn>
	{
		typedef ox::utl::good_buffer<char_new_tn[size_tc]> type;
	};
};

template <typename print_str,typename str_tn>
struct print_ref
{
	print_str& _m_ps;
	print_ref(print_str& ps)
		: _m_ps(ps)
	{
	}
	template <typename str_tn,typename char_tn>
	int let_v(str_tn& str_out,const char_tn *format_str,va_list arglist)
	{
		if (!format().get(str_out,format_str,arglist))
			return 0;
		return _m_ps(format().get_data(str_out));
	}
	template <typename char_tn>
	int let_format(char_tn const* format_str, ...)
	{
		int size = 0;
		va_list arglist;
		va_start(arglist,format_str);
		typename format::rebind_char<str_tn,char_tn>::type str_out;
		format().get(str_out,format_str,arglist);
		va_end(arglist);
		char_tn const* cstr=format().get_data(str_out).begin;
		_m_ps(cstr);
		return 0;
	}
};
template <>
struct print_ref <void,void>
{
	template <typename t>
	print_ref(t const&)
	{
	}
	template <typename str_tn,typename char_tn>
	int let_v(str_tn& str_out,const char_tn *format_str,va_list arglist)
	{
		return 0;
	}
	template <typename char_tn>
	int let_format(const char_tn * format_str, ...)
	{
		return 0;
	}
};


___namespace2_end()