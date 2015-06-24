
#include <assert.h>
#include "../ox/nsab.h"
#include "../met/infof_array.h"
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
	template <typename event,typename object_tn>
	static void destruct_objects(event ev,object_tn* begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
		{
			ev(begin,i);
			(begin++)->~object_tn();
		}
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
	template <typename event,typename object_tn>
	static void construct_objects(event& ev,object_tn* begin,size_t size)
	{
		for (size_t i=0;i<size;++i)
		{
			new (begin) object_tn();
			ev(*begin,i);
			++begin;
		}
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

struct vector_on_value_inserted
{
	template <typename rooter_tn>
	void operator()(rooter_tn& rooter,size_t off,typename rooter_tn::value_type& value)
	{}
};
struct vector_on_value_deleting
{
	template <typename rooter_tn>
	void operator()(rooter_tn& rooter,size_t off,typename rooter_tn::value_type& value)
	{}
};

struct vector_event
{
	typedef vector_on_size_changed on_size_changed;
	//typedef vector_on_value_inserted on_value_inserted;
	//typedef vector_on_value_deleting on_value_deleting;
};

struct string_event
{
	typedef string_on_size_changed on_size_changed;
	//typedef vector_on_value_inserted on_value_inserted;
	//typedef vector_on_value_deleting on_value_deleting;
};

template<typename value_tn,typename events_tn=vector_event,typename allocator_tn=cppmalloc>
struct vector_rooter : allocator_tn
{
	friend vector_tt<vector_rooter>;
	friend vector_on_size_changed;
	friend string_on_size_changed;
	typedef value_tn value_type;
	typedef typename events_tn::on_size_changed on_size_changed;
	//typedef typename events_tn::on_value_inserted on_value_inserted;
	//typedef typename events_tn::on_value_deleting on_value_deleting;
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
	void swap(self& other)
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
	template <typename deleting_event_tn>
	struct object_deleting_event_tt
	{
		object_deleting_event_tt(self& root,size_t const& off_begin)
			: _m_root(root)
			, _m_off_begin(off_begin)
		{}
		self& _m_root; size_t const& _m_off_begin;
		void operator()(value_type* value,size_t off)
		{
			deleting_event_tn()(_m_root,_m_off_begin+off,1,value);
		}
	};
	template <typename inserted_event>
	struct object_inserted_event_tt
	{
		object_inserted_event_tt(self& root,size_t const& off_begin)
			: _m_root(root)
			, _m_off_begin(off_begin)
		{}
		self& _m_root; size_t const& _m_off_begin;
		void operator()(value_type& value,size_t off)
		{
			inserted_event()(_m_root,_m_off_begin+off,&value);
		}
	};
	template <>
	struct object_deleting_event_tt <void>
	{
		object_deleting_event_tt(self& root,size_t const& off_begin)
		{}
		void operator()(value_type* value,size_t off)
		{}
	};
	template <>
	struct object_inserted_event_tt <void>
	{
		object_inserted_event_tt(self& root,size_t const& off_begin)
		{}
		void operator()(value_type& value,size_t off)
		{}
	};

	void resize(size_t size)
	{
		resize<void,void>(size);
	}
	template <typename object_deleting_event,typename object_inserted_event>
	void resize(size_t size)
	{
		if (size<=_m_head._m_size)
		{
			object_deleting_event_tt<object_deleting_event> deleting_event(*this,size);
			vector_helper::destruct_objects(deleting_event,_m_head._m_data_begin+size,_m_head._m_size-size);
			_m_head._m_size = size;
		}
		else if (size<_m_head._m_capacity)
		{
			object_inserted_event_tt<object_inserted_event> inserted_event(*this,_m_head._m_size);
			vector_helper::construct_objects(inserted_event,_m_head._m_data_begin+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		else
		{
			reserve(size-_m_head._m_size);
			object_inserted_event_tt<object_inserted_event> inserted_event(*this,_m_head._m_size);
			vector_helper::construct_objects(inserted_event,_m_head._m_data_begin+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		on_size_changed()(*this,0);
	}
};

template<typename value_tn,typename string_events_tn=string_event,typename allocator_tn=cppmalloc>
struct string_rooter : vector_rooter<value_tn,string_events_tn,allocator_tn>
{
	friend vector_tt<string_rooter>;
	string_rooter()
	{
		_m_head._m_data_begin = allocator::allocate(0);
		string_events_tn::on_size_changed()(*this,0);
	}
};


template<typename array,size_t added,typename vector_events_tn=vector_event>
struct static_vector_rooter;
template<typename value_tn,size_t n,size_t added,typename vector_events_tn>
struct static_vector_rooter<value_tn[n],added,vector_events_tn>
{
	friend vector_tt<static_vector_rooter>;
	friend vector_on_size_changed;
	friend string_on_size_changed;
	typedef value_tn value_type;
	typedef value_tn array_type[n+added];
protected:
	typedef static_vector_rooter self;
	typedef typename vector_events_tn::on_size_changed on_size_changed;
	//typedef typename vector_events_tn::on_value_inserted on_value_inserted;
	//typedef typename vector_events_tn::on_value_deleting on_value_deleting;
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

template<typename array,typename string_events_tn=string_event>
struct static_string_rooter;
template<typename value_tn,size_t n,typename string_events_tn>
struct static_string_rooter<value_tn[n],string_events_tn>
	: static_vector_rooter<value_tn[n],1,string_events_tn>
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
		template <typename vec,typename node>
		void operator()(vec&,size_t off,node&)
		{}
	};
	struct on_deleting_nothing
	{
		template <typename vec,typename value_tn>
		void operator()(vec&,size_t off,size_t size,value_tn*)
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
			rooter::swap(_old_rooter);
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
		on_inserted(*this,off,*value_begin);
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
		on_pushed(*this,rooter::size(),*added_begin);
		return added_begin;
	}
	template <typename on_deleting_tn>
	void erase(size_t off,size_t size,on_deleting_tn& on_deleteing)
	{
		assert(off<rooter::size() && off+size<rooter::size());
		value_type* value_begin = rooter::data_begin();
		value_type* out = value_begin+off;
		value_type* src = out+size;
		on_deleteing(*this,off,size,out);
		vector_helper::move_objects<value_type>(out,-1,src,rooter::size()-off-size);
		rooter::on_size_changed()(root(),-size);
	}
	void erase(size_t i)
	{
		erase(i,1,on_deleting_nothing());
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
struct vector : vector_tt<vector_rooter<value_tn,vector_event,allocator_tn>>
{};

template <typename array_tn>
struct static_vector: vector_tt<static_vector_rooter<array_tn,0,vector_event>>
{};

template <typename value_tn,typename allocator_tn=cppmalloc_more<cppmalloc,sizeof(value_tn)>>
struct string : vector_tt<string_rooter<value_tn,string_event,allocator_tn>>
{};

template <typename array_tn>
struct static_string: vector_tt<static_string_rooter<array_tn,string_event>>
{};

template <typename value_tn,typename node_tn=value_tn*,typename allocator_tn=cppmalloc>
struct indirect_vector_rooter
{
	typedef value_tn value_type;
	typedef allocator_tn allocator_type;
	typedef node_tn node_type;
	typedef vector_tt<vector_rooter<node_type,vector_event,allocator_tn>> vector_type;
	//typedef node_type index_value_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
	char* allocate(size_t size)
	{
		return index().allocate(size);
	}
};
template <typename value_tn,typename node_array,typename allocator_tn>
struct static_indirect_vector_rooter : allocator_tn
{
	typedef value_tn value_type;
	typedef static_vector<node_array> vector_type;
	typedef typename vector_type::value_type node_type;
	//typedef node_type index_value_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
};

//template <typename value_tn,typename allocator_tn=cppmalloc>
template <typename indirect_vector_rooter>
struct indirect_vector_tt: indirect_vector_rooter
{
	typedef indirect_vector_rooter rooter_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename rooter_type::value_type value_type;
	struct deleting_event
	{
		template <typename rooter_tn>
		void operator()(rooter_tn& root,size_t off,size_t size,node_type* pnode)
		{
			(*pnode)->~value_type();
		}
	};
	struct inserted_event
	{
		template <typename rooter_tn>
		void operator()(rooter_tn& root,size_t off,node_type* value)
		{
			(*value) = (value_type*)root.allocate(sizeof(value_type));
			new (*value) value_type();
		}
	};

	void resize(size_t size)
	{
		index().resize<deleting_event,inserted_event>(size);
	}
	void reserve(size_t size)
	{
		index().reserve(size);
	}
	size_t size() const
	{
		return index().size();
	}
	size_t capacity() const
	{
		return index().capacity();
	}
	value_type** insert(size_t off,value_type const& value)
	{
		value_type* pv = index().allocate(sizeof(value_type));
		new (pv) value_type(value);
		return index().insert(off,pv);
	}
	value_type** push_back(value_type const& value)
	{
		char* p = allocate(sizeof(value_type));
		value_type* pv = new (p) value_type(value);
		return index().push_back(pv);
	}
	void erase(size_t off,size_t size=1)
	{
		index().erase(off,size,deleting_event());
	}
	template <typename act_tn>
	struct indirect_act
	{
		act_tn& _m_act;
		indirect_act(act_tn& act): _m_act(act) {}
		bool operator()(node_type& node)
		{
			return _m_act(*node);
		}
	};
	/// act(node)
	template <typename action>
	void foreach(action& act)
	{
		index().foreach(indirect_act<action>(act));
	}
};

template <typename value_tn,typename allocator_tn=cppmalloc>
struct indirect_vector: indirect_vector_tt<indirect_vector_rooter<value_tn,value_tn*,allocator_tn>>
{};
template <typename value_tn,typename node_array,typename allocator_tn=cppmalloc>
struct static_indirect_vector: indirect_vector_tt<static_indirect_vector_rooter<value_tn,node_array,allocator_tn>>
{};



___namespace2_end()