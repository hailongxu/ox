
#include <string.h>
#include <string>
#include "../fit/array.h"
#include "../ox/nsab.h"
#include "data_t.h"


#pragma once


___namespace2_begin(ox,utl)


static cdata_t to_cdata(char const* str)
{
	return cdata_t(str,strlen(str));
}
static wcdata_t to_cdata(wchar_t const* str)
{
	return wcdata_t(str,wcslen(str));
}
static cdata_t to_cdata(std::string const& str)
{
	return cdata_t(str.c_str(),str.size());
}
template <size_t size>
static cdata_t to_cdata(ox::fit::static_vector<char[size]> const& vec)
{
	return cdata_t(vec.data(),vec.size());
}
template <size_t size>
static cdata_t to_cdata(ox::fit::static_string<char[size]> const& vec)
{
	return cdata_t(vec.data(),vec.size());
}

static data_t to_data(char const* str)
{
	return data_t((char*)str,strlen(str));
}
static data_t to_data(std::string const& str)
{
	return data_t((char*)(str.c_str()),str.size());
}
static std::string to_string(cdata_t const& data)
{
	return std::string(data.begin,data.size);
}
static void assign(char* str,cdata_t const& data)
{
	memmove(str,data.begin,data.size);
	str[data.size] = 0;
}
static void assign(wchar_t* str,wcdata_t const& data)
{
	wmemmove(str,data.begin,data.size);
	str[data.size] = 0;
}
static void assign(std::string& out,cdata_t const& data)
{
	out.assign(data.begin,data.size);
}
static void assign(std::wstring& out,wcdata_t const& data)
{
	out.assign(data.begin,data.size);
}
static void append(std::string& out,cdata_t const& data)
{
	out.append(data.begin,data.size);
}


___namespace2_end()