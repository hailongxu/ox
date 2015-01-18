

#include "typetrait.h"
#include "veclist.h"
#include "assert.h"
#include "bintree.h"

#pragma once


namespace abc
{
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
} /// end of namespace


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

	template<typename character_tn,typename allocator_tn=cppmalloc>
	struct linecache
		: allocator_tn
	{
		typedef character_tn character;
		typedef allocator_tn allocator_type;
		typedef interval_tt<character*> interval_t;

		static_assert(sizeof(character)==sizeof(char) || sizeof(character)==sizeof(wchar_t), "ch is char or wchar_t");

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
		bool operator==(wstring const& str) const
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
		bool operator!=(wstring const& str) const
		{
			return !operator==(o);
		}
		bool operator!=(character const* str) const
		{
			return !operator==(o);
		}
	};



	template <typename node_tn,typename value_cmp_tn=compare_default>
	struct bplustree_noder_default
	{
		static size_t const __rank = node_tn::__rank;
		typedef bplustree_noder_default self; ///
		typedef node_tn node_type;
		typedef typename node_type::internal_value_type internal_value_type;
		typedef typename node_type::value_type raw_value_type;
		typedef typename ifft<is_con<node_type>::value,const raw_value_type,raw_value_type>::type value_type;
		typedef value_cmp_tn value_cmp_type;
		typedef btree_noder_default<node_tn const,value_cmp_type> const_noder_type; ///
		typedef const_noder_type const_self; ///
		typedef assi_dchilder<self> assi_dchilder_type;
		typedef assi_parenter<self> assi_parenter_type;
		typedef assi_dvaluer<self> assi_valuer_type;

		/// common operation
		node_type* _m_node;
		self(node_type* node=0): _m_node(node) {}
		self& operator=(node_type* node){_m_node = node;return *this;}
		operator node_type* () {return _m_node;}
		operator node_type const* () const {return _m_node;}
		value_type& operator*() {return *node();}
		node_type const* operator->() const {return _m_node;}
		node_type* operator->() {return _m_node;}
		node_type* node() {return _m_node;}
		node_type const* node() const {return _m_node;}
		bool is_empty() const {return 0==_m_node;}
		bool has_children() const{if (is_empty())return false;return 0==countref();}

		/// state operation
		bool is_full() const {return _m_node?countref()==__rank-1:true;}
		bool is_greater_than_min() const {return value_size()>(__rank-1)/2;}
		bool is_greater_equal_than_min() const {return value_size()>=(__rank-1)/2;}

		/// assign
	public:
		assi_dchilder_type child_assi(size_t index) {return assi_dchilder_type(*this,index);}
		assi_valuer_type value_assi(size_t index) {return assi_valuer_type(*this,index);}
		self& set_child(node_type* node,size_t index) {childref(index)=node;return *this;}
		self& set_value(value_type const& value,size_t index) {valueref(index)=value;return *this;}
		self& set_parent(node_type* node) {parentref()=node;return *this;}

		/// get : size,child(i),value(i),parent
		size_t value_size() const {return countref();}

		node_type* child(size_t index) {return childref(index);}
		value_type& value(size_t index) {return valueref(index);}
		node_type const* child(size_t index) const {return childref(index);}
		value_type const& value(size_t index) const {return valueref(index);}
		value_type const& front_value() const {return value(0);}
		value_type const& back_value() const {return value(value_size()-1);}
		value_type const& back_value(size_t i) const {assert(i<value_size());return value(value_size()-1-i);}
		value_type& front_value() {return value(0);}
		value_type& back_value() {return value(value_size()-1);}
		node_type* front_child() {return child(0);}
		node_type* back_child() {return child(value_size());}

		const_self childer(size_t index) const {return const_self(child(index));}
		self childer(size_t index) {return self(child(index));}
		const_self front_childer() const {return child(0);}
		const_self back_childer() const {return child(value_size()-1);}
		self front_childer() {return child(0);}
		self back_childer() {return child(value_size()-1);}

		static bool cmp(value_type const& a,value_type const& b) {return value_cmp_type()(a,b);}

		/// extended complex operation
		node_type* get_next_sibling_child(node_type* node) {return (get_internal(node)+1)->second;}
		node_type* get_prev_sibling_child(node_type* node) {return (get_internal(node)-1)->second;}
		static size_t get_offsetof_child() {return (size_t)(&(((internal_value_type*)(0))->second));}
		static size_t get_offsetof_value() {return (size_t)(&(((internal_value_type*)(0))->first));}
		static internal_value_type* get_internal(node_type* node) {return (internal_value_type*)((char*)node-get_offsetof_child());}
		//// extended
		size_t find(value_type const& value) const
		{
			internal_value_type const* ret = _m_node->_m_data.find(value);
			if (ret==_m_node->_m_data.end())
				return -1;
			return ret-_m_node->_m_data.begin();
		}
		pair<size_t,bool> try_find(value_type const& value) const
		{
			pair<internal_value_type const*,bool> ret = _m_node->_m_data.try_find(value);
			return pair<size_t,bool>(ret.first-_m_node->_m_data.begin(),ret.second);
		}
		/// return offset
		size_t insert(value_type const& value)
		{
			return insert(value,(node_type*)(0));
		}
		size_t insert(value_type const& value,node_type* pnode)
		{
			internal_value_type* ret = _m_node->_m_data.insert(value,pnode);
			return _m_node->_m_data.distance(ret);
		}
		size_t unsafe_insert_at(size_t off,value_type const& value,node_type* pnode)
		{
			internal_value_type* ret = _m_node->_m_data.unsafe_insert_at(off,value,pnode);
			return _m_node->_m_data.distance(ret);
		}
		//size_t insert(node_type* leftnode,value_type const& value,node_type* rightnode)
		//{
		//	internal_value_type* ret = _m_node->_m_data.insert(value,rightnode);
		//	(ret-1)->second = leftnode;
		//	return _m_node->_m_data.distance(ret);
		//}
		void remove(value_type const& value)
		{
			_m_node->_m_data.remove(value);
		}
		void remove_at(size_t off)
		{
			_m_node->_m_data.remove_at(off);
		}
		void remove_front()
		{
			_m_node->_m_data.remove_front();
		}
		void remove_back()
		{
			_m_node->_m_data.remove_back();
		}
		void push_front(value_type const& value,node_type* pnode)
		{
			_m_node->_m_data.push_front(value,front_child());
		}
		void push_back(value_type const& value,node_type* pnode)
		{
			_m_node->_m_data.push_back(value,pnode);
		}
		void append(self& noder)
		{
			_m_node->_m_data.append(noder->_m_data.begin(),noder->_m_data.size());
		}
		pair<value_type&,node_type*> internal_remove_back()
		{
			internal_value_type* removed = _m_node->_m_data.internal_remove_back();
			return pair<value_type&,node_type*>(removed->first,removed->second);
		}
		void split(size_t prevoff,self& newer)
		{
			_m_node->_m_data.split(prevoff,newer._m_node->_m_data);
		}
		value_type* split_out(size_t off,self& newer)
		{
			assert(off>=0 && off<value_size());
			split(off+1,newer);
			return &back_value();
		}
		static void swap(self a,size_t aoff,self b,size_t boff)
		{
			value_type t = a.value(aoff);
			a.value_assi(aoff) = b.value(boff);
			b.value_assi(boff) = t;
		}
	protected:
		node_type const*& childref(size_t index) const {
			node_type* & ret = _m_node->_m_data.at(index)->second;
			return const_cast<node_type const*&>(ret);
		}
		node_type*& childref(size_t index) {return _m_node->_m_data.at(index)->second;}
		//node_type const*& parentref() const {return _m_node->_m_parent;}
		//node_type*& parentref() {return _m_node->_m_parent;}
		value_type& valueref(size_t index) {return _m_node->_m_data.at(index)->first;}
		value_type const& valueref(size_t index) const {return _m_node->_m_data.at(index)->first;}
		size_t const& countref() const {return _m_node->_m_data.size();}
		size_t& countref() {return _m_node->_m_data.size();}
	};

	template <typename value_tn,size_t rank_tc>
	struct bplusnode_tt
	{
		typedef value_tn value_type;
		static size_t const __rank = rank_tc;
		static_assert(__rank>=3, "rank should be >= 3");
		typedef typename static_keyvalue_sorted_podliner_off_tt<value_type,bplusnode_tt*,__rank-1,size_t>::type static_data_type;
		typedef typename static_data_type::value_type internal_value_type;
		static_data_type _m_data;
	};


}


namespace abc
{

	template <typename noder_tn,typename path_tn,typename action_tn>
	static bool walk_pre_bplus(noder_tn noder,path_tn& path,action_tn& action)
	{
		//if (!noder.is_empty())
		//	assert(noder.parent()?noder.parent()==path.top().first:path.is_empty());
		if (noder.is_empty())
			return false;

		path.push(noder,0);
		size_t const value_size = noder.value_size();
		for (size_t i=0;i<value_size;++i)
		{
			path.top().second = i;
			action(path);
			walk_pre_bplus(noder.childer(i),path,action);
		}
		path.pop();
		return true;
	};


	template <typename stringify_value_tn>
	struct stringify_bplusnode
	{
		typedef stringify_value_tn stringify_value;
		template <typename bnode_tn>
		string operator()(bnode_tn* node,size_t off) const
		{
			typedef btree_noder_default<bnode_tn> noder_type;
			noder_type noder = node;
			string str;
			str.append(" ") += stringify_value()(noder.value(off));
			return str;
		}
	};

	template <typename stringify_value_tn=stringify_value_basic>
	struct print_pre_bplusnode
	{
		typedef stringify_bplusnode<stringify_value_tn> stringify_node;

		template <typename node_tn>
		bool operator()(node_tn* node)
		{
			typedef bplustree_noder_default<node_tn> noder_type;
			print_pre_bplus_ts<noder_type,stringify_node> printer;
			return printer(noder_type(node));
		}
	};

	template <typename noder_tn,typename stringify_node_tn>
	struct print_pre_bplus_ts
	{
		typedef typename stack_tt<pair<noder_tn,size_t>>::type path_type;
		typedef typename path_type::value_type path_value_type;

		typedef noder_tn noder_type;
		typedef typename noder_tn::node_type node_type;
		typedef typename noder_tn::const_noder_type const_noder_type;
		typedef stringify_node_tn stringify_node;
		static bool islast(path_value_type const& parenter)
		{
			size_t size = parenter.first.value_size();
			size_t off = parenter.second;
			return off+1<size?false:true;
		}
		static bool isfirst(path_value_type const& parenter)
		{
			size_t off = parenter.second;
			return off==0;
		}
		static void get_pretext(path_type& path,string& str)
		{
			noder_type& noder = path.top().first;
			size_t off = path.top().second;
			str += islast(path.top())||isfirst(path.top()) ? '`' : '+';
			str.append("--");
			str += stringify_node()(noder.node(),off);
			if (path.size()<=1) return;
			get_pretext_rest(path,1,str);
		}
		static void get_pretext_rest(path_type const& path,size_t itop,string& str)
		{
			if (islast(path.top(itop)))
				str.insert(0,"      ");
			else
				str.insert(0,"|     ");
			if (++itop<path.size()) get_pretext_rest(path,itop,str);
		}

		struct action_print
		{
			bool operator()(path_type& path) const
			{
				string str;
				if (!path.top().first.is_empty())
				{
					get_pretext(path,str);
					printf("%s\n",str.c_str());
				}
				return true;
			}
		};
		bool operator()(noder_tn noder)
		{
			path_type path;
			return noder ? walk_pre_bplus(noder,path,action_print()) : false;
		}
	};


	/// node* is the first, not the last
	template <typename rooter_tn>
	struct bplustree_ts
	{
		typedef rooter_tn rooter_type;
		typedef typename rooter_type::noder_type noder_type;
		typedef typename rooter_type::node_type node_type;
		typedef typename noder_type::value_type value_type;
		typedef typename noder_type::value_cmp_type value_cmp_type;
		typedef typename noder_type::assi_valuer_type valuer_type;
		typedef pair<noder_type,size_t> location_type;
		typedef typename stack_tt<location_type>::type path_type;

		rooter_type _m_rooter;

		bplustree_ts() {}

		node_type* root() {return _m_rooter.rooter().node();}
		node_type const* root() const {return _m_rooter.rooter().node();}

		location_type find(value_type const& value)
		{
			if (_m_rooter.rooter().is_empty())
				return location_type(_m_rooter.rooter(),-1);
			return find(_m_rooter.rooter(),value);
		}
		bool test_find(value_type const& value)
		{
			path_type path;
			return find(path,value);
		}
		bool find(path_type& path,value_type const& value)
		{
			int r = value_cmp_type()(value,_m_rooter.rooter().back_value());
			if (r>0) return false;
			return find(path,_m_rooter.rooter(),value);
		}
		bool find(path_type& path,noder_type noder,value_type const& value)
		{
			if (noder.is_empty()) return false;
			pair<size_t,bool> off = noder.try_find(value);
			path.push(noder,off.first);
			if (off.second) return find_max(path,noder.childer(off.first)), true;
			return find(path,noder.childer(off.first),value);
		}
		void find_max(path_type& path,noder_type noder)
		{
			if (noder.is_empty()) return;
			path.push(noder,noder.value_size()-1);
			return find_max(path,noder.back_childer());
		}
		location_type find(noder_type noder,value_type const& value)
		{
			int off_within_node = -1;
			do {
				int r = value_cmp_type()(value,noder.front_value());
				if (r==0) return location_type(noder,0);
				else if (r<0) {off_within_node=0;noder=noder.front_childer();break;}
				r = value_cmp_type()(value,noder.back_value());
				if (r==0) return location_type(noder,noder.value_size()-1);
				else if (r<0) {off_within_node=noder.value_size();noder=noder.back_childer();break;}
			} while(0);
			if (noder.is_empty()) return location_type(noder,off_within_node);
			if (off_within_node!=-1) return find(noder,value);

			pair<size_t,bool> off = noder.try_find(value);
			noder_type childer = noder.childer(off.first-1);
			if (off.second || childer.is_empty())
				return location_type(noder,off.first);
			return find(childer,value);
		}
		static void find_predecessor(noder_type noder,path_type& path)
		{
			for (;!noder.is_empty();noder=noder.back_childer())
				path.push(noder,-1);
		}
		static void find_successor(noder_type noder,path_type& path)
		{
			while (!noder.is_empty();noder=noder.front_childer())
				path.push(noder,0);
		}
		static void find_leaf_inserted(noder_type noder,value_type const& value,path_type& path)
		{
			/// if has no children, ok, it the noder, but we should calculate the offset insertion
			if (noder.front_childer().is_empty())
			{
				pair<size_t,bool> off = noder.try_find(value);
				path.push(noder,off.first);
				return;
			}
			/// if at the left side
			int r = value_cmp_type()(value,noder.front_value());
			if (r<0)
				return find_leaf_inserted(noder.front_childer(),value,path.push(noder,0));
			/// if at the right side
			r = value_cmp_type()(value,noder.back_value());
			if (r>0)
				return find_leaf_inserted(noder.back_childer(),value,path.push(noder,noder.value_size()));
			/// ok, it is inside the current node
			pair<size_t,bool> off = noder.try_find(value);
			if (!off.second)
				--off.first;
			return find_leaf_inserted(noder.childer(off.first),value,path.push(noder,off.first));
		}
		/// will generate a new right, and a parent if the parent is empty
		noder_type split_children(noder_type& parent,size_t parent_off,noder_type child,size_t off)
		{
			//bool is_parent_empty = parent.is_empty();
			//if (is_parent_empty)
			//	parent = _m_rooter.make<node_type>();
			assert(!parent.is_empty());
			if (parent.is_full()) return noder_type();
			noder_type new_noder = _m_rooter.make<node_type>();
			if (new_noder.is_empty())
				return noder_type();
			value_type const* new_value = child.split_out(off,new_noder);
			if (!new_value) /// will never happen
				return noder_type();
			parent.unsafe_insert_at(parent_off,*new_value,child);
			//if (is_parent_empty)
			//	;//parent.push_back(new_noder.back_value(),new_noder);
			//else
				parent.child_assi(parent_off+1) = new_noder;
			return new_noder;
		}
		/// return the child pointer index
		static size_t find_inserted(noder_type noder,value_type const& value)
		{
			//assert(noder.value_size()>=2);
			/// if at the left side
			int r = value_cmp_type()(value,noder.front_value());
			if (r<0 || noder.value_size()==1) return 0;
			/// if at the right side
			r = value_cmp_type()(value,noder.back_value(1));
			if (r>0) return noder.value_size()-1;
			/// ok, it is inside the current node
			pair<size_t,bool> off = noder.try_find(value);
			//if (!off.second) --off.first;
			return off.second?off.first:off.first-1;
		}
		template <typename spliter_tn>
		struct internal_inserter_tt
		{
			internal_inserter_tt(spliter_tn& _spliter) : spliter(_spliter) {}
			spliter_tn& spliter;
			location_type newer;
			location_type toper;

			internal_inserter_tt& operator()(noder_type parenter,size_t poff,noder_type noder,value_type const& value)
			{
				assert(parenter.is_empty() || !parenter.is_full());
				/// if noder is full, ok, split first
				if (noder.is_full())
				{
					noder_type new_noder = spliter.split_children(parenter,poff,noder,noder.value_size()/2);
					int r = value_cmp_type()(value,noder.back_value());
					if (r>0) noder=new_noder,++poff;
				}
				assert(!noder.is_full());
				/// if has no children, ok, it's the noder, but we should calculate the offset insertion
				if (noder.front_childer().is_empty())
				{
					noder.insert(value);
					return *this;
				}
				size_t off = find_inserted(noder,value);
				return operator()(noder,off,noder.childer(off),value);
			}
		};
		template <typename spliter_tn>
		struct internal_inserter_max_tt
		{
			internal_inserter_max_tt(spliter_tn& _spliter) : spliter(_spliter) {}
			spliter_tn& spliter;
			location_type newer;
			location_type toper;

			internal_inserter_max_tt& operator()(noder_type parenter,noder_type noder,value_type const& value)
			{
				if (noder.is_empty()) return *this;
				if (noder.is_full())
				{
					size_t parent_last_index = parenter.is_empty()?-1:parenter.value_size()-1;
					noder_type new_noder = spliter.split_children(parenter,parent_last_index,noder,noder.value_size()/2);
					noder=new_noder;
				}
				if (noder.front_childer().is_empty())
				{
					noder.push_back(value,0);
					return *this;
				}
				noder.value_assi(noder.value_size()-1) = value;
				return operator()(noder,noder.back_childer(),value);
			}
		};

		struct trace_null_tt {void operator()(){}};
		pair<noder_type,size_t> insert(value_type const& value)
		{
			return insert(value,trace_null_tt());
		}
		template <typename trace_tn>
		pair<noder_type,size_t> insert(value_type const& value,trace_tn& trace)
		{
			if (_m_rooter.is_empty())
			{
				_m_rooter.root_assi()=_m_rooter.make<node_type>();
				pair<noder_type,size_t> ret(0,-1);
				if (_m_rooter.is_empty())
					return ret;
				ret.second = _m_rooter.rooter().insert(value);
				ret.first = _m_rooter.rooter();
				return ret;
			}
			if (0 < value_cmp_type()(value,_m_rooter.rooter().back_value()))
			{
				noder_type parenter;
				noder_type noder = _m_rooter.rooter();
				if (noder.is_full())
				{
					_m_rooter.root_assi()=parenter=_m_rooter.make<node_type>();
					parenter.push_back(value,noder);
				}
				internal_inserter_max_tt<bplustree_ts> inserter_max(*this);
				inserter_max(parenter,noder,value);
				return inserter_max.newer;
			}
			/// the root is full or not full
			noder_type parenter;
			size_t poff = -1;
			noder_type noder = _m_rooter.rooter();
			if (noder.is_full())
			{
				trace();

				_m_rooter.root_assi()=parenter=_m_rooter.make<node_type>();
				parenter.push_back(noder.back_value(),noder);
				poff = 0;
				trace();
			}
			internal_inserter_tt<bplustree_ts> inserter(*this);
			inserter(parenter,poff,noder,value);
			return inserter.newer;
		}
		/// if find, return the index of value, or else return the child index which value maybe is in
		static pair<size_t,bool> try_find(noder_type noder,value_type const& value)
		{
			/// if at the left side
			int r = value_cmp_type()(value,noder.front_value());
			if (r<0) return pair<size_t,bool>(0,false);
			/// if at the right side
			r = value_cmp_type()(value,noder.back_value());
			if (r>0) return pair<size_t,bool>(noder.value_size(),false);
			/// ok, it is inside the current node
			return  noder.try_find(value);
		}
		static void borrow_one_from_successor(noder_type noder,noder_type parenter,size_t childoff)
		{
			assert(childoff+1<=parenter.value_size());
			noder_type next_sibling = parenter.child(childoff+1);
			noder.push_back(parenter.value(childoff),next_sibling.front_child());
			parenter.value_assi(childoff) = next_sibling.front_value();
			next_sibling.remove_front();
		}
		static void borrow_one_from_predecessor(noder_type noder,noder_type parenter,size_t childoff)
		{
			assert(childoff>0);
			noder_type prev_sibling = parenter.child(childoff-1);
			noder.push_front(parenter.value(childoff-1),prev_sibling.back_child());
			parenter.value_assi(childoff-1) = prev_sibling.back_value();
			prev_sibling.remove_back();
		}
		/// return the new childer
		static noder_type merge_two_children(noder_type parenter,size_t index_of_value)
		{
			/// parenter maybe is the root, which is beyond the rule of >1/2
			/// so, we can not add the assertion here
			//assert (parenter.is_greater_than_min());
			assert(index_of_value<=parenter.value_size());
			noder_type lefter = parenter.childer(index_of_value);
			noder_type righter = parenter.childer(index_of_value+1);
			lefter.push_back(parenter.value(index_of_value),0);
			lefter.append(righter);
			parenter.remove_at(index_of_value);
			return lefter;
		}
		/// poff: child off of noder, return the new childoff
		/// return : <noder,pchildoff>, if merged, noder is the merged node, off is the new, if not, node is noder, off keeping
		pair<pair<noder_type,size_t>,pair<size_t,size_t>> balance_node(noder_type noder,noder_type parenter,size_t pchildoff)
		{
			pair<size_t,size_t> delta(0,0);
			while (!noder.is_greater_than_min() && !parenter.is_empty())
			{
				size_t child_off_last = parenter.value_size();
				assert(pchildoff>=0 && pchildoff<=child_off_last);
				noder_type prever = parenter.childer(pchildoff-1);
				noder_type nexter = parenter.childer(pchildoff+1);

				/// positioned, -1:borrow from left, 1:borrow from right, 0:merge
				int borrow_index = 0;
				int merge_value_pindex = pchildoff; /// default is right
				if (pchildoff==0) nexter.is_greater_than_min() ? borrow_index=1 : (merge_value_pindex);
				else if (pchildoff==child_off_last) prever.is_greater_than_min() ? (borrow_index=-1) : --merge_value_pindex;
				else if (nexter.is_greater_than_min()) borrow_index = 1;
				else if (prever.is_greater_than_min()) borrow_index = -1;
				else if (nexter.value_size()>prever.value_size()) --merge_value_pindex;
				else (merge_value_pindex); /// merge nexter

				/// still 0, ok, we should merge
				if (borrow_index==1)
				{
					++delta.second;
					borrow_one_from_successor(noder,parenter,pchildoff);
				}
				else if (borrow_index==-1)
				{
					++delta.first;//,--pchildoff,noder=parenter.childer(pchildoff);
					borrow_one_from_predecessor(noder,parenter,pchildoff);
					//pchildoff;
				}
				else /// merge action
				{
					if (merge_value_pindex<pchildoff)
						delta.first += (1+parenter.childer(merge_value_pindex).value_size());
					else
						delta.second += (1+parenter.childer(merge_value_pindex+1).value_size());
					pchildoff=merge_value_pindex;
					noder = merge_two_children(parenter,merge_value_pindex);
				}
				break;
			};
			pair<noder_type,size_t> first (noder,pchildoff);
			return pair<pair<noder_type,size_t>,pair<size_t,size_t>>(first,delta);
		}
		/// if finded, return noder,offset, or return noder,-1
		/// the last fined node, which we do balance here, balanced after removed, which we can reduce data-moving once
		pair<location_type,location_type> remove_higher(noder_type parenter,size_t pchildoff,noder_type noder,value_type const& value)
		{
			if (noder.is_empty())
				return pair<location_type,location_type>(location_type(noder,-1),location_type(parenter,pchildoff));
			/// we will combine the nodes, except the root node
			/// because, removing will never create node, so if the parent is null, we can find sibling, do nothing,
			/// and under this condition, only it is the node that is root node
			pair<size_t,bool> offed = try_find(noder,value);
			if (offed.second)
				return pair<location_type,location_type>(location_type(noder,offed.first),location_type(parenter,pchildoff));
			balance_node(noder,parenter,pchildoff);
			size_t off = offed.first;
			return remove_higher(noder,off,noder.childer(off),value);
		}

		pair<location_type,location_type> remove_below_successor(noder_type noder,noder_type parenter,size_t pchildoff)
		{
			if (noder.front_childer().is_empty())
				return pair<location_type,location_type>(location_type(noder,0),location_type(parenter,pchildoff));
			noder_type noder_front = noder.front_childer();
			pair<pair<noder_type,size_t>,pair<size_t,size_t>> ret = balance_node(noder,parenter,pchildoff);
			noder_type new_noder_right = ret.first.first;
			size_t off_of_noder_right = ret.second.first+0; /// the number of left inserted
			noder_type the_prev_leftest_of_childer_right = noder_front;
			assert (new_noder_right.childer(off_of_noder_right)==the_prev_leftest_of_childer_right);
			return remove_below_successor(the_prev_leftest_of_childer_right,new_noder_right,off_of_noder_right);
		}
		void remove(value_type const& value)
		{
			remove(noder_type(),0,_m_rooter.rooter(),value);
			if (_m_rooter.rooter().value_size()==0)
				_m_rooter.root_assi() = _m_rooter.rooter().front_child();
		}
		void remove(noder_type parenter, size_t poff, noder_type noder, value_type const& value)
		{
			/// probably, the root will be changed
			pair<location_type,location_type> finded = remove_higher(parenter,poff,noder,value);
			if (finded.first.second==-1) return; /// not found
			/// if the hit node is not leaf
			//{
			size_t child_right_off = finded.first.second + 1;
			noder_type childer_right = finded.first.first.childer(child_right_off);
			/// continue to balance until the next successor
			pair<location_type,location_type> successor = finded;
			if (!childer_right.is_empty())
			{
				/// balance the hit node
				pair<pair<noder_type,size_t>,pair<size_t,size_t>> ret = balance_node(finded.first.first,finded.second.first,finded.second.second);
				/// recalculate the offset of the hit node
				finded.first.first = ret.first.first; /// new_noder;
				finded.first.second += ret.second.first; /// adds the number of left inserted
				finded.second.second = ret.first.second; /// the new index of parent
				successor = finded;
				child_right_off = finded.first.second + 1;
				childer_right = finded.first.first.childer(child_right_off);
				successor = remove_below_successor(childer_right,finded.first.first,child_right_off);
			}
			//}
			location_type& leafed = successor.first;
			location_type& pleafed = successor.second;
			/// exchange finded and leafed
			if (finded.first.first!=leafed.first)
				noder_type::swap(finded.first.first,finded.first.second,leafed.first,leafed.second);
			/// remove the leaf node
			noder_type victim = leafed.first;
			size_t value_off = leafed.second;
			size_t leafoff = value_off;
			/// when the parent is root
			//assert (parenter.is_empty() || parenter.is_greater_than_min());
			victim.remove_at(value_off);
			if (victim.is_greater_equal_than_min()) return;

			/// re-balance the victim node
			noder_type parenter_of_victim = pleafed.first;
			size_t pleafoff = pleafed.second;
			balance_node(victim,parenter_of_victim,pleafoff);
			return;
		}

		noder_type get_leftest_child(noder_type noder) {return noder.child(0);}
		noder_type get_rightest_child(noder_type noder) {return noder.child(0);}
		value_type const& get_leftest_value(noder_type noder) {return noder.value(0);}
		value_type const& get_rightest_value(noder_type noder) {return noder.value(noder.value_size());}
	};


	template <size_t rank,typename value_tn,typename cmp_tn=compare_default,typename allocator_tn=cppmalloc>
	struct bplustree_tt
	{
		typedef cmp_tn cmp_type;
		typedef bplustree_noder_default<bplusnode_tt<value_tn,rank>,cmp_type> noder_type;
		typedef btree_rooter<noder_type,allocator_tn> rooter_type;
		typedef bplustree_ts<rooter_type> type;
	};



} /// end of namespace abc