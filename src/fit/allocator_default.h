
#include <new.h>
#include "../ox/nsab.h"


#pragma once


___namespace2_begin(ox,fit)


template <typename data_tn>
struct cppmalloc_tt : data_tn
{
	static char* allocate(size_t size)
	{
		return new (std::nothrow) char [size];
	}
	static void deallocate(void* p)
	{
		delete [] p;
	}
};
template <>
struct cppmalloc_tt<void>
{
	static char* allocate(size_t size)
	{
		return new (std::nothrow) char [size];
	}
	static void deallocate(void* p)
	{
		delete [] p;
	}
};
typedef cppmalloc_tt<void> cppmalloc;

template <typename value_tn>
struct object_allocator
{
	typedef value_tn value_type;
	typedef value_tn* value_pointer;
	static value_pointer allocate(value_tn const& value)
	{
		return new value_tn(value);
	}
	static value_pointer allocate()
	{
		return new value_tn();
	}
	static void deallocate(value_pointer vp)
	{
		delete vp;
	}
};

struct nullmalloc
{
	//char* allocate(size_t size)
	//{
	//	return 0;
	//}
	//void deallocate(void* p)
	//{
	//}
};
template <typename mallocator,size_t size_added_tc>
struct cppmalloc_before_more : mallocator
{
	static char* allocate(size_t size)
	{
		return mallocator::allocate(size+size_added_tc)+size_added_tc;
	}
	static void deallocate(void* p)
	{
		mallocator::deallocate((char*)p-size_added_tc);
	}
};
template <typename mallocator,size_t size_added_tc>
struct cppmalloc_after_more : mallocator
{
	static char* allocate(size_t size)
	{
		return mallocator::allocate(size+size_added_tc);
	}
	static void deallocate(void* p)
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


___namespace2_end()
