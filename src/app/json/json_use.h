

#include <assert.h>
#include "json.h"

namespace json
{

	template <typename character>
	static __int64 to_int64(character const* str)
	{
		return _wtoi64(str);
	}
	template <>
	static __int64 to_int64(char const* str)
	{
		return _atoi64(str);
	}


template <typename character>
static json_value<character>* get_json_value(json_object<character> jsobj, character const* key)
{
	json::json_object<character>::const_iterator i = jsobj.find(key);
	if (i != jsobj.end())
		return i->second;
	return 0;
}

template <typename character>
static void get_str_value(json_object<character> jsobj, character const* key,std::basic_string<character>& str_value)
{
	json::json_object<character>::const_iterator i = jsobj.find(key);
	if (i != jsobj.end())
		str_value = i->second->as_string().c_str();
	str_value.clear();
}

template <typename character,typename integer_tn>
static bool get_num_value(json_object<character> jsobj, character const* key,integer_tn& num_value)
{
	json::json_object<character>::const_iterator i = jsobj.find(key);
	if (i == jsobj.end()) return false;
	if (i->second->is_number()) num_value = i->second->as_number();
	else if (i->second->is_string()) num_value = to_int64(i->second->as_string().c_str());
	return true;
}

}
