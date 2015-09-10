
#include <assert.h>
#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../utl/data_t.h"
#include "allocator_default.h"
#include "vector_head.h"
#include "cpmv.h"




___namespace2_begin(ox,fit)


template <typename nothing_tn>
struct call_forward
{
	template <typename e,typename t1,typename t2>
	void operator()(e& event,t1 p1,t2 p2)
	{
		event(p1,p2);
	}
	template <typename t1,typename t2>
	void operator()(nothing_tn const& event,t1 p1,t2 p2)
	{
	}
	template <typename t1,typename t2,typename t3>
	void operator()(nothing_tn const& event,t1 p1,t2 p2,t3 p3)
	{
	}
};


template<typename vector_rooter_tn>
struct vector_tt : vector_rooter_tn
{
	typedef vector_rooter_tn rooter_type;
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::value_construct value_construct;
	typedef typename rooter_type::on_size_changed on_size_changed;

	template <typename on_inserted_tn>
	value_type* insert(size_t off,value_type const& value,on_inserted_tn& on_inserted)
	{
		if (rooter_type::size()>=rooter_type::capacity())
		{
			size_t old_size = rooter_type::size();
			rooter_type old; /// the allocator is static, so swap does not influence the objects
			rooter_type::swap(old);
			rooter_type::reserve(old_size==0?2:old_size*2);
			/// copy the first part objects
			value_type* new_begin = rooter_type::data_begin();
			value_type* old_begin = old.data_begin();
			cm::copy_objects(new_begin,-1,old_begin,off);
			/// copy the second part objects
			new_begin += (off+1); /// reserve one value space
			old_begin += off;
			cm::copy_objects(new_begin,-1,old_begin,old_size-off);
			/// keep the old size
			rooter_type::add_size(old_size);
		}
		else
		{
			value_type* old_begin = rooter_type::data_begin()+off;
			value_type* new_begin = old_begin+1;
			size_t size = rooter_type::size()-off;
			cm::move_objects_backward(new_begin,-1,old_begin,size);
		}
		value_type* value_begin = rooter_type::data_begin()+off;
		value_construct()(value_begin,value);
		rooter_type::add_size(1);
		call_forward<int const>()(on_size_changed(),static_cast<rooter_type*>(this),1);
		call_forward<int const>()(on_inserted,off,*value_begin,1);
		return value_begin;
	}
	/// on_deleting(off,size-to-be-deleted,data_begin)
	/// on_deleted(off,size-deleted)
	template <typename on_deleting_tn,typename on_deleted_tn>
	void erase(size_t off,size_t size,on_deleting_tn& on_deleteing,on_deleted_tn& on_deleted)
	{
		assert(off<rooter_type::size() && off+size<rooter_type::size());
		value_type* value_begin = rooter_type::data_begin();
		value_type* out = value_begin+off;
		value_type* src = out+size;
		size_t vec_size = rooter_type::size();
		call_forward<int const>()(on_deleteing,off,size,out);
		cm::move_objects<value_type>(out,-1,src,vec_size-off-size);
		rooter_type::add_size(-1);
		call_forward<int const>()(on_size_changed(),static_cast<rooter_type*>(this),-size);
		call_forward<int const>()(on_deleteing,off,size);
	}


	bool is_empty() const
	{
		return rooter_type::size()==0;
	}
	value_type* insert(size_t off,value_type const& value)
	{
		return insert<int const>(off,value,0);
	}
	value_type* push_front(value_type const& value)
	{
		return insert(0,value,0);
	}
	value_type* push_back(value_type const& value)
	{
		return push_back<int const>(value,0);
	}
	template <typename on_pushed_tn>
	value_type* push_back(value_type const& value,on_pushed_tn& on_pushed)
	{
		return insert(rooter_type::size(),value,on_pushed);
	}
	void erase(size_t i)
	{
		erase<int const,int const>(i,1,0,0);
	}
	void erase_to_end(size_t i)
	{
		erase(i,rooter_type::size()-i);
	}
	value_type& at(size_t i)
	{
		assert (i<=rooter_type::size());
		return *(rooter_type::data_begin()+i);
	}
	value_type const& at(size_t i) const
	{
		assert (i<=rooter_type::size());
		return *(rooter_type::data_begin()+i);
	}

	template <typename action>
	void foreach(action& act)
	{
		value_type* value_begin = rooter_type::data_begin();
		for (size_t i=0;i<rooter_type::size();++i)
		{
			if (!act(*value_begin++)) break;
		}
	}
	void clear()
	{
		rooter_type::resize(0);
	}
protected:
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
};


/// vector=>{head,allcotor}
/// head=>{size,capacity,data_begin}
/// data_begin=>{value,value,,,,}

struct size_gettter_default
{
	size_t operator()(size_t value_count) {return value_count;}
};
template <typename event>
struct get_on_size_changed
{
	typedef typename event::on_size_changed type;
};
template <>
struct get_on_size_changed<void>
{
	typedef int const type;
};

template<typename value_trait_tn,typename head_tn,typename events_tn=void,typename allocator_tn=cppmalloc>
struct vector_rooter : head_tn
{
	friend vector_tt<vector_rooter>;
	friend allocator_tn;
	typedef value_trait_tn value_trait;
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_construct value_construct;
	typedef head_tn head_t;
	typedef typename get_on_size_changed<events_tn>::type on_size_changed;
	friend on_size_changed;

protected:
	typedef vector_rooter self;
	typedef allocator_tn allocator_type;
	head_t& head()
	{
		return *this;
	}
	head_t const& head() const
	{
		return *this;
	}

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
	value_type const* data_begin() const
	{
		return head().data_begin();
	}
	void add_size(int delta)
	{
		head().add_size(delta);
	}
public:
	allocator_type allocator()
	{
		return allocator_type();
	}
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
	bool reserve(size_t size_reserved)
	{
		size_t size_old = size();
		size_t capacity_old = capacity();
		size_t usable_size = capacity_old-size_old;
		if (usable_size>=size_reserved)
			return true;
		size_t bytes_needed = (size_old+size_reserved)*sizeof(value_type);
		value_type* data_begin_new = (value_type*)allocator().allocate(bytes_needed);
		assert (data_begin_new);
		if (!data_begin_new)
			return false;
		cm::move_objects(data_begin_new,-1,data_begin(),size_old);
		head().set_data_begin(data_begin_new);
		head().set_capacity(size_reserved);
		head().set_size(size_old);
		call_forward<int const>()(on_size_changed(),this,0);
		return true;
	}
	void resize(size_t size)
	{
		resize<int const,int const>(size,0,0,size_gettter_default());
	}
	template <typename object_deleting_event,typename object_inserted_event,typename size_reget_tn>
	void resize(size_t size_new,object_deleting_event& del_event,object_inserted_event& ins_event,size_reget_tn& size_getter)
	{
		int delta = 0;
		if (size_new<=size())
		{
			value_type* value_begin = data_begin()+size_new;
			size_t size_deled = size()-size_new;
			call_forward<int const>()(del_event,value_begin,size_deled);
			cm::destruct_objects(value_begin,size_deled);
			delta = -size_deled;
		}
		else if (size_new<capacity())
		{
			value_type* value_begin = data_begin()+size();
			size_t size_added = size_new-size();
			cm::construct_objects(value_begin,size_added);
			call_forward<int const>()(ins_event,value_begin,size_added);
			delta = size_added;
		}
		else
		{
			size_t capacity_new = size_getter(size_new);
			reserve(capacity_new);
			value_type* value_begin = data_begin()+size();
			size_t size_added = size_new-size();
			cm::construct_objects(value_begin,size_added);
			call_forward<int const>()(ins_event,value_begin,size_added);
			delta = size_added;
		}
		head().set_size(size_new);
		call_forward<int const>()(on_size_changed(),this,delta);
	}
};


template <typename events>
struct string_events_tt
{
	struct on_size_changed
	{
		template <typename string_rooter_tn>
		void operator()(string_rooter_tn* rooter,int delta)
		{
			*(rooter->data_begin()+rooter->size())=0;
			typedef typename get_on_size_changed<events>::type on_size_changed;
			call_forward<int const>()(on_size_changed(),rooter,delta);
		}
	};
};

template<typename value_trait_tn,typename head_tn,typename events=void,typename allocator_tn=cppmalloc>
struct string_rooter
	: vector_rooter<
		value_trait_tn,
		head_tn,
		string_events_tt<events>,
		cppmalloc_after_more<allocator_tn,sizeof(typename value_trait_tn::value_type)>>
{
	friend vector_tt<string_rooter>;
	string_rooter()
	{
		set_data_begin(allocator().allocate(0));
		resize(0);
	}
	value_type const* data() const
	{
		return data_begin();
	}
	value_type const* c_str() const
	{
		return data_begin();
	}
};


___namespace2_end()




#include "value_trait.h"
___namespace2_begin(ox,fit)
template <typename value_tn,typename allocator_tn=cppmalloc>
struct vector
	: vector_tt<
		vector_rooter<
			value_trait<value_tn>,
			vector_head_iso_tt<value_tn>,
			void,
			allocator_tn>>
{};
___namespace2_end()

___namespace2_begin(ox,fit)
template <typename value_tn,typename allocator_tn=cppmalloc>
struct string
	: vector_tt<
		string_rooter<
			value_trait<value_tn>,
			vector_head_iso_tt<value_tn>,
			void,
			allocator_tn>>
{};
___namespace2_end()




//template<typename array,size_t added,typename vector_events_tn=vector_event>
//struct static_vector_rooter;
//template<typename value_tn,size_t n,size_t added,typename vector_events_tn>
//struct static_vector_rooter<value_tn[n],added,vector_events_tn>
//{
//	friend vector_tt<static_vector_rooter>;
//	friend vector_on_size_changed;
//	friend string_on_size_changed;
//	typedef value_tn value_type;
//	typedef value_tn array_type[n+added];
//protected:
//	typedef static_vector_rooter self;
//	typedef typename vector_events_tn::on_size_changed on_size_changed;
//	struct head_t
//	{
//		head_t(): _m_size(0) {}
//		void set_size(size_t size) {_m_size=size;}
//		size_t size() const {return _m_size;}
//		size_t _m_size;
//		array_type _m_data;
//	};
//	~static_vector_rooter()
//	{
//		_m_head._m_size = 0;
//	}
//	value_type* data_begin()
//	{
//		return _m_head._m_data;
//	}
//	head_t head() {return _m_head;}
//	head_t _m_head;
//public:
//	void swap(self& other)
//	{
//		assert (false && "not support");
//	}
//	size_t size() const
//	{
//		return _m_head._m_size;
//	}
//	size_t capacity() const
//	{
//		return n;
//	}
//	void reserve(size_t size,size_t size_copyed=-1)
//	{
//	}
//	void resize(size_t size)
//	{
//		if (size<=_m_head._m_size)
//		{
//			vector_helper::destruct_objects(_m_head._m_data+size,_m_head._m_size-size);
//			_m_head._m_size = size;
//		}
//		else if (size<capacity())
//		{
//			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
//			_m_head._m_size = size;
//		}
//		else
//		{
//			reserve(size-_m_head._m_size);
//			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
//			_m_head._m_size = size;
//		}
//		on_size_changed()(*this,0);
//	}
//};
//
//template<typename array,typename string_events_tn=string_event>
//struct static_string_rooter;
//template<typename value_tn,size_t n,typename string_events_tn>
//struct static_string_rooter<value_tn[n],string_events_tn>
//	: static_vector_rooter<value_tn[n],1,string_events_tn>
//{
//	friend vector_tt<static_string_rooter>;
//	typedef static_string_rooter self;
//	self()
//	{
//		*data_begin()=0;
//	}
//};
//
//template <typename value_tn,typename allocator_tn=cppmalloc>
//struct vector : vector_tt<vector_rooter<value_trait<value_tn>,vector_head_iso_tt<value_tn>,vector_event,allocator_tn>>
//{};

//template <typename array_tn>
//struct static_vector: vector_tt<static_vector_rooter<array_tn,0,vector_event>>
//{};
//
//template <typename value_tn,typename allocator_tn=cppmalloc_after_more<cppmalloc,sizeof(value_tn)>>
//struct string : vector_tt<string_rooter<value_tn,vector_head_iso_tt<value_tn>,allocator_tn>>
//{};
//
//template <typename array_tn>
//struct static_string: vector_tt<static_string_rooter<array_tn,string_event>>
//{};




