
#pragma once

#include <assert.h>
#include <string>
#include "veclist.h"

namespace json
{
	static inline bool isnan(double x) { return x != x; }
	static inline bool isinf(double x) { return !isnan(x) && isnan(x - x); }

	template <typename ch,typename m>
	static char const& string_back(std::basic_string<ch,m> const& str) {return *(--str.end());}
	template <typename ch,typename m>
	static void string_pop_back(std::basic_string<ch,m>& str) {str.erase(--str.end());}

	template<typename> struct json_value;
	template<typename> struct json_null;
	template<typename> struct json_bool;
	template<typename> struct json_number;
	template<typename> struct json_string;
	template<typename> struct json_array;
	template<typename> struct json_object;
	template<typename> struct json_document;


	struct json_allocator
	{
		json_allocator():_m_cache(0) {}
		char* allocate(size_t size)
		{
			assert(_m_cache);
			return _m_cache->allocate(size);
		}
		void deallocate(char* p)
		{
			(void)(p);
			assert(_m_cache);
		}
		abc::mem_cache* _m_cache;
	};

	typedef bool bool_type;
	typedef double number_type;

	template<typename T>
	struct scope_ptr
	{
		/*explicit*/ scope_ptr(T* tp):p(tp) {}
		~scope_ptr() {if (p) {/*assert(false);*/p->~T();}}
		T* p;
		T* operator->() {return p;}
		T& operator*() {return *p;}
		bool is_empty() const {return 0==p;}
		operator bool() {return 0!=p;}
		T* detach() {T*t=p;p=0;return t;}
		scope_ptr& operator=(scope_ptr const&);
	};
	template <typename character>
	static bool skip_whitespace(const character*& data);
	template <typename character>
	static int parse_int(const character*& data);

	enum json_type
	{
		__json_type_not_valid=-1,
		__json_type_null,
		__json_type_string,
		__json_type_bool,
		__json_type_number,
		__json_type_array,
		__json_type_object
	};

	template <typename character>
	struct strptr2_tt
		: abc::strptr_tt<character>
	{
		character const* c_str() const {return begin;}
		int cmp(character const* str)
		{
			return abc::helper::strncmp(begin,size,str,abc::helper::strlen(str));
		}
	};

	template <typename character>
	struct str_tt
	{
		str_tt() : str(0) {}
		str_tt(character const* ptr) : str(ptr) {}
		character const* str;
	};

	template <typename ptr_tn>
	struct ptr_tt
	{
		ptr_tt() : ptr(0) {}
		ptr_tt(ptr_tn const* p) : ptr(p) {}
		ptr_tn const* ptr;
	};

	template <typename character>
	struct inner_object_type
	{
		typedef strptr2_tt<character> first_type;
		typedef json_value<character>* second_type;
		inner_object_type():second(0){}
		inner_object_type(first_type const& p1,second_type p2):first(p1),second(p2){}
		first_type first;
		second_type second;
	};

	template <typename character>
	static bool isstreq(abc::strptr_tt<character> const& a,abc::strptr_tt<character> const& b)
	{
		if (a.size!=b.size)
			return false;
		if (!!a.begin ^ !!b.begin)
			return false;
		return 0==abc::helper::strnicmp(a.begin,b.begin,a.size);
	}
	template <typename character>
	static bool isstreq(strptr2_tt<character> const& a,strptr2_tt<character> const& b)
	{
		if (a.size!=b.size)
			return false;
		if (!a.begin && !b.begin)
			return true;
		if (!(a.begin && b.begin))
			return false;
		return 0==abc::helper::strnicmp(a.begin,b.begin,a.size);
	}

	template <typename character>
	bool operator==(inner_object_type<character> const& a, inner_object_type<character> const& b)
	{
		return isstreq(a.first,b.first);
	}

	template<typename character>
	struct complex_types
	{
		typedef character character;
		typedef std::basic_string<character> std_string;
		typedef std::basic_stringstream<character, std::char_traits<character>,std::allocator<character>> std_stringstream;
		typedef abc::strptr_tt<character> strptr_t;
		typedef str_tt<character> str_t;
		typedef inner_object_type<character> inner_object_type;
		typedef json_value<character> json_value;
		typedef abc::string_ptrs_tt<character,abc::getsize_default,json_allocator> string_type;
		typedef abc::veclist<json_value*,abc::getsize_default,json_allocator> array_type;
		typedef abc::veclist<inner_object_type,abc::getsize_default,json_allocator> object_type;
		typedef json_null<character> json_null;
		typedef json_bool<character> json_bool;
		typedef json_number<character> json_number;
		typedef json_string<character> json_string;
		typedef json_array<character> json_array;
		typedef json_object<character> json_object;
		struct stringify_interface {virtual bool operator()(json_value const* jvalue){return true;}};
	};
	template <typename character> struct const_string;
	template <> struct const_string<char>
	{
		typedef char character;
		static character const* null_string() {static character const* local="null";return local;}
		static character const* true_string() {static character const* local="true";return local;}
		static character const* false_string() {static character const* local="false";return local;}
	};
	template <> struct const_string<wchar_t>
	{
		typedef wchar_t character;
		static character const* null_string() {static character const* local=L"null";return local;}
		static character const* true_string() {static character const* local=L"true";return local;}
		static character const* false_string() {static character const* local=L"false";return local;}
	};

	template <typename character>
	struct stringify_helper : complex_types<character>
	{
		struct stringify_true {bool operator()(json_value const* jvalue){return true;}};
		template<typename condition_tn> static bool stringify(json_value const* node,condition_tn& condition,std_string& stradded);
		static void stringify_string(strptr_t const& str,std_string& stradded);
		static void stringify_string(string_type const& str,std_string& stradded)
		{
			string_type::ptr_const_iterator i = str.ptr_begin();
			string_type::ptr_const_iterator end = str.ptr_end();
			for (;i!=end;++i)
			{
				stringify_string(*i,stradded);
			}
		}
		static void stringify(json_value const* node,std_string& stradded)
		{
			stringify_true always_true;
			stringify(node,always_true,stradded);
		}
		static std_string stringify_string(character const* str,size_t len)
		{
			std_string strfy;
			strptr_t strptr = {(character*)str,len};
			stringify_string(strptr,strfy);
			return strfy;
		}
		static std_string stringify_string(character const* str)
		{
			return stringify_string(str,abc::helper::strlen(str));
		}
	};
	template <typename character>
	static std::basic_string<character> stringify_string(character const* str)
	{
		return stringify_helper<character>::stringify_string(str);
	}

	template <typename character>
	struct make_helper
		: abc::make_helper<abc::mem_cache>
		, complex_types<character>
	{
		make_helper(abc::mem_cache* cache)
			: abc::make_helper<abc::mem_cache>(cache)
		{}

		json_null* make_json_null() {return make<json_null>();}
		json_bool* make_json_bool() {return make<json_bool>();}
		json_bool* make_json_bool(bool b) {return make<json_bool>(b);}
		json_number* make_json_number() {return make<json_number>();}
		json_number* make_json_number(double num) {return make<json_number>(num);}
		json_string* make_json_string(character const* str)
		{
			if (!str)
				return 0;
			json_string* jstr = make<json_string>(_m_allocator);
			if (!jstr)
				return 0;
			if (!jstr->reset(str))
			{
				jstr->~json_string();
				return 0;
			}
			return jstr;
		}
		json_array* make_json_array() {return make<json_array>(_m_allocator);}
		json_object* make_json_object() {return make<json_object>(_m_allocator);}
	};

	template<typename character>
	struct json_value
	{
		friend stringify_helper<character>;
		friend json_document<character>;
		typedef str_tt<character> str_t;
		typedef abc::strptr_tt<character> strptr_t;
		typedef strptr2_tt<character> strptr2_t;
		typedef json_null<character> json_null;
		typedef json_bool<character> json_bool;
		typedef json_number<character> json_number;
		typedef json_string<character> json_string;
		typedef json_array<character> json_array;
		typedef json_object<character> json_object;
		typedef std::basic_string<character> std_string;
		typedef typename complex_types<character>::array_type array_type;
		typedef typename complex_types<character>::object_type object_type;
		typedef typename complex_types<character>::string_type string_type;
		typedef typename complex_types<character>::stringify_interface stringify_interface;
		typedef typename array_type::const_iterator array_const_iterator;
		typedef typename object_type::const_iterator object_const_iterator;
		typedef abc::strptr_tt<character> strptr_t;

		virtual ~json_value()
		{
			//printf ("~json_value\n");
		}
		virtual bool is_valid() const {return false;}

		virtual json_type get_type() const {return __json_type_not_valid;}
		virtual bool is_null() const {return false;}
		virtual bool is_bool() const {return false;}
		virtual bool is_number() const {return false;}
		virtual bool is_string() const {return false;}
		virtual bool is_array() const {return false;}
		virtual bool is_object() const {return false;}
		virtual bool_type as_bool() const {return false;}
		virtual number_type as_number() const { return 0;}
		virtual string_type as_stringptrs() const { return string_type();}
		virtual std_string as_string() const { return std_string(); }
		virtual json_array& as_array() { return null_array();}
		virtual json_object& as_object() { return null_object();}
		virtual json_array const& as_array() const { return null_array();}
		virtual json_object const & as_object() const { return null_object();}
		virtual std_string stringify() const {return std_string();}
		virtual std_string& stringify(std_string& str) const {return str;}
		virtual std_string stringify(stringify_interface* condition) {return std_string();}
		//virtual array_const_iterator find(size_t index) const {return null_value();}
		//virtual object_const_iterator find(character const* key) const {return null_value();}
		virtual json_value const& operator[](size_t) const {return null_value();}
		virtual json_value const& operator[](str_t) const {return null_value();}

	protected:
		static json_value& null_value();
		static json_array& null_array();
		static json_object& null_object();
	};

	template <typename character>
	struct json_null : json_value<character>
	{
		friend stringify_helper<character>;
		typedef void value_type;
		virtual json_type get_type() const {return __json_type_null;}

		virtual bool is_valid() const {return true;}
		virtual bool is_null() const {return true;}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition,void* parm=0) {std_string str;stringify_helper<character>::stringify(this,*condition,str);return str;}
		~json_null()
		{}
	};
	template<typename character>
	struct json_bool : json_value<character>
	{
		friend stringify_helper<character>;
		typedef bool_type value_type;
		virtual json_type get_type() const {return __json_type_bool;}
		virtual bool is_valid() const {return true;}
		virtual bool is_bool() const {return true;}
		virtual value_type as_bool() const {return _m_value;}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition) {std_string str; stringify_helper<character>::stringify(this,*condition,str);return str;}

		json_bool& operator = (bool b) {_m_value = b;}
		~json_bool()
		{}
		json_bool()
		{}
		json_bool(value_type v)
			: _m_value(v)
		{}
	private:
		value_type _m_value;
	};
	template <typename character>
	struct json_number : json_value<character>
	{
		friend stringify_helper<character>;
		friend json_document<character>;
		typedef number_type value_type;
		virtual json_type get_type() const {return __json_type_number;}
		virtual bool is_valid() const {return true;}
		virtual bool is_number() const {return true;}
		virtual value_type as_number() const {return _m_value;}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition) {std_string str; stringify_helper<character>::stringify(this,*condition,str);return str;}

		value_type& operator = (double num) {_m_value = num; return *this;}
		~json_number()
		{}
		json_number()
		{}
		json_number(value_type const& v)
			: _m_value(v)
		{}
	private:
		value_type _m_value;
	};
	template<typename character>
	struct json_string : json_value<character>
	{
		friend make_helper<character>;
		friend stringify_helper<character>;
		friend json_document<character>;
		typedef string_type value_type;
		virtual json_type get_type() const {return __json_type_string;}
		virtual bool is_valid() const {return true;}
		virtual bool is_string() const {return true;}
		virtual value_type as_stringptrs() const {return _m_value;}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition) {std_string str; stringify_helper<character>::stringify(this,*condition,str);return str;}
		virtual std_string as_string() const
		{
			std_string str;
			typedef string_type::ptr_const_iterator I;
			I i = _m_value.ptr_begin(), end = _m_value.ptr_end();
			for (;i!=end;++i)
			{
				str.append(i->begin,i->size);
			}
			return str;
		}
		json_string& operator=(character const* str)
		{
			reset(str);
			return *this;
		}
		character* reset(character const* str)
		{
			_m_value.~value_type();
			size_t len = abc::helper::strlen(str);
			character* begin = (character*)_m_value.allocator().allocate((len+1)*sizeof(character));
			abc::helper::memcpy(begin,str,len);
			begin[len] = 0;
			return _m_value.push_back(begin,len);
		}
		~json_string()
		{}
		json_string()
		{}
		json_string(json_document<character>* doc);
		json_string(abc::mem_cache* cache)
		{
			_m_value.allocator()._m_cache = cache;
		}

	private:
		value_type _m_value;
	};
	template <typename character>
	struct json_array : json_value<character>
	{
		friend stringify_helper<character>;
		friend json_document<character>;
		typedef json_value<character> json_value;
		typedef array_type value_type;
		typedef json_document<character> json_document;
		typedef make_helper<character> make_helper;

		json_array(json_document* doc);
		json_array(abc::mem_cache* cache)
		{
			_m_value.allocator()._m_cache = cache;
		}
		~json_array()
		{
			for (value_type::const_iterator i=_m_value.begin();i!=_m_value.end();++i)
				(*i)->~json_value();
		}

		//typedef value_type::iterator iterator;
		typedef typename value_type::const_iterator const_iterator;
		virtual json_type get_type() const {return __json_type_array;}
		virtual bool is_valid() const {return true;}
		virtual bool is_array() const {return true;}
		virtual json_array& as_array() {return *this;}
		virtual json_array const& as_array() const {return *this;}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition) {std_string str;stringify_helper<character>::stringify(this,*condition,str);return str;}
		virtual json_value const& operator[](size_t index) const
		{
			const_iterator i = find(index);
			if (i==_m_value.end())
				return null_value();
			return *(*i);
		}
		virtual json_value const& operator[](str_t) const
		{
			return null_value();
		}
		const_iterator find(size_t index) const
		{
			return _m_value.of(index);
		}
		//iterator begin() {return _m_value.begin();}
		//iterator end() {return _m_value.end();}
		const_iterator begin() const {return _m_value.begin();}
		const_iterator end() const {return _m_value.end();}
		bool is_empty() const {return _m_value.is_empty();}
		size_t size() const {return _m_value.size();}

		json_null* push_back_null()
		{
			json_null* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_null();
			if (!push_back(jvalue)) return 0;
			return jvalue;
		}
		json_bool* push_back(bool b)
		{
			json_bool* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_bool(b);
			if (!push_back(jvalue)) return 0;
			return jvalue;
		}
		json_number* push_back(double num)
		{
			json_number* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_number(num);
			if (!push_back(jvalue)) return 0;
			return jvalue;
		}
		json_string* push_back(character const* str)
		{
			if (!str) return 0;
			json_string* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_string(str);
			if (!push_back(jvalue)) return 0;
			return jvalue;
		}
		json_value* push_back(json_value* jvalue)
		{
			if (!jvalue) return 0;
			json_value** p = _m_value.push_back(jvalue);
			return p?*p:0;
		}
		void remove(size_t index)
		{
			_m_value.remove(index);
		}
	private:
		value_type _m_value;
	};
	template <typename character>
	struct json_object : json_value<character>
	{
		friend stringify_helper<character>;
		friend json_document<character>;
		typedef json_document<character> json_document;
		typedef json_value<character> json_value;
		typedef inner_object_type<character> inner_object_type;
		typedef typename complex_types<character>::object_type object_type;
		typedef object_type value_type;
		typedef typename object_type::const_iterator const_iterator;
		typedef make_helper<character> make_helper;

		json_object(json_document* doc);
		json_object(abc::mem_cache* cache)
		{
			_m_value.allocator()._m_cache = cache;
		}
		~json_object()
		{
			for (value_type::const_iterator i=_m_value.begin();i!=_m_value.end();++i)
			{
				if (i->second)
					i->second->~json_value();
			}
		}

		virtual json_type get_type() const {return __json_type_object;}
		virtual bool is_valid() const {return true;}
		virtual bool is_object() const {return true;}
		virtual json_object& as_object() {return *this;}
		virtual json_object const& as_object() const {return *this;}
		json_value const& operator[](size_t) const {return null_value();}
		json_value const& operator[](str_t key) const
		{
			const_iterator i = find(key.str);
			if (i!=end())
				if (i->second)
					return *i->second;
			return null_value();
		}
		virtual std_string stringify() const {std_string str;return stringify(str);}
		virtual std_string& stringify(std_string& str) const {stringify_helper<character>::stringify(this,str);return str;}
		virtual std_string stringify(stringify_interface* condition) {std_string str; stringify_helper<character>::stringify(this,*condition,str);return str;}


		bool empty() const {return _m_value.is_empty();}
		size_t size() const {return _m_value.size();}
		const_iterator begin() const {return _m_value.begin();}
		const_iterator end() const {return _m_value.end();}
		const_iterator find(strptr_t const& str) const
		{
			value_type::const_iterator i = _m_value.begin();
			value_type::const_iterator e = _m_value.end();
			for (;i!=e;++i)
				if (isstreq(i->first,str))
					return i;
			return e;
		}
		const_iterator find(character const* str) const
		{
			if (!str) return end();
			strptr_t strptr = {(character*)str,abc::helper::strlen(str)};
			return find(strptr);
		}
		json_value const& find(std_string const& str) const
		{
			if (str.empty()) return end();
			strptr_t strptr = {(character*)str.data(),str.length()};
			return find(strptr);
		}
		json_value* push_back(character const* key, json_value* value)
		{
			if (!key) return 0;
			strptr2_t str;
			str.begin = const_cast<character*>(key);
			str.size = abc::helper::strlen(key);
			inner_object_type inner_obj (str,value);
			const_iterator i = _m_value.find(inner_obj);
			if (i==_m_value.end())
				_m_value.push_back(inner_obj);
			else
				i->second = value;
			return value;
		}

		json_null* push_back_null(character const* key)
		{
			if (!key) return 0;
			json_null* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_null();
			if (!jvalue)
				return 0;
			if (!push_back(key,jvalue))
				return 0;
			return jvalue;
		}
		json_bool* push_back(character const* key, bool b)
		{
			if (!key) return 0;
			json_bool* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_bool(b);
			if (!jvalue)
				return 0;
			if (!push_back(key,jvalue))
				return 0;
			return jvalue;
		}
		json_number* push_back(character const* key, double num)
		{
			if (!key) return 0;
			json_number* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_number(num);
			if (!jvalue)
				return 0;
			if (!push_back(key,jvalue))
				return 0;
			return jvalue;
		}
		json_string* push_back(character const* key, character const* str)
		{
			if (!key) return 0;
			json_string* jvalue = make_helper(_m_value.allocator()._m_cache).make_json_string(str);
			if (!jvalue)
				return 0;
			if (!push_back(key,jvalue))
				return 0;
			return jvalue;
		}
	private:
		value_type _m_value;
	};

	template <typename character>
	struct json_document : make_helper<character>
	{
		friend json_string;
		friend json_array;
		friend json_object;
		typedef abc::mem_cache mem_cache;
		typedef abc::linecache<character,json_allocator> linecache;
		typedef std::basic_string<character> std_string;
		typedef typename complex_types<character>::string_type string_type;
		typedef typename complex_types<character>::inner_object_type inner_object_type;
		typedef abc::strptr_tt<character> strptr_t;

		typedef typename json_array::const_iterator array_const_iterator;
		typedef typename json_object::const_iterator object_const_iterator;

		typedef make_helper<character> make_helper;

		json_document()
			: _m_root(0)
			, make_helper(&_m_memcache)
		{
			_m_linecache.allocator()._m_cache = &_m_memcache;
			_m_linecache.reserve(32);
		}
		~json_document()
		{
			if(_m_root) _m_root->~json_value();
		}
		json_value& parse(character const* data,size_t len)
		{
			_m_memcache._m_each_size = len>>3;
			return parse(data);
		}
		json_value& parse(const character* data)
		{
			reset();
			character const* internal_data = data;
			if (internal_data)
				_m_root = internal_parse(internal_data);
			return _m_root?*_m_root:json_value::null_value();
		}

		array_const_iterator find(size_t index) const
		{
			if (!_m_root)
				return array_const_iterator();
			return _m_root->as_array().find(index);
		}

		object_const_iterator find(character const* str) const
		{
			if (!_m_root)
				return object_const_iterator();
			return _m_root->as_object().find(str);
		}

		json_value& root()
		{
			if (!_m_root)
				return json_value::null_value();
			return *_m_root;
		}
		json_value* push_back(json_value* jvalue)
		{
			if  (_m_root) return _m_root;
			return _m_root = jvalue;
		}
		std_string stringify()
		{
			std_string str;
			stringify_helper<character>::stringify(_m_root,str);
			return str;
		}

	private:
		bool extrace_string(const character*& data, string_type& str);
		bool extrace_string(const character*& data, linecache& str);
		json_value* internal_parse(const character*& data);

		void reset() {this->~json_document();new (this) json_document();_m_root=0;}

	private:
		static character const* str_null() {return const_string<character>::null_string();}
		static character const* str_true() {return const_string<character>::true_string();}
		static character const* str_false() {return const_string<character>::false_string();}
		mem_cache _m_memcache;
		json_value* _m_root;
	public:
		linecache _m_linecache;
	};
};



typedef json::json_value<wchar_t> wjson_value;
typedef json::json_null<wchar_t> wjson_null;
typedef json::json_bool<wchar_t> wjson_bool;
typedef json::json_number<wchar_t> wjson_number;
typedef json::json_string<wchar_t> wjson_string;
typedef json::json_array<wchar_t> wjson_array;
typedef json::json_object<wchar_t> wjson_object;
typedef json::json_document<wchar_t> wjson_document;
typedef wjson_value::stringify_interface wjson_stringify_interface;

typedef json::json_value<char> ajson_value;
typedef json::json_null<char> ajson_null;
typedef json::json_bool<char> ajson_bool;
typedef json::json_number<char> ajson_number;
typedef json::json_string<char> ajson_string;
typedef json::json_array<char> ajson_array;
typedef json::json_object<char> ajson_object;
typedef json::json_document<char> ajson_document;
typedef ajson_value::stringify_interface ajson_stringify_interface;

//typedef json::json_value<wchar_t> JSONValue;
//typedef json::json_null<wchar_t> JSONNull;
//typedef json::json_bool<wchar_t> JSONBool;
//typedef json::json_number<wchar_t> JSONNumber;
//typedef json::json_string<wchar_t> JSONString;
//typedef json::json_array<wchar_t> JSONArray;
//typedef json::json_object<wchar_t> JSONObject;
//typedef json::json_document<wchar_t> JSONDocument;
//typedef JSONValue::stringify_interface StringifyInterface;
//
//typedef json::json_value<char> JSONValueA;
//typedef json::json_null<char> JSONNullA;
//typedef json::json_bool<char> JSONBoolA;
//typedef json::json_number<char> JSONNumberA;
//typedef json::json_string<char> JSONStringA;
//typedef json::json_array<char> JSONArrayA;
//typedef json::json_object<char> JSONObjectA;
//typedef json::json_document<char> JSONDocumentA;
//typedef JSONValueA::stringify_interface StringifyInterfaceA;


template <typename character>
inline json::json_value<character>& json::json_value<character>::null_value()
{
	static json::json_value<character> const local;
	return *const_cast<json_value<character>*>(&local);
}
template <typename character>
inline json::json_array<character>& json::json_value<character>::null_array()
{
	static json_array const local((abc::mem_cache*)(0));
	return *const_cast<json_array*>(&local);
}
template <typename character>
inline json::json_object<character>& json::json_value<character>::null_object()
{
	static json_object const local((abc::mem_cache*)(0));
	return *const_cast<json_object*>(&local);
}

template <typename character>
inline json::json_string<character>::json_string(json::json_document<character>* doc)
{
	_m_value.allocator()._m_cache = &doc->_m_memcache;
}
template <typename character>
inline json::json_array<character>::json_array(json::json_document<character>* doc)
{
	_m_value.allocator()._m_cache = &doc->_m_memcache;
}
template <typename character>
inline json::json_object<character>::json_object(json::json_document<character>* doc)
{
	_m_value.allocator()._m_cache = &doc->_m_memcache;
}

#include <sstream>

template<typename character>
inline json::json_value<character>* json::json_document<character>::internal_parse(const character*& buf)
{
	character const*& data = buf;
	// Is it a string?
	if (*data == '"')
	{
		scope_ptr<json_string> o = make<json_string>(this);
		if (!o)
			return 0;
		if (!extrace_string(++data, o->_m_value))
			return 0;
		return o.detach();
	}
	
	// An object?
	else if (*data == '{')
	{
		scope_ptr<json_object> object = make<json_object>(this);
		if (object.is_empty())
		{
			assert(false);
			return 0;
		}
		
		++data;
	
		while (*data != 0)
		{
			// Whitespace at the start?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// Special case - empty object
			if (object->_m_value.is_empty() && *data == '}')
			{
				++data;
				return object.detach();
			}
			
			// We want a string now...
			inner_object_type* inner = object->_m_value.push_back();
			if (!inner)
				return false;
			if (!extrace_string(++data, _m_linecache))
			{
				assert(false);
				return 0;
			}
			character* keystr = (character*)_m_memcache.allocate((_m_linecache.size()+1)*sizeof(character));
			if (!keystr)
			{
				assert(false);
				return 0;
			}
			abc::helper::memcpy(keystr,_m_linecache.data(),_m_linecache.size());
			inner->first.begin = keystr;
			inner->first.size = _m_linecache.size();
			*(inner->first.begin+inner->first.size)=0;

			// More whitespace?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// Need a : now
			if (*(data++) != ':')
			{
				assert(false);
				return 0;
			}
			
			// More whitespace?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// The value is here			
			json_value *value = internal_parse(data);
			if (value == 0)
			{
				assert(false);
				return 0;
			}
			
			// Add the name:value
			inner->second = value;
			
			// More whitespace?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// End of object?
			if (*data == '}')
			{
				++data;
				return object.detach();
			}
			
			// Want a , now
			if (*data != ',')
			{
				assert(false);
				return 0;
			}
			
			++data;
		}
		assert(false);
		return 0;
	}
	
	// An array?
	else if (*data == '[')
	{
		scope_ptr<json_array> array = make<json_array>(this);
		if (array.is_empty())
			return 0;
		
		(data)++;
		
		while (*data != 0)
		{
			// Whitespace at the start?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// Special case - empty array
			if (array->_m_value.is_empty() && *data == ']')
			{
				++data;
				return array.detach();
			}
			
			// Get the value
			json_value* value = internal_parse(data);
			if (value == 0)
			{
				assert(false);
				return 0;
			}
			
			// Add the value
			array->_m_value.push_back(value);
			
			// More whitespace?
			if (!skip_whitespace(data))
			{
				assert(false);
				return 0;
			}
			
			// End of array?
			if (*data == ']')
			{
				++data;
				return array.detach();
			}
			
			// Want a , now
			if (*data != ',')
			{
				assert(false);
				return 0;
			}
			
			(data)++;
		}
		// Only here if we ran out of data
		assert(false);
		return 0;
	}

	// Is it a boolean?
	else if ((abc::is_length_ge<4>(data) && abc::helper::strnicmp(data, str_true(), 4) == 0) ||
		(abc::is_length_ge<5>(data) && abc::helper::strnicmp(data, str_false(), 5) == 0))
	{
		bool value = abc::helper::strnicmp(data, str_true(), 4) == 0;
		(data) += value ? 4 : 5;
		json_bool* o = make<json_bool>(value);
		return o;
	}

	// Is it a null?
	else if (abc::is_length_ge<4>(data) && abc::helper::strnicmp(data, str_null(), 4) == 0)
	{
		(data) += 4;
		json_null* o = make<json_null>();
		return o;
	}

	// Is it a number?
	else if (*data == '-' || (*data >= '0' && *data <= '9'))
	{
		// Negative?
		bool neg = *data == '-';
		if (neg) (data)++;

		double number = 0.0;

		// parse the whole part of the number - only if it wasn't 0
		if (*data == '0')
			(data)++;
		else if (*data >= '1' && *data <= '9')
			number = (double)parse_int(data);
		else
			return 0;

		// Could be a decimal now...
		if (*data == '.')
		{
			(data)++;

			// Not get any digits?
			if (!(*data >= '0' && *data <= '9'))
				return 0;

			// Find the decimal and sort the decimal place out
			double decimal = (double)parse_int(data);
			while((int)decimal > 0) decimal /= 10.0f;

			// Save the number
			number += decimal;
		}

		// Could be an exponent now...
		if (*data == 'E' || *data == 'e')
		{
			(data)++;

			// Check signage of expo
			bool neg_expo = false;
			if (*data == '-' || *data == '+')
			{
				neg_expo = *data == '-';
				(data)++;
			}

			// Not get any digits?
			if (!(*data >= '0' && *data <= '9'))
				return 0;

			// Sort the expo out
			int expo = parse_int(data);
			for (int i = 0; i < expo; i++)
				number = neg_expo ? (number / 10.0) : (number * 10);
		}

		// Was it neg?
		if (neg) number *= -1;

		json_number* o = make<json_number>(number);
		return o;
	}

	// Ran out of possibilities, it's bad!
	else
	{
		return 0;
	}
}


template <typename character> template<typename condition_tn>
inline bool json::stringify_helper<character>::stringify(
	json::json_value<character> const* node,condition_tn& condition,std::basic_string<character>& stradded)
{
	if (!node || !condition(node)) return false;
	
	switch (node->get_type())
	{
	case __json_type_null:
		stradded += const_string<character>::null_string();
		break;
		
	case __json_type_string:
		stringify_string(node->as_stringptrs(),stradded);
		break;
		
	case __json_type_bool:
		stradded += node->as_bool() ? const_string<character>::true_string() : const_string<character>::false_string();
		break;
		
	case __json_type_number:
		{
			if (isinf(node->as_number()) || isnan(node->as_number()))
				stradded += const_string<character>::null_string();
			else
			{
				std_stringstream ss;
				ss << node->as_number();
				stradded += ss.str();
			}
			break;
		}
		
	case __json_type_array:
		{
			stradded += '[';
			array_type const& array_value = node->as_array()._m_value;
			array_type::const_iterator iter = array_value.begin();
			while (iter != array_value.end())
			{
				if (!stringify(*iter,condition,stradded))
				{
					++iter;
					continue;
				}
				// Not at the end - add a separator
				if (++iter != array_value.end())
					stradded += ',';
			}
			//if (stradded.back()==',') stradded.pop_back();
			if (string_back(stradded)==',') string_pop_back(stradded);
			stradded += ']';
			break;
		}
		
	case __json_type_object:
		{
			stradded += '{';
			object_type const& object_value = node->as_object()._m_value;
			object_type::const_iterator iter = object_value.begin();
			while (iter != object_value.end())
			{
				stringify_string(iter->first,stradded);
				stradded += ':';
				stringify(iter->second,condition,stradded);
				
				// Not at the end - add a separator
				if (++iter != object_value.end())
					stradded += ',';
			}
			stradded += '}';
			break;
		}
	default:
		;
	}

	return true;
}

template <typename character>
inline void json::stringify_helper<character>::stringify_string(
	abc::strptr_tt<character> const& str,std::basic_string<character>& stradded)
{
	stradded += '\"';
	
	character const* iter = str.begin;
	character const* end = str.begin+str.size;
	while (iter != end)
	{
		character chr = *iter;

		if (chr == '"' || chr == '\\' || chr == '/')
		{
			stradded += '\\';
			stradded += chr;
		}
		else if (chr == '\b')
		{
			stradded += '\\';
			stradded += 'b';
		}
		else if (chr == '\f')
		{
			stradded += '\\';
			stradded += 'f';
		}
		else if (chr == '\n')
		{
			stradded += '\\';
			stradded += 'n';
		}
		else if (chr == '\r')
		{
			stradded += '\\';
			stradded += 'r';
		}
		else if (chr == '\t')
		{
			stradded += '\\';
			stradded += 't';
		}
		else if (chr < ' ')
		{
			stradded += '\\';
			stradded += 'u';
			for (int i = 0; i < 4; i++)
			{
				int value = (chr >> 12) & 0xf;
				if (value >= 0 && value <= 9)
					stradded += (character)('0' + value);
				else if (value >= 10 && value <= 15)
					stradded += (character)('A' + (value - 10));
				chr <<= 4;
			}
		}
		else
		{
			stradded += chr;
		}
		
		iter++;
	}
	
	stradded += '\"';
}
///// this usage is not supported int vs 2005: typename json::complex_types<character>::string_type
///// string.back() is not added int vs 2005 either
//template <typename character>
//inline void json::stringify_helper<character>::stringify_string(typename json::complex_types<character>::string_type const& str,std::basic_string<character>& stradded)
//{
//	string_type::ptr_const_iterator i = str.ptr_begin();
//	string_type::ptr_const_iterator end = str.ptr_end();
//	for (;i!=end;++i)
//	{
//		stringify_string(*i,stradded);
//	}
//}

template <typename character>
inline bool json::skip_whitespace(const character *&data)
{
	while (*data != 0 && (*data == ' ' || *data == '\t' || *data == '\r' || *data == '\n'))
		(data)++;
	return *data != 0;
}

template <typename character>
inline bool json::json_document<character>::extrace_string(character const*& data, typename json::complex_types<character>::string_type& str2)
{
	character const* begin = data;

	while (*data != 0)
	{
		// Save the char so we can change it if need be
		character next_char = *data;
		
		// Escaping something?
		if (next_char == '\\')
		{
			size_t len = data-begin;
			if (len)
				str2.push_back((character*)begin,len);
			//wchar_t const* estr = data;
			_m_linecache.clear();

			// Move over the escape char

			do {
				(data)++;
				// Deal with the escaped char
				switch (*data)
				{
				case '\'': next_char = '\''; break;
				case '"': next_char = '"'; break;
				case '\\': next_char = '\\'; break;
				case '/': next_char = '/'; break;
				case 'b': next_char = '\b'; break;
				case 'f': next_char = '\f'; break;
				case 'n': next_char = '\n'; break;
				case 'r': next_char = '\r'; break;
				case 't': next_char = '\t'; break;
				case 'u':
					{
						// We need 5 chars (4 hex + the 'u') or its not valid
						if (abc::is_length_l<5>(data))
							return false;
						
						// Deal with the chars
						next_char = 0;
						for (int i = 0; i < 4; i++)
						{
							// Do it first to move off the 'u' and leave us on the 
							// final hex digit as we move on by one later on
							(data)++;
							
							next_char <<= 4;
							
							// parse the hex digit
							if (*data >= '0' && *data <= '9')
								next_char |= (*data - '0');
							else if (*data >= 'A' && *data <= 'F')
								next_char |= (10 + (*data - 'A'));
							else if (*data >= 'a' && *data <= 'f')
								next_char |= (10 + (*data - 'a'));
							else
							{
								// Invalid hex digit = invalid json
								return false;
							}
						}
						break;
					}

					// By the spec, only the above cases are allowed
					default:
						return false;
				}
				_m_linecache.push_back(next_char);
			} while (*(++data)=='\\');

			if (*data==0) /// error
			{
				return false;
			}
			begin = data;
			if (_m_linecache.size()>0)
			{
				character* buf = (character*)_m_memcache.allocate(_m_linecache.size()*sizeof(character));
				abc::helper::memcpy(buf,_m_linecache.data(),_m_linecache.size());
				str2.push_back(buf,_m_linecache.size());
			}
			continue;
		}
		
		// End of the string?
		else if (next_char == '"')
		{
			size_t len = data-begin;
			if (len) str2.push_back((character*)begin,len);
			(data)++;
			//str.length = str_length;
			//str.reserve(); // Remove unused capacity
			return true;
		}
		
		// Disallowed char?
		else if (next_char < ' ' && next_char != '\t')
		{
			// SPEC Violation: Allow tabs due to real world cases
			return false;
		}

		// Move on
		(data)++;
	}
	
	// If we're here, the string ended incorrectly
	return false;
}

template <typename character>
inline bool json::json_document<character>::extrace_string(character const*& data, typename json::json_document<character>::linecache& str)
{
	str.clear();
	
	while (*data != 0)
	{
		// Save the char so we can change it if need be
		character next_char = *data;
		
		// Escaping something?
		if (next_char == '\\')
		{
			// Move over the escape char
			++(data);
			
			// Deal with the escaped char
			switch (*data)
			{
				case '\'': next_char = '\''; break;
				case '"': next_char =  '"'; break;
				case '\\': next_char = '\\'; break;
				case '/': next_char =  '/'; break;
				case 'b': next_char =  '\b'; break;
				case 'f': next_char =  '\f'; break;
				case 'n': next_char =  '\n'; break;
				case 'r': next_char =  '\r'; break;
				case 't': next_char =  '\t'; break;
				case 'u':
				{
					// We need 5 chars (4 hex + the 'u') or its not valid
					/*if (wcslen(*data) < 5)
						return false;*/
					
					// Deal with the chars
					next_char = 0;
					for (int i = 0; i < 4; i++)
					{
						// Do it first to move off the 'u' and leave us on the 
						// final hex digit as we move on by one later on
						++(data);
						if(*data == 0){return false;}
						next_char <<= 4;
						
						// Parse the hex digit
						if (*data >= '0' && *data <= '9')
							next_char |= (*data - '0');
						else if (*data >= 'A' && *data <= 'F')
							next_char |= (10 + (*data - 'A'));
						else if (*data >= 'a' && *data <= 'f')
							next_char |= (10 + (*data - 'a'));
						else
						{
							// Invalid hex digit = invalid JSON
							return false;
						}
					}
					break;
				}
				
				// By the spec, only the above cases are allowed
				default:
					return false;
			}
		}
		
		// End of the string?
		else if (next_char == '"')
		{
			++(data);
			return true;
		}
		
		// Disallowed char?
		else if (next_char < ' ' && next_char != '\t')
		{
			// SPEC Violation: Allow tabs due to real world cases
			return false;
		}
		
		// Add the next char
		str.push_back(next_char);
		
		// Move on
		++(data);
	}
	
	// If we're here, the string ended incorrectly
	return false;
}


template <typename character>
inline int json::parse_int(const character *&data)
{
	int integer = 0;
	while (*data != 0 && *data >= '0' && *data <= '9')
		integer = integer * 10 + (*(data)++ - '0');
	return integer;
}

