#pragma once

#include <new.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "../met/mod.h"


namespace ox
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
	struct nullmalloc
	{
		char* allocate(size_t size)
		{
			return 0;
		}
		void deallocate(void* p)
		{
		}
	};
	template <typename mallocator,size_t size_added_tc>
	struct cppmalloc_more : mallocator
	{
		char* allocate(size_t size)
		{
			return mallocator::allocate(size+size_added_tc);
		}
		void deallocate(void* p)
		{
			mallocator::deallocate(p);
		}
	};
	struct except_address_nothing
	{
		bool operator()(void* ptr) {return false;}
	};
	template <typename mallocator,typename except_address_tn=except_address_nothing>
	struct cppmalloc_except : mallocator
	{
		void deallocate(void* p)
		{
			if (!except_address_tn()(p))
				mallocator::deallocate(p);
		}
	};
	template <typename mallocator,size_t size_preadded_tc,size_t size_added_tc>
	struct cppmalloc_both_more : mallocator
	{
		char* allocate(size_t size)
		{
			char* p = mallocator::allocate(size_preadded_tc+size+size_added_tc);
			if (p) return p+size_preadded_tc;
			return 0;
		}
		void deallocate(void* p)
		{
			if (p) mallocator::deallocate((char*)p-size_preadded_tc);
		}
	};

	template <typename allocator_tn,size_t size_preadded_tc,size_t size_added_tc,typename except_tn>
	struct malloc_both_and_except : allocator_tn /// both and except
	{
		char* allocate(size_t size)
		{
			char* p = allocator_tn::allocate(size_preadded_tc+size+size_added_tc);
			if (p) return p+size_preadded_tc;
			return 0;
		}
		void deallocate(void* ptr)
		{
			if ((except_tn()()+size_preadded_tc)!=ptr)
				allocator_tn::deallocate((char*)ptr-size_preadded_tc);
		}
	};

	template <typename relative_tn>
	struct cppmalloc_ref
	{
		char* allocate(size_t size)
		{
			return static_cast<relative_tn*>(this)->allocator().allocate(size);
		}
		void deallocate(void* p)
		{
			static_cast<relative_tn*>(this)->allocator().deallocate(p);
		}
	};
	template <typename allocator_tn>
	struct malloc_pointer_default
	{
		typedef allocator_tn allocator_type;
		char* allocate(size_t size)
		{
			return _m_allocator->allocate(size);
		}
		void deallocate(void* p)
		{
			_m_allocator->deallocate(p);
		}

		allocator_type* _m_allocator;
	};

	template <typename data_tn,typename allocator_tn>
	struct common_data_tt
		: private allocator_tn
	{
		typedef allocator_tn allocator_type;

		template <typename T>
		struct ptrfreer
		{
			ptrfreer(allocator_type& _allocator,T* ptr): _m_pointer(ptr),_m_allocator(_allocator) {}
			~ptrfreer() {_m_allocator.deallocate(_m_pointer);}
			operator T* () {return _m_pointer;}
			allocator_type& _m_allocator;
			T* _m_pointer;
		};

		allocator_type& allocator() {return *(allocator_type*)(this);}
		allocator_type const& allocator() const {return *(allocator_type*)(this);}

		template <typename T>
		T* make_uninitialized(size_t size)
		{
			char* p = allocator_type::allocate(sizeof(T)*size);
			return reinterpret_cast<T*>(p);
		}
		template <typename T>
		T* make_uninitialized()
		{
			char* p = allocator_type::allocate(sizeof(T));
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
		template <typename T,typename P1,typename P2>
		T* make(P1 const& p1,P2 const& p2)
		{
			T* o = make_uninitialized<T>();
			if (!o) return 0;
			new (o) T (p1,p2);
			return o;
		}

		template <typename T>
		void destroy(T* p)
		{
			if (p) p->~T();
			allocator().deallocate(p);
		}

		void free(void* p) {allocator().deallocate(p);}

		data_tn _m_data;
	};

	template <typename data_tn,typename relative_tn>
	struct common_dataref_tt
	{
		typedef typename relative_tn::allocator_type allocator_type;

		template <typename T>
		struct ptrfreer
		{
			ptrfreer(allocator_type& _allocator,T* ptr): _m_pointer(ptr),_m_allocator(_allocator) {}
			~ptrfreer() {_m_allocator.deallocate(_m_pointer);}
			operator T* () {return _m_pointer;}
			allocator_type& _m_allocator;
			T* _m_pointer;
		};

		allocator_type& allocator() {return static_cast<relative_tn*>(this)->allocator();}

		template <typename T>
		T* make_uninitialized(size_t size)
		{
			char* p = allocator().allocate(sizeof(T)*size);
			return reinterpret_cast<T*>(p);
		}
		template <typename T>
		T* make_uninitialized()
		{
			char* p = allocator().allocate(sizeof(T));
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
		template <typename T,typename P1,typename P2>
		T* make(P1 const& p1,P2 const& p2)
		{
			T* o = make_uninitialized<T>();
			if (!o) return 0;
			new (o) T (p1,p2);
			return o;
		}

		template <typename T>
		void destroy(T* p)
		{
			if (p) p->~T();
			allocator().deallocate(p);
		}

	protected:
		data_tn _m_data;
	};
	//template <typename data_tn,typename allocator_tn>
	//struct common_allref_tt : allocator_tn
	//{
	//	typedef allocator_tn allocator_type;

	//	template <typename T>
	//	struct ptrfreer
	//	{
	//		ptrfreer(allocator_type& _allocator,T* ptr): _m_pointer(ptr),_m_allocator(_allocator) {}
	//		~ptrfreer() {_m_allocator.deallocate(_m_pointer);}
	//		operator T* () {return _m_pointer;}
	//		allocator_type& _m_allocator;
	//		T* _m_pointer;
	//	};

	//	allocator_type& allocator() {return static_cast<allocator_type*>(this);}

	//	template <typename T>
	//	T* make_uninitialized(size_t size)
	//	{
	//		char* p = allocator().allocate(sizeof(T)*size);
	//		return reinterpret_cast<T*>(p);
	//	}
	//	template <typename T>
	//	T* make_uninitialized()
	//	{
	//		char* p = allocator().allocate(sizeof(T));
	//		return reinterpret_cast<T*>(p);
	//	}
	//	template <typename T>
	//	T* make()
	//	{
	//		T* o = make_uninitialized<T>();
	//		if (!o) return 0;
	//		new (o) T ();
	//		return o;
	//	}
	//	template <typename T,typename P1>
	//	T* make(P1 const& p1)
	//	{
	//		T* o = make_uninitialized<T>();
	//		if (!o) return 0;
	//		new (o) T (p1);
	//		return o;
	//	}
	//	template <typename T,typename P1,typename P2>
	//	T* make(P1 const& p1,P2 const& p2)
	//	{
	//		T* o = make_uninitialized<T>();
	//		if (!o) return 0;
	//		new (o) T (p1,p2);
	//		return o;
	//	}

	//	template <typename T>
	//	void destroy(T* p)
	//	{
	//		if (p) p->~T();
	//		allocator().deallocate(p);
	//	}

	//protected:
	//	data_tn* _m_data;
	//};

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
		template <typename T,typename P1,typename P2>
		T* make(P1 const& p1,P2 const& p2)
		{
			T* o = make_uninitialized<T>();
			if (!o) return 0;
			new (o) T (p1,p2);
			return o;
		}
	};

	struct helper
	{
		template<typename character> static inline int strnicmp(character const* a,character const* b,size_t n);
		template<typename character> static inline int strncmp(character const* a,character const* b,size_t n);
		template<typename character> static inline int strlen(character const* a);
		template<typename pod_tn> static inline void memset(pod_tn* d,pod_tn const& value,size_t n)
		{
			for (size_t i=0;i<n;++i) *d++ = value;
		}
		template<typename pod_tn> static inline pod_tn* memcpy(pod_tn* d,pod_tn const* s,size_t n)
		{
			return (pod_tn*)::memcpy(d,s,n*sizeof(pod_tn));
		}
		template<typename pod_tn> static inline pod_tn* memmove(pod_tn* d,pod_tn const* s,size_t n)
		{
			return (pod_tn*)::memmove(d,s,n*sizeof(pod_tn));
		}

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
		template<> static inline void memset<char>(char* d,char const& value,size_t n)
		{
			::memset(d,value,n);
		}
		template<> static inline void memset<wchar_t>(wchar_t* d,wchar_t const& value,size_t n)
		{
			::wmemset(d,value,n);
		}
	};

	template <size_t len,typename character>
	static bool is_length_ge(character const* str)
	{
		static_assert(len<=8, "length should be <= 8");
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
		static_assert(len<=8, "length should be <= 8");
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
} /// end of namespace


///
/// line container

#include "typetrait.h"

namespace ox
{
	/*
		noder++
		++noder
		noder.value()->value&
		noder.at(i)->noder
		noder.cmp(v1,v2)->int
		noder.middle(noder)->noder
		noder==noder
		noder!=noder
	*/

	struct compare_default
	{
		template <typename t> int cmp(t const& a,t const& b) const {return a-b;}
		//template <> int cmp<int>(int const& a,int const& b) const {return a-b;}
		template <typename t>
		int operator()(t const& a, t const& b) const
		{
			return cmp<t>(a,b);
		}
	};
	template <typename keyof_tn>
	struct key_compare_default
	{
		template <typename t> int cmp(t const& a,t const& b) const {return a-b;}
		//template <> int cmp<int>(int const& a,int const& b) const {return a-b;}
		template <typename t>
		int operator()(t const& a, t const& b) const
		{
			return cmp<t>(keyof_tn()(a),keyof_tn()(b));
		}
	};
	//struct pair_compare_default
	//{
	//	template <typename t> int cmp(t const& a,t const& b) const;
	//	template <> int cmp<int>(int const& a,int const& b) const {return a-b;}
	//	template <typename t>
	//	int operator()(t const& a, t const& b) const
	//	{
	//		return cmp(a.first,b.first);
	//	}
	//};

	template <typename key_tn>
	struct keyof_default
	{
		template <typename value_tn>
		key_tn& operator()(value_tn& value) const {return value;}
		template <typename value_tn>
		key_tn const& operator()(value_tn const& value) const {return value;}
	};
	template <typename key_tn>
	struct keyof_pair_default
	{
		template <typename value_tn>
		key_tn& operator()(value_tn& value) const {return value.first;}
		template <typename value_tn>
		key_tn const& operator()(value_tn const& value) const {return value.first;}
	};

	template <typename keyof_tn,typename key_cmp_tn,typename noder_tn,typename key_tn>
	static inline noder_tn binary_search(noder_tn begin, noder_tn end,key_tn const& key,keyof_tn const& keyof,key_cmp_tn const& cmp)
	{
		while(begin!=end)
		{
			noder_tn middle = begin+(end-begin)/2;
			int r = cmp(key,keyof(*middle));
			if (r<0)
				end = middle;
			else if (r>0)
				begin = ++middle;
			else
				return middle;
		}
		return noder_tn();///null
	}
	template <typename keyof_tn,typename key_cmp_tn,typename noder_tn,typename key_tn>
	static inline pair<noder_tn,bool> try_binary_search(noder_tn begin, noder_tn end,key_tn const& key,keyof_tn const& keyof,key_cmp_tn const& cmp)
	{
		while(begin!=end)
		{
			noder_tn middle = begin+(end-begin)/2;
			int r = cmp(key,keyof(*middle));
			if (r<0)
				end = middle;
			else if (r>0)
				begin = ++middle;
			else
				return pair<noder_tn,bool>(middle,true);
		}
		return pair<noder_tn,bool>(end,false);///null
	}
	template <typename keycmp_tn,typename noder_tn,typename value_tn>
	static inline noder_tn binary_search(noder_tn begin, noder_tn end,value_tn const& value,keycmp_tn const cmp)
	{
		return binary_search(begin,end,value,keyof_default<value_tn>(),cmp);
	}
	template <typename keycmp_tn,typename noder_tn,typename value_tn>
	static inline pair<noder_tn,bool> try_binary_search(noder_tn begin, noder_tn end,value_tn const& value,keycmp_tn const cmp)
	{
		return try_binary_search(begin,end,value,keyof_default<value_tn>(),cmp);
	}
	template <typename noder_tn,typename value_tn>
	static inline noder_tn binary_search(noder_tn begin, noder_tn end,value_tn const& value)
	{
		keyof_default<value_tn>();
		return binary_search(begin,end,value,keyof_default<value_tn>(),compare_default());
		return noder_tn();
	}
	template <typename keycmp_tn,typename noder_tn,typename value_tn>
	static inline pair<noder_tn,bool> try_binary_search(noder_tn begin, noder_tn end,value_tn const& value)
	{
		return try_binary_search(begin,end,value,keyof_default<value_tn>(),key_compare_default());
	}
	template <typename keyof_tn,typename noder_tn,typename value_tn>
	static inline noder_tn key_binary_search(noder_tn begin, noder_tn end,value_tn const& value,keyof_tn const& keyof)
	{
		return binary_search(begin,end,value,keyof,compare_default());
	}
	template <typename keyof_tn,typename noder_tn,typename value_tn>
	static inline pair<noder_tn,bool> try_key_binary_search(noder_tn begin, noder_tn end,value_tn const& value,keyof_tn const& keyof)
	{
		return try_binary_search(begin,end,value,keyof,compare_default());
	}

	//template <typename value_tn>
	//struct dot_of_line
	//{
	//	typedef value_tn value_type;
	//	typedef value_type& reference;
	//	typedef value_type const const_value;
	//	typedef value_type const& const_reference;
	//	reference operator*() {return *_m_valueptr;}
	//	const_reference operator*() const {return *_m_valueptr;}
	//	reference value() {return *_m_valueptr;}
	//	const_reference value() const {return *_m_valueptr;}
	//	value_type* _m_valueptr;
	//};

	template <typename value_tn>
	struct iterator_of_line
	{
		typedef iterator_of_line self_type;
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const const_value;
		typedef value_type const& const_reference;
		typedef value_type* dot_type;

		self_type() {}
		self_type(dot_type dot):_m_dot(dot) {}
		dot_type _m_dot;
		dot_type dot() {return _m_dot;}
		self_type& operator++() {++_m_dot;return *this;}
		self_type& operator--() {--_m_dot;return *this;}
		reference operator*() {return *_m_dot;}
		const_reference operator*() const {return *_m_dot;}
		reference value() {return *_m_dot;}
		const_reference value() const {return *_m_dot;}
	};
	template <typename dot_tn>
	struct reverse_iterator_of_line
	{
		typedef reverse_iterator_of_line self_type;
		typedef typename dot_tn::value_type value_type;
		typedef value_type& reference;
		typedef value_type const const_value;
		typedef value_type const& const_reference;
		typedef dot_tn dot_type;

		self_type() {}
		self_type(dot_tn dot):_m_dot(dot) {}
		dot_type _m_dot;
		dot_type dot() {return _m_dot;}
		self_type& operator++() {--_m_dot._m_valueptr;return *this;}
		self_type& operator--() {++_m_dot._m_valueptr;return *this;}
		reference operator*() {return _m_dot.value();}
		const_reference operator*() const {return _m_dot.value();}
		reference value() {return _m_dot.value();}
		const_reference value() const {return _m_dot.value();}
	};

	/// {head_t:{capacity,size},data0,data1,data2,,,,,,}
	template <typename head_tn,typename value_tn,typename allocator_tn>
	struct line_value_container_rooter_ref
	{
		typedef line_value_container_rooter_ref self_type;
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef head_tn head_t;

		struct vector_malloc_ref
		{
			vector_malloc_ref(allocator_type* _allocator): _m_allocator(_allocator)
			{}
			static head_t* default_head() {return (head_t*)(get_readonly_memory<sizeof(head_t)>());}
			allocator_type* _m_allocator;
			char* allocate(size_t size)
			{
				char* p = _m_allocator->allocate(sizeof(head_t)+size);
				if (p) return p+sizeof(head_t);
				return 0;
			}
			void deallocate(void* ptr)
			{
				if (((head_t*)(default_head())+1)!=ptr)
					_m_allocator->deallocate((char*)ptr-sizeof(head_t));
			}
		};

	private:
		head_t** _m_headptr;
		vector_malloc_ref _m_allocator;
	public:
		self_type(allocator_type* _allocator=0,head_t** headptr=0)
			: _m_allocator(_allocator)
		{
			assert(headptr);
			_m_headptr = headptr;
			if (!(*_m_headptr))
				*_m_headptr = vector_malloc_ref::default_head();
		}
		allocator_type& allocator() {return *_m_allocator._m_allocator;}
		size_t size() const {assert(_m_headptr && *_m_headptr);return (*_m_headptr)->_m_size;}
		size_t capacity() const {return (*_m_headptr)->_m_capacity;}
		value_type const* begin() const {return (value_type*)((*_m_headptr)+1);}
		value_type const* end() const {return begin()+size();}
		value_type* begin() {return (value_type*)((*_m_headptr)+1);}
		value_type* end() {return begin()+size();}
		void set_begin(value_type* off) {}
		void set_end(value_type* off) {}
		template <typename value_tn>
		value_tn* make_uninitialized(size_t size) {return (value_tn*)(_m_allocator.allocate(size*sizeof(value_tn)));}
		void free(void* p) {_m_allocator.deallocate(p);}
		value_type const* next() const {return begin()+size();}
		value_type* next() {return begin()+size();}
		void set_next(value_type* off) {(*_m_headptr)->_m_size=off-begin();}
		void move_next_relative_distance(int delta) {(*_m_headptr)->_m_size+=delta;}
		void set_next_value(value_type const& value,int off=0) {*(next()+off)=value;}
		void on_size_changed() {}
		void on_buffer_changing(value_type* buf,size_t value_count) {(*_m_headptr)=(head_t*)((char*)buf-sizeof(head_t));(*_m_headptr)->_m_capacity=value_count;}
	};

	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct podline_rooter
		: allocator_tn
	{
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef value_type* dot_type;
		typedef value_type const* cdot_type;
		typedef interval_tt<value_type*> interval_t;
		typedef allocator_tn allocator_type;

		static bool is_owner() {return true;}
		allocator_type& allocator() {return *(allocator_type*)(this);}
		size_t size() const {return _m_next-_m_interval.begin;}
		size_t capacity() const {return _m_interval.size();}
		value_type* begin() const {return _m_interval.begin;}
		value_type* end() const {return _m_interval.end;}
		void set_begin(value_type* off) {_m_interval.begin=off;}
		void set_end(value_type* off) {_m_interval.end=off;}
		template <typename _value_tn> _value_tn* make_uninitialized(size_t size) {return (_value_tn*)allocator().allocate(size*sizeof(_value_tn));}
		void free(void* p) {allocator().deallocate(p);}
		value_type const* next() const {return _m_next;}
		value_type* next() {return _m_next;}
		void set_next(value_type* off) {_m_next=off;}
		void move_next_relative_distance(int delta) {_m_next+=delta;}
		void set_next_value(value_type const& value,int off) {*(_m_next+off)=value;}
		void on_size_changed() {}
		void on_buffer_changing(value_type* buf,size_t value_count) {}
	protected:
		interval_t _m_interval;
		value_type* _m_next; // the next position
	};


	template <typename value_tn,typename size_tn,typename allocator_tn=cppmalloc>
	struct podline2_rooter
		: allocator_tn
	{
		typedef value_tn value_type;
		typedef interval_tt<value_type*> interval_t;
		typedef allocator_tn allocator_type;
		//typedef value_type* dot_type;
		//typedef value_type* iterator;

		static bool is_owner() {return true;}
		allocator_type& allocator() {return *(allocator_type*)(this);}
		size_t size() const {return _m_next-_m_interval.begin;}
		size_t capacity() const {return _m_interval.size();}
		value_type* begin() const {return _m_interval.begin;}
		value_type* end() const {return _m_interval.end;}
		void set_begin(value_type* off) {_m_interval.begin=off;}
		void set_end(value_type* off) {_m_interval.end=off;}
		value_type* make(size_t size) {return (value_type*)allocator().allocate(size*sizeof(value_type));}
		void free(void* p) {allocator().deallocate(p);}
		value_type const* next() const {return begin()+_m_size;}
		value_type* next() {return begin()+_m_size;}
		void set_next(value_type* off) {_m_next=off};
		void move_next_relative_distance(int delta) {_m_next+=delta;}
		void set_next_value(value_type const& value,int off) {*(_m_next+off)=value;}
		void on_size_changed() {}
	private:
		size_tn _m_size; // the next position
		interval_t _m_interval;
	};

	template <typename value_tn,size_t size_tc,typename size_tn,typename allocator_tn=nullmalloc>
	struct podline3_rooter
		: allocator_tn
	{
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		podline3_rooter():_m_size(0) {}
		allocator_type& allocator() {return *(allocator_type*)(this);}
		size_t size() const {return _m_size;}
		size_t capacity() const {return size_tc;}
		value_type const* begin() const {return &_m_value[0];}
		value_type const* end() const {return &_m_value[size_tc];}
		value_type* begin() {return &_m_value[0];}
		value_type* end() {return &_m_value[size_tc];}
		void set_begin(value_type* off) {}
		void set_end(value_type* off) {}
		template <typename _value_tn>
		_value_tn* make_uninitialized(size_t size) {return (_value_tn*)0;}
		void free(void* p) {}
		value_type const* next() const {return begin()+_m_size;}
		value_type* next() {return begin()+_m_size;}
		void set_next(value_type* off) {_m_size=off-begin();};
		void move_next_relative_distance(int delta) {_m_size+=delta;}
		void set_next_value(value_type const& value,int off) {*(next()+off)=value;}
		void on_size_changed() {}
		void on_buffer_changing(value_type* buf,size_t value_count) {}
	private:
		size_tn _m_size; // the next position
		value_tn _m_value[size_tc];
	};

	//template <typename head_tn,typename value_tn,typename allocator_tn>
	//struct line_value_container_rooter_ref;

	template <typename size_tn>
	struct podline_head_tt
	{
		typedef size_t size_type;
		size_type _m_capacity;
		size_type _m_size;
	};


	template <typename value_tn,typename size_tn,typename head_tn=podline_head_tt<size_t>,size_t backmore_tc=0,typename allocator_tn=cppmalloc>
	struct podline4_rooter
		: common_data_tt<head_tn*,
			malloc_both_and_except<allocator_tn,sizeof(head_tn),backmore_tc,readonly_memory<sizeof(head_tn)> >>
	{
		typedef head_tn head_t;
		friend line_value_container_rooter_ref<head_t,value_tn,allocator_tn>;
		typedef malloc_both_and_except<allocator_type,sizeof(podline_head_tt<size_t>),backmore_tc,readonly_memory<sizeof(podline_head_tt<size_t>)>> allocatored_type;

		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		allocator_type& allocator() {return *static_cast<allocator_type*>(this);}
		static bool is_owner() {return true;}
		podline4_rooter() {_m_data = (head_t*)readonly_memory<sizeof(head_tn)>()();}
		bool is_empty() const {return !_m_data||_m_data->_m_size==0;}
		size_t size() const {return _m_data->_m_size;}
		size_t capacity() const {return _m_data->_m_capacity;}
		value_type const* begin() const {return (value_type const*)(_m_data+1);}
		value_type const* end() const {return begin()+_m_data->_m_capacity;}
		value_type* begin() {return (value_type*)(_m_data+1);}
		value_type* end() {return begin()+_m_data->_m_capacity;}
		void set_begin(value_type* off) {}
		void set_end(value_type* off) {}
		value_type const* next() const {return begin()+size();}
		value_type* next() {return begin()+size();}
		void set_next(value_type* off) {_m_data->_m_size=off-begin();};
		void move_next_relative_distance(int delta) {_m_data->_m_size+=delta;}
		void set_next_value(value_type const& value,int off=0) {*(next()+off)=value;}
		void on_size_changed() {}
		void on_buffer_changing(value_type* buf,size_t value_count) {_m_data=(head_t*)((char*)buf-sizeof(head_t));_m_data->_m_capacity=value_count;}
	};

	template <typename value_tn,typename guard_tn,typename allocator_tn=cppmalloc>
	struct strline_rooter
		: podline4_rooter<value_tn,size_t,podline_head_tt<size_t>,sizeof(value_tn),allocator_tn>
	{
		void on_size_changed() {if(next())*next()=guard_tn()();}
	};


	//template <typename other_rooter_tn>
	//struct line_value_container_rooter_ref2
	//	: common_dataref_tt<void,line_value_container_rooter_ref2<other_rooter_tn>>
	//{
	//	typedef other_rooter_tn other_rooter_type;

	//	typedef line_value_container_rooter_ref2 self_type;
	//	typedef typename other_rooter_type::value_type value_type;
	//	typedef typename other_rooter_type::allocator_type allocator_type;
	//	
	//	typedef podline_head_tt<size_t> head_t;
	//private:
	//	other_rooter_type* _m_other_rooter;
	//public:
	//	self_type(other_rooter_type& other): _m_other_rooter(&other) {}
	//	allocator_type& allocator() {return _m_other_rooter->allocator();}
	//	size_t size() const {return _m_other_rooter->size();}
	//	size_t capacity() const {return _m_other_rooter->capacity();}
	//	value_type const* begin() const {return _m_other_rooter->begin();}
	//	value_type const* end() const {return _m_other_rooter->end();}
	//	value_type* begin() {return _m_other_rooter->begin();}
	//	value_type* end() {return _m_other_rooter->end();}
	//	void set_begin(value_type* off) {_m_other_rooter->set_begin(off);}
	//	void set_end(value_type* off) {_m_other_rooter->set_end(off);}
	//	//template <typename value_tn>
	//	//value_tn* make_uninitialized(size_t size) {return _m_other_rooter->make_uninitialized<value_tn>(size);}
	//	//void free(void* p) {_m_other_rooter->free(p);}
	//	value_type const* next() const {return _m_other_rooter->next();}
	//	value_type* next() {return _m_other_rooter->next();}
	//	void set_next(value_type* off) {_m_other_rooter->set_next(off);}
	//	void move_next_relative_distance(int delta) {_m_other_rooter->move_next_relative_distance(delta);}
	//	void set_next_value(value_type const& value,int off=0) {_m_other_rooter->set_next_value(value,off);}
	//	void on_size_changed() {_m_other_rooter->on_size_changed();}
	//	void on_buffer_changing(value_type*,size_t) {_m_other_rooter->on_buffer_changing();}
	//};

	template<typename rooter_tn> struct line_value_center
	{
		typedef line_value_center self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef value_type node_type;
		typedef value_type* dot_type;
		typedef size_t offset_t;

		self_type() {}
		self_type(self_type&& other): _m_rooter(other.rooter()) {}
		self_type(self_type const&& other): _m_rooter(other.rooter()) {}
		self_type(self_type const& other): _m_rooter(other.rooter()) {}
		self_type(rooter_type&& _rooter): _m_rooter(_rooter) {}
		self_type(rooter_type const&& _rooter): _m_rooter(_rooter) {}
		self_type(rooter_type const& _rooter): _m_rooter(_rooter) {}
		template <typename p1> self_type(p1&& v1): _m_rooter(forward<p1>(v1)) {}
		template <typename p1,typename p2> self_type(p1&& v1,p2&& v2): _m_rooter(forward<p1>(v1),forward<p2>(v2)) {}

		rooter_type& rooter() {return _m_rooter;}
		rooter_type const& rooter() const {return _m_rooter;}

		bool is_empty() const {return rooter().begin()==0 || rooter().size()==0;}
		size_t size() const {return rooter().size(); }
		size_t capacity() const {return rooter().capacity();}
		void clear()
		{
			rooter().nexter() = _m_interval.begin();
		}
		bool reserve(size_t size)
		{
			return 0!=reallocate(size);
		}
		void resize_uninitialized(size_t size)
		{
			insert_uninitialized<true>(this->size(),size);
		}
		void resize_pad(size_t size,value_type const& valuepaded)
		{
			size_t oldsize = this->size();
			if (size<oldsize)
				rooter().set_next(size);
			else if (insert_uninitialized<false>(oldsize,size-oldsize))
				helper::memset(rooter().begin()+oldsize,valuepaded,size-oldsize);
			else
				return;
			if (size!=oldsize) rooter().on_size_changed();
		}
		value_type const* data() const
		{
			return rooter().begin();
		}
		value_type* push_front(value_type const& c)
		{
			return insert(size_t(0),c);
		}
		value_type* push_back(value_type const& c)
		{
			if (!insert_uninitialized<false>(-1,1))
				return 0;
			rooter().set_next_value(c,-1);
			rooter().on_size_changed();
			return rooter().next()-1;
		}
		value_type const* pop_back()
		{
			return remove_back();
		}
		value_type* remove_front()
		{
			return internal_remove_front();
		}
		value_type* internal_remove_front()
		{
			return internal_remove_at(begin());
		}
		value_type* internal_remove_back()
		{
			rooter().move_next_relative_distance(-1);
			return rooter().next();
		}
		value_type const* remove_back()
		{
			if (is_empty()) return end();
			value_type const* ret = rooter().next();
			rooter().move_next_relative_distance(-1);
			rooter().on_size_changed();
			return ret;
		}
		void sub(value_type* begin,value_type* end,self_type& newliner)
		{
			assert(begin>=rooter().begin() && begin<=rooter().next());
			assert(begin<=end);
			assert(end>=rooter().begin() && begin<=rooter().next());
			size_t size = rooter().next()-begin;
			newliner.resize_uninitialized(size);
			helper::memcpy(newliner.rooter().begin(),begin,size);
		}
		self_type sub(value_type* begin,value_type* end)
		{
			self_type newliner;
			sub(begin,end,newliner);
			return abc::move(newliner);
		}
		void split(value_type* prev,self_type& newliner)
		{
			sub(prev,rooter().next(),newliner);
			rooter().set_next(prev);
			rooter().on_size_changed();
		}
		self_type split(value_type* prev)
		{
			self_type&& rline = sub(prev,rooter().next());
			rooter().nexter() = prev;
			rooter().on_size_changed();
			return rline;
		}
		self_type split(size_t off)
		{
			return split(rooter().begin()+off);
		}
		void split(size_t off,self_type& newliner)
		{
			split(rooter().begin()+off,newliner);
		}

		template <bool event_enabled>
		value_type* insert_uninitialized(size_t offsetbefore,size_t count)
		{
			size_t _size = size();
			if (offsetbefore>size()) offsetbefore=_size;
			size_t const newsize = _size+count;
			size_t beforesize = offsetbefore;
			value_type* before = begin()+offsetbefore;

			if (newsize<=capacity())
			{
				helper::memmove(before+count,before,end()-before);
				rooter().move_next_relative_distance(count);
				if (event_enabled) rooter().on_size_changed();
				return before;
			}
			else
			{
				size_t aftersize = _size-beforesize;
				value_type* newbegin = rooter().make_uninitialized<value_type>(newsize);
				if (!newbegin)
				{
					//assert(newbegin && "memory allocate failed");
					return 0;
				}
				helper::memcpy(newbegin,begin(),beforesize);
				helper::memcpy(newbegin+(beforesize+count),before,aftersize);
				rooter().free(rooter().begin());
				rooter().on_buffer_changing(newbegin,newsize);

				rooter().set_begin(newbegin);
				rooter().set_end(newbegin+newsize);
				rooter().set_next(rooter().begin()+_size+count);

				if (event_enabled) rooter().on_size_changed();
				return newbegin+beforesize;
			}
		}
		value_type* insert(size_t offset,value_type const& v)
		{
			return insert(offset,1,v);
		}
		value_type* insert(value_type* before,value_type const& v)
		{
			return insert(before,1,v);
		}
		value_type* insert(value_type* before,size_t const& count,value_type const& v)
		{
			return insert(distance(before),count,v);
		}
		value_type* insert(size_t before,size_t const& count,value_type const& v)
		{
			assert(before<=size());
			value_type* ret = insert_uninitialized<false>(before,count);
			if (!ret) return 0;
			helper::memset(ret,v,count);
			rooter().on_size_changed();
			return ret;
		}
		value_type* insert(size_t prevoff,value_type const* _begin,size_t const& size)
		{
			return insert(begin()+prevoff,_begin,size);
		}
		value_type* insert(value_type* before,value_type const* _begin,size_t const& _size)
		{
			size_t beforesize = before-begin();
			value_type* ret = insert_uninitialized<false>(beforesize,_size);
			if (!ret) return 0;
			helper::memmove(ret+beforesize,_begin,_size);
			rooter().on_size_changed();
			return ret;
		}

		self_type& append(value_type const& v)
		{
			if (insert_uninitialized<false>(size(),1))
			{
				*(end()-1) = v;
				rooter().on_size_changed();
			}
			return *this;
		}
		self_type& append(size_t count,value_type const& v)
		{
			if (insert_uninitialized<false>(size(),count))
			{
				helper::memset(end()-count,v,count);
				rooter().on_size_changed();
			}
			return *this;
		}
		self_type& append(value_type const* _begin,size_t count)
		{
			if (insert_uninitialized<false>(size(),count))
			{
				helper::memcpy(end()-count,_begin,count);
				rooter().on_size_changed();
			}
			//return begin()+oldsize;
			return *this;
		}
		value_type* remove_at(size_t off,size_t count=1)
		{
			if (off>=size()) return 0;
			if (off+count>size()) count = size()-off;
			return internal_remove(off,count);
		}
		template <typename action_tn>
		value_type* remove_at(size_t off,size_t count,action_tn& action)
		{
			if (off>=size()) return 0;
			if (off+count>size()) count = size()-off;
			return internal_remove(begin()+off,count);
		}
		value_type* internal_remove_at(size_t off)
		{
			return internal_remove_at(off);
		}
		value_type* internal_remove_at(value_type const* pos)
		{
			return internal_remove(distance(pos),1);
		}
		value_type* internal_remove(size_t off,size_t count=1)
		{
			assert(off<size());
			size_t before_size = off;
			helper::memmove(begin()+off,begin()+off+count,size()-off-count);
			rooter().move_next_relative_distance(-(long)count);
			rooter().on_size_changed();
			return begin()+off;
		}
		void replace(value_type* pos,value_type const& newvalue)
		{
			*pos = newvalue;
		}
		void replace(size_t off,value_type const& value)
		{
			*at(off) = value;
		}
		value_type* begin() {return rooter().begin();}
		value_type* end() {return rooter().next();}
		value_type const* begin() const {return rooter().begin();}
		value_type const* end() const {return rooter().next();}
		value_type& front() {return *begin();}
		value_type& back() {return *(end()-1);}
		value_type* fronter() {return rooter().begin();}
		value_type* backer() {return &back();}
		value_type* ender() {return rooter().next();}
		value_type const* fronter() const {return rooter().begin();}
		value_type const* backer() const {return &back();}
		value_type const* ender() const {return rooter().next();}
		value_type const* cfronter() {return rooter().begin();}
		value_type const* cbacker() {return &back();}
		value_type const* cender() {return rooter().next();}
		value_type* at(size_t i) {assert(i>=0&&i<size());return rooter().begin()+i;}
		value_type const* at(size_t i) const {assert(i>=0&&i<size());return rooter().begin()+i;}
		size_t distance(value_type const* pos) const {return pos-rooter().begin();}
		bool is_between(value_type const* pos) const {return pos>=fronter() && pos<ender();}

	private:
		rooter_tn _m_rooter;
	};

	/// just provide the construct and destruct, and operator assignment which are different with owner container
	/// we can also inherit the construction from base, but, the function will be ok at C++0x14, corresponding to VC2013
	template <typename rooter_tn>
	struct line_value_container_owner
		: line_value_center<rooter_tn>
	{
		typedef line_value_center<rooter_tn> base_type;
		typedef line_value_container_owner self_type;

		~line_value_container_owner()
		{
			rooter().deallocate(rooter().begin());
			rooter().set_begin(0);
			rooter().set_end(0);
		}
		self_type()
		{
			rooter().set_begin(0);
			rooter().set_end(0);
			rooter().set_next(rooter().begin());
		}
		//self_type(rooter_tn const& rooter)
		//	: base_type(rooter)
		//{}
		self_type(self_type const& other)
		{
			rooter().set_begin(0);
			rooter().set_end(0);
			rooter().set_next(0);
			if (insert_uninitialized<false>(0,other.size()))
			{
				helper::memcpy<value_type>(begin(),other.begin(),other.size());
				rooter().on_size_changed();
			}
		}
		self_type(self_type&& other)
		{
			rooter() = other.rooter();
			other.rooter().set_begin(0);
			other.rooter().set_end(0);
			other.rooter().set_next(0);
		}
		self_type& operator=(self_type&& other)
		{
			this->~self_type();
			rooter() = other.rooter();
			return *this;
		}
	};

	///// the reference container should not manager the referenced object life-cycle
	//template <typename rooter_tn>
	//struct line_value_container_ref
	//	: line_value_center<rooter_tn>
	//{
	//	typedef line_value_center<rooter_tn> base_type;
	//	typedef line_value_container_ref self_type;
	//	self_type () {}
	//	self_type (line_value_container_ref&& other)
	//		: base_type(other.rooter())
	//	{}
	//	template <typename param_tn> self_type (param_tn&& param)
	//		: base_type(forward<param_tn>(param))
	//	{
	//		//rooter().set_param(param);
	//	}
	//};


	template<typename rooter_tn,typename keyof_tn,typename cmp_tn>
	struct sortedline_value_center
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef value_type node_type;
		typedef value_type* dot_type;
		typedef keyof_tn keyof_type;
		typedef cmp_tn compare_type;

	private:
		typedef line_value_center<rooter_tn> line_value_center;
		typedef sortedline_value_center self_type;

		line_value_center _m_line;
	public:
		/// inherits from the base class
/*		self_type()
		{}
		~ self_type()
		{}
		self_type(rooter_tn const& rooter)
			: line_value_center(rooter)
		{}
		self_type(self_type const& other)
		{
			resize_uninitialized(other.size());
			helper::memcpy<char>((char*)begin(),(char*)other.begin(),other.size()*sizeof(value_type));
		}
		self_type(self_type&& other)
		{
			_m_rooter = other._m_rooter;
			other._m_rooter.zero();
		}*/

		self_type() {}
		self_type(self_type&& other): _m_line(other.rooter()) {}
		self_type(self_type const&& other): _m_line(other.rooter()) {}
		self_type(self_type const& other): _m_line(other.rooter()) {}
		self_type(rooter_type&& _rooter): _m_line(_rooter) {}
		self_type(rooter_type const&& _rooter): _m_line(_rooter) {}
		self_type(rooter_type const& _rooter): _m_line(_rooter) {}
		template <typename p1> self_type(p1&& v1): _m_line(forward<p1>(v1)) {}
		template <typename p1,typename p2> self_type(p1&& v1,p2&& v2): _m_line(forward<p1>(v1),forward<p2>(v2)) {}

		rooter_type& rooter() {return _m_line.rooter();}
		rooter_type const& rooter() const {return _m_line.rooter();}

		line_value_center& baser() {return _m_line;}
		line_value_center const& baser() const {return _m_line;}

		bool is_empty() const {return _m_line.is_empty();}
		size_t size() const {return _m_line.size();}
		size_t capacity() const {return _m_line.capacity();}


		value_type* begin() {return _m_line.begin();}
		value_type* end() {return _m_line.end();}
		value_type const* begin() const {return _m_line.begin();}
		value_type const* end() const {return _m_line.end();}
		value_type& front() {return _m_line.front();}
		value_type& back() {return _m_line.back();}
		value_type* fronter() {return _m_line.fronter();}
		value_type* backer() {return _m_line.backer();}
		value_type* ender() {return _m_line.ender();}
		value_type const* fronter() const {return _m_line.fronter();}
		value_type const* backer() const {return _m_line.backer();}
		value_type const* ender() const {return _m_line.ender();}
		value_type const* cfronter() {return _m_line.cfronter();}
		value_type const* cbacker() {return _m_line.cbacker();}
		value_type const* cender() {return _m_line.cender();}

		value_type const* find(value_type const& v) const
		{
			return binary_search(begin(),end(),v,compare_type());
		}
		value_type* insert(value_type const& v)
		{
			keyof_type keyof;
			pair<value_type*,bool> finded = try_binary_search(begin(),end(),keyof(v),keyof,compare_type());
			return baser().insert(finded.first,v);
		}
		value_type* insert(size_t off,value_type const& v)
		{
			value_type const* curr = baser().at(off);
			value_type const* prev = baser().at(off);
			compare_type cmp;
			int r = cmp(v,*curr);
			int rprev = cmp(v,*prev);
			if (r>0 || rprev<0) return 0;
			return baser().insert(off,v);
		}
		/// that we could not ensure the order, you should ensure the order of the line
		value_type* unsafe_insert(size_t off,value_type const& v)
		{
			assert(off>=0 && off<=size());
			return baser().insert(off,v);
		}
		value_type* remove(value_type const& v)
		{
			keyof_type keyof;
			value_type* victim = binary_search(begin(),end(),keyof(v),keyof,compare_type());
			if (!victim) return 0;
			return baser().internal_remove_at(victim);
		}
		value_type* remove(value_type* v)
		{
			return baser().internal_remove_at(v);
		}
		value_type* remove_at(size_t off)
		{
			return baser().internal_remove_at(off);
		}
	};

	template<typename rooter_tn,typename cmp_tn>
	struct keyvalue_sortedline_value_container
		: line_value_center<rooter_tn>
	{
		typedef keyvalue_sortedline_value_container self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef value_type node_type;
		typedef value_type* noder_type;
		typedef cmp_tn compare_type;
		typedef keyof_pair_default<typename value_type::first_type> keyof_type;

	private:
		typedef line_value_center<rooter_tn> line_value_center;
		typedef line_value_center base_type;
		//line_value_center _m_line;

		line_value_center& baser() {return *this;}
		line_value_center const& baser() const {return *this;}

	public:
		/// inherits from the base class
		keyvalue_sortedline_value_container()
		{}
		~keyvalue_sortedline_value_container()
		{}
		keyvalue_sortedline_value_container(rooter_tn const& rooter)
			: base_type(rooter)
		{}
		keyvalue_sortedline_value_container(keyvalue_sortedline_value_container const& other)
		{
			baser().resize_uninitialized(other.size());
			helper::memcpy<char>((char*)begin(),(char*)other.begin(),other.size()*sizeof(value_type));
		}
		keyvalue_sortedline_value_container(keyvalue_sortedline_value_container&& other)
		{
			baser()._m_rooter = other.baser()._m_rooter;
			other.baser()._m_rooter.zero();
		}

		//bool is_empty() const {return _m_line.is_empty();}
		//size_t size() const {return _m_line.size();}
		//size_t capacity() const {return _m_line.capacity();}


		//value_type* begin() {return _m_line.begin();}
		//value_type* end() {return _m_line.end();}
		//value_type const* begin() const {return _m_line.begin();}
		//value_type const* end() const {return _m_line.end();}
		//value_type& front() {return _m_line.front();}
		//value_type& back() {return _m_line.back();}
		//value_type* fronter() {return _m_line.fronter();}
		//value_type* backer() {return _m_line.backer();}
		//value_type* ender() {return _m_line.ender();}
		//value_type const* fronter() const {return _m_line.fronter();}
		//value_type const* backer() const {return _m_line.backer();}
		//value_type const* ender() const {return _m_line.ender();}
		//value_type const* cfronter() {return _m_line.cfronter();}
		//value_type const* cbacker() {return _m_line.cbacker();}
		//value_type const* cender() {return _m_line.cender();}

		//value_type* at(size_t i) {return _m_line.at(i);}
		//value_type const* at(size_t i) const {return _m_line.at(i);}
		//size_t distance(value_type const* d) const {return _m_line.distance(d);}

		self_type& append(value_type const* _begin,size_t count)
		{
			baser().append(_begin,count);
			return *this;
		}
		void push_front(typename value_type::first_type const& first,typename value_type::second_type const& second)
		{
			if (compare_type()(first,front().first)<=0);
				baser().push_front(value_type(first,second));
		}
		void push_back(typename value_type::first_type const& first,typename value_type::second_type const& second)
		{
			if (compare_type()(first,back().first)>=0);
				baser().push_back(value_type(first,second));
		}

		value_type const* find(typename value_type::first_type const& v) const
		{
			return binary_search(begin(),end(),v,keyof_type(),compare_type());
		}
		pair<value_type const*,bool> try_find(typename value_type::first_type const& v) const
		{
			return try_binary_search(begin(),end(),v,keyof_type(),compare_type());
		}
		template <typename first_tn,typename second_tn>
		value_type* insert(first_tn const& key, second_tn const& value)
		{
			return insert(value_type(key,value));
		}
		value_type* insert(value_type const& v)
		{
			pair<value_type*,bool> finded = try_binary_search(begin(),end(),v.first,keyof_type(),compare_type());
			return baser().insert(finded.first,v);
		}
		value_type* insert_at(value_type const* prev,value_type const& v)
		{
			return insert_at(line_value_center::distance(prev),v);
		}
		value_type* insert_at(size_t off,value_type const& v)
		{
			value_type const* curr = baser().at(off);
			value_type const* prev = baser().at(off);
			compare_type cmp;
			int r = cmp(v,keyof()(*curr));
			int rprev = cmp(v,keyof()(*prev));
			if (r>0 || rprev<0) return 0;
			return baser().insert(off,v);
		}
		/// that we could not ensure the order, you should ensure the order of the line
		template <typename first_tn,typename second_tn>
		value_type* unsafe_insert_at(size_t off,first_tn const& key, second_tn const& value)
		{
			assert(off>=0 && off<=size());
			return baser().insert(off,value_type(key,value));
		}
		value_type* unsafe_insert_at(size_t off,value_type const& v)
		{
			assert(off>=0 && off<=size());
			return baser().insert(off,v);
		}
		value_type* remove(typename value_type::first_type const& v)
		{
			value_type* victim = binary_search(begin(),end(),v,keyof_type(),compare_type());
			if (!victim) return 0;
			return baser().internal_remove_at(victim);
		}
		//value_type* remove(value_type* v)
		//{
		//	return baser().internal_remove_at(v);
		//}
		//value_type* remove_at(size_t off)
		//{
		//	return baser().internal_remove_at(off);
		//}
		//value_type* remove_front()
		//{
		//	return baser().remove_front();
		//}
		//void remove_back()
		//{
		//	baser().remove_back();
		//}
		//value_type* internal_remove_back() {return baser().internal_remove_back();}
	};
	template<typename rooter_tn>
	struct string_container_tt
		: line_value_center<rooter_tn>
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef value_type node_type;
		typedef value_type* noder_type;
	private:
		typedef line_value_center<rooter_tn> line_value_center;

	private:
		line_value_center& line_center() {return *static_cast<line_value_center*>(this);}
	public:
		string_container_tt()
		{}
		~string_container_tt()
		{}
		string_container_tt(rooter_tn const& rooter)
			: line_value_center(rooter)
		{}
		string_container_tt(string_container_tt const& other)
		{
			append(other.data(),other.size());
		}
		string_container_tt(string_container_tt&& other)
			: line_value_center(move(other.line_center()))
		{}
		string_container_tt(value_type const* str)
		{
			append(str);
		}
		
		value_type const* c_str() {return data();}
		string_container_tt& append(value_type const& v)
		{
			line_value_center::append(v);
			return *this;
		}
		string_container_tt& append(size_t count,value_type const& v)
		{
			line_value_center::append(count,v);
			return *this;
		}
		string_container_tt& append(value_type const* p,size_t length)
		{
			line_value_center::append(p,length);
			return *this;
		}
		string_container_tt& append(value_type const* str)
		{
			line_value_center::append(str,helper::strlen(str));
			return *this;
		}
		string_container_tt& append(string_container_tt const& str)
		{
			line_value_center::append(str.begin(),str.size());
			return *this;
		}
		string_container_tt& operator+= (value_type const& v)
		{
			return append(v);
		}
		string_container_tt& operator+= (value_type const* str)
		{
			return append(str);
		}
		string_container_tt& operator+= (string_container_tt const& str)
		{
			return append(str);
		}
		value_type* insert(size_t offset,value_type const& v)
		{
			return line_value_center::insert(offset,v);
		}
		value_type* insert(size_t offset,value_type const* str)
		{
			return line_value_center::insert(offset,str,helper::strlen(str));
		}
		value_type* insert(size_t offset,size_t count,value_type const& v)
		{
			return line_value_center::insert(offset,count,v);
		}
	};

	/*
		noder.*();
		noder.++
		value == value
	*/

	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct podliner_tt
	{
		typedef podline_rooter<value_tn,allocator_tn> rooter_type;
		typedef line_value_container_owner<rooter_type> type;
	};
	template <typename value_tn,size_t size_tc,typename size_tn=size_t>
	struct static_podliner_off_tt
	{
		typedef podline3_rooter<value_tn,size_tc,size_tn,nullmalloc> rooter_type;
		typedef line_value_container_owner<rooter_type> type;
	};
	template <typename value_tn,size_t size_tc,typename size_tn=size_t,typename cmp_tn=compare_default>
	struct static_sorted_podliner_off_tt
	{
		typedef podline3_rooter<value_tn,size_tc,size_tn,nullmalloc> rooter_type;
		typedef sortedline_value_center<rooter_type,keyof_default<value_tn>,cmp_tn> type;
	};
	template <typename value_tn,typename cmp_tn=compare_default,
		typename head_tn=podline_head_tt<size_t>,typename allocator_tn=cppmalloc>
	struct sorted_podliner_off_tt
	{
		typedef allocator_tn allocator_type;
		typedef line_value_container_rooter_ref<head_tn,value_tn,allocator_type> rooter_type;
		typedef sortedline_value_center<rooter_type,keyof_default<value_tn>,cmp_tn> type;
	};
	template <typename key_tn,typename value_tn,size_t size_tc,typename size_tn=size_t,typename cmp_tn=compare_default>
	struct static_keyvalue_sorted_podliner_off_tt
	{
		typedef pair<key_tn,value_tn> value_type;
		typedef podline3_rooter<value_type,size_tc,size_tn,nullmalloc> rooter_type;

		typedef keyvalue_sortedline_value_container<rooter_type,cmp_tn> type;
	};

	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct podstring_tt
	{
		typedef allocator_tn allocator_type;
		typedef strline_rooter<value_tn,nullcharacter_tt<value_tn>,allocator_type> rooter_type;
		typedef string_container_tt<rooter_type> type;
	};
	typedef podstring_tt<char>::type string;
	typedef podstring_tt<wchar_t>::type wstring;


	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct podstring_ref_tt
	{

	};
} /// end of namespace abc


namespace ox
{
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


	//template <typename space_tn>
	//struct space_pointer
	//{
	//	typedef space_tn space_type;
	//	typedef space_pointer self;
	//	typedef space_type value_type;
	//	self(value_tn ptr=0) : _m_pointer(ptr) {}
	//	self& operator=(value_tn ptr) {return*this;}
	//	self& operator=(value_type const& value) {return*this;}
	//	self& operator++() {return *this;}
	//	self operator++(int) {return old;}
	//	typename space_type::value_type* _m_pointer;
	//};

	template <typename value_tn> struct chunk_node_fixed_with_next
	{
		typedef chunk_node_fixed_with_next self_type;
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef size_t size_type;

		self_type* _m_next;
		value_type _m_value;
		size_type size_of_chunk() const {return sizeof(self_type);}
		void set_size_of_chunk(size_t) {}
		void set_next(self_type* next) {_m_next=next;}
		self_type* next() const {return _m_next;}
		void set_prev(self_type*) {}
		self_type* prev() const {return 0;}
		reference value() {return _m_value;}
		const_reference value() const {return _m_value;}
	};
	template <typename value_tn> struct chunk_node_fixed_with_both
	{
		typedef chunk_node_fixed_with_both self_type;
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef size_t size_type;

		self_type* _m_next;
		self_type* _m_prev;
		value_type _m_value;
		size_type size_of_chunk() const {return sizeof(self_type);}
		void set_size_of_chunk(size_type) {}
		void set_next(self_type* next) {_m_next=next;}
		self_type* next() const {return _m_next;}
		void set_prev(self_type* prev) {_m_prev=prev;}
		self_type* prev() const {return _m_prev;}
		reference value() {return _m_value;}
		const_reference value() const {return _m_value;}
	};
	template <typename value_tn> struct chunk_node_sizable_with_next
	{
		typedef chunk_node_sizable_with_next self_type;
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef size_t size_type;

		self_type* _m_next;
		size_type _m_chunk_size;
		size_type size_of_chunk() const {return _m_chunk_size;}
		void set_size_of_chunk(size_type size) {_m_chunk_size=size;}
		void set_next(self_type* next) {_m_next=next;}
		self_type* next() const {return _m_next;}
		void set_prev(self_type*) {}
		self_type* prev() const {return 0;}
		reference value() {return *(value_type*)(this+1);}
		const_reference value() const {return *(value_type*)(this+1);}
	};
	template <typename value_tn> struct chunk_node_sizable_with_both
	{
		typedef chunk_node_sizable_with_both self_type;
		typedef value_tn value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef size_t size_type;

		chunk_node_sizable_with_both* _m_next;
		chunk_node_sizable_with_both* _m_prev;
		size_type _m_chunk_size;
		size_type size_of_chunk() const {return _m_chunk_size;}
		void set_size_of_chunk(size_type size) {_m_chunk_size=size;}
		void set_next(self_type* next) {_m_next=next;}
		self_type* next() const {return _m_next;}
		void set_prev(self_type* prev) {_m_prev=prev;}
		self_type* prev() const {return _m_prev;}
		reference value() {return *(value_type*)(this+1);}
		const_reference value() const {return *(value_type*)(this+1);}
	};

	template <typename chunk_tn,bool is_prev_enabled_tc,bool is_sizable_tc>
	struct chunk_dot_tt
	{
		typedef chunk_dot_tt self_type;
		typedef chunk_dot_tt<chunk_tn const,is_prev_enabled_tc,is_sizable_tc> self_const_type;
		typedef chunk_tn node_type;
		typedef typename met::add_con<chunk_tn>::to<typename node_type::value_type>::type value_type;
		typedef value_type& reference;
		typedef value_type const& const_reference;
		typedef typename value_type* pointer_type;
		typedef typename value_type const* pointer_const_type;

		//typedef value_type unit_type;
		//static_assert(is_same<sizable_space_tt<unit_type>,value_type>::value,"the type is not sizable space type");

		static bool is_sizable() {return is_sizable_tc;}
		bool is_empty() const {return _m_chunk==0;}
		explicit self_type(node_type* node=0) : _m_chunk(node) {}
		self_type(self_type const& other) : _m_chunk((node_type*)(other.node())) {}
		operator node_type* () {return _m_chunk;}
		node_type* node() {return _m_chunk;}
		node_type const* node() const {return _m_chunk;}
		void set_node(node_type* node) {_m_chunk=node;}
		//size_t size_of_chunk() const {return (is_sizable()?size_of_chunk()-sizeof(node_type):1); }
		static size_t size_of_space(size_t value_count)
		{
			//assert(value_size>=sizeof(value_type) && "value size should be >= size of value-type");
			return is_sizable()?sizeof(node_type)+value_count*sizeof(value_type):sizeof(node_type);
		}
		static size_t size_of_meta()
		{
			return is_sizable()?sizeof(node_type):sizeof(node_type)-sizeof(value_type);
		}
		size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
		size_t size_of_chunk() const {return _m_chunk->size_of_chunk();}
		void set_size_of_chunk(size_t size) {_m_chunk->set_size_of_chunk(size);}
		size_t size() const {return size_of_data()/sizeof(value_type);}
		void set_next(node_type* chunk) {_m_chunk->set_next(chunk);}
		node_type* next() {return _m_chunk->next();}
		node_type const* next() const {return _m_chunk->next();}
		void set_prev(node_type* chunk) {_m_chunk->set_prev(chunk);}
		node_type* prev() {return _m_chunk->prev();}
		node_type const* prev() const {return _m_chunk->prev();}
		bool is_prev_enabled() const {return is_prev_enabled_tc;}
		pointer_type begin() {return &value();}
		pointer_const_type begin() const {return &value();}
		pointer_type end() {return begin()+(is_sizable()?size_of_chunk()-sizeof(node_type):1);}
		pointer_const_type end() const {return begin()+(is_sizable()?size_of_chunk()-sizeof(node_type):1);}
		reference value() {return _m_chunk->value();}
		const_reference value() const {return _m_chunk->value();}
		

		self_type& assign(value_type const& _value)
		{
			if (is_sizable())
				helper::memmove(begin(),&_value,sizeof(_value));
			else
				value() = _value;
			return *this;
		}
		self_type& construct(value_type const& _value)
		{
			if (is_sizable())
				helper::memmove(begin(),&_value,sizeof(_value));
			else
				new (begin()) value_type (_value);
			return *this;
		}
		template <typename type_tn>
		self_type& assign(sizable_space_tt<type_tn const> const& _value)
		{
			if (is_sizable())
				helper::memmove(begin(),_value.begin,min(_value.size,size_of_chunk()));
			else
				value() = *_value.begin;
			return *this;
		}
		template <typename type_tn>
		self_type& construct(sizable_space_tt<type_tn const> const& _value)
		{
			if (is_sizable())
				helper::memmove(begin(),_value.begin,min(_value.size,size_of_chunk()));
			else
				new (begin()) value_type (*_value.begin);
			return *this;
		}
		node_type* _m_chunk;
	};

	template <typename node_tn,bool is_tail_enabled_tc>
	struct chunk_root_data
	{
		typedef node_tn node_type;
		node_type* _m_head;
		node_type* _m_tail;
		node_type* head() {return _m_head;}
		node_type* tail() {return _m_tail;}
		node_type const* head() const {return _m_head;}
		node_type const* tail() const {return _m_tail;}
		void set_head(node_type* _head) {_m_head=_head;}
		void set_tail(node_type* _tail) {_m_tail=_tail;}
	};
	template <typename node_tn>
	struct chunk_root_data <node_tn,false>
	{
		typedef node_tn node_type;
		node_type* _m_head;
		node_type* head() {return _m_head;}
		node_type* tail() {return 0;}
		node_type const* head() const {return _m_head;}
		node_type const* tail() const {return 0;}
		void set_head(node_type* _head) {_m_head=_head;}
		void set_tail(node_type* _tail) {}
	};
	template <typename data_tn>
	struct chunk_root_dataptr
	{
		typedef typename data_tn::node_type node_type;
		data_tn* _m_data;
		node_type* head() {return _m_data->head();}
		node_type* tail() {return _m_data->tail();}
		node_type const* head() const {return _m_data->head();}
		node_type const* tail() const {return _m_data->tail();}
		data_tn const* data() const {return _m_data;}
		void set_head(node_type* _head) {_m_data->set_head(_head);}
		void set_tail(node_type* _tail) {_m_data->set_tail(_tail);}
		void set_data(data_tn* _data) {_m_data=_data;}
	};
	template <typename chunker_tn,typename allocator_tn,bool is_tail_enabled_tc>
	struct chunk_list_rooter
		: common_data_tt<chunk_root_data<typename chunker_tn::node_type,is_tail_enabled_tc>,allocator_tn>
	{
		typedef chunk_list_rooter self_type;
		typedef typename chunker_tn::node_type node_type;
		typedef chunker_tn dot_type;
		typedef typename chunker_tn::self_const_type cdot_type;
		typedef allocator_tn allocator_type;
		typedef typename dot_type::value_type value_type;
		typedef typename dot_type::reference reference;
		typedef chunk_list_rooter<cdot_type,allocator_type,is_tail_enabled_tc> self_const_type;

		self_type() {_m_data.set_head(0),_m_data.set_tail(0);}
		static bool is_owner() {return true;}
		bool is_empty() const {return _m_data.head()==0;}
		node_type* head() {return _m_data.head();}
		dot_type header() {return dot_type(_m_data.head());}
		node_type const* head() const {return _m_data.head();}
		cdot_type header() const {return _m_data.head();}
		void set_head(node_type* _chunk_root) {_m_data.set_head(_chunk_root);}
		bool is_tail_enabled() const {return is_tail_enabled_tc;}
		node_type* tail() {return _m_data.tail();}
		dot_type tailer() {return dot_type(_m_data.tail());}
		node_type const* tail() const {return _m_data.tail();}
		cdot_type tailer() const {return _m_data.tail();}
		void set_tail(node_type* _chunk_root) {_m_data.set_tail(_chunk_root);}
		void on_after_inserted(dot_type prev,dot_type nodernew) {}
	};
	template <typename chunker_tn,typename root_data_refed_tn,typename allocator_tn,bool is_tail_enabled_tc>
	struct chunk_list_rooter_ref
		: common_data_tt<chunk_root_dataptr<root_data_refed_tn>,allocator_tn>
	{
		typedef chunk_list_rooter_ref self_type;
		typedef typename chunker_tn::node_type node_type;
		typedef chunker_tn dot_type;
		typedef typename chunker_tn::self_const_type cdot_type;
		typedef allocator_tn allocator_type;
		typedef typename dot_type::value_type value_type;
		typedef typename dot_type::reference reference;
		typedef chunk_list_rooter_ref<cdot_type,root_data_refed_tn,allocator_type,is_tail_enabled_tc> self_const_type;

		self_type() {_m_data.set_data(0);}
		self_type(root_data_refed_tn* _data) {_m_data.set_data(_data);}
		static bool is_owner() {return false;}
		void mount(root_data_refed_tn* _data) {_m_data.set_data(_data);}
		void umount() {_m_data.set_data(0);}
		bool is_empty() const {return _m_data.data()==0||_m_data.head()==0;}
		node_type* head() {return _m_data.data()?_m_data.head():0;}
		dot_type header() {return dot_type(_m_data.data()?_m_data.head():0);}
		node_type const* head() const {return _m_data.data()?_m_data.head():0;}
		cdot_type header() const {return cdot_type(_m_data.data()?_m_data.head():0);}
		void set_head(node_type* _chunk_root) {_m_data.data()?_m_data.set_head(_chunk_root):0;}
		bool is_tail_enabled() const {return is_tail_enabled_tc;}
		node_type* tail() {return _m_data.data()?_m_data.tail():0;}
		dot_type tailer() {return dot_type(_m_data.data()?_m_data.tail():0);}
		node_type const* tail() const {return _m_data.data()?_m_data.tail():0;}
		cdot_type tailer() const {return cdot_type(_m_data.data()?_m_data.tail():0);}
		void set_tail(node_type* _chunk_root) {_m_data.data()?_m_data.set_tail(_chunk_root):0;}
		void on_after_inserted(dot_type prev,dot_type nodernew) {}
	};
	template <typename chunker_tn,typename root_data_refed_tn,typename allocator_tn,bool is_tail_enabled_tc>
	struct chunk_list_rooter_allref
		: common_data_tt<chunk_root_dataptr<root_data_refed_tn>,malloc_pointer_default<allocator_tn>>
	{
		typedef chunk_list_rooter_allref self_type;
		typedef root_data_refed_tn rootdata_type;
		typedef typename chunker_tn::node_type node_type;
		typedef chunker_tn dot_type;
		typedef typename chunker_tn::self_const_type cdot_type;
		typedef allocator_tn allocator_type;
		typedef typename dot_type::value_type value_type;
		typedef typename dot_type::reference reference;
		typedef chunk_list_rooter_ref<cdot_type,root_data_refed_tn,allocator_type,is_tail_enabled_tc> self_const_type;

		self_type() {_m_data.set_data(0);allocator()._m_allocator=0;}
		self_type(allocator_type* _alloc,root_data_refed_tn* _data)
		{
			allocator()._m_allocator = _alloc;
			_m_data.set_data(_data);
		}
		static bool is_owner() {return false;}
		void mount(allocator_type* _alloc,root_data_refed_tn* _data)
		{
			allocator()._m_allocator = _alloc;
			_m_data.set_data(_data);
		}
		void umount() {_m_data.set_data(0);}
		bool is_empty() const {return _m_data.data()==0||_m_data.head()==0;}
		node_type* head() {return _m_data.data()?_m_data.head():0;}
		dot_type header() {return dot_type(_m_data.data()?_m_data.head():0);}
		node_type const* head() const {return _m_data.data()?_m_data.head():0;}
		cdot_type header() const {return cdot_type(_m_data.data()?_m_data.head():0);}
		void set_head(node_type* _chunk_root) {_m_data.data()?_m_data.set_head(_chunk_root):0;}
		bool is_tail_enabled() const {return is_tail_enabled_tc;}
		node_type* tail() {return _m_data.data()?_m_data.tail():0;}
		dot_type tailer() {return dot_type(_m_data.data()?_m_data.tail():0);}
		node_type const* tail() const {return _m_data.data()?_m_data.tail():0;}
		cdot_type tailer() const {return cdot_type(_m_data.data()?_m_data.tail():0);}
		void set_tail(node_type* _chunk_root) {_m_data.data()?_m_data.set_tail(_chunk_root):0;}
		void on_after_inserted(dot_type prev,dot_type nodernew) {}
	};

	template <typename dot_const_tn>
	struct iterator_of_chunk_list_center_const
	{
		typedef iterator_of_chunk_list_center_const self_type;
		typedef dot_const_tn dot_type;
		typedef typename dot_type::value_type value_type;
		typedef typename dot_type::reference reference;
		typedef typename dot_type::const_reference const_reference;
		typedef typename dot_type::node_type node_type;
		self_type(node_type* node=0): _m_dot(node) {}
		dot_type dot() const {return _m_dot;}
		bool is_empty() const {return _m_dot.is_empty();}
		size_t size() const {return _m_dot.size_of_data();}
		size_t size_of_whole() const {return _m_dot.size_of_chunk();}
		const_reference operator*() const {return _m_dot.value();}
		reference operator*() {return _m_dot.value();}
		self_type& operator++() {_m_dot.set_node(_m_dot.next());return *this;}
		self_type& operator--() {_m_dot.set_node(_m_dot.prev());return *this;}
		value_type* operator->() {return _m_dot.value();}
		bool operator==(self_type const& other) {return _m_dot.node()==other._m_dot.node();}
		bool operator!=(self_type const& other) {return _m_dot.node()!=other._m_dot.node();}
		dot_type _m_dot;
	};
	template <typename dot_nonconst_tn>
	struct iterator_of_chunk_list_center
		: iterator_of_chunk_list_center_const<typename dot_nonconst_tn::self_const_type>
	{
		typedef dot_nonconst_tn dot_type;
		typedef iterator_of_chunk_list_center_const<typename dot_nonconst_tn::self_const_type> base_type;
		typedef iterator_of_chunk_list_center self_type;
		typedef typename dot_type::node_type node_type;
		self_type(node_type* node=0): base_type(node) {}
		dot_type dot() const {return dot_type((node_type*)(_m_dot.node()));}
	};

	template <typename rooter_tn>
	struct chunk_list_center : rooter_tn
	{
		typedef chunk_list_center self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::node_type node_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef typename rooter_type::cdot_type cdot_type;
		typedef typename rooter_type::allocator_type allocator_type;
		typedef typename rooter_type::value_type value_type;

		typedef iterator_of_chunk_list_center_const<cdot_type> const_iterator;
		typedef iterator_of_chunk_list_center<dot_type> iterator;


		/// the fixed stereotype routine
		self_type() {}
		self_type(self_type&& other): rooter_type(other.rooter()) {}
		self_type(self_type const&& other): rooter_type(other.rooter()) {}
		self_type(self_type const& other): rooter_type(other.rooter()) {}
		self_type(rooter_type&& _rooter): rooter_type(_rooter) {}
		self_type(rooter_type const&& _rooter): rooter_type(_rooter) {}
		self_type(rooter_type const& _rooter): rooter_type(_rooter) {}
		template <typename p1> self_type(p1&& v1): rooter_type(forward<p1>(v1)) {}
		template <typename p1,typename p2> self_type(p1&& v1,p2&& v2): rooter_type(forward<p1>(v1),forward<p2>(v2)) {}

		rooter_type& rooter() {return *static_cast<rooter_type*>(this);}
		rooter_type const& rooter() const {return *(rooter_type const*)(this);}
		bool is_empty() const  {return rooter().is_empty();}
		value_type& front() {return *rooter().header().begin();}
		value_type const& front() const {return *rooter().header().begin();}
		dot_type fronter() {return rooter().header();}
		cdot_type fronter() const {return rooter().header();}
		value_type& back() {return *rooter().tailer().begin();}
		value_type const& back() const {*return rooter().tailer().begin();}
		dot_type backer() {return rooter().tailer();}
		cdot_type backer() const {return rooter().tailer();}
		dot_type ender() {return dot_type();}
		cdot_type ender() const {return cdot_type();}
		iterator begin() {return iterator(fronter());}
		iterator end() {return iterator();}
		const_iterator begin() const {return const_iterator(fronter());}
		const_iterator end() const {return const_iterator();}

		template <typename itn>
		pair<dot_type,dot_type> copyfrom(itn begin,itn end)
		{
			pair<dot_type,dot_type> ret;
			if (begin.is_empty()) return ret;

			dot_type current ((node_type*)rooter().make_uninitialized<char>(dot_type::size_of_space(begin.size())));
			if (current.is_empty()) return ret;
			current.construct(*begin);
			current.set_prev(0);
			current.set_next(0);
			dot_type beginnew = current;
			dot_type prev = beginnew;
			ret.first = beginnew;
			for (itn i=begin;i!=end;++i)
			{
				current.set_node((node_type*)rooter().make_uninitialized<char>(dot_type::size_of_space(i.size())));
				if (current.is_empty()) return ret.second=prev,ret;
				current.construct(*i);
				prev.set_next(current);
				current.set_prev(prev);
			}
			return pair<dot_type,dot_type>(beginnew,current);
		}
		dot_type insert_after_uninitialized(dot_type noder,size_t count_of_value=1)
		{
			assert(count_of_value>=1);
			size_t size = dot_type::size_of_space(count_of_value);
			dot_type chunker_new ((node_type*)rooter().make_uninitialized<char>(size));
			if (chunker_new.is_empty()) return dot_type();
			chunker_new.set_size_of_chunk(size);
			return insert_node_after(noder,chunker_new);
		}
		dot_type insert_node_after(dot_type noder,dot_type nodernew)
		{
			if (nodernew.is_empty()) return dot_type();
			dot_type nexter (noder.is_empty() ? rooter().head() : noder.next());
			nodernew.set_next(nexter.node());
			nodernew.set_prev(noder.node());
			if (!noder.is_empty()) noder.set_next(nodernew);
			if (!nexter.is_empty()) nexter.set_prev(nodernew);
			if (noder.is_empty()) rooter().set_head(nodernew);
			if (nexter.is_empty()) rooter().set_tail(nodernew);

			rooter().on_after_inserted(noder,nodernew);
			return nodernew;
		}
		dot_type insert_after(dot_type node,value_type const& value)
		{
			dot_type nodenew = insert_after_uninitialized(node);
			if (!nodenew.is_empty()) new (nodenew.begin()) value_type (value);
			return node;
		}
		//template <typename unit_tn>
		dot_type insert_after(dot_type node,sizable_space_tt<value_type> const& space)
		{
			return insert_after_uninitialized(node,space.size);
		}
		dot_type insert_before_uninitialized(dot_type noder,size_t count_of_value=1)
		{
			dot_type prever = noder;
			assert(prever.is_prev_enabled() && "the container does not support insert before");
			if (!noder.is_prev_enabled()) return dot_type();
			prever.set_node(prever.is_empty() ? rooter().tail() : prever.prev());
			return insert_after_uninitialized(prever,count_of_value);
		}
		dot_type insert_node_before(dot_type noder,dot_type node_new)
		{
			dot_type prever = noder;
			assert(prever.is_prev_enabled() && "the container does not support insert before");
			if (noder.is_prev_enabled()) return dot_type();
			prever.set_node(prever.is_empty() ? rooter().tail() : prever.prev());
			return insert_node_after(prever,node_new);
		}
		dot_type insert_before(dot_type node,sizable_space_tt<value_type> const& space)
		{
			return insert_before_uninitialized(node,space.size);
		}
		dot_type insert_before(dot_type node,value_type const& value)
		{
			dot_type dot = insert_before_uninitialized(node);
			if (!dot.is_empty()) new (dot.begin()) value_type (value);
			return dot;
		}
		dot_type push_front_uninitialized(size_t count_of_value=1)
		{
			return insert_after_uninitialized(dot_type(),count_of_value);
		}
		dot_type push_front(value_type const& value)
		{
			dot_type noder = push_front_uninitialized();
			if (!noder.is_empty()) new (noder.begin()) value_type (value);
			return noder;
		}
		dot_type push_front(sizable_space_tt<value_type> const& space)
		{
			return push_front_uninitialized(space.size);
		}
		dot_type push_front(node_type* node)
		{
			return insert_node_after(dot_type(),dot_type(node));
		}
		/// when node is zero, we will remove front node
		dot_type remove_after(node_type* node)
		{
			dot_type noder (node);
			dot_type victimer (noder.is_empty() ? rooter().head() : noder.next());

			if (victimer.is_empty()) return dot_type();
			dot_type nexter (victimer.next());

			if (!noder.is_empty()) noder.set_next(nexter.node());
			if (!nexter.is_empty()) nexter.set_prev(noder.node());
			if (noder.is_empty()) rooter().set_head(nexter.node());
			if (nexter.is_empty()) rooter().set_tail(noder.node());

			return victimer;
		}
		dot_type remove(dot_type dot)
		{
			return remove(dot.node());
		}
		dot_type remove(node_type* node)
		{
			if (!node) return dot_type();
			dot_type noder (node);
			assert(noder.is_prev_enabled() && "the container can't support remove, please use remove after.");
			return remove_after(noder.prev());
		}
		dot_type remove_front()
		{
			return remove_after(0);
		}
		dot_type remove_back()
		{
			return remove(backer().node());
		}
		dot_type push_back_uninitialized(size_t count_of_value=1)
		{
			assert(rooter().is_tail_enabled() && "the container does not support push back");
			if (!rooter().is_tail_enabled()) return dot_type();
			return insert_after_uninitialized(rooter().tailer(),count_of_value);
		}
		dot_type push_back(value_type const& value)
		{
			dot_type noder = push_back_uninitialized();
			if (!noder.is_empty()) new (noder.begin()) value_type (value);
			return noder;
		}
		dot_type push_back(sizable_space_tt<value_type> const& space)
		{
			return push_back_uninitialized(space.size);
		}
		dot_type push_back(node_type* node)
		{
			return insert_node_after(rooter().tailer(),dot_type(node));
		}
		self_type& reverse()
		{
			dot_type h = rooter().header();
			dot_type nh = internal_reverse();
			rooter().set_head(nh);
			rooter().set_tail(h);
			return *this;
		}
		/// the new head
		dot_type internal_reverse()
		{
			return internal_reverse(dot_type(),fronter(),dot_type());
		}
		dot_type internal_reverse(dot_type pbegin,dot_type begin,dot_type end)
		{
			dot_type prev, i, n;
			prev = begin;
			if (prev.is_empty()) return prev;
			i.set_node(prev.next());
			prev.set_next(end);
			while (i!=end)
			{
				n.set_node(i.next());
				i.set_next(prev);
				prev.set_prev(i);
				prev = i;
				i = n;
			}
			if (!prev.is_empty()) prev.set_prev(pbegin);
			return prev;
		}
		void clear()
		{
			dot_type next;
			for (dot_type chunker=rooter().header();!chunker.is_empty();)
			{
				next = chunker.next();
				rooter().allocator().deallocate(chunker.node());
				chunker = next;
			}
			rooter().set_head(0);
		}
		allocator_type& allocator()
		{
			return *static_cast<allocator_type*>(this);
		}
	};

	template <typename rooter_tn>
	struct chunk_list_owner
		: chunk_list_center<rooter_tn>
	{
		typedef chunk_list_owner self_type;
		~chunk_list_owner() {if (rooter().is_owner()) clear();}
		self_type() {}
		self_type(self_type const& other)
		{
			pair<dot_type,dot_type> ret = copy(other.begin(),other.end());
			rooter().set_head(ret.first);
			rooter().set_tail(ret.second);
		}
		self_type(self_type&& other)
		{
			rooter().set_head(other.rooter().head());
			rooter().set_tail(other.rooter().tail());
			other.rooter().set_head(0);
		}
	};

	template <typename value_tn=char,typename allocator_tn=cppmalloc>
	struct chunk2_list
	{
		static_assert(!met::is_same<value_tn,void>::value,"the value type can't be void");
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef chunk_node_sizable_with_next<value_type> node_type;
		static bool const __is_sizable = true;
		static bool const __is_prev_enabled = false;
		static bool const __is_tail_enabled = false;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> noder_type;
		typedef chunk_list_rooter<noder_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};

	template <typename value_tn=char,typename allocator_tn=cppmalloc>
	struct chunk3_list
	{
		static_assert(!met::is_same<value_tn,void>::value,"the value type can't be void");
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef chunk_node_sizable_with_both<value_type> node_type;
		static bool const __is_sizable = true;
		static bool const __is_prev_enabled = true;
		static bool const __is_tail_enabled = true;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> noder_type;
		typedef chunk_list_rooter<noder_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};

	template <typename root_data_refed_tn,typename value_tn=char,typename allocator_tn=cppmalloc>
	struct chunk_list_ref_root_data
	{
		static_assert(!met::is_same<value_tn,void>::value,"the value type can't be void");
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef chunk_node_sizable_with_both<value_type> node_type;
		static bool const __is_sizable = true;
		static bool const __is_prev_enabled = true;
		static bool const __is_tail_enabled = true;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> noder_type;
		typedef chunk_list_rooter_ref<noder_type,root_data_refed_tn,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};

	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct slist_tt
	{
		typedef allocator_tn allocator_type;
		typedef chunk_node_fixed_with_next<value_tn> node_type;
		static bool const __is_sizable = false;
		static bool const __is_prev_enabled = false;
		static bool const __is_tail_enabled = false;
		typedef chunk_dot_tt<node_type,__is_sizable,__is_prev_enabled> noder_type;
		typedef chunk_list_rooter<noder_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};
	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct list_tt
	{
		typedef allocator_tn allocator_type;
		typedef chunk_node_fixed_with_both<value_tn> node_type;
		static bool const __is_sizable = false;
		static bool const __is_prev_enabled = true;
		static bool const __is_tail_enabled = true;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> noder_type;
		typedef chunk_list_rooter<noder_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};
	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct listref_tt
	{
		typedef allocator_tn allocator_type;
		typedef chunk_node_fixed_with_both<value_tn> node_type;
		static bool const __is_sizable = false;
		static bool const __is_prev_enabled = true;
		static bool const __is_tail_enabled = true;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> noder_type;
		typedef chunk_root_data<node_type,true> rootdata_type;
		typedef chunk_list_rooter_allref<noder_type,rootdata_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> type;
	};

	struct mem_cache
	{
		typedef chunk2_list<char>::type chunk_list_type;
		typedef chunk_list_type::dot_type dot_type;
		chunk_list_type _m_chunk_slist;
		char* _m_next;
		size_t _m_min_size;

		mem_cache(): _m_next(0),_m_min_size(32*1024) {}

		size_t get_size(size_t size) const
		{
			return _m_min_size>=size?_m_min_size:size;
		}

		char* allocate(size_t size)
		{
			if (size==0) return 0;
			char* pback = _m_next+size-1;
			if (!_m_next || !_m_chunk_slist.front() || !validate_infront(pback))
			{
				dot_type r = _m_chunk_slist.push_front_uninitialized(get_size(size));
				if (!r.is_empty()) _m_next = r.begin();
				pback = _m_next+size-1;
			}
			if (!validate_infront(pback)) return 0;
			char* retp = _m_next;
			_m_next = pback + 1;
			return retp;
		}

	private:
		char* validate_infront(char* p)
		{
			bool isin = p>=_m_chunk_slist.fronter().begin() && p<_m_chunk_slist.fronter().end();
			return isin ? p : 0;
		}
	};

	template <typename noder_tn,typename value_tn>
	noder_tn seqence_find(noder_tn begin,noder_tn end, value_tn const& v)
	{
		for (noder_tn i=begin;i!=end;++i)
			if (*i==v) i;
		return end;
	}
	/*
		range.begin(),range.end()
	*/
	template <typename containter_tn,typename value_tn>
	typename containter_tn::node_type seqence_find(containter_tn& range, value_tn const& v)
	{
		typedef typename containter_tn::node_type node_type;
		return seqence_find(range.begin(),range.end(),v);
	}

	struct getsize_default
	{
		size_t operator()(size_t currsize,size_t minsize) const
		{
			(void)(currsize);
			return minsize*3;
		}
	};

	template <typename node_tn>
	struct zvector_noder_tt
	{
		typedef node_tn node_type;
		typedef node_type value_type;
		node_type* _m_node;
	};


	template <typename noder_tn,typename allocator_tn>
	struct chunk_list_rooter_for_zvector
		: chunk_list_rooter<noder_tn,allocator_tn,true>
	{
		void on_after_inserted(dot_type prev,dot_type nodernew)
		{
			nodernew.extension().value() += 1;
		}
	};
	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct typetrait_of_zvector
	{
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;

		struct zvector_node
		{
			typedef zvector_node self_type;
			typedef value_type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef size_t size_type;

			self_type* _m_next;
			self_type* _m_prev;
			size_type _m_chunk_size; /// the chunk node size
			size_type _m_vector_size; /// the vector's count size

			size_type size_of_chunk() const {return _m_chunk_size;}
			void set_size_of_chunk(size_type size) {_m_chunk_size=size;}
			size_t size_of_meta() const {return sizeof(self_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			void set_next(self_type* next) {_m_next=next;}
			self_type* next() const {return _m_next;}
			void set_prev(self_type* prev) {_m_prev=prev;}
			self_type* prev() const {return _m_prev;}
			value_type& value() {return *(value_type*)(this+1);}
			value_type const& value() const {return *(value_type*)(this+1);}
			size_type size_of_vector() const {return _m_vector_size;}
			void set_size_of_vector(size_type size) {_m_vector_size=size;}
		};


		typedef zvector_node node_type;

		struct podline_rooter_for_zvector//: nullmalloc
		{
			typedef podline_rooter_for_zvector self_type;
			typedef node_type node_type;
			typedef value_type value_type;
			typedef nullmalloc allocator_type;
			self_type(node_type* head=0): _m_head(head) {}
			self_type(self_type const& other): _m_head(other._m_head) {}
			self_type(self_type&& other): _m_head(other._m_head) {other._m_head=0;}
			bool is_empty() const {return !_m_head||size()==0;}
			void construct() {_m_head->set_size_of_vector(0);}
			allocator_type& allocator() {return *(allocator_type*)(this);}
			size_t size() const {return _m_head->size_of_vector();}
			size_t capacity() const {return (_m_head->size_of_data())/sizeof(value_type);}
			value_type* begin() const {return (value_type*)(_m_head+1);}
			value_type* end() const {return begin()+capacity();}
			void set_begin(value_type*) {}
			void set_end(value_type*) {}
			value_type* make_uninitialized(size_t) {return 0;}
			void free(void*) {}
			value_type const* next() const {return begin()+size();}
			value_type* next() {return begin()+size();}
			void set_next(value_type* off) {_m_head?_m_head->set_size_of_vector(off-begin()):0;}
			void move_next_relative_distance(int delta) {_m_head?_m_head->set_size_of_vector(_m_head->size_of_vector()+delta):0;}
			void set_next_value(value_type const& value,int off=0) {_m_head?(*(next()+off)=value):0;}
			void on_size_changed() {}
			void on_buffer_changing(value_type*,size_t) {}
		public:
			node_type* _m_head;
		};

		static bool const __is_sizable = true;
		static bool const __is_prev_enabled = true;
		static bool const __is_tail_enabled = true;
		typedef chunk_root_data<node_type,__is_tail_enabled> root_data_type;
		typedef chunk_dot_tt<node_type,__is_prev_enabled,__is_sizable> chunker_type;
		typedef chunk_list_rooter_ref<chunker_type,root_data_type,allocator_type,__is_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> chunk_list_type;
		typedef line_value_center<podline_rooter_for_zvector> vector_type;

		struct zvector_dot
		{
			typedef zvector_dot self_type;
			typedef node_type node_type;
			typedef value_type value_type;

			self_type(node_type* chunk=0,value_type* value=0)
				: _m_node(chunk) ,_m_value(value)
			{}

			value_type& value() {return *_m_value;}
			value_type const& value() const {return *_m_value;}
			node_type* node() {return _m_node;}
			node_type const* node() const {return _m_node;}
			value_type* valueptr() {return _m_value;}
			value_type const* valueptr() const {return _m_value;}
			void set_node(node_type* node) {_m_node=node;}
			void set_valueptr(value_type* valueptr) {_m_value=valueptr;}
			void set(node_type* node,value_type* valueptr) {_m_node=node,_m_value=valueptr;}

			bool operator==(self_type const& other) const {return _m_node==other._m_node && _m_value==other._m_value;}
			bool operator!=(self_type const& other) const {return _m_node!=other._m_node || _m_value!=other._m_value;}

			node_type* _m_node;
			value_type* _m_value;
		};

		struct zvector_rooter
			: common_data_tt<root_data_type,allocator_type>
		{
			typedef value_type value_type;
			typedef allocator_tn allocator_type;
			typedef zvector_dot dot_type;
			typedef node_type node_type;

			zvector_rooter(): _m_current(0) {_m_data.set_head(0),_m_data.set_tail(0);}
			bool is_empty() const {return as_chunk_list().is_empty();}

			node_type* current() {return _m_current;}
			node_type const* current() const {return _m_current;}
			void set_current(node_type* curr) {_m_current=curr;}
		private:
			node_type* _m_current;

		public:
			typedef chunker_type chunker_type;
			typedef vector_type vector_type;
			static chunker_type as_chunker(node_type* node) {return move(chunker_type(node));}
			static vector_type as_vector(node_type* node) {return move(vector_type(node));}
			static chunker_type const as_chunker(node_type const* node) {return move(chunker_type(const_cast<node_type*>(node)));}
			static vector_type const as_vector(node_type const* node) {return move(vector_type(const_cast<node_type*>(node)));}

			chunk_list_type as_chunk_list() {return move(chunk_list_type(&_m_data));}
			chunk_list_type const as_chunk_list() const {return move(chunk_list_type(const_cast<root_data_type*>(&_m_data)));}
		};
	};

	template <typename rooter_tn>
	struct zvector_center: rooter_tn
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::node_type node_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef value_type* pointer_type;

		typedef typename rooter_type::chunker_type chunker_type;
		typedef typename rooter_type::vector_type vector_type;
		//typedef typetrait_of_zvector<value_tn,allocator_tn> trait;

		rooter_type& rooter() {return *(rooter_type*)(this);}
		rooter_type const& rooter() const {return *(rooter_type const*)(this);}
		bool is_empty() const {return rooter_type::is_empty();}
		dot_type fronter()
		{
			return dot_type(
				rooter().as_chunk_list().fronter().node(),
				as_vector(rooter().as_chunk_list().fronter().node()).begin()
				);
		}
		dot_type backer()
		{
			return dot_type(
				rooter().current(),
				as_vector(rooter().current()).backer()
				);
		}
		dot_type ender()
		{
			return dot_type(
				rooter().current(),
				as_vector(rooter().current()).ender()
				);
		}
		dot_type reserve(size_t size)
		{
			size_t size_usable = size_reserved();
			if (size_usable>=size) return dot_type(current(),as_vector(current()).end());
			chunker_type r = as_chunk_list().push_back(sizable_space_tt<value_type>(size-size_usable));
			if (r.is_empty()) return dot_type();
			as_vector(r.node()).rooter().construct();
			if (!current()) set_current(r.node());
			return dot_type(current(),r.begin());
		}
		void reserve_tegether(size_t size)
		{
			size_t all = 0;
			for (node_type* node =current(); node; node=as_chunker(node).next())
			{
				size_t size_left = as_vector(node).capacity()-as_vector(node).size();
				if (size<=size_left)
				{
					if (node==current()) return;
					node_type* victim = as_chunk_list().remove(node);
					as_chunk_list().insert_node_before(chunker_type(current()),chunker_type(victim));
					set_current(victim);
					return;
				}
			}
			chunker_type chunker_new = as_chunk_list().insert_before_uninitialized(chunker_type(current()),size);
			if (chunker_new.is_empty()) return;
			as_vector(chunker_new.node()).rooter().construct();
			set_current(chunker_new.node());
		}
		size_t size_reserved() const
		{
			size_t all = 0;
			for (node_type const* node =current(); node; node=as_chunker(node).next())
				all += as_vector(node).capacity()-as_vector(node).size();
			return all;
		}
		size_t size() const
		{
			size_t all = 0;
			node_type const* node=as_chunk_list().fronter();
			for (;node;node = as_chunker(node).next())
				all += as_vector(node).size();
			return all;
		}
		dot_type push_back(value_type const& value)
		{
			chunker_type chunker (current());
			value_type* pvalue = as_vector(chunker.node()).push_back(value);
			if (pvalue) return dot_type(chunker.node(),pvalue);
			/// if the vector's size is zero, whether we will skip it, or delete it, until we get an non-zero chunker
			chunker.set_node(chunker.next());
			if (chunker.is_empty())
			{
				chunker = as_chunk_list().push_back_uninitialized(16); /// each reserve 16 items
				if (chunker.is_empty()) return dot_type();
				as_vector(chunker.node()).rooter().construct();
			}
			set_current(chunker.node());
			pvalue = as_vector(chunker.node()).push_back(value);
			return dot_type(chunker.node(),pvalue);
		}
		dot_type push_front(value_type const& value)
		{
			chunker_type chunker (as_chunk_list().fronter());
			value_type* pvalue = as_vector(chunker.node()).push_front(value);
			if (pvalue) return dot_type(chunker.node(),pvalue);
			chunker_type chunker_new = as_chunk_list().push_front_uninitialized(16); /// each reserve 16 items
			if (chunker_new.is_empty()) return dot_type();
			as_vector(chunker_new.node()).rooter().construct();
			if (chunker.is_empty()) set_current(chunker_new.node());
			pvalue = as_vector(chunker_new.node()).push_back(value);
			return dot_type(chunker_new.node(),pvalue);
		}
		dot_type insert_before(dot_type dot,value_type const& value)
		{
			chunker_type chunker (dot.node());
			value_type* value_new = as_vector(chunker.node()).insert(dot._m_value,value);
			if (value_new) return dot_type(chunker.node(),value_new);
			chunker_type chunker_new = as_chunk_list().insert_after_uninitialized(chunker,16); /// reserve 16 items
			if (chunker_new.is_empty()) return dot_type();
			vector_type vector_new = as_vector(chunker_new.node());
			vector_new.rooter().construct();
			as_vector(chunker.node()).split(dot.valueptr(),vector_new);//push_back(value);
			chunker_type chunker_hit (vector_new.is_empty()?chunker_new.node():dot.node());
			value_new = as_vector(chunker_hit.node()).push_back(value);
			if (current()==dot.node()) set_current(chunker_new.node());
			return dot_type(chunker_hit.node(),value_new);
		}
		dot_type remove(dot_type dot)
		{
			as_vector(dot.node()).internal_remove_at(dot.valueptr());
			if (as_vector(dot.node()).is_empty())
			{
				chunker_type nexter (chunker_type(dot.node()).next()); 
				if (!nexter.is_empty() && as_vector(nexter.node()).size()!=0)
					as_chunk_list().remove(dot.node()),
					as_chunk_list().push_back(dot.node());
				else
					set_current(chunker_type(dot.node()).prev());
			}
			return dot;
		}
	};

	template <typename rooter_tn>
	struct iterator_of_zvector
	{
		typedef iterator_of_zvector self_type;
		typedef typename rooter_tn::dot_type dot_type;
		typedef typename rooter_tn::value_type value_type;
		typedef value_type* pointer;
		self_type () {}
		self_type (dot_type const& _dot) :_m_dot(_dot) {}
		dot_type _m_dot;
		dot_type dot() {return _m_dot;}
		self_type& operator++()
		{
			if (_m_dot.valueptr()+1 >= rooter_tn::as_vector(_m_dot.node()).end())
			{
				typedef typename rooter_tn::node_type node_type;
				node_type* node = rooter_tn::as_chunker(_m_dot.node()).next();
				if (node && rooter_tn::as_vector(node).size()>0)
				{
					_m_dot.set_node(rooter_tn::as_chunker(_m_dot.node()).next()),
					_m_dot.set_valueptr(rooter_tn::as_chunker(_m_dot.node()).begin());
					return *this;
				}
			}
			_m_dot.set_valueptr(_m_dot.valueptr()+1);
			return *this;
		}
		self_type& operator--()
		{
			if (_m_dot.valueptr() <= rooter_tn::as_vector(_m_dot.node()).begin())
				_m_dot.set_node(rooter_tn::as_chunker(_m_dot.node()).prev()),
				_m_dot.set_valueptr(rooter_tn::as_chunker(_m_dot.node()).backer().node());
			else
				_m_dot.set_valueptr(_m_dot.valueptr()-1);
			return *this;
		}
		pointer operator->() {return _m_dot.valueptr();}
		value_type& operator*() {return _m_dot.value();}
		value_type const& operator*() const {return _m_dot.value();}
		value_type& value() {return _m_dot.value();}
		value_type const& value() const {return _m_dot.value();}
		bool operator==(self_type const& other) const {return _m_dot==other._m_dot;}
		bool operator!=(self_type const& other) const {return _m_dot!=other._m_dot;}
		
	};
	template <typename rooter_tn>
	struct reverse_iterator_of_zvector
	{
		typedef iterator_of_zvector self_type;
		typedef typename rooter_tn::dot_type dot_type;
		typedef typename rooter_tn::value_type value_type;
		typedef value_type* pointer;

		self_type& operator++()
		{
			--_m_iterator;
			return *this;
		}
		self_type& operator--()
		{
			++_m_iterator;
			return *this;
		}
		pointer operator->() {return _m_iterator.operator->();}
		value_type& operator*() {return _m_iterator.operator*();}
		value_type const& operator*() {return _m_iterator.operator*();}
		value_type& value() {return value();}
		value_type const& value() const {return value();}

		iterator_of_zvector<rooter_tn> _m_iterator;
	};

	template <typename rooter_tn>
	struct scan_line
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef iterator_of_line<dot_type> iterator;

		rooter_type& _m_rooter;
		scan_line(rooter_tn& _rooter): _m_rooter(_rooter) {}
		rooter_type& rooter() {return _m_rooter;}

		iterator begin() {return rooter().begin();}
		iterator end() {return rooter().end();}

		template <typename action_tn>
		bool operator()(action_tn& action)
		{
			iterator begin = begin();
			iterator end = end();
			for (iterator i=begin;i!=end;++i)
			{
				if (!action(i.dot())) return false;
			}
			return true;
		}
	};

	template <typename rooter_tn>
	struct scan_list
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef iterator_of_chunk_list_center<dot_type> iterator;

		rooter_type& _m_rooter;
		scan_list(rooter_tn& _rooter): _m_rooter(_rooter) {}
		rooter_type& rooter() {return _m_rooter;}
		
		iterator begin() {return rooter().head();}
		iterator end() {return iterator();}

		template <typename action_tn>
		bool operator()(action_tn& action)
		{
			iterator _begin = this->begin();
			iterator _end = this->end();
			for (iterator i=_begin;i!=_end;++i)
			{
				if (!action(i.dot())) return false;
			}
			return true;
		}
	};

	template <typename rooter_tn>
	struct scan_zvector
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef iterator_of_zvector<rooter_tn> iterator;

		scan_zvector(rooter_tn& rooter) :_m_rooter(rooter) {}
		rooter_type& rooter() {return _m_rooter;}

		dot_type fronter()
		{
			return dot_type(
				rooter().as_chunk_list().fronter().node(),
				rooter_type::as_vector(rooter().as_chunk_list().fronter().node()).begin()
				);
		}
		dot_type backer()
		{
			return dot_type(
				rooter().current(),
				rooter_type::as_vector(rooter().current()).backer()
				);
		}
		iterator begin() {return iterator(fronter());}
		iterator end()
		{
			return dot_type(
				rooter().current(),
				rooter_type::as_vector(rooter().current()).ender()
				);
		}
		template <typename action_tn>
		bool operator()(action_tn action)
		{
			iterator _begin = begin();
			iterator _end = end();
			for (iterator i=_begin;i!=_end;++i)
			{
				if (!action(i.dot())) return false;
			}
			return true;
		}

		rooter_tn& _m_rooter;
	};

	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct zvector_tt
	{
		typedef typetrait_of_zvector<value_tn,allocator_tn> trait;
		typedef typename trait::zvector_rooter rooter_type;
		typedef zvector_center<rooter_type> type;
	};



	template<typename container_tn>
	struct stack_ts
	{
		typedef typename container_tn::value_type value_type;

		bool is_empty() const {return _m_container.is_empty();}
		stack_ts& push(value_type const& value)
		{
			_m_container.push_back(value);
			return *this;
		}
		template <typename t1,typename t2>
		stack_ts& push(t1& v1,t2& v2)
		{
			_m_container.push_back(value_type(v1,v2));
			return *this;
		}
		template <typename t1,typename t2>
		stack_ts& push(t1 const& v1,t2 const& v2)
		{
			_m_container.push_back(value_type(v1,v2));
			return *this;
		}
		stack_ts& pop()
		{
			_m_container.pop_back();
			return *this;
		}
		stack_ts& pop(size_t size)
		{
			//_m_container.
			return *this;
		}
		value_type const& top() const {return _m_container.back();}
		value_type& top() {return _m_container.back();}
		value_type const& top(size_t i) const {return *_m_container.at(_m_container.size()-1-i);}
		value_type& top(size_t i) {return *_m_container.at(_m_container.size()-i);}
		size_t const size() const {return _m_container.size();}

	private:
		container_tn _m_container;
	};

	template <typename value_tn>
	struct stack_tt
	{
		typedef stack_ts<typename podliner_tt<value_tn>::type> type;
	};

	template <typename unit_tn=char,typename allocator_tn=cppmalloc>
	struct bilateral_space_trait
	{
		typedef unit_tn value_type;
		typedef allocator_tn allocator_type;

		struct chunk_list_node
		{
			typedef chunk_list_node self_type;
			typedef value_type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef size_t size_type;

			self_type* _m_next;
			size_type _m_size_of_chunk;

			size_type size_of_chunk() const {return _m_size_of_chunk;}
			void set_size_of_chunk(size_type size) {_m_size_of_chunk=size;}
			size_t size_of_meta() const {return sizeof(self_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			void set_next(self_type* next) {_m_next=next;}
			self_type* next() const {return _m_next;}
			void set_prev(self_type* prev) {}
			self_type* prev() const {return 0;}
			value_type& value() {return *(value_type*)(this+1);}
			value_type const& value() const {return *(value_type*)(this+1);}
		};

		static bool const __is_chunk_sizable = true;
		static bool const __is_chunk_prev_enabled = false;
		static bool const __is_chunk_tail_enabled = true;

		template <typename size_tn>
		struct share_space_node
		{
			size_tn _m_chunk_size; /// the chunk node size
			bool _m_is_used; /// is used chunk or unused chunk
		};

		struct unused_space_node : share_space_node<size_t>
		{
			typedef unused_space_node self_type;
			typedef value_type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef size_t size_type;

			self_type* _m_prev;
			self_type* _m_next;

			size_type size_of_chunk() const {return _m_chunk_size;}
			void set_size_of_chunk(size_type size) {_m_chunk_size=size;}
			size_t size_of_meta() const {return sizeof(self_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			void set_next(self_type* next) {_m_next=next;}
			self_type* next() const {return _m_next;}
			void set_prev(self_type* prev) {_m_prev=prev;}
			self_type* prev() const {return _m_prev;}
			value_type& value() {return *(value_type*)(this+1);}
			value_type const& value() const {return *(value_type*)(this+1);}
			/// extra
			bool is_used() const {return _m_is_used;}
			void set_is_used(bool _is_used) {_m_is_used=_is_used;}
		};

		struct used_space_node : share_space_node<size_t>
		{
			typedef used_space_node self_type;
			typedef value_type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef size_t size_type;

			self_type* _m_prev;
			self_type* _m_next;
			chunk_list_node* _m_chunk;

			size_type size_of_chunk() const {return _m_chunk_size;}
			void set_size_of_chunk(size_type size) {_m_chunk_size=size;}
			size_t size_of_meta() const {return sizeof(self_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			void set_next(self_type* next) {_m_next=next;}
			self_type* next() const {return _m_next;}
			void set_prev(self_type* prev) {_m_prev=prev;}
			self_type* prev() const {return _m_prev;}
			value_type& value() {return *(value_type*)(this+1);}
			value_type const& value() const {return *(value_type*)(this+1);}
			/// extra
			bool is_used() const {return _m_is_used;}
			void set_is_used(bool _is_used) {_m_is_used=_is_used;}
			chunk_list_node* chunk() const {return _m_chunk;}
			void set_chunk(chunk_list_node* _chunk) {_m_chunk=_chunk;}
		};

		static bool const __is_space_prev_enabled = true;
		static bool const __is_space_sizable = true;
		static bool const __is_space_tail_enabled = true;
		struct space_root_data
		{
			chunk_list_node* _m_head_chunk_list;
			chunk_list_node* _m_tail_chnnk_list;
			used_space_node* _m_head_used_list;
			used_space_node* _m_tail_used_list;
			unused_space_node* _m_head_unused_list;
			unused_space_node* _m_tail_unused_list;
		};
		struct chunk_list_root_data: space_root_data
		{
			typedef chunk_list_node node_type;
			node_type* head() {return _m_head_chunk_list;}
			node_type* tail() {return _m_tail_chnnk_list;}
			node_type const* head() const {return _m_head_chunk_list;}
			node_type const* tail() const {return _m_tail_chnnk_list;}
			void set_head(node_type* _head) {_m_head_chunk_list=_head;}
			void set_tail(node_type* _tail) {_m_tail_chnnk_list=_tail;}
		};
		struct used_list_root_data: space_root_data
		{
			typedef used_space_node node_type;
			node_type* head() {return _m_head_used_list;}
			node_type* tail() {return _m_tail_used_list;}
			node_type const* head() const {return _m_head_used_list;}
			node_type const* tail() const {return _m_tail_used_list;}
			void set_head(node_type* _head) {_m_head_used_list=_head;}
			void set_tail(node_type* _tail) {_m_tail_used_list=_tail;}
		};
		struct unused_list_root_data: space_root_data
		{
			typedef unused_space_node node_type;
			node_type* head() {return _m_head_unused_list;}
			node_type* tail() {return _m_tail_unused_list;}
			node_type const* head() const {return _m_head_unused_list;}
			node_type const* tail() const {return _m_tail_unused_list;}
			void set_head(node_type* _head) {_m_head_unused_list=_head;}
			void set_tail(node_type* _tail) {_m_tail_unused_list=_tail;}
		};

		//// {size_t,flag,next,data(...),size_t}
		//// {size_t,flag,next,chunk-head,data(...),size_t}
		template <bool is_used_node_tc,bool is_const_node_tc,typename derived_tn=void>
		struct space_dot_tt
		{
			typedef space_dot_tt self_type;
			//typedef space_dot_tt<is_used_node_tc,true,derived_tn> self_const_type;
			typedef typename met::iff<met::is_same<derived_tn,void>::value,self_type,derived_tn>::type derived_type;
			typedef typename met::iff<is_used_node_tc,used_space_node,unused_space_node>::type which_node;
			typedef typename met::iff<is_const_node_tc,which_node const,which_node>::type node_type;
			typedef typename met::iff<is_const_node_tc,typename node_type::value_type const,typename node_type::value_type>::type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef typename value_type* pointer_type;
			typedef typename value_type const* pointer_const_type;
			typedef size_t size_type;

			static bool is_sizable() {return __is_space_sizable;}
			bool is_empty() const {return _m_chunk==0;}
			explicit self_type(node_type* node=0) : _m_chunk(node) {}
			operator node_type* () {return _m_chunk;}
			node_type* node() {return _m_chunk;}
			node_type const* node() const {return _m_chunk;}
			void set_node(node_type* node) {_m_chunk=node;}
			static size_t size_of_space(size_t value_count) {return size_of_meta()+value_count*sizeof(value_type);}
			static size_t size_of_meta() {return sizeof(node_type)+sizeof(size_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			size_t size_of_chunk() const {return _m_chunk->size_of_chunk();}
			void set_size_of_chunk(size_t size) {_m_chunk->set_size_of_chunk(size);*((size_t*)((char*)_m_chunk+size_of_chunk())-1) = size;}
			size_t size() const {return size_of_data()/sizeof(value_type);}
			void set_next(node_type* chunk) {_m_chunk->set_next(chunk);}
			node_type* next() {return _m_chunk->next();}
			node_type const* next() const {return _m_chunk->next();}
			void set_prev(node_type* chunk) {_m_chunk->set_prev(chunk);}
			node_type* prev() {return _m_chunk->prev();}
			node_type const* prev() const {return _m_chunk->prev();}
			bool is_prev_enabled() const {return __is_space_prev_enabled;}
			pointer_type begin() {return &value();}
			pointer_const_type begin() const {return &value();}
			pointer_type end() {return begin()+size_of_data();}
			pointer_const_type end() const {return begin()+size_of_data();}
			reference value() {return _m_chunk->value();}
			const_reference value() const {return _m_chunk->value();}


			derived_type& assign(value_type const& _value){helper::memmove(begin(),&_value,sizeof(_value));return *this;}
			derived_type& construct(value_type const& _value){helper::memmove(begin(),&_value,sizeof(_value));return *this;}
			template <typename type_tn>	derived_type& assign(sizable_space_tt<type_tn const> const& _value)
			{
				helper::memmove(begin(),_value.begin,min(_value.size,size_of_chunk()));
				return *this;
			}
			template <typename type_tn>
			derived_type& construct(sizable_space_tt<type_tn const> const& _value)
			{
				helper::memmove(begin(),_value.begin,min(_value.size,size_of_chunk()));
				return *this;
			}
			node_type* _m_chunk;


			/// extended method

			derived_type& construct(bool _is_used)
			{
				set_size_of_chunk(size_of_meta());
				set_next(0);
				set_prev(0);
				set_is_used(_is_used);
				return *static_cast<derived_type*>(this);
			}

			bool is_used() const {return _m_chunk->is_used();}
			void set_is_used(bool _is_used) {_m_chunk->set_is_used(_is_used);}

			/// shrink the node, and return the available space
			/// all from the head position
			derived_type& shrink_head(int size_shrinked)
			{
				assert(!is_used() && "node should be unused node type");
				size_t old_size = size_of_chunk();
				node_type* node_shrinked = (node_type*)((char*)node()+size_shrinked*sizeof(value_type));
				helper::memmove<char>((char*)node_shrinked,(char*)node(),sizeof(node_type));
				node_shrinked->set_size_of_chunk(old_size-size_shrinked);
				set_node(node_shrinked); /// the head position changed
				return *static_cast<derived_type*>(this);
			}
			derived_type& shrink_tail(int size_shrinked)
			{
				_m_chunk->set_size_of_chunk(_m_chunk->size_of_chunk()+size_shrinked);
				return *static_cast<derived_type*>(this);
			}
			derived_type get_prev()
			{
				size_t* left_space_size = (size_t*)(node())-1;
				node_type* left_space = (node_type*)((char*)node() - *left_space_size);
				derived_type left_unused_dot (left_space);
				return left_unused_dot;
			}
			derived_type get_next()
			{
				node_type* right_space = (node_type*)((char*)node() + size_of_chunk());
				derived_type right_unused_dot (right_space);
				return right_unused_dot;
			}
			static node_type* to_node(value_type const* _dataptr)
			{
				return (node_type*)((char*)(_dataptr)-size_of_meta()+sizeof(size_type));
			}
		};

		template <bool is_const_tc>
		struct used_space_dot_tt : space_dot_tt<true,is_const_tc,used_space_dot_tt<is_const_tc>>
		{
			typedef space_dot_tt<true,is_const_tc,used_space_dot_tt> base_type;
			typedef used_space_dot_tt self_type;
			typedef used_space_dot_tt<true> self_const_type;
			explicit self_type(node_type* node=0) : base_type(node) {}
			self_type& construct() {base_type::construct(true),node()->set_chunk(0);return *this;}
			void set_chunk(chunk_list_node* _chunk) {node()->set_chunk(_chunk);}
		};
		template <bool is_const_tc>
		struct unused_space_dot_tt : space_dot_tt<false,is_const_tc,unused_space_dot_tt<is_const_tc>>
		{
			typedef space_dot_tt<false,is_const_tc,unused_space_dot_tt> base_type;
			typedef unused_space_dot_tt self_type;
			typedef unused_space_dot_tt<true> self_const_type;
			explicit self_type(node_type* node=0) : base_type(node) {}
			self_type& construct() {base_type::construct(false);return *this;}
		};
		typedef used_space_dot_tt<false> used_space_dot;
		typedef unused_space_dot_tt<false> unused_space_dot;

		typedef chunk_dot_tt<chunk_list_node,__is_chunk_prev_enabled,__is_chunk_sizable> chunk_noder_type;
		typedef chunk_list_rooter_ref<chunk_noder_type,chunk_list_root_data,allocator_type,__is_chunk_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> chunk_list_type;

		typedef chunk_list_rooter_ref<used_space_dot,used_list_root_data,nullmalloc,__is_space_tail_enabled> used_rooter_type;
		typedef chunk_list_center<used_rooter_type> used_list_type;

		typedef chunk_list_rooter_ref<unused_space_dot,unused_list_root_data,nullmalloc,__is_space_tail_enabled> unused_rooter_type;
		typedef chunk_list_center<unused_rooter_type> unused_list_type;

		//typedef used_space_dot bilateral_space_dot;

		//struct bilateral_space_dot
		//{
		//	typedef bilateral_space_dot self_type;
		//	typedef node_type node_type;
		//	typedef value_type value_type;

		//	self_type(node_type* chunk=0,value_type* value=0)
		//		: _m_node(chunk) ,_m_value(value)
		//	{}

		//	value_type& value() {return *_m_value;}
		//	value_type const& value() const {return *_m_value;}
		//	node_type* node() {return _m_node;}
		//	node_type const* node() const {return _m_node;}
		//	value_type* valueptr() {return _m_value;}
		//	value_type const* valueptr() const {return _m_value;}
		//	void set_node(node_type* node) {_m_node=node;}
		//	void set_valueptr(value_type* valueptr) {_m_value=valueptr;}
		//	void set(node_type* node,value_type* valueptr) {_m_node=node,_m_value=valueptr;}

		//	bool operator==(self_type const& other) const {return _m_node==other._m_node && _m_value==other._m_value;}
		//	bool operator!=(self_type const& other) const {return _m_node!=other._m_node || _m_value!=other._m_value;}

		//	node_type* _m_node;
		//	value_type* _m_value;
		//};


		struct bilateral_space_rooter
			: common_data_tt<space_root_data,allocator_type>
		{
			typedef bilateral_space_rooter self_type;
			typedef value_type value_type;
			typedef allocator_tn allocator_type;
			typedef used_space_dot dot_type;
			typedef used_space_node node_type;

			self_type()
			{
				static_cast<chunk_list_root_data*>(&_m_data)->set_head(0);
				static_cast<chunk_list_root_data*>(&_m_data)->set_tail(0);
				static_cast<used_list_root_data*>(&_m_data)->set_head(0);
				static_cast<used_list_root_data*>(&_m_data)->set_tail(0);
				static_cast<unused_list_root_data*>(&_m_data)->set_head(0);
				static_cast<unused_list_root_data*>(&_m_data)->set_tail(0);
			}
			bool is_empty() const {return as_chunk_list().is_empty();}


		public:
			typedef space_root_data space_root_data_type;
			typedef chunk_list_type chunk_list_type;
			typedef used_list_type used_list_type;
			typedef unused_list_type unused_list_type;

			typedef typename unused_list_type::dot_type unused_dot;
			typedef typename used_list_type::dot_type used_dot;

			used_list_type as_used_list() {return move(used_list_type(static_cast<used_list_root_data*>(&_m_data)));}
			unused_list_type as_unused_list() {return move(unused_list_type(static_cast<unused_list_root_data*>(&_m_data)));}
			chunk_list_type as_chunk_list() {return move(chunk_list_type(static_cast<chunk_list_root_data*>(&_m_data)));}
			used_list_type const as_used_list() const {return move(used_list_type(static_cast<used_list_root_data*>(const_cast<space_root_data_type*>(&_m_data))));}
			unused_list_type const as_unused_list() const {return move(unused_list_type(static_cast<unused_list_root_data*>(const_cast<space_root_data_type*>(&_m_data))));}
			chunk_list_type const as_chunk_list() const {return move(chunk_list_type(static_cast<chunk_list_root_data*>(const_cast<space_root_data_type*>(&_m_data))));}
		};
	};

	template <typename rooter_tn>
	struct bilateral_space_center: rooter_tn
	{
		typedef bilateral_space_center self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::node_type node_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef value_type* pointer_type;

		typedef typename rooter_type::unused_dot unused_dot;
		typedef typename rooter_type::used_dot used_dot;

		rooter_type& rooter() {return *(rooter_type*)(this);}
		rooter_type const& rooter() const {return *(rooter_type const*)(this);}
		bool is_empty() const {return rooter_type::is_empty();}

		unused_dot push_back_chunk(size_t size)
		{
			/// get the aux size of the chunk
			typedef typename used_dot::node_type used_node;
			typedef typename used_dot::size_type size_type;
			size_t _size_of_chunk = used_dot::size_of_space(0) + unused_dot::size_of_space(size) + used_dot::size_of_space(0);
			/// new a chunk
			typedef typename chunk_list_type::dot_type chunk_dot_type;
			chunk_dot_type d = rooter().as_chunk_list().push_back(sizable_space_tt<value_type>(_size_of_chunk));
			if (d.is_empty()) return unused_dot();
			/// adds the wall of the both ends
			used_node* p = (used_node*)d.begin();
			value_type* pend = d.end();
			used_dot headpad (p);
			headpad.construct();
			used_dot tailpad ((used_node*)(d.end()-used_dot::size_of_space(0)));
			tailpad.construct();
			/// push back the middle part to the unused list
			unused_dot middle_unused_dot ((typename unused_dot::node_type*)((char*)p + headpad.size_of_chunk()));
			middle_unused_dot.set_is_used(false);
			middle_unused_dot.set_size_of_chunk(d.size_of_data()-headpad.size_of_chunk()-tailpad.size_of_chunk());
			assert((char*)d.begin()==(char*)headpad.node());
			assert((char*)headpad.end()+sizeof(size_type)==(char*)middle_unused_dot.node());
			assert((char*)middle_unused_dot.end()+sizeof(size_type)==(char*)tailpad.node());
			as_unused_list().push_back(middle_unused_dot.node()); /// must be successfully, just some linking action
			return middle_unused_dot;
		}
		/// find the first fitting
		/// this is the strategy that we can change, here is the default way
		unused_dot find_unused(size_t size)
		{
			typedef typename unused_list_type::iterator I;
			unused_list_type& unused_list = as_unused_list();
			I _begin = unused_list.begin();
			I _end = unused_list.end();
			for (I i=_begin;i!=_end;++i)
				if (i.dot().size()>=size)
					return i.dot();
			return unused_dot();
		}
		value_type* allocate(size_t size)
		{
			size_t size_bck = size;
			/// if container is empty, create one
			if (rooter().is_empty())
			{
				unused_dot undot = push_back_chunk(size);
				if (undot.is_empty()) return 0;
			}

			/// find one from unused chunk-list
			size_t size_of_used_node = used_dot::size_of_space(size);
			unused_dot undot = find_unused(size_of_used_node);
			/// if unused chunk-list is out, we get a new from base memory
			if (undot.is_empty())
			{
				size_t minsize = used_dot::size_of_space(size);
				undot = push_back_chunk(minsize);
				if (undot.is_empty()) return 0;
			}
			bool is_head_hit = undot.node()==as_unused_list().head();
			/// shrink the chunk
			value_type* data = (value_type*)undot.node();
			if (undot.size_of_data()<=used_dot::size_of_space(size))
			{
				/// if the whole unused-chunk is used
				as_unused_list().remove(undot.node());
				size_of_used_node = undot.size_of_chunk();
			}
			else
			{
				/// if the node is head, we should change the head
				undot.shrink_head(size_of_used_node);
				if (is_head_hit)
				{
					if (as_unused_list().head()==as_unused_list().tail())
						as_unused_list().set_tail(undot.node());
					as_unused_list().set_head(undot.node());
				}
			}
			/// push the used chunk to the used chunk-list
			used_dot udot ((typename used_dot::node_type*)(data));
			udot.set_is_used(true);
			udot.set_size_of_chunk(size_of_used_node);
			udot.set_chunk(0); /// where is the chunk, we can get from last used_chunk or, get the chunk from the result of find_unused()
			as_used_list().push_back(udot.node());

			///return the data
			return udot.begin();
		}
		void deallocate(void* ptr)
		{
			if (!ptr) return;

			used_dot victim (used_dot::to_node((value_type*)(ptr)));
			unused_dot victim_bck ((typename unused_dot::node_type*)(victim.node()));
			used_dot prever = victim.get_prev();
			used_dot nexter = victim.get_next();
			size_t victim_size = victim.size_of_chunk();
			
			as_used_list().remove(victim);

			if (!prever.is_used())
			{
				size_t size_shrink = victim_size;
				if (!nexter.is_used()) size_shrink += nexter.size_of_chunk();
				prever.shrink_tail(size_shrink);
				victim = prever;
			}
			else if (!nexter.is_used())
			{
				nexter.shrink_head(victim_size);
			}
			else
			{
				victim_bck.set_is_used(false);
				as_unused_list().push_back(victim_bck);
			}
		}
	};

	template <typename value_tn=char,typename allocator_tn=cppmalloc>
	struct bilateral_space_tt
	{
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef typename bilateral_space_trait<value_tn,allocator_tn>::bilateral_space_rooter rooter_type;
		typedef bilateral_space_center<rooter_type> type;
	};

	/// the same width space allocator
	template <typename value_tn,typename allocator_tn=cppmalloc>
	struct same_space_trait
	{
		typedef value_tn value_type;
		typedef size_t size_type;
		typedef allocator_tn allocator_type;

		template <typename t,typename address_tn=size_t>
		struct ptr_tt
		{
			typedef ptr_tt self_type;
			typedef t value_type;
			address_tn v;
			//value_type& operator*() {return 0;}
			value_type& value(value_type* base) {return *(base+v);}
			value_type* memory(value_type* base) {return base+v;}
			value_type address() {return v;}
			self_type& operator=(value_type addr) {v=addr;return *this;}
			self_type& operator+=(value_type relsize) {v+=relsize;return *this;}
			self_type& operator-=(value_type relsize) {v-=relsize;return *this;}
			bool operator==(self_type const& other) {return v==other.v;}
			bool operator!=(self_type const& other) {return v!=other.v;}
			bool operator>(self_type const& other) {return v>other.v;}
			bool operator<(self_type const& other) {return v<other.v;}
			bool operator>=(self_type const& other) {return v>=other.v;}
			bool operator<=(self_type const& other) {return v<=other.v;}
		};
		typedef ptr_tt<value_type,size_t> ptr_t;
		struct chunk_list_node
		{
			typedef chunk_list_node self_type;
			typedef value_type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef size_type size_type;

			self_type* _m_next;
			size_type _m_size_of_chunk;
			ptr_t _m_begin; /// if _m_begin == -1, no usable data
			self_type* _m_next_unused;

			size_type size_of_chunk() const {return _m_size_of_chunk;}
			void set_size_of_chunk(size_type size) {_m_size_of_chunk=size;}
			size_t size_of_meta() const {return sizeof(self_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			void set_next(self_type* next) {_m_next=next;}
			self_type* next() const {return _m_next;}
			void set_prev(self_type* prev) {}
			self_type* prev() const {return 0;}
			value_type& value() {return *(value_type*)(this+1);}
			value_type const& value() const {return *(value_type*)(this+1);}
			/// extended method
			ptr_t& unused_begin() {return _m_begin;}
			void set_next_unused(self_type* next) {_m_next_unused=next;}
			self_type* next_unused() const {return _m_next_unused;}
		};
		static bool const __is_chunk_sizable = true;
		static bool const __is_chunk_prev_enabled = false;
		static bool const __is_chunk_tail_enabled = false;
		//// {size_t,flag,next,data(...),size_t}
		//// {size_t,flag,next,chunk-head,data(...),size_t}
		template <bool is_const_node_tc,typename derived_tn=void>
		struct chunk_dot_tt
		{
			typedef chunk_dot_tt self_type;
			typedef chunk_dot_tt<true,derived_tn> self_const_type;
			typedef typename met::iff<met::is_same<derived_tn,void>::value,self_type,derived_tn>::type derived_type;
			typedef typename met::iff<is_const_node_tc,chunk_list_node const,chunk_list_node>::type node_type;
			typedef typename met::iff<is_const_node_tc,value_type const,value_type>::type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef typename value_type* pointer_type;
			typedef typename value_type const* pointer_const_type;
			typedef size_t size_type;

			static bool is_sizable() {return __is_space_sizable;}
			bool is_empty() const {return _m_chunk==0;}
			explicit self_type(node_type* node=0) : _m_chunk(node) {}
			operator node_type* () {return _m_chunk;}
			node_type* node() {return _m_chunk;}
			node_type const* node() const {return _m_chunk;}
			void set_node(node_type* node) {_m_chunk=node;}
			static size_t size_of_space(size_t value_count) {return size_of_meta()+value_count*sizeof(value_type);}
			static size_t size_of_meta() {return sizeof(node_type);}
			size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			size_t size_of_chunk() const {return _m_chunk->size_of_chunk();}
			void set_size_of_chunk(size_t size) {_m_chunk->set_size_of_chunk(size);*((size_t*)((char*)_m_chunk+size_of_chunk())-1) = size;}
			size_t size() const {return size_of_data()/sizeof(value_type);}
			void set_next(node_type* chunk) {_m_chunk->set_next(chunk);}
			node_type* next() {return _m_chunk->next();}
			node_type const* next() const {return _m_chunk->next();}
			void set_prev(node_type* chunk) {_m_chunk->set_prev(chunk);}
			node_type* prev() {return _m_chunk->prev();}
			node_type const* prev() const {return _m_chunk->prev();}
			bool is_prev_enabled() const {return __is_space_prev_enabled;}
			pointer_type begin() {return &value();}
			pointer_const_type begin() const {return &value();}
			pointer_type end() {return begin()+size_of_data();}
			pointer_const_type end() const {return begin()+size_of_data();}
			reference value() {return _m_chunk->value();}
			const_reference value() const {return _m_chunk->value();}

			node_type* _m_chunk;

			/// extended method

			derived_type& construct_data()
			{
				value_type* _begin = &_m_chunk->value();
				size_type _count = _m_chunk->size_of_data();
				for (size_type i=1;i<_count;++i)
					*_begin++ = i;
				*_begin = 0;
				_m_chunk->unused_begin() = 1; /// when is 0, means, no usable space
				return *static_cast<derived_type*>(this);
			}

			ptr_t unused_begin() const {return _m_chunk->unused_begin();}
			void set_unused_begin(size_type _used_begin) {_m_chunk->unused_begin()=_used_begin;}
			bool is_any_used() const {return _m_chunk->unused_begin()==-1;}
		};

		template <bool is_const_node_tc,typename derived_tn=void>
		struct unused_dot_tt
		{
			typedef unused_dot_tt self_type;
			typedef unused_dot_tt<true,derived_tn> self_const_type;
			typedef typename met::iff<met::is_same<derived_tn,void>::value,self_type,derived_tn>::type derived_type;
			typedef typename met::iff<is_const_node_tc,chunk_list_node const,chunk_list_node>::type node_type;
			typedef typename met::iff<is_const_node_tc,value_type const,value_type>::type value_type;
			typedef value_type& reference;
			typedef value_type const& const_reference;
			typedef typename value_type* pointer_type;
			typedef typename value_type const* pointer_const_type;
			typedef size_t size_type;

			static bool is_sizable() {return __is_space_sizable;}
			bool is_empty() const {return _m_chunk==0;}
			explicit self_type(node_type* node=0) : _m_chunk(node) {}
			operator node_type* () {return _m_chunk;}
			node_type* node() {return _m_chunk;}
			node_type const* node() const {return _m_chunk;}
			void set_node(node_type* node) {_m_chunk=node;}
			//static size_t size_of_space(size_t value_count) {return size_of_meta()+value_count*sizeof(value_type);}
			//static size_t size_of_meta() {return sizeof(node_type);}
			//size_t size_of_data() const {return size_of_chunk()-size_of_meta();}
			//size_t size_of_chunk() const {return _m_chunk->size_of_chunk();}
			//void set_size_of_chunk(size_t size) {}
			//size_t size() const {return size_of_data()/sizeof(value_type);}
			void set_next(node_type* chunk) {_m_chunk->set_next_unused(chunk);}
			node_type* next() {return _m_chunk->next_unused();}
			node_type const* next() const {return _m_chunk->next_unused();}
			void set_prev(node_type* chunk) {_m_chunk->set_prev(chunk);}
			node_type* prev() {return _m_chunk->prev();}
			node_type const* prev() const {return _m_chunk->prev();}
			bool is_prev_enabled() const {return __is_space_prev_enabled;}
			pointer_type begin() {return &value();}
			//pointer_const_type begin() const {return &value();}
			//pointer_type end() {return begin()+size_of_data();}
			//pointer_const_type end() const {return begin()+size_of_data();}
			reference value() {return _m_chunk->value();}
			//const_reference value() const {return _m_chunk->value();}

			node_type* _m_chunk;

			/// extended method

			//ptr_t unused_begin() const {return _m_chunk->unused_begin();}
			//void set_unused_begin(size_type _used_begin) {_m_chunk->unused_begin()=_used_begin;}
			bool unused_is_empty() const {return _m_chunk->unused_begin().address()==0;}
			ptr_t unused_pop_front()
			{
				ptr_t r =_m_chunk->unused_begin();
				_m_chunk->unused_begin()=_m_chunk->unused_begin().value(begin());
				return r;
			}
			void unused_push_front(value_type* ptr)
			{
				size_t index = ptr-_m_chunk->begin();
				*ptr = _m_chunk->unused_begin().address();
				_m_chunk->unused_begin() = index;
			}
		};

		struct same_root_data
		{
			chunk_list_node* _m_head_chunk_list;
			chunk_list_node* _m_head_unused_list;
		};
		struct chunk_list_root_data: same_root_data
		{
			typedef chunk_list_node node_type;
			node_type* head() {return _m_head_chunk_list;}
			node_type* tail() {return 0;}
			node_type const* head() const {return _m_head_chunk_list;}
			node_type const* tail() const {return 0;}
			void set_head(node_type* _head) {_m_head_chunk_list=_head;}
			void set_tail(node_type* _tail) {}
		};
		struct unused_list_root_data: same_root_data
		{
			typedef chunk_list_node node_type;
			node_type* head() {return _m_head_unused_list;}
			node_type* tail() {return 0;}
			node_type const* head() const {return _m_head_unused_list;}
			node_type const* tail() const {return 0;}
			void set_head(node_type* _head) {_m_head_unused_list=_head;}
			void set_tail(node_type* _tail) {}
		};

		typedef chunk_dot_tt<false> chunk_noder_type;
		typedef chunk_list_root_data chunk_root_data_type;
		typedef chunk_list_rooter_ref<chunk_noder_type,chunk_root_data_type,allocator_type,__is_chunk_tail_enabled> rooter_type;
		typedef chunk_list_center<rooter_type> chunk_list_type;

		typedef unused_dot_tt<false> unused_noder_type;
		typedef unused_list_root_data unused_root_data_type;
		typedef chunk_list_rooter_ref<unused_noder_type,unused_root_data_type,nullmalloc,__is_chunk_tail_enabled> unused_rooter_type;
		typedef chunk_list_center<unused_rooter_type> unused_list_type;

		struct same_space_rooter
			: common_data_tt<same_root_data,allocator_type>
		{
			typedef same_space_rooter self_type;
			typedef value_type value_type;
			typedef allocator_tn allocator_type;
			typedef chunk_dot_tt<false> dot_type;
			typedef chunk_list_node node_type;
			typedef unused_dot_tt<false> unused_dot_type;

			self_type()
			{
				static_cast<chunk_list_root_data*>(&_m_data)->set_head(0);
				static_cast<unused_list_root_data*>(&_m_data)->set_head(0);
			}
			bool is_empty() const {return as_chunk_list().is_empty();}

		public:
			typedef same_root_data same_root_data_type;
			typedef chunk_list_type chunk_list_type;
			typedef unused_list_type unused_list_type;
			typedef ptr_t ptr_t;

			chunk_list_type as_chunk_list() {return move(chunk_list_type(static_cast<chunk_root_data_type*>(&_m_data)));}
			chunk_list_type const as_chunk_list() const {return move(chunk_list_type(static_cast<same_root_data_type*>(const_cast<space_root_data_type*>(&_m_data))));}

			unused_list_type as_unused_list() {return move(unused_list_type(static_cast<unused_root_data_type*>(&_m_data)));}
			unused_list_type const as_unused_list() const {return move(unused_list_type(static_cast<unused_root_data_type*>(const_cast<same_root_data_type*>(&_m_data))));}
		};

	};
	template <typename rooter_tn>
	struct same_space_center: rooter_tn
	{
		typedef same_space_center self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::node_type node_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::dot_type dot_type;
		typedef value_type* pointer_type;
		typedef typename rooter_type::unused_dot_type unused_dot;

		rooter_type& rooter() {return *(rooter_type*)(this);}
		rooter_type const& rooter() const {return *(rooter_type const*)(this);}
		bool is_empty() const {return rooter_type::is_empty();}

		dot_type push_back_chunk(size_t size)
		{
			static size_t const __max_size_of_value_type = sizeof(value_type)>sizeof(size_t)?sizeof(size_t):sizeof(value_type);
			static size_t const __max_number_of_value_type = ~((-1)<<(__max_size_of_value_type*8));
			assert(size<=__max_number_of_value_type && "size is greater than max number of the value_type");
			/// get the aux size of the chunk
			size_t _size_of_chunk = size+1; /// the extra one reserved for null ptr
			/// new a chunk
			dot_type d = rooter().as_chunk_list().push_front(sizable_space_tt<value_type>(_size_of_chunk));
			if (d.is_empty()) return dot_type();
			d.construct_data();
			rooter().as_unused_list().push_front(d.node());
			return d;
		}
		dot_type find(value_type* ptr)
		{
			typedef typename chunk_list_type::iterator I;
			I i = rooter().as_chunk_list().begin();
			I iend = rooter().as_chunk_list().end();
			for (;i!=iend;++i)
			{
				bool isin = ptr>i.dot().begin() && ptr<i.dot().end();
				if (isin) break;
			}
			return i.dot();
		}
		value_type* allocate()
		{
			unused_list_type& unused_list = rooter().as_unused_list();
			if (unused_list.is_empty())
			{
				dot_type d = push_back_chunk(2);
				if (d.is_empty())
					return 0;
			}
			if (unused_list.is_empty())
				return 0;
			unused_dot_type dot = unused_list.fronter();
			ptr_t ptr = dot.unused_pop_front();
			if (dot.unused_is_empty())
				unused_list.remove_front();
			return ptr.memory(dot.begin());
		}
		void deallocate(value_type* ptr)
		{
			dot_type d = find(ptr);
			if (d.is_empty()) return;
			unused_dot undot (d.node());
			bool unused_is_empty = undot.unused_is_empty();
			undot.unused_push_front(ptr);
			if (unused_is_empty)
				as_unused_list().push_front(undot);
		}
	};
	template <typename value_tn=char,typename allocator_tn=cppmalloc>
	struct same_space_tt
	{
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef typename same_space_trait<value_tn,allocator_tn>::same_space_rooter rooter_type;
		typedef same_space_center<rooter_type> type;
	};

	/// indirect container
	template <typename value_tn,typename key_tn,typename keyof_tn,typename cmp_tn,typename allocator_tn>
	struct veclist_trait
	{
		typedef veclist_trait traiter;
		struct compare_tt;
		typedef value_tn value_type;
		typedef key_tn key_type;
		typedef value_tn base_value_type;
		typedef value_tn const const_value_type;
		typedef value_type* valueptr_type;
		typedef value_type const* const_valueptr_type;
		typedef podline_head_tt<size_t> vector_head_t;
		typedef allocator_tn allocator_type;
		typedef cmp_tn key_compare_type;
		template <bool iscon_tc> struct value_tt {typedef value_type type;};
		template <> struct value_tt<true> {typedef value_type const type;};
		template <bool iscon_tc> struct valueptr_tt {typedef value_type* type;};
		template <> struct valueptr_tt<true> {typedef value_type const* type;};

		/// the first condition, owner
		//typedef podline4_rooter<valueptr_type,size_t,vector_head_t,0,allocator_type> suffix_owner_rooter;
		//typedef line_value_center<suffix_owner_rooter> suffix_lineowner_type;
		typedef typename listref_tt<value_type>::type value_listref_type;
		typedef typename listref_tt<value_type>::rootdata_type value_listrootdata_type;
		typedef typename value_listref_type::node_type list_node_type;
		typedef typename value_listref_type::dot_type list_dot_type;
		typedef typename value_listref_type::iterator list_iterator_type;
		typedef list_node_type* list_nodeptr_type;
		struct vekeyof_type : keyof_tn
		{
			key_type const& operator()(list_nodeptr_type const& v) const
			{
				return keyof_tn::operator()(v->value());
			}
		};
		typedef line_value_container_rooter_ref<vector_head_t,list_nodeptr_type,allocator_type> sorted_vector_rooter_type;
		typedef sortedline_value_center<sorted_vector_rooter_type,vekeyof_type,key_compare_type> sorted_vectorref_type;
		//typedef typename sorted_vector_rooter_type::iterator vector_iterator_type;

		struct simple_list_tt
		{
			typedef value_type value_type;
			typedef valueptr_type dot_type;
			typedef dot_type iterator;
			/// ********************************* a very crazy drawback in VC2010
			/// simple_list_tt(allocator_type* _allocate,void const*=0): _m_alloator(*_allocate) {}
			/// what's the difference with the followed line, the space between "const*" and "=0" in the second parameter
			/// if you do not insert an space, the compiler will report an error in VC2010
			/// *********************************
			simple_list_tt(allocator_type* _allocate,void const* =0): _m_alloator(*_allocate) {}
			allocator_type& _m_alloator;
			void remove(valueptr_type ptr)
			{
				_m_alloator.deallocate(ptr);
			}
			valueptr_type insert(value_type const& v)
			{
				char* ptr = _m_alloator.allocate(sizeof(value_type));
				if (!ptr) return 0;
				new (ptr) value_type (v);
				return (valueptr_type)ptr;
			}
		};

		template <bool iscon_tc>
		struct dot_type
		{
			typedef dot_type self_type;
			self_type(list_nodeptr_type nodeptr=0)
			{
				_m_list_nodeptr = nodeptr;
				_m_list_nodepptr = 0;
			}
			self_type(list_nodeptr_type nodeptr,list_nodeptr_type* nodepptr)
			{
				_m_list_nodeptr = nodeptr;
				_m_list_nodepptr = nodepptr;
				assert((nodepptr || nodeptr==*nodepptr) && "not the same node");
			}
			list_node_type* _m_list_nodeptr; /// for list
			list_node_type** _m_list_nodepptr; /// for vector
			value_type& value() {return _m_list_nodeptr->value();}
			value_type const& value() const {return _m_list_nodeptr->value();}
			value_type& operator*() {return value();}
			value_type const& operator*() const {return value();}
		};
		template <>
		struct dot_type <true>
		{
			typedef dot_type self_type;
			self_type(list_node_type const* nodeptr=0)
			{
				_m_list_nodeptr = nodeptr;
				_m_list_nodepptr = 0;
			}
			self_type(list_node_type const* nodeptr,list_node_type const** nodepptr)
			{
				_m_list_nodeptr = nodeptr;
				_m_list_nodepptr = nodepptr;
				assert((nodepptr || nodeptr==*nodepptr) && "not the same node");
			}
			list_node_type const* _m_list_nodeptr; /// for list
			list_node_type const** _m_list_nodepptr; /// for vector
			value_type const& value() const {return _m_list_nodeptr->value();}
			value_type const& operator*() const {return value();}
		};

		struct rooterref_tt
		{
			typedef value_type value_type;
			typedef allocator_type allocator_type;
			typedef vector_head_t vector_head_t;
			typedef sorted_vectorref_type vectorref_type;
			typedef value_listref_type listref_type;
			typedef value_listrootdata_type list_head_type;
			typedef typename vectorref_type::dot_type vdot_type;
			typedef typename listref_type::dot_type ldot_type;

			allocator_type* _m_allocator;
			vector_head_t** _m_vector_head;
			list_head_type* _m_list_head;

			rooterref_tt(vector_head_t** vechead,list_head_type* listhead,allocator_type* _allocator)
			{
				_m_vector_head=vechead;
				_m_list_head=listhead;
				_m_allocator=_allocator;
			}
			bool is_list_enabled() const {return true;}
			allocator_type& allocator() {return *_m_allocator;}
			vectorref_type as_vector() {return vectorref_type(_m_allocator,_m_vector_head);}
			vectorref_type const as_vector() const {return vectorref_type((allocator_type*)0,_m_vector_head);}
			listref_type as_list() {return listref_type(_m_allocator,_m_list_head);}
			listref_type const as_list() const {return listref_type((allocator_type*)0,_m_list_head);}
		};
	};

	//template <typename dot_tn>
	//struct iterator_of_veclist
	//{
	//	typedef iterator_of_veclist self_type;
	//	typedef dot_tn dot_type;
	//	typedef typename dot_type::value_type value_type;
	//	vector_iterator_type _m_vector_iterator;
	//	list_iterator_type _m_list_iterator;
	//	dot_type dot() {return dot_type(_m_vector_iterator,_m_list_iterator->dot().node());}
	//	value_type& value() {return _m_list_iterator.value();}
	//	value_type const& value() const {return _m_list_iterator.value();}
	//	self_type& operator++() {++_m_list_iterator;return*this;}
	//	self_type& operator--() {--_m_list_iterator;return*this;}
	//};

	template <typename rooter_tn>
	struct veclist_center
	{
		typedef veclist_center self_type;
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::value_type value_type;
		typedef typename rooter_type::listref_type listref_type;
		typedef typename rooter_type::vectorref_type vectorref_type;
		typedef typename rooter_type::vdot_type vdot_type;
		typedef typename rooter_type::ldot_type ldot_type;

		rooter_type _m_rooter;

		self_type() {}
		self_type(self_type&& other): _m_rooter(other.rooter()) {}
		self_type(self_type const&& other): _m_rooter(other.rooter()) {}
		self_type(self_type const& other): _m_rooter(other.rooter()) {}
		self_type(rooter_type&& _rooter): _m_rooter(_rooter) {}
		self_type(rooter_type const&& _rooter): _m_rooter(_rooter) {}
		self_type(rooter_type const& _rooter): _m_rooter(_rooter) {}
		template <typename p1> self_type(p1&& v1): _m_rooter(forward<p1>(v1)) {}
		template <typename p1,typename p2> self_type(p1&& v1,p2&& v2): _m_rooter(forward<p1>(v1),forward<p2>(v2)) {}
		template <typename p1,typename p2,typename p3> self_type(p1&& v1,p2&& v2,p3&& v3): _m_rooter(forward<p1>(v1),forward<p2>(v2),forward<p3>(v3)) {}

		rooter_type& rooter() {return _m_rooter;}
		rooter_type const& rooter() const {return _m_rooter;}

		vectorref_type as_vector() {return rooter().as_vector();}
		listref_type as_list() {return rooter().as_list();}

		bool is_empty() const
		{
			if (rooter().is_list_enabled())
				assert(rooter().as_list().is_empty()==rooter().as_vector().is_empty());
			return rooter().as_vector().is_empty();
		}
		size_t size() const
		{
			return rooter().as_vector().size();
		}
		size_t capacity() const
		{
			return rooter().as_vector().capacity();
		}
		
		vdot_type insert(vdot_type before,value_type const& v)
		{
			return insert(ldot_type(*before),v);
		}
		vdot_type insert(ldot_type before,value_type const& v)
		{
			ldot_type dotnew = rooter().as_list().insert_before(before,v);
			if (dotnew.is_empty()) {assert(false && "new value failed");return 0;}
			return rooter().as_vector().insert(dotnew.node());
		}

		vdot_type insert(ldot_type before,size_t const& count,value_type const& v)
		{
			if (!count) return vdot_type();
			vdot_type r = insert(before,v);
			before.set_node(ldot_type((*r)).next());
			for (size_t i=1;i<count;++i)
			{
				ldot_type dotnew = rooter().as_list().insert_before(before,v);
				if (dotnew.is_empty()) {assert(false && "new value failed");return 0;}
				rooter().as_vector().insert(dotnew.node());
			}
			return r;
		}
		value_type* insert(size_t before,value_type const* buf,size_t const& count)
		{
			if (!count) return dot_type();
			dot_type r = insert(before,v);
			before = r.next();
			value_type const* src = buf;
			for (size_t i=1;i<count;++i)
			{
				ldot_type dotnew = rooter().as_list().insert(*src++);
				if (dotnew.is_empty()) {assert(false && "new value failed");return 0;}
				rooter().as_vector().insert(dotnew.node());
			}
			return r;
		}
		vdot_type remove(vdot_type i)
		{
			rooter().as_list().remove(i.value());
			return rooter().as_vector().remove(i);
		}
		vdot_type remove(ldot_type i)
		{
			rooter().as_list().remove(i);
			return rooter().as_vector().remove(i);
		}
		vdot_type remove(ldot_type from,size_t count)
		{
			vdot_type r = 0;
			ldot_type _end = rooter().as_list().ender();
			ldot_type d = from;
			for (size_t i=0;i<count;++i)
			{
				rooter().as_list().remove(d);
				r = rooter().as_vector().remove(d.node());
				d.set_node(d.next());
			}
			return r;
		}
		vdot_type push_front(value_type const& v)
		{
			return insert(rooter().as_list().fronter(),v);
		}
		vdot_type push_back(value_type const& v)
		{
			return insert(rooter().as_list().ender(),v);
		}
		void pop_front()
		{
			remove(rooter().as_list().fronter());
		}
		void pop_back()
		{
			remove(rooter().as_list().backer());
		}
	};


	//template <
	//	typename value_tn,
	//	typename keyof_tn=keyof_default<value_tn>,
	//	typename keycmp_tn=key_compare_default<keyof_default<value_tn*>>,
	//	typename allocator_tn=cppmalloc>
	//struct veclist_tt
	//{
	//	typedef value_tn value_type;
	//	typedef allocator_tn allocator_type;
	//	typedef typename veclist_trait<value_type,keyof_tn,keycmp_tn,allocator_type>::rooter_tt rooter_type;
	//	typedef veclist_center<rooter_type> type;
	//};
	template <
		typename value_tn,
		typename key_tn=value_tn,
		typename keyof_tn=keyof_default<key_tn>,
		typename keycmp_tn=key_compare_default<keyof_default<key_tn>>,
		typename allocator_tn=cppmalloc>
	struct veclistref_tt
	{
		typedef value_tn value_type;
		typedef allocator_tn allocator_type;
		typedef typename veclist_trait<value_type,key_tn,keyof_tn,keycmp_tn,allocator_type>::rooterref_tt rooter_type;
		typedef veclist_center<rooter_type> type;
	};
	//template <
	//	typename value_tn,
	//	typename keyof_tn=keyof_default<value_tn>,
	//	typename keycmp_tn=key_compare_default<keyof_default<value_tn>>,
	//	typename allocator_tn=cppmalloc>
	//struct indirect_doublelineref_tt
	//{
	//	typedef value_tn value_type;
	//	typedef allocator_tn allocator_type;
	//	typedef typename veclist_trait<value_type,keyof_tn,keycmp_tn,allocator_type>::rooter_doubleref_tt rooter_type;
	//	typedef veclist_center<rooter_type> type;
	//};
} /// end of abc