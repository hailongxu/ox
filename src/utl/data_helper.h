
#include <string.h>
#include <string>
#include "data_t.h"


#pragma once


namespace ox
{
namespace utl
{

	static cdata_t to_cdata(char const* str)
	{
		return cdata_t(str,strlen(str));
	}
	static cdata_t to_cdata(std::string const& str)
	{
		return cdata_t(str.c_str(),str.size());
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
	static void assign(std::string& str,cdata_t const& data)
	{
		str.assign(data.begin,data.size);
	}

}
}