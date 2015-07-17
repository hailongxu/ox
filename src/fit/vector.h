
#include <assert.h>
#include "../ox/nsab.h"
#include "allocator_default.h"
#include "../utl/data_t.h"
#include "vector_head.h"




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
	static size_t copy_objects(object_tn** out,size_t out_size,object_tn** begin,size_t size)
	{
		if (out_size<size) return 0;
		for (size_t i=0;i<size;++i)
			**out = **begin, ++out,++begin;
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
		rooter.head().set_size(rooter.head().size()+size);
		*(rooter.data_begin()+rooter.size())=0;
	}
};

struct vector_on_size_changed
{
	template <typename rooter_tn>
	void operator()(rooter_tn& rooter,int size)
	{
		assert (rooter.size()+size<=rooter.capacity());
		rooter.head().set_size(rooter.head().size()+size);
	}
};

struct vector_on_value_inserted
{
	template <typename value_tn>
	void operator()(size_t off,value_tn& value)
	{}
};
struct vector_on_value_deleting
{
	template <typename value_tn>
	void operator()(size_t off,value_tn& value)
	{}
};
struct vector_event
{
	typedef vector_on_size_changed on_size_changed;
};

struct string_event
{
	typedef string_on_size_changed on_size_changed;
};

struct size_got_default
{
	size_t operator()(size_t bytes) {return bytes;}
};

template<typename value_tn,typename head_tn,typename events_tn=vector_event,typename allocator_tn=cppmalloc>
struct vector_rooter : head_tn
{
	friend vector_tt<vector_rooter>;
	friend vector_on_size_changed;
	friend string_on_size_changed;
	friend allocator_tn;
	typedef value_tn value_type;
	typedef typename events_tn::on_size_changed on_size_changed;
	typedef head_tn head_t;

protected:
	typedef vector_rooter self;
	typedef allocator_tn allocator_type;
	head_t& head() {return *this;}
	head_t const& head() const {return *this;}

	~vector_rooter()
	{
		resize(0);
		allocator().deallocate(data_begin());
		head().set_data_begin(0);
	}
	value_type* data_begin()
	{
		return head().data_begin();
	}
public:
	allocator_type allocator() {return allocator_type();}
	void swap(self& other)
	{
		head_t t = head();
		head() = other.head();
		other.head() = t;
	}
	size_t size() const
	{
		return head().size();
	}
	size_t capacity() const
	{
		return head().capacity();
	}
	void reserve(size_t size_reserved,size_t size_copyed=-1)
	{
		reserve(size_reserved,size_copyed,size_got_default());
	}
	template <typename size_got_tn>
	bool reserve(size_t size_reserved,size_t size_copyed/*=-1*/,size_got_tn& size_getter)
	{
		size_t size_old = size();
		size_t capacity_old = capacity();
		if (size_copyed>size_old) size_copyed = size_old;
		size_t usable_size = capacity_old-size_old;
		if (usable_size>=size_reserved) return true;
		size_t size_needed = (size_old+size_reserved)*sizeof(value_type);
		size_t size_gotted = size_getter(size_needed);
		assert (size_gotted>=size_needed);
		value_type* data_begin_new = (value_type*)allocator().allocate(size_gotted);
		assert (data_begin_new);
		if (!data_begin_new) return false;
		vector_helper::move_objects(data_begin_new,-1,data_begin(),size_copyed);
		head().set_data_begin(data_begin_new);
		head().set_capacity(size_reserved);
		head().set_size(size_copyed);
		return true;
	}
	template <typename deleting_event_tn>
	struct object_deleting_event_tt
	{
		object_deleting_event_tt(deleting_event_tn& event,size_t const& off_begin)
			: _m_off_begin(off_begin)
			, _m_event(event)
		{}
		deleting_event_tn& _m_event;
		size_t const& _m_off_begin;
		void operator()(value_type* value,size_t off)
		{
			_m_event(_m_off_begin+off,1,value);
		}
	};
	template <typename inserted_event>
	struct object_inserted_event_tt
	{
		object_inserted_event_tt(inserted_event& event,size_t const& off_begin)
			: _m_off_begin(off_begin)
			, _m_event(event)
		{}
		inserted_event& _m_event;
		size_t const& _m_off_begin;
		void operator()(value_type& value,size_t off)
		{
			_m_event(_m_off_begin+off,&value);
		}
	};
	template <>
	struct object_deleting_event_tt <int const>
	{
		template <typename t>
		object_deleting_event_tt(t& e,size_t const& off_begin)
		{}
		void operator()(value_type* value,size_t off)
		{}
	};
	template <>
	struct object_inserted_event_tt <int const>
	{
		template <typename t>
		object_inserted_event_tt(t& e,size_t const& off_begin)
		{}
		void operator()(value_type& value,size_t off)
		{}
	};

	void resize(size_t size)
	{
		resize<int const,int const>(size);
	}
	template <typename object_deleting_event,typename object_inserted_event>
	void resize(size_t size_new)
	{
		resize<object_deleting_event,object_inserted_event>(size_new,
			object_deleting_event(),object_inserted_event(),size_got_default());
	}
	template <typename object_deleting_event,typename object_inserted_event,typename size_reget_tn>
	void resize(size_t size_new,object_deleting_event& del_event,object_inserted_event& ins_event,size_reget_tn& size_getter)
	{
		if (size_new<=size())
		{
			object_deleting_event_tt<object_deleting_event> deleting_event(del_event,size_new);
			vector_helper::destruct_objects(deleting_event,data_begin()+size_new,size()-size_new);
			head().set_size(size_new);
		}
		else if (size_new<capacity())
		{
			object_inserted_event_tt<object_inserted_event> inserted_event(ins_event,size());
			vector_helper::construct_objects(inserted_event,data_begin()+size(),size_new-size());
			head().set_size(size_new);
		}
		else
		{
			reserve(size_new-size(),-1,size_getter);
			object_inserted_event_tt<object_inserted_event> inserted_event(ins_event,size());
			vector_helper::construct_objects(inserted_event,data_begin()+size(),size_new-size());
			head().set_size(size_new);
		}
		on_size_changed()(*this,0);
	}
};

template<typename value_tn,typename head_tn,typename string_events_tn/*=string_event*/,typename allocator_tn>
struct string_rooter : vector_rooter<value_tn,head_tn,string_events_tn,allocator_tn>
{
	friend vector_tt<string_rooter>;
	string_rooter()
	{
		head().set_data_begin(allocator_type::allocate(0));
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
	struct head_t
	{
		head_t(): _m_size(0) {}
		void set_size(size_t size) {_m_size=size;}
		size_t size() const {return _m_size;}
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
	head_t head() {return _m_head;}
	head_t _m_head;
public:
	void swap(self& other)
	{
		assert (false && "not support");
	}
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
		template <typename node>
		void operator()(size_t off,node&)
		{}
	};
	struct on_deleting_nothing
	{
		template <typename value_tn>
		void operator()(size_t off,size_t size,value_tn*)
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
		on_inserted(off,*value_begin);
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
		on_pushed(rooter::size(),*added_begin);
		return added_begin;
	}
	template <typename on_deleting_tn>
	void erase(size_t off,size_t size,on_deleting_tn& on_deleteing)
	{
		assert(off<rooter::size() && off+size<rooter::size());
		value_type* value_begin = rooter::data_begin();
		value_type* out = value_begin+off;
		value_type* src = out+size;
		on_deleteing(off,size,out);
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
		assert (i<=rooter::size());
		return *(rooter::data_begin()+i);
	}
	value_type const& at(size_t i) const
	{
		assert (i<=rooter::size());
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
struct vector : vector_tt<vector_rooter<value_tn,vector_head_iso_tt<value_tn>,vector_event,allocator_tn>>
{};

template <typename array_tn>
struct static_vector: vector_tt<static_vector_rooter<array_tn,0,vector_event>>
{};

template <typename value_tn,typename allocator_tn=cppmalloc_after_more<cppmalloc,sizeof(value_tn)>>
struct string : vector_tt<string_rooter<value_tn,vector_head_iso_tt<value_tn>,string_event,allocator_tn>>
{};

template <typename array_tn>
struct static_string: vector_tt<static_string_rooter<array_tn,string_event>>
{};

template <typename value_tn>
struct indirect_vector_deleting_event
{
	/// the rooter is vector rooter
	/// value type of vector is node type of indirect vector
	template <typename node_tn>
	void operator()(size_t off,size_t size,node_tn* pnode)
	{
		(*pnode)->~value_tn();
	}
};
template <typename value_tn,typename value_allocator>
struct indirect_vector_inserted_event
{
	/// the rooter is vector rooter
	/// value type of vector is node type of indirect vector
	template <typename node_tn>
	void operator()(size_t off,node_tn* value)
	{
		typedef value_tn value_type;
		(*value) = value_allocator::allocate();
	}
};

template <typename type_tn>
struct value_trait
{
	typedef type_tn value_type;
	typedef value_type* value_pointer;
	typedef value_pointer node_type;
	struct value_size{size_t operator()(value_type const& value) {return sizeof(value);}};
};
template <typename type_tn>
struct space_value_trait
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef value_type value_pointer;
	typedef value_pointer node_type;
	struct value_size{size_t operator()(value_type const& value) {return value.size;}};
};

template <typename value_tn>
struct space_indirect_allocator
{
	typedef ox::utl::data_tt<value_tn> value_type;
	typedef value_type value_pointer;
	static value_pointer allocate(value_type const& value)
	{
		value_type v = value;
		v.begin = new char[value.size];
		return v;
	}
	static void deallcoate(value_pointer const& vp)
	{
		delete vp.begin;
	}
};

template <typename value_tn>
struct allocator_trait
{
	typedef cppmalloc index_allocator;
	typedef object_allocator<value_tn> value_allocator;
};
template <typename value_tn>
struct space_allocator_trait
{
	typedef cppmalloc index_allocator;
	typedef space_indirect_allocator<value_tn> value_allocator;
};


template <typename value_trait,typename allocator_trait>
struct indirect_vector_rooter
{
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::node_type node_type;
	typedef typename allocator_trait::index_allocator index_allocator_type;
	typedef typename allocator_trait::value_allocator value_allocator_type;

	typedef cppmalloc_before_more<index_allocator_type,vector_head_mono_tt<node_type>::__head_size> mono_index_allocator_type;
	typedef vector_tt<vector_rooter<node_type,vector_head_mono_tt<node_type>,vector_event,mono_index_allocator_type>> vector_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
	value_pointer allocate_object(value_type const& value)
	{
		return value_allocator_type::allocate(value);
	}
	void resize(size_t size)
	{
		typedef indirect_vector_deleting_event<value_type> deleting_event;
		typedef indirect_vector_inserted_event<value_type,value_allocator_type> inserted_event;
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
};
template <typename value_tn,typename node_array,typename allocator_tn>
struct static_indirect_vector_rooter
{
	typedef value_tn value_type;
	typedef value_type* value_pointer;
	typedef static_vector<node_array> vector_type;
	typedef typename vector_type::value_type node_type;
	typedef allocator_tn allocate_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
	value_pointer allocate_object(value_type const& value)
	{
		return allocate_type().allocate(value);
	}
};

template <typename indirect_vector_rooter>
struct indirect_vector_tt: indirect_vector_rooter
{
	typedef indirect_vector_rooter rooter_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::value_pointer value_pointer;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	node_type* insert(size_t off,value_type const& value)
	{
		value_pointer pv = rooter().allocate(value);
		return rooter().index().insert(off,pv);
	}
	node_type* push_back(value_type const& value)
	{
		value_pointer pv = rooter().allocate_object(value);
		return rooter().index().push_back(pv);
	}
	void erase(size_t off,size_t size=1)
	{
		rooter().index().erase(off,size,indirect_vector_deleting_event<value_type>());
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
		rooter().index().foreach(indirect_act<action>(act));
	}
};

template <typename value_tn>
struct indirect_vector: indirect_vector_tt<indirect_vector_rooter<value_trait<value_tn>,allocator_trait<value_tn>>>
{};
template <typename value_tn>
struct variable_indirect_vector: indirect_vector_tt<indirect_vector_rooter<space_value_trait<value_tn>,space_allocator_trait<value_tn>>>
{};
template <typename value_tn,typename node_array,typename allocator_tn=object_allocator<value_tn>>
struct static_indirect_vector: indirect_vector_tt<static_indirect_vector_rooter<value_tn,node_array,allocator_tn>>
{};

template <typename value_tn>
struct mono_value_trait : value_trait<value_tn>
{
	static value_type* construct(char* buff,value_type const& value)
	{
		return (new (buff) value_type(value));
	}
};
template <typename value_tn>
struct space_mono_value_trait : value_trait<value_tn>
{
	static value_pointer construct(char* buff,value_type const& value)
	{
		return value_pointer(buff,value.size);
	}
};
template <typename value_trait,typename allocator_tn=cppmalloc>
struct mono_indirect_vector_rooter
{
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::node_type node_type;
	typedef typename value_trait::value_size value_size;
	typedef allocator_tn allcator_type;
	struct index_whole_size_getter
	{
		size_t _m_bytes_more;
		size_t operator()(size_t size)
		{
			return size+_m_bytes_more;
		}
	};
	struct data_head : vector_head_mono_tt<char>
	{
		void attach(char* vector_begin)
		{
			_m_vector_begin = vector_begin;
		}
	};
	typedef cppmalloc_before_more<allcator_type,vector_head_mono_tt<node_type>::__head_size> mono_index_allocator_type;
	typedef vector_tt<vector_rooter<node_type,vector_head_mono_tt<node_type>,vector_event,mono_index_allocator_type>> index_vector_type;
	typedef vector_rooter<char,data_head,vector_event,nullmalloc> data_vector_type;

	struct data_vector_wrapper : data_vector_type
	{
	public:
		using data_vector_type::data_begin;
		using data_vector_type::~data_vector_type;
		using data_vector_type::allocator;
	};
	struct index_vector_public : index_vector_type
	{
	public:
		using index_vector_type::data_begin;
		using index_vector_type::head;
	};


	index_vector_type _m_index;
	index_vector_type& index() {return _m_index;}

	node_type* vector_data_begin(index_vector_type& index_ref)
	{
		return ((index_vector_public*)(&index_ref))->head().data_begin()+index_ref.capacity();
	}
	value_pointer allocate_object(value_type const& value)
	{
		size_t byte_data_size = value_size()(value);
		reserve(index().size()+1,byte_data_size);
		char* data_begin = (char*)(vector_data_begin(index()));
		data_vector_wrapper data_vector;
		data_vector.attach(data_begin);
		assert(data_vector.capacity()>=data_vector.size()+byte_data_size);

		size_t old_size = data_vector.size();
		data_vector.resize(old_size+byte_data_size);
		return value_trait::construct(data_begin+old_size,value);
	}
	void resize(size_t size)
	{
		typedef indirect_vector_deleting_event<value_type> deleting_event;
		typedef indirect_vector_inserted_event<value_type> inserted_event;
		index().resize<deleting_event,inserted_event>(size);
	}
	void reserve(size_t size,size_t byte_data_size)
	{
		char* data_begin = (char*)(vector_data_begin(index()));
		data_vector_wrapper data_vector;
		data_vector.attach(data_begin);
		size_t usable = data_vector.capacity()-data_vector.size();
		size_t index_usable = index().capacity()-index().size();
		/// whether we need to reallocate the memory
		if (index_usable>=size && usable>=byte_data_size) return;
		size_t index_capacity = index_usable<size?index().size()+size:index().capacity();
		size_t total_data_size = data_vector.total_bytes()+byte_data_size-usable;
		index_whole_size_getter size_getter = {total_data_size};
		index_vector_type index_new;
		index_new.reserve(index_capacity,-1,size_getter);
		/// get the new data vector, copy the old data to the new data
		data_vector_wrapper data_vector_new;
		data_begin = (char*)(vector_data_begin(index_new));
		data_vector_new.attach(data_begin);
		char* value_begin = data_vector_new.data_begin();
		for (size_t i=0;i<index_new.size();++i)
		{
			node_type* node = index_new.push_back(node_type());
			*node = (value_type*)(data_vector_new.allocator().allocate(byte_data_size));
			new (*node) value_type (*(index().at(i))); /// copy construct the value
		}
		index().swap(index_new);
	}
	size_t size() const
	{
		return index().size();
	}
	size_t capacity() const
	{
		return index().capacity();
	}
};
template <typename value_tn,typename allocator_tn=cppmalloc>
struct mono_indirect_vector:
	indirect_vector_tt<
		mono_indirect_vector_rooter<
			mono_value_trait<value_tn>,
			allocator_tn>>
{};
template <typename value_tn,typename allocator_tn=cppmalloc>
struct mono_space_indirect_vector:
	indirect_vector_tt<
		mono_indirect_vector_rooter<
			space_value_trait<value_tn>,
			allocator_tn>>
{};


___namespace2_end()