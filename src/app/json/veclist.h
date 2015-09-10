#pragma once

#include <new.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "../../met/static_assert.h"


namespace abc
{

struct cppmalloc
{
	char* allocate(size_t size)
	{
		return new (std::nothrow) char [size];
	}
	void deallocate(void* p)
	{
		delete [] p;
	}
};

template <typename allocator_tn>
struct make_helper
{
	typedef allocator_tn allocator_type;
	allocator_type* _m_allocator;
	make_helper(allocator_type* allocator)
		: _m_allocator(allocator)
	{}
	template <typename T>
	T* make_uninitialized()
	{
		assert(_m_allocator);
		char* p = _m_allocator->allocate(sizeof(T));
		return reinterpret_cast<T*>(p);
	}
	template <typename T>
	T* make()
	{
		T* o = make_uninitialized<T>();
		if (!o) return 0;
		new (o) T ();
		return o;
	}
	template <typename T,typename P1>
	T* make(P1 const& p1)
	{
		T* o = make_uninitialized<T>();
		if (!o) return 0;
		new (o) T (p1);
		return o;
	}
};

struct helper
{
	template<typename character> static inline int strnicmp(character const* a,character const* b,size_t n);
	template<typename character> static inline int strncmp(character const* a,character const* b,size_t n);
	template<typename character> static inline int strlen(character const* a);
	template<typename character> static inline character* memcpy(character* d,character const* s,size_t n);
	template<typename character> static inline character* memmove(character* d,character const* s,size_t n);

	template<> static inline int strnicmp<char>(char const* a,char const* b,size_t n)
	{
		return ::_strnicmp(a,b,n);
	}
	template<> static inline int strnicmp<wchar_t>(wchar_t const* a,wchar_t const* b,size_t n)
	{
		return ::_wcsnicmp(a,b,n);
	}
	template<> static  inline int strncmp<char>(char const* a,char const* b,size_t n)
	{
		return ::strncmp(a,b,n);
	}
	template<> static inline int strncmp<wchar_t>(wchar_t const* a,wchar_t const* b,size_t n)
	{
		return ::wcsncmp(a,b,n);
	}
	template<typename character> static inline int strncmp(character const* a,size_t an,character const* b,size_t bn)
	{
		size_t cn = an>bn?an:bn;
		int r = strncmp<character>(a,b,cn);
		if (r!=0 || an==bn)
			return r;
		return an-bn;
	}
	template<> static  inline int strlen<char>(char const* a)
	{
		return ::strlen(a);
	}
	template<> static inline int strlen<wchar_t>(wchar_t const* a)
	{
		return ::wcslen(a);
	}

	template<> static  inline char* memcpy<char>(char* d,char const* s,size_t len)
	{
		return (char*)::memcpy(d,(void const*)(s),len);
	}
	template<> static inline wchar_t* memcpy<wchar_t>(wchar_t* d,wchar_t const* s,size_t len)
	{
		return (wchar_t*)::wmemcpy(d,s,len);
	}
	template<> static  inline char* memmove<char>(char* d,char const* s,size_t len)
	{
		return (char*)::memmove(d,(void const*)(s),len);
	}
	template<> static inline wchar_t* memmove<wchar_t>(wchar_t* d,wchar_t const* s,size_t len)
	{
		return (wchar_t*)::wmemmove(d,s,len);
	}
};


template <typename info_tn=void,typename allocator_tn=cppmalloc>
struct chunk_list : protected allocator_tn
{
	typedef allocator_tn allocator_type;
	typedef char unit;
	template <typename inf_tn>
	struct chunk_tt
	{
		typedef inf_tn inf_type;
		unit* _m_begin;
		size_t _m_size;
		chunk_tt* _m_next;
		inf_type _m_inf;
		unit* begin() { return _m_begin; }
		unit* end() { return _m_begin+_m_size; }
		unit const* begin() const { return _m_begin; }
		unit const* end() const { return _m_begin+_m_size; }
	};
	template <>
	struct chunk_tt <void>
	{
		typedef void inf_type;
		unit* _m_begin;
		size_t _m_size;
		chunk_tt* _m_next;
		unit const* begin() const { return _m_begin; }
		unit const* end() const { return _m_begin+_m_size; }
	};
	typedef chunk_tt<info_tn> chunk;
	chunk_list():_m_head(0) {}
	~chunk_list()
	{
		clear();
	}
	bool is_empty() const  {return _m_head==0;}
	chunk* front() 
	{
		return _m_head;
	}
	chunk const* front() const
	{
		return _m_head;
	}
	chunk* push_front(size_t size)
	{
		char* buffer = allocate_chunk(sizeof(chunk)+size);
		if (!buffer) return 0;
		chunk* chk = (chunk*)buffer;
		chk->_m_begin = buffer + sizeof(chunk);
		chk->_m_size = size;
		chk->_m_next = _m_head;
		_m_head = chk;
		return _m_head;
	}
	void clear()
	{
		chunk* next = 0;
		for (chunk* ch=_m_head;ch;ch=next)
		{
			next = ch->_m_next;
			deallocate_chunk((char*)ch);
		}
		_m_head = 0;
	}
	allocator_type& allocator()
	{
		return *static_cast<allocator_type*>(this);
	}
private:
	char* allocate_chunk(size_t size)
	{
		return allocator_type::allocate(size);
	}
	void deallocate_chunk(char* p)
	{
		allocator_type::deallocate(p);
	}
	chunk* _m_head;
};

struct mem_cache
{
	chunk_list<void> _m_list;
	char* _m_p;
	size_t _m_each_size;

	mem_cache() : _m_p(0),_m_each_size(32*1024) {}
	~mem_cache()
	{}

	size_t get_size(size_t size) const
	{
		return _m_each_size>=size?_m_each_size:size;
	}

	char* allocate(size_t size)
	{
		if (size==0) return 0;
		char* pback = _m_p+size-1;
		if (!_m_p || !_m_list.front() || !validate_infront(pback))
		{
			_m_list.push_front(get_size(size));
			if (_m_list.front())
				_m_p = const_cast<char*>(_m_list.front()->begin());
			pback = _m_p+size-1;
		}
		if (!validate_infront(pback))
			return 0;
		char* retp = _m_p;
		_m_p = pback + 1;
		return retp;
	}

private:
	char* validate_infront(char* p)
	{
		bool isin = p>=_m_list.front()->begin() && p<_m_list.front()->end();
		return isin ? p : 0;
	}
};

template <size_t len,typename character>
static bool is_length_ge(character const* str)
{
	//static_assert(len<=8, "length should be <= 8");
	switch(len)
	{
	case 0: return true;
	case 1: return str[0]&&1;
	case 2: return str[0]&&str[1];
	case 3: return str[0]&&str[1]&&str[2];
	case 4: return str[0]&&str[1]&&str[2]&&str[3];
	case 5: return str[0]&&str[1]&&str[2]&&str[3]&&str[4];
	case 6: return str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5];
	case 7: return str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5]&&str[6];
	case 8: return str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5]&&str[6]&&str[7];
	}
	return false;
}
template <size_t len,typename character>
static bool is_length_g(character const* str)
{
	return is_length_ge<len+1>(str);
}
template <size_t len,typename character>
static bool is_length_le(character const* str)
{
	//static_assert(len<=8, "length should be <= 8");
	switch(len)
	{
	case 0: return !true;
	case 1: return !str[0];
	case 2: return !(str[0]&&str[1]);
	case 3: return !(str[0]&&str[1]&&str[2]);
	case 4: return !(str[0]&&str[1]&&str[2]&&str[3]);
	case 5: return !(str[0]&&str[1]&&str[2]&&str[3]&&str[4]);
	case 6: return !(str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5]);
	case 7: return !(str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5]&&str[6]);
	case 8: return !(str[0]&&str[1]&&str[2]&&str[3]&&str[4]&&str[5]&&str[6]&&str[7]);
	}
	return !false;
}
template <size_t len,typename character>
static bool is_length_l(character const* str)
{
	return is_length_le<len==0?0:len-1>(str);
}

struct getsize_default
{
	size_t operator()(size_t currsize,size_t minsize) const
	{
		(void)(currsize);
		return minsize*3;
	}
};

template <typename value_tn, typename getsize_tn=getsize_default,typename allocator_tn=cppmalloc>
struct veclist : private getsize_tn
{
	typedef value_tn value_type;
	struct info_type
	{
		void* _m_next;
		size_t size;
	};
	typedef allocator_tn allocator_type;
	typedef chunk_list<info_type,allocator_type> chunk_list_type;
	typedef typename chunk_list_type::chunk chunk_type;
	typedef getsize_tn getsize_type;

	veclist() : _m_head(0)
	{}
	~veclist()
	{
		clear();
	}
	void clear()
	{
		iterator i = begin();
		iterator e = end();
		for (;i!=e;++i)
			i.curr->~value_type();
		_m_chunk_list.clear();
		_m_head = 0;
	}
	bool is_empty() const
	{
		return begin()==end();
	}
	size_t size() const
	{
		size_t all = 0;

		chunk_type const* chunk = _m_chunk_list.front();
		for (;chunk;chunk = chunk->_m_next)
			all += chunk->_m_inf.size;

		return all;
	}
	value_type* push_back_unitialized(size_t size)
	{
		size_t bytes = getsize(size*sizeof(value_type));
		chunk_type* chunk = _m_chunk_list.front();
		/// if chunk is empty
		if (!chunk)
		{
			chunk = _m_chunk_list.push_front(bytes);
			if (!chunk)
				return 0;
			chunk->_m_inf.size = 0;
			chunk->_m_inf._m_next = 0;
			_m_head = chunk;
		}
		/// if not enough
		if ((chunk->_m_inf.size+size)*sizeof(value_type)>chunk->_m_size)
		{
			chunk = _m_chunk_list.push_front(bytes);
			if (!chunk)
				return 0;
			chunk->_m_inf.size = 0;
			chunk->_m_inf._m_next = 0;
			chunk->_m_next->_m_inf._m_next = chunk;
		}
		value_type* ret = ((value_type*)chunk->begin()) + chunk->_m_inf.size;
		chunk->_m_inf.size += size;
		return ret;
	}
	value_type* push_back_unitialized()
	{
		return push_back_unitialized(1);
	}
	value_type* push_back()
	{
		value_type* p = push_back_unitialized();
		if (!p)
			return 0;
		return new (p) value_type();
	}
	value_type* push_back(value_type const& v)
	{
		value_type* p = push_back_unitialized();
		if (!p)
			return 0;
		return new (p) value_type(v);
	}
	value_type* front()
	{
		return begin().curr;
	}
	value_type* back()
	{
		return (--_m_chunk_list.end()).curr;
	}
	value_type* append(value_type*v,size_t len)
	{
		value_type* p = push_back_unitialized(len);
		if (!p)
			return 0;
		value_type* ret = p;
		value_type* end = v+len;
		for (value_type* i=v; i<end; ++i)
			new (p++) value_type (*i);
		return ret;
	}
	template <typename chunk_tn,typename vlaue_tn>
	struct iterator_tt
	{
		friend veclist;

		typedef chunk_tn chunk_type;
		typedef value_tn value_type;

		iterator_tt& operator++()
		{
			if (!chunk || !curr)
				return *this;
			value_type* end = (value_type*)(chunk->begin())+chunk->_m_inf.size;
			if (curr+1<end)
			{
				++curr;
				return *this;
			}
			/// next chunk, skip the zero length
			chunk_type* ch = (chunk_type*)(chunk->_m_inf._m_next);
			while (ch && 0==ch->_m_inf.size)
				ch = (chunk_type*)(chunk->_m_inf._m_next);
			if (!ch)
			{
				curr = end;
				return *this;
			}
			chunk = ch;
			curr = (value_type*)(chunk->begin());

			return *this;
		}
		iterator_tt& operator--()
		{
			if (!chunk || !curr)
				return *this;
			value_type* begin = (value_type*)(chunk->begin());
			if (curr>begin)
			{
				--curr;
				return *this;
			}
			/// next chunk, skip the zero length
			chunk_type* ch = chunk->_m_next;
			while (ch && 0==ch->_m_inf.size)
				ch = chunk->_m_next;
			if (!ch)
			{
				//curr = begin;
				return *this;
			}
			chunk = ch;
			curr = (value_type*)(chunk->begin())+(chunk->_m_inf.size - 1);

			return *this;
		}
		iterator_tt operator++(int)
		{
			iterator_tt i = *this;
			operator++();
			return i;
		}
		value_type& operator*()
		{
			return *curr;
		}
		value_type* operator->()
		{
			return curr;
		}
		bool operator==(iterator_tt const& i) const
		{
			return i.chunk == this->chunk && i.curr==this->curr;
		}
		bool operator!=(iterator_tt const& i) const
		{
			return !operator==(i);
		}
		iterator_tt(chunk_type* ch=0,value_type* v=0)
			: chunk(ch)
			, curr(v)
		{}
	protected:
		chunk_type* chunk;
		value_type* curr;
	};

	typedef iterator_tt<chunk_type const,value_type const> const_iterator;

	struct iterator : const_iterator
	{
		friend veclist;
		iterator(chunk_type* ch=0,value_type* v=0)
			: const_iterator(ch,v)
		{}
		chunk_type* get_chunk() {return (chunk_type*)(chunk);}
	};

	/// when the size is zero? skip?
	iterator begin()
	{
		iterator i (_m_head);
		if (!i.chunk)
			return i;

		while(i.chunk && !i.chunk->_m_inf.size)
			i.chunk = (chunk_type*)(i.chunk->_m_inf._m_next);
		if (!i.chunk)
			return end();
		i.curr = (value_type*)(i.chunk->begin());

		return i;
	}
	iterator end()
	{
		iterator i (_m_chunk_list.front(),0);
		if (!i.chunk)
			return i;
		i.curr = (iterator::value_type*)(i.chunk->begin())+i.chunk->_m_inf.size;
		return i;
	}
	const_iterator begin() const
	{
		return const_cast<veclist*>(this)->begin();
	}
	const_iterator end() const
	{
		return const_cast<veclist*>(this)->end();
	}
	iterator find(value_type const& v)
	{
		iterator i = begin();
		iterator e = end();
		for (;i!=e;++i)
			if (*i.curr==v)
				return i;
		return e;
	}
	template<typename cond_tn>
	iterator find_if(cond_tn const& cond)
	{
		iterator i = begin();
		iterator e = end();
		for (;i!=e;++i)
			if (cond(*i.curr))
				return i;
		return e;
	}
	const_iterator of(size_t index) const
	{
		return internal_of(index);
	}
	iterator of(size_t index)
	{
		return *(iterator*)(&(internal_of(index)));
	}
	void remove(size_t index)
	{
		iterator i = of(index);
		return remove(i);
	}
	void remove(const_iterator pos)
	{
		if (pos==end())
			return;
		auto aaa = pos.chunk->end();
		helper::memmove(
			(char*)(pos.curr),
			(char*)(pos.curr+1),
			(endvalue(pos)-pos.curr-1)*sizeof(value_type)
			);
		chunk_type* chk = (chunk_type*)pos.chunk;
		--chk->_m_inf.size;
	}
	allocator_type& allocator()
	{
		return _m_chunk_list.allocator();
	}
protected:
	const_iterator internal_of(size_t index) const
	{
		size_t count = index+1;

		const_iterator i (_m_head);
		if (!i.chunk)
			return i;

		while(i.chunk)
		{
			size_t size = i.chunk->_m_inf.size;
			if (count>size)
				count -= size;
			else if (count<=size)
			{
				i.curr = (value_type*)(i.chunk->begin())+count-1;
				return i;
			}
			i.chunk = (chunk_type*)(i.chunk->_m_inf._m_next);
		}

		return end();
	}
	static value_type* value_address(iterator& i)
	{
		return i.curr;
	}
	static value_type* endvalue(iterator& i)
	{
		return (value_type*)(i.chunk->begin())+i.chunk->_m_inf.size;
	}
	static value_type const* endvalue(const_iterator const& i)
	{
		return (value_type*)(i.chunk->begin())+i.chunk->_m_inf.size;
	}

private:
	size_t getsize(size_t minsize) const
	{
		return (*static_cast<getsize_type const*>(this))(0,minsize);
	}
	chunk_type* _m_head;
	chunk_list_type _m_chunk_list;
};

}

#include <string>

namespace abc
{

template<typename ch_tn>
struct strptr_tt
{
	typedef ch_tn character;
	character* begin;
	size_t size;
	strptr_tt& zero() {begin=0;size=0;}
};
template<typename ch_tn>
struct inteval_tt
{
	typedef ch_tn data_type;
	data_type* begin;
	data_type* end;
	size_t size() const {return end-begin;}
	inteval_tt& zero() {begin=end=0;return *this;}
};

template <typename character_tn, typename getsize_tn=getsize_default,typename allocator_tn=cppmalloc>
struct string_ptrs_tt
	: veclist<strptr_tt<character_tn>,getsize_tn,allocator_tn>
{
	typedef character_tn character;
	typedef strptr_tt<character> value_type;
protected:
	typedef veclist<value_type,getsize_tn,allocator_tn> base;
	typedef string_ptrs_tt self;
public:
	typedef typename base::iterator ptr_iterator;
	typedef typename base::const_iterator ptr_const_iterator;

public:
	/// con
	/// des
	character* push_back(character* str,size_t size)
	{
		value_type v = {str,size};
		value_type* p = base::push_back(v);
		return p->begin;
	}
	character* push_back(character* str)
	{
		return push_back(str,helper::strlen(str));
	}
	character* push_back(value_type const& v)
	{
		return base::push_back(v)->begin;
	}
	struct const_iterator
	{
		friend self;
		const_iterator& operator++()
		{
			if (!p) /// necessary?
				return *this;
			if (p+1 < i->begin+i->size)
				return ++p,*this;
			//value_type* end = base_containter::endvalue(i);
			//if ((++i).operator->()!=end)
			//	p=i->begin;
			//else
			//	++p;
			p = (++i)->begin;
			return *this;
		}
		const_iterator& operator--()
		{
			if (!p)
				return *this;
			if (p > i->begin)
				return --p,*this;
			p = (--i)->begin;
			return *this;
		}
		character const& operator*() const
		{
			return *p;
		}
		character& operator*()
		{
			return *p;
		}
		bool operator==(const_iterator const& o) const
		{
			return i==*(ptr_iterator*)(&o.i) && p==o.p;
		}
		bool operator!=(const_iterator const& o) const
		{
			return !operator==(o);
		}

	protected:
		const_iterator(ptr_const_iterator j,character* ptr=0)
			: i(j), p(ptr)
		{}
		ptr_const_iterator i;
		character* p;
	};
	struct iterator
		: const_iterator
	{
		friend self;
	protected:
		iterator(ptr_iterator j,character* ptr=0)
			: const_iterator(j,ptr)
		{}
	};
	const_iterator begin() const
	{
		const_iterator i(base::begin());
		if (i.i.operator->())
			i.p = i.i->begin;
		return i;
	}
	const_iterator end() const
	{
		const_iterator i(base::end());
		if (i.i.operator->())
			i.p = i.i->begin;
		return i;
	}
	iterator begin()
	{
		return *(iterator*)(&const_cast<self const*>(this)->begin());
	}
	iterator end()
	{
		return *(iterator*)(&const_cast<self const*>(this)->end());
	}
	ptr_iterator ptr_begin()
	{
		return base::begin();
	}
	ptr_iterator ptr_end()
	{
		return base::end();
	}
	ptr_const_iterator ptr_begin() const
	{
		return base::begin();
	}
	ptr_const_iterator ptr_end() const
	{
		return base::end();
	}

	size_t size() const
	{
		size_t size = 0;
		ptr_const_iterator i = ptr_begin();
		ptr_const_iterator end = ptr_end();
		for (;i!=end;++i)
			size += i->size;
		return size;
	}
	character const& nullchar() const
	{
		static character const local = 0;
		return local;
	}
	bool is_empty() const
	{
		return begin()==end();
	}
	iterator find(size_t index)
	{
		long xth = index+1;
		ptr_iterator i = base::begin();
		ptr_iterator end = base::end();
		for (;i!=end;++i)
		{
			if (xth>i->size)
				xth -= i->size;
			else
				return iterator(i,i->begin+(xth-1));
		}
		return iterator(end,end->begin);
	}
	character& operator[](size_t index)
	{
		iterator i = find(index);
		if (i==end())
			return *(character*)(&nullchar());
		return *i;
	}
	bool operator==(self const& o) const
	{
		size_t this_size = size();
		size_t other_size = o.size();
		if (this_size!=other_size)
			return false;

		const_iterator i = begin();
		const_iterator end = end();
		const_iterator oi = o.begin();
		for (;i!=end;++i,++oi)
		{
			if (*i!=*oi)
				return false;
		}
		return true;
	}
	//int cmp(self const& o) const
	//{
	//	size_t this_size = size();
	//	size_t other_size = o.size();
	//	if (this_size!=other_size)
	//		return false;

	//	const_iterator i = begin();
	//	const_iterator end = end();
	//	const_iterator oi = o.begin();
	//	const_iterator oend = o.end();
	//	for (;i!=end;++i,++oi)
	//	{
	//		if (*i!=*oi)
	//			return false;
	//	}
	//	return true;
	//}
	bool iseq(character const* str,size_t len) const
	{
		if (size() != len)
			return false;
		ptr_const_iterator i = base::begin();
		ptr_const_iterator end = base::end();
		character const* p = str;
		for (;i!=end;++i)
		{
			int r = helper::strncmp(p,i->begin,i->size);
			if (0!=r)
				return false;
			p += i->size;
		}
		return true;
	}
	bool isieq(character const* str,size_t len) const
	{
		if (size() != len)
			return false;
		ptr_const_iterator i = base::begin();
		ptr_const_iterator end = base::end();
		character const* p = str;
		for (;i!=end;++i)
		{
			int r = helper::strnicmp(p,i->begin,i->size);
			if (0!=r)
				return false;
			p += i->size;
		}
		return true;
	}
	bool operator==(std::wstring const& str) const
	{
		return iseq(str.data(),str.length());
	}
	bool operator==(character const* str) const
	{
		return iseq(str,helper::strlen(str));
	}
	bool operator!=(self const& o) const
	{
		return !operator==(o);
	}
	bool operator!=(std::wstring const& str) const
	{
		return !operator==(o);
	}
	bool operator!=(character const* str) const
	{
		return !operator==(o);
	}
};

template<typename character_tn,typename allocator_tn=cppmalloc>
struct linecache
	: allocator_tn
{
	typedef character_tn character;
	typedef allocator_tn allocator_type;
	typedef inteval_tt<character> interval_t;

	//static_assert(sizeof(character)==sizeof(char) || sizeof(character)==sizeof(wchar_t), "ch is char or wchar_t");

	linecache()
	{
		_m_interval.zero();
		_m_next = _m_interval.begin;
	}
	~linecache()
	{
		deallocate_ch(_m_interval.begin);
		_m_interval.zero();
		_m_next = 0;
	}
	allocator_type& allocator() {return *(allocator_type*)(this);}
	bool is_empty() const {return !_m_interval.begin !_m_next || _m_interval.size()==0;}
	size_t size() const {return _m_next-_m_interval.begin; }
	void clear()
	{
		_m_next = _m_interval.begin;
	}
	bool reserve(size_t size)
	{
		return 0!=reallocate(size);
	}
	character const* data() const
	{
		return _m_interval.begin;
	}
	character* push_back(character const& c)
	{
		if (!_m_interval.begin)
			if (!reallocate(1))
				return 0;

		if (_m_next+1>_m_interval.end)
			if (!reallocate(_m_interval.size()<<1))
				return 0;

		return *_m_next=c, ++_m_next;
	}
	
protected:
	/// the whole size of the buffer
	character* reallocate(size_t reall_size)
	{
		if (_m_interval.size()>=reall_size)
			return _m_next;
		character* buf = allocate_ch(reall_size);
		if (!buf)
			return 0;
		size_t used = size();
		if (used) helper::memcpy(buf,_m_interval.begin,used);
		deallocate_ch(_m_interval.begin);
		_m_interval.begin = buf;
		_m_interval.end = buf+reall_size;
		_m_next = _m_interval.begin+used;
		return _m_next;
	}
	character* allocate_ch(size_t size)
	{
		return (character*)(allocator().allocate(size*sizeof(character)));
	}
	void deallocate_ch(void* p)
	{
		return allocator().deallocate((char*)p);
	}

private:
	interval_t _m_interval;
	character* _m_next; // the next position
};

}

//#include <windows.h>
//#include <vector>
//namespace jsonall
//{
//	struct tls
//	{
//		static const size_t null = TLS_OUT_OF_INDEXES;
//		static DWORD alloc()  {return TlsAlloc();}
//		static bool free(DWORD id) {return TlsFree(id)?true:false;}
//		static bool set(DWORD id,void* p) {return TlsSetValue(id,p)?true:false;}
//		static void* get(DWORD id) {return TlsGetValue(id);}
//	};
//
//	struct threadids
//	{
//		typedef std::pair<DWORD,DWORD> value_type;
//		std::vector<value_type> _m_tidvector;
//		typedef std::vector<value_type>::iterator I;
//
//		struct auto_cs
//		{
//			auto_cs(CRITICAL_SECTION* cs):_m_cs(cs){::EnterCriticalSection(_m_cs);}
//			~auto_cs(){::LeaveCriticalSection(_m_cs);}
//			CRITICAL_SECTION* _m_cs;
//		};
//		threadids()
//		{
//			init();
//		}
//		~threadids()
//		{
//			lize();
//		}
//		void init()
//		{
//			::InitializeCriticalSection(&_m_cs);
//			_m_tidvector.reserve(32);
//		}
//		void lize()
//		{
//			::DeleteCriticalSection(&_m_cs);
//		}
//		DWORD merge_current()
//		{
//			auto_cs cs(&_m_cs);
//			DWORD currthid = ::GetCurrentThreadId();
//			I iblank;
//			I i = find(currthid,iblank);
//			if (i==_m_tidvector.end())
//			{
//				if (iblank!=_m_tidvector.end())
//				{
//					iblank->first = currthid;
//					iblank->second = tls::null;
//					i = iblank;
//				}
//				_m_tidvector.push_back(value_type(currthid,tls::null));
//				i = _m_tidvector.end();
//				--i;
//			}
//			if (i->second==tls::null)
//			{
//				i->second = tls::alloc();
//				assert(i->second!=tls::null);
//			}
//			return i->second;
//		}
//		void clear_current()
//		{
//			auto_cs cs(&_m_cs);
//			DWORD currthid = ::GetCurrentThreadId();
//			I iblank;
//			I i = find(currthid,iblank);
//			if (i==_m_tidvector.end())
//				return;
//			i->first = 0;
//			i->second = tls::null;
//		}
//		static threadids& instance()
//		{
//			static threadids local;
//			return local;
//		}
//	private:
//		CRITICAL_SECTION _m_cs;
//		I find(DWORD tid,I& out)
//		{
//			out = _m_tidvector.end();
//			I i=_m_tidvector.begin();
//			for (;i!=_m_tidvector.end();++i)
//			{
//				if (out!=_m_tidvector.end() && !i->first)
//					out = i;
//				if (i->second==tid)
//					return i;
//			}
//			return i;
//		}
//	};
//	struct auto_ptr
//	{
//		auto_ptr(threadids& thid):_m_threadids(thid)
//		{
//			_m_id = _m_threadids.merge_current();
//		}
//		~auto_ptr()
//		{
//			_m_threadids.clear_current();
//		}
//		void* get()
//		{
//			return tls::get(_m_id);
//		}
//		DWORD _m_id;
//		threadids& _m_threadids;
//	};
//}


