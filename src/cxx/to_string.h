/*
	file : interval_carray.h
	author : xuhailong
	date: 2012-10-3
	convert a type to a string
 */

#include "../ox/nsab.h"
#include "../mix/mixture_param.h"
#include <stdio.h>



#pragma once



___namespace2_begin(ox,cxx)


template <typename t>
size_t to_string(t const& v,char* str);



template <typename t>
struct obj_to_string;

template <>
struct obj_to_string <char>
{
	size_t operator()(char const& c,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%c",c);
	}
	size_t operator()(char const& c,char* buf,char)
	{
		if (!buf) return 0;
		return sprintf (buf,"'%c'",c);
	}
};

template <>
struct obj_to_string <unsigned char>
{
	size_t operator()(unsigned char const& c,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%c",c);
	}
	size_t operator()(unsigned char const& c,char* buf,char)
	{
		if (!buf) return 0;
		return sprintf (buf,"'%c'",c);
	}
};

template <>
struct obj_to_string <int>
{
	size_t operator()(int const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%d",v);
	}
	size_t operator()(int const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <unsigned int>
{
	size_t operator()(unsigned int const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%u",v);
	}
	size_t operator()(unsigned int const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <long>
{
	size_t operator()(long const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%ld",v);
	}
	size_t operator()(long const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <unsigned long>
{
	size_t operator()(unsigned long const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%lu",v);
	}
	size_t operator()(unsigned long const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <short>
{
	size_t operator()(short const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%d",v);
	}
	size_t operator()(short const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <unsigned short>
{
	size_t operator()(unsigned short const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%u",v);
	}
	size_t operator()(unsigned short const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <double>
{
	size_t operator()(double const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%lf",v);
	}
	size_t operator()(double const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};

template <>
struct obj_to_string <float>
{
	size_t operator()(float const& v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%f",v);
	}
	size_t operator()(float const& v,char* buf,char)
	{
		return operator()(v,buf);
	}
};


template <>
struct obj_to_string<char const*>
{
	size_t operator()(char const* v,char* buf)
	{
		if (!buf) return 0;
		return sprintf (buf,"%s",v);
	}
	size_t operator()(char const* v,char* buf,char)
	{
		if (!buf) return 0;
		return sprintf (buf,"\"%s\"",v);
	}
};


___namespace2_end()






#include "../mix/mixture_decl.h"
#include "../mix/mixture_scan.h"


___namespace2_begin(ox,cxx)

/// the type is mixture

template <typename sl,typename ms,typename uz,typename edi>
struct obj_to_string <ox::mix::mixture<sl,ms,uz,edi>>
{
	template <typename path,typename ii,typename tag,typename bformat>
	struct print;
	template <typename path,typename ii,typename bformat>
	struct print <path,ii,ox::tags::begin,bformat>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(char*& str) : _m_str(str) {}
		char*& _m_str;
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			size_t size = 0;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string('[',_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] {",_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};
	template <typename path,typename ii,typename bformat>
	struct print <path,ii,void,bformat>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(char*& str) : _m_str(str) {}
		char*& _m_str;
		template <typename unit,typename owner,typename origin>
		ox::treectrlid operator()(unit& u,owner& o,origin& or)
		{
			size_t size = 0;
			size = to_string('\n',_m_str); _m_str += size;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string('[',_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] ",_m_str); _m_str += size;
			size = to_string(unit::ifeature::to_string(),_m_str); _m_str += size;
			size = to_string(':',_m_str); _m_str += size;
			size = to_string(u.value(),_m_str); _m_str += size;
			size = to_string(',',_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};
	template <typename path,typename ii,typename bformat>
	struct print <path,ii,ox::tags::end,bformat>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(char*& str) : _m_str(str) {}
		char*& _m_str;
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			size_t size = 0;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string("\n[",_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] }",_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};

	template <typename path,typename ii>
	struct print<path,ii,ox::tags::begin,ox::boolean<true>>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(ox::mix::tparam2<char*,char const>& param) :_m_ext(param) {}
		ox::mix::tparam2<char*,char const>& _m_ext;
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			char*& _m_str = _m_ext.v<0>();
			size_t size = 0;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string('[',_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] {",_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};
	template <typename path,typename ii>
	struct print<path,ii,void,ox::boolean<true>>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(ox::mix::tparam2<char*,char const>& param) :_m_ext(param) {}
		ox::mix::tparam2<char*,char const>& _m_ext;
		template <typename unit,typename owner,typename origin>
		ox::treectrlid operator()(unit& u,owner& o,origin& or)
		{
			char*& _m_str = _m_ext.v<0>();
			char const& _m_c = _m_ext.v<1>();
			size_t size = 0;
			size = to_string('\n',_m_str); _m_str += size;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string('[',_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] ",_m_str); _m_str += size;
			size = to_string(unit::ifeature::to_string(),_m_str); _m_str += size;
			size = to_string(':',_m_str); _m_str += size;
			size = to_string(u.value(),_m_str,_m_c); _m_str += size;
			size = to_string(',',_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};
	template <typename path,typename ii>
	struct print<path,ii,ox::tags::end,ox::boolean<true>>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(ox::mix::tparam2<char*,char const>& param) :_m_ext(param) {}
		ox::mix::tparam2<char*,char const>& _m_ext;
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			char*& _m_str = _m_ext.v<0>();
			size_t size = 0;
			size = to_indent(_m_str,path::size-1); _m_str += size;
			size = to_string("\n[",_m_str); _m_str += size;
			size = to_string(ii::value,_m_str); _m_str += size;
			size = to_string("] }",_m_str); _m_str += size;
			return ox::treectrlid::make<true>();
		}
	};

	size_t operator()(ox::mix::mixture<sl,ms,uz,edi> const& v,char* buf)
	{
		typedef ox::met::lambda<print<ox::_1,ox::_2,ox::_3,ox::boolean<false>>> handle_lmd;
		char* str = buf;
		ox::mix::mixture_scan<handle_lmd,'+'>()(v,str);
		return str-buf;
	}
	size_t operator()(ox::mix::mixture<sl,ms,uz,edi> const& v,char* buf,char c)
	{
		typedef ox::met::lambda<print<ox::_1,ox::_2,ox::_3,ox::boolean<true>>> handle_lmd;
		char* str = buf;
		ox::mix::tparam2<char*,char const> param(str,c);
		ox::mix::mixture_scan<handle_lmd,'+'>()(v,param);
		return param.v<0>()-buf;
	}

	static size_t to_indent(char* out,size_t count=1)
	{
		return put_string("    ",out,count);
	}
	template <size_t n>
	static size_t put_string(char const (&str)[n],char* out,size_t count=1)
	{
		size_t size = 0;
		for(size_t i=0;i<count;i++)
			size += to_string(str,out+size);
		return size;
	}
};



template <typename t>
size_t to_string(t const& v,char* str)
{
	return obj_to_string<t>()(v,str);
}
template <typename t>
size_t to_string(t const& v,char* str,char c)
{
	return obj_to_string<t>()(v,str,c);
}
template <typename t>
size_t to_string(t* const v,char* str)
{
	return obj_to_string<t const*>()(v,str);
}
template <typename t>
size_t to_string(t* const v,char* str,char c)
{
	return obj_to_string<t const*>()(v,str,c);
}


___namespace2_end()

