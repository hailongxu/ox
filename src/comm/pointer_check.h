

#include "../ox/nsab.h"
#include <set>
#include "../mos/scope_raii.h"


#pragma once





___namespace2_begin(ox,comm)


template <typename type_tn>
struct pointer_check
{
	typedef type_tn* pointer;
	typedef std::set<pointer> pointer_set;
	pointer_set _m_set;
	void add(pointer p)
	{
#ifdef _DEBUG
		printf("pointer check ++++ *:%p \r\n",p);
#endif
		pointer_set::iterator i = _m_set.find(p);
		if (i!=_m_set.end())
			DebugBreak();
		_m_set.insert(p);
	}
	void uncheck_add(pointer p)
	{
		_m_set.insert(p);
	}
	bool has_and_remove(pointer p)
	{
		pointer_set::iterator i = _m_set.find(p);
		bool has_one = i!=_m_set.end();
		if (has_one) _m_set.erase(i);
		return has_one;
	}
	bool has(pointer p) const
	{
		pointer_set::const_iterator i = _m_set.find(p);
		return i!=_m_set.end();
	}
	void remove(pointer p)
	{
#ifdef _DEBUG
		printf("pointer check ---- *:%p \r\n",p);
#endif
		pointer_set::iterator i = _m_set.find(p);
		if (i==_m_set.end())
			DebugBreak();
		_m_set.erase(p);
	}
};

template <typename type_tn>
struct safe_pointer_check
{
	ox::mos::critical_section _m_cs;
	pointer_check<type_tn> _m_check;
	typedef type_tn* pointer;

	void add(pointer p)
	{
		ox::mos::scope_cs lock(_m_cs);
		_m_check.add(p);
	}
	bool has(pointer p) const
	{
		ox::mos::scope_cs lock(_m_cs);
		return _m_check.has(p);
	}
	void remove(pointer p)
	{
		ox::mos::scope_cs lock(_m_cs);
		_m_check.remove(p);
	}
	static safe_pointer_check& instance()
	{
		static safe_pointer_check _static_set;
		return _static_set;
	}
};


struct mybuffer
{
	typedef safe_pointer_check<char> safe_check;
	mybuffer()
		: _m_buffer(0), _m_size(0), _m_id(0), _m_id_destruct(0)
	{}
	~mybuffer()
	{
		if (_m_id_destruct!=0) DebugBreak();
		free(_m_buffer);
		_m_buffer=0,_m_size=0;
		_m_id_destruct = GetCurrentThreadId();
	}
	void append(char const* data,size_t size)
	{
		check_thread();
		char* p = alloc(_m_size+size);
		if (_m_buffer && _m_size) memmove(p,_m_buffer,_m_size);
		memmove(p+_m_size,data,size);
		free(_m_buffer);
		_m_buffer = p;
		_m_size += size;
	}
	void erase(size_t off,size_t size)
	{
		check_thread();
		if (off!=0) DebugBreak();
		erase_left(size);
	}
	void clear()
	{
		check_thread();
		free(_m_buffer);
		_m_buffer = 0;
		_m_size = 0;
	}
	char const* data() const
	{
		check_thread();
		return _m_buffer;
	}
	size_t size() const
	{
		return _m_size;
	}
private:
	void erase_left(size_t size)
	{
		if (size>_m_size) size = _m_size;
		size_t new_size = _m_size-size;
		if (new_size==0) return clear();
		char* p = alloc(new_size);
		memmove(p,_m_buffer+size,new_size);
		free(_m_buffer);
		_m_buffer = p;
		_m_size = new_size;
	}
	void check_thread() const
	{
		if (_m_id==0) return;
		if (_m_id!=GetCurrentThreadId()) DebugBreak();
	}
	static char* alloc(size_t size)
	{
		char* p = (char*)malloc(size);
		if (!p) DebugBreak();
		safe_check::instance().add(p);
		return p;
	}
	static void free(char* p)
	{
		if (!p) return;
		safe_check::instance().remove(p);
		::free(p);
	}
	char* _m_buffer;
	size_t _m_size;
	size_t _m_id;
	size_t _m_id_destruct;
};


___namespace2_end()