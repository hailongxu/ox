
#include <assert.h>
#include "../ox/nsab.h"



___namespace2_begin(ox,fit)


template<class ty>
class stl_allocator
{
	template<class t>
	struct remove_const
	{
		typedef t type;
	};

	template<class t>
	struct remove_const<const t>
	{
		typedef t type;
	};

public:
	typedef typename remove_const<ty>::type value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef int difference_type;

	template<class other_tn>
	struct rebind
	{
		typedef stl_allocator<other_tn> other;
	};

	pointer address(reference val) const
	{
		return (&val);
	}

	const_pointer address(const_reference val) const
	{
		return (&val);
	}

	stl_allocator() throw()
	{
	}

	stl_allocator(const stl_allocator<ty>&) throw()
	{
	}

	template<class other>
	stl_allocator(const stl_allocator<other>&) throw()
	{
	}

	template<class other>
	stl_allocator<ty>& operator=(const stl_allocator<other>&)
	{
		return (*this);
	}

	void deallocate(pointer ptr, size_type)
	{
		::operator delete(ptr);
	}

	pointer allocate(size_type count)
	{
		if (count <= 0) count = 0;
		else if (((size_type)(-1) / count) < sizeof (value_type))
			throw std::bad_alloc(0);
		return (pointer)(::operator new(count * sizeof (ty)));
	}

	pointer allocate(size_type count, const void*)
	{
		return (allocate(count));
	}

	void construct(pointer ptr, const ty& val)
	{
		::new ((void*)ptr) ty(val);
	}

	void destroy(pointer ptr)
	{
		ptr->~ty();
	}

	size_type max_size() const throw()
	{
		size_type count = (size_type)(-1) / sizeof (ty);
		return (0 < count ? count : 1);
	}
};


template<class ty>
class stl_allocator_verify
{
	template<class t>
	struct remove_const
	{
		typedef t type;
	};

	template<class t>
	struct remove_const<const t>
	{
		typedef t type;
	};

public:
	typedef typename remove_const<ty>::type value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef int difference_type;

	template<class other_tn>
	struct rebind
	{
		typedef stl_allocator_verify<other_tn> other;
	};

	pointer address(reference val) const
	{
		return (&val);
	}

	const_pointer address(const_reference val) const
	{
		return (&val);
	}

	stl_allocator_verify() throw()
	{
	}

	stl_allocator_verify(const stl_allocator_verify<ty>&) throw()
	{
	}

	template<class other>
	stl_allocator_verify(const stl_allocator_verify<other>&) throw()
	{
	}

	template<class other>
	stl_allocator_verify<ty>& operator=(const stl_allocator_verify<other>&)
	{
		return (*this);
	}

	void deallocate(pointer ptr, size_type)
	{
		printf("-[%p]\n",ptr);
		char* p = (char*)ptr;
		p -= 8;
		if(*(size_t*)p != 'nnnn')
		{
			DebugBreak();
		}
		*(size_t*)p = 'dddd';
		size_t count = *((size_t*)p+1);
		if (*(size_t*)((char*)ptr+count) != 'mmmm')
		{
			DebugBreak();
		}
		*(size_t*)(((char*)ptr)+count) = 'eeee';
		::operator delete(p);
	}

	pointer allocate(size_type count)
	{
		if (count <= 0) count = 0;
		else if (((size_type)(-1) / count) < sizeof (value_type))
			throw std::bad_alloc(0);
		size_t size_needed = count * sizeof (ty);
		char* p = (char*)(::operator new(8+size_needed+4));
		*(size_t*)p = 'nnnn';
		*((size_t*)p+1) = size_needed;
		*(size_t*)(p+8+size_needed) = 'mmmm';
		printf("+[%p]\n",p+8);
		return (pointer)(p+8);
	}

	pointer allocate(size_type count, const void*)
	{
		return (allocate(count));
	}

	void construct(pointer ptr, const ty& val)
	{
		::new ((void*)ptr) ty(val);
	}

	void destroy(pointer ptr)
	{
		ptr->~ty();
	}

	size_type max_size() const throw()
	{
		size_type count = (size_type)(-1) / sizeof (ty);
		return (0 < count ? count : 1);
	}
};


___namespace2_end()