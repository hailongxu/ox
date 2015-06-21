
#include <assert.h>
#include "../ox/nsab.h"
#include "allocator_default.h"



___namespace2_begin(ox,fit)




struct vector_helper
{
	template <typename object_tn>
	static void destruct_objects(object_tn* begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
			(begin++)->~object_tn();
	}
	template <typename object_tn>
	static void destruct_objects(object_tn** begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
			(*(begin++))->~object_tn();
	}
	template <typename values_tn>
	static void destruct_values(values_tn* begin,size_t size)
	{
	}
	template <typename object_tn>
	static void construct_objects(object_tn* begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
			new (begin++) object_tn();
	}
	template <typename object_tn>
	static void construct_objects(object_tn** begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
			new (*(begin++)) object_tn();
	}

	template <typename object_tn>
	static size_t copy_objects(object_tn* out,size_t out_size,object_tn* begin,size_t size)
	{
		if (out_size<size) return 0;
		for (size_t i=0;i<size;++i)
			*out++ = *begin++;
		return size;
	}
	template <typename object_tn>
	static size_t move_objects(object_tn* out,size_t out_size,object_tn* begin,size_t size)
	{
		if (out_size<size) return 0;
		for (size_t i=0;i<size;++i)
		{
			*out++ = *begin;
			begin->~object_tn();
			++begin;
		}
		return size;
	}
	template <typename object_tn>
	static size_t move_objects_backward(object_tn* out,size_t out_size,object_tn* begin,size_t size)
	{
		if (out_size<size || size==0) return 0;
		out += size-1;
		begin += size-1;
		for (size_t i=size;i>0;--i)
		{
			*out-- = *begin;
			begin->~object_tn();
			--begin;
		}
		return size;
	}
};

template<typename vector_rooter_tn>
struct vector_tt;

/// vector=>{head,allcotor}
/// head=>{size,capacity,data_begin}
/// data_begin=>{value,value,,,,}

struct string_on_size_changed
{
	template <typename rooter_tn>
	void operator()(rooter_tn& rooter,int size)
	{
		assert (rooter.size()+size<=rooter.capacity());
		rooter._m_head._m_size += size;
		*(rooter.data_begin()+rooter.size())=0;
	}
};

struct vector_on_size_changed
{
	template <typename rooter_tn>
	void operator()(rooter_tn& rooter,int size)
	{
		assert (rooter.size()+size<=rooter.capacity());
		rooter._m_head._m_size += size;
	}
};

template<typename value_tn,typename size_changed_tn=vector_on_size_changed,typename allocator_tn=cppmalloc>
struct vector_rooter : allocator_tn
{
	friend vector_tt<vector_rooter>;
	friend vector_on_size_changed;
	friend string_on_size_changed;
	typedef value_tn value_type;
	typedef size_changed_tn on_size_changed;
protected:
	typedef vector_rooter self;
	typedef allocator_tn allocator;
	struct head_t
	{
		head_t() : _m_size(0), _m_capacity(0), _m_data_begin(0) {}
		size_t _m_size;
		size_t _m_capacity;
		value_type* _m_data_begin;
	};
	~vector_rooter()
	{
		resize(0);
		allocator::deallocate(_m_head._m_data_begin);
		_m_head._m_data_begin = 0;
	}
	value_type* data_begin()
	{
		return _m_head._m_data_begin;
	}
	void exchange(self& other)
	{
		head_t t = _m_head;
		_m_head = other._m_head;
		other._m_head = t;
	}
	head_t _m_head;
public:
	size_t size() const
	{
		return _m_head._m_size;
	}
	size_t capacity() const
	{
		return _m_head._m_capacity;
	}
	void reserve(size_t size,size_t size_copyed=-1)
	{
		if (size_copyed==-1) size_copyed = _m_head._m_size;
		size_t usable_size = _m_head._m_capacity-_m_head._m_size;
		if (usable_size>=size) return;
		value_type* data_begin = _m_head._m_data_begin;
		data_begin = (value_type*)allocator::allocate((_m_head._m_size+size)*sizeof(value_type));
		assert (data_begin);
		vector_helper::move_objects(data_begin,-1,_m_head._m_data_begin,size_copyed);
		_m_head._m_capacity = size;
		_m_head._m_data_begin = data_begin;
	}
	void resize(size_t size)
	{
		if (size<=_m_head._m_size)
		{
			vector_helper::destruct_objects(_m_head._m_data_begin+size,_m_head._m_size-size);
			_m_head._m_size = size;
		}
		else if (size<_m_head._m_capacity)
		{
			vector_helper::construct_objects(_m_head._m_data_begin+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		else
		{
			reserve(size-_m_head._m_size);
			vector_helper::construct_objects(_m_head._m_data_begin+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		on_size_changed()(*this,0);
	}
};

template<typename value_tn,typename size_changed_tn=vector_on_size_changed,typename allocator_tn=cppmalloc>
struct string_rooter : vector_rooter<value_tn,size_changed_tn,allocator_tn>
{
	friend vector_tt<string_rooter>;
	string_rooter()
	{
		_m_head._m_data_begin = allocator::allocate(0);
		size_changed_tn()(*this,0);
	}
};


template<typename array,size_t added,typename size_changed_tn=vector_on_size_changed>
struct static_vector_rooter;
template<typename value_tn,size_t n,size_t added,typename size_changed_tn>
struct static_vector_rooter<value_tn[n],added,size_changed_tn>
{
	friend vector_tt<static_vector_rooter>;
	friend vector_on_size_changed;
	friend string_on_size_changed;
	typedef value_tn value_type;
	typedef value_tn array_type[n+added];
protected:
	typedef static_vector_rooter self;
	typedef size_changed_tn on_size_changed;
	struct head_t
	{
		head_t(): _m_size(0) {}
		size_t _m_size;
		array_type _m_data;
	};
	~static_vector_rooter()
	{
		_m_head._m_size = 0;
	}
	value_type* data_begin()
	{
		return _m_head._m_data;
	}
	void exchange(self& other)
	{
		assert (false && "not support");
	}
	head_t _m_head;
public:
	size_t size() const
	{
		return _m_head._m_size;
	}
	size_t capacity() const
	{
		return n;
	}
	void reserve(size_t size,size_t size_copyed=-1)
	{
	}
	void resize(size_t size)
	{
		if (size<=_m_head._m_size)
		{
			vector_helper::destruct_objects(_m_head._m_data+size,_m_head._m_size-size);
			_m_head._m_size = size;
		}
		else if (size<capacity())
		{
			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		else
		{
			reserve(size-_m_head._m_size);
			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		on_size_changed()(*this,0);
	}
};

template<typename array,typename size_changed_tn=string_on_size_changed>
struct static_string_rooter;
template<typename value_tn,size_t n,typename size_changed_tn>
struct static_string_rooter<value_tn[n],size_changed_tn>
	: static_vector_rooter<value_tn[n],1,size_changed_tn>
{
	friend vector_tt<static_string_rooter>;
	typedef static_string_rooter self;
	self()
	{
		*data_begin()=0;
	}
};

template<typename vector_rooter_tn>
struct vector_tt : vector_rooter_tn
{
	typedef vector_rooter_tn rooter;
	typedef typename rooter::value_type value_type;
	struct on_inserted_nothing
	{
		template <typename vec,typename node,typename value>
		void operator()(vec&,size_t off,node&,value&)
		{}
	};

	bool is_empty() const
	{
		return rooter::size()==0;
	}
	value_type* insert(size_t off,value_type const& value)
	{
		return insert(off,value,on_inserted_nothing());
	}
	template <typename on_inserted_tn>
	value_type* insert(size_t off,value_type const& value,on_inserted_tn& on_inserted)
	{
		value_type* value_begin = 0;
		if (rooter::size()>=rooter::capacity())
		{
			rooter _old_rooter;
			rooter::exchange(_old_rooter);
			rooter::reserve(_old_rooter.size()==0?2:_old_rooter.size()*2);
			vector_helper::move_objects(rooter::data_begin(),-1,_old_rooter.data_begin(),off);
			vector_helper::move_objects(rooter::data_begin()+off+1,-1,_old_rooter.data_begin()+off,_old_rooter.size()-off);
			value_begin = rooter::data_begin()+off;
			*value_begin = value;
			rooter::on_size_changed()(*this,1+_old_rooter.size());
		}
		else
		{
			vector_helper::move_objects_backward(rooter::data_begin()+off+1,-1,rooter::data_begin()+off,rooter::size()-off);
			value_begin = rooter::data_begin()+off;
			*value_begin = value;
			rooter::on_size_changed()(*this,1);
		}
		on_inserted(*this,off,value_begin,value);
		return value_begin;
	}
	value_type* push_front(value_type const& value)
	{
		return insert(0,value,on_inserted_nothing());
	}
	value_type* push_back(value_type const& value)
	{
		return push_back(value,on_inserted_nothing());
	}
	template <typename on_pushed_tn>
	value_type* push_back(value_type const& value,on_pushed_tn& on_pushed)
	{
		if (rooter::size()>=rooter::capacity())
			rooter::reserve(is_empty()?2:rooter::size()*2);
		assert(rooter::size()<rooter::capacity());
		value_type* begin = rooter::data_begin();
		value_type* added_begin = begin+rooter::size();
		*added_begin = value;
		rooter::on_size_changed()(root(),1);
		on_pushed(*this,rooter::size(),added_begin,value);
		return added_begin;
	}
	void erase(size_t off,size_t size)
	{
		assert(off<rooter::size() && off+size<rooter::size());
		value_type* value_begin = rooter::data_begin();
		value_type* out = value_begin+off;
		value_type* src = out+size;
		vector_helper::move_objects<value_type>(out,-1,src,rooter::size()-off-size);
		rooter::on_size_changed(-size);
	}
	void erase(size_t i)
	{
		erase(off,1);
	}
	void erase_to_end(size_t i)
	{
		erase(i,rooter::size()-i);
	}
	value_type& at(size_t i)
	{
		assert (index<=rooter::size());
		return *(rooter::data_begin()+i);
	}
	value_type const& at(size_t i) const
	{
		assert (index<=rooter::size());
		return *(rooter::data_begin()+i);
	}

	template <typename action>
	void foreach(action& act)
	{
		value_type* value_begin = rooter::data_begin();
		for (size_t i=0;i<rooter::size();++i)
		{
			if (!act(*value_begin++)) break;
		}
	}
	void clear()
	{
		rooter::resize(0);
	}
protected:
	rooter& root() {return *this;}
};

template <typename value_tn,typename allocator_tn=cppmalloc>
struct vector : vector_tt<vector_rooter<value_tn,vector_on_size_changed,allocator_tn>>
{};

template <typename array_tn>
struct static_vector: vector_tt<static_vector_rooter<array_tn,0,vector_on_size_changed>>
{};

template <typename value_tn,typename allocator_tn=cppmalloc_more<cppmalloc,sizeof(value_tn)>>
struct string : vector_tt<string_rooter<value_tn,string_on_size_changed,allocator_tn>>
{};

template <typename array_tn>
struct static_string: vector_tt<static_string_rooter<array_tn,string_on_size_changed>>
{};


template <typename value_tn,typename allocator_tn=cppmalloc>
struct indirect_vector
{
	typedef value_tn value_type;
	typedef vector_tt<vector_rooter<value_tn*,vector_on_size_changed,allocator_tn>> vector_type;
	vector_type _m_vector;
	size_t size() const
	{
		return _m_vector.size();
	}
	size_t capacity() const
	{
		return _m_vector.capacity();
	}
	bool is_empty() const
	{
		return _m_vector.is_empty();
	}
	void reserve(size_t size)
	{
		_m_vector.reserve(size);
	}
	void resize(size_t size)
	{
		size_t old_size = _m_vector.size();
		if (size<=old_size)
			vector_helper::destruct_objects(&_m_vector.at(size),old_size-size);
		_m_vector.resize(size);
		if (size<=old_size) return;
		value_type** begin = _m_vector.data_begin();
		for (size_t i=old_size;i<size;++i)
		{
			*begin = _m_vector.allocator::allocate(sizeof(value_type));
			new (*begin) value_type();
		}
	}
	value_type* insert(size_t off,value_type const& value)
	{
		value_type* pv = _m_vector.allocator::allocate(sizeof(value_type));
		assert(pv);
		_m_vector.insert(off,pv);
		return pv;
	}
	value_type* push_back(value_type const& value)
	{
		value_type* pv = _m_vector.allocator::allocate(sizeof(value_type));
		assert(pv);
		_m_vector.push_back(pv);
		return pv;
	}
	value_type* push_front(value_type const& value)
	{
		return insert(0,value);
	}
	template <typename action>
	struct indirect_act
	{
		action& _m_action;
		indirect_act(action& act): _m_action(act) {}
		bool operator()(value_type*& pvalue)
		{
			return _m_action(*pvalue);
		}
	};
	template <typename action>
	void foreach(action& act)
	{
		_m_vector.foreach(indirect_act(act));
	}
};


___namespace2_end()