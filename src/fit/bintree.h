

#include "typetrait.h"
#include "veclist.h"
#include "assert.h"

#pragma once


namespace ox
{
	template <typename derived_tn,typename internal_tn>
	struct bintree_empty_class
	{};

	struct less_default
	{
		template <typename value_tn>
		bool operator()(value_tn const& a,value_tn const& b) const
		{
			return a<b;
		}
	};

	enum rbcolor_enum {__rb_red,__rb_black};

	template <typename noder_tn>
	struct assi_dchilder
	{
		typedef typename noder_tn::node_type node_type;
		assi_dchilder(noder_tn& noder,size_t index)
			: _m_noder(noder), _m_index(index)
		{}
		noder_tn& _m_noder;
		size_t const& _m_index;
		noder_tn& operator=(noder_tn childer) {_m_noder.set_child(childer,_m_index);return _m_noder;}
		noder_tn& operator=(node_type* child) {_m_noder.set_child(child,_m_index);return _m_noder;}
	};
	template <typename noder_tn>
	struct assi_dvaluer
	{
		assi_dvaluer(noder_tn& noder,size_t index)
			: _m_noder(noder), _m_index(index)
		{}
		noder_tn& _m_noder;
		size_t const& _m_index;
		template <typename value_tn>
		noder_tn& operator=(value_tn const& value) {_m_noder.set_value(value,_m_index);return _m_noder;}
	};
	template <typename noder_tn,size_t index_tn>
	struct assi_childer
	{
		assi_childer(noder_tn& noder)
			: _m_noder(noder)
		{}
		noder_tn& _m_noder;
		template <typename node_tn>
		noder_tn& operator=(node_tn* node) {_m_noder.set_child(node);return _m_noder;}
		noder_tn& operator=(noder_tn /*const*/& noder) {_m_noder.set_child(noder.node());return _m_noder;}
	};
	template <typename noder_tn>
	struct assi_lefter
	{
		assi_lefter(noder_tn& noder)
			: _m_noder(noder)
		{}
		noder_tn& _m_noder;
		template <typename node_tn>
		noder_tn& operator=(node_tn* node) {_m_noder.set_left(node);return _m_noder;}
		noder_tn& operator=(noder_tn /*const*/& noder) {_m_noder.set_left(noder.node());return _m_noder;}
	};
	template <typename noder_tn>
	struct assi_righter
	{
		assi_righter(noder_tn& noder)
			: _m_noder(noder)
		{}
		noder_tn& _m_noder;
		template <typename node_tn>
		noder_tn& operator=(node_tn* node) {_m_noder.set_right(node);return _m_noder;}
		noder_tn& operator=(noder_tn& noder) {_m_noder.set_right(noder.node());return _m_noder;}
	};
	template <typename noder_tn>
	struct assi_parenter
	{
		assi_parenter(noder_tn& noder)
			: _m_noder(noder)
		{}
		noder_tn& _m_noder;
		template <typename node_tn>
		noder_tn& operator=(node_tn* node) {_m_noder.set_parent(node);return _m_noder;}
		noder_tn& operator=(noder_tn& noder) {_m_noder.set_parent(noder.node());return _m_noder;}
	};
	template <typename noder_tn>
	struct assi_colorer
	{
		assi_colorer(noder_tn& noder)
			: _m_noder(noder)
		{}
		noder_tn& _m_noder;
		noder_tn& operator=(rbcolor_enum color) {_m_noder.set_color(color);return _m_noder;}
	};
	template <typename node_tn,typename value_less_tn=less_default>
	struct binnoder_default
	{
		typedef binnoder_default self; ///
		typedef node_tn node_type;
		typedef typename node_type::value_type internal_value_type;
		typedef typename met::iff<met::is_con<node_type>::value,const internal_value_type,internal_value_type>::type value_type;
		typedef value_less_tn value_less_type;
		typedef binnoder_default<node_tn const,value_less_type> const_noder_type; ///
		typedef const_noder_type const_self; ///
		typedef assi_lefter<self> assi_lefter_type;
		typedef assi_righter<self> assi_righter_type;
		typedef assi_parenter<self> assi_parenter_type;
		node_type* _m_node;
		self(node_type* node=0): _m_node(node) {}
		self& operator=(node_type* node){_m_node = node;return *this;}
		operator node_type* () {return _m_node;}
		operator node_type const* () const {return _m_node;}
		bool is_empty() const {return 0==_m_node;}
		bool has_children() const{if (is_empty())return false;return left()||right();}
		const_self lefter() const {return const_self(left());}
		const_self righter() const {return const_self(right());}
		const_self parenter() const {return const_self(parent());}
		self lefter() {return self(left());}
		self righter() {return self(right());}
		self parenter() {return self(parent());}
		assi_lefter_type left_assi() {return assi_lefter_type(*this);}
		assi_righter_type right_assi() {return assi_righter_type(*this);}
		assi_parenter_type parent_assi() {return assi_parenter_type(*this);}
		node_type* left() {return _m_node->_m_left;}
		node_type* parent() {return _m_node->_m_parent;}
		node_type* right() {return _m_node->_m_right;}
		node_type const* left() const {return _m_node->_m_left;}
		node_type const* right() const {return _m_node->_m_right;}
		node_type const* parent() const {return _m_node->_m_parent;}
		value_type& value() {return valueref();}
		value_type const& value() const {return valueref();}
		value_type& operator*() {return *node();}
		node_type const* operator->() const {return _m_node;}
		node_type* operator->() {return _m_node;}
		node_type* node() {return _m_node;}
		node_type const* node() const {return _m_node;}
		bool less(self const& noder) const {return value_less_type()(value(),noder.value());}
		self& set_left(node_type* node) {leftref()=node;return *this;}
		self& set_right(node_type* node) {rightref()=node;return *this;}
		self& set_parent(node_type* node) {parentref()=node;return *this;}
	protected:
		node_type const*& leftref() const {return _m_node->_m_left;}
		node_type const*& rightref() const {return _m_node->_m_right;}
		node_type const*& parentref() const {return _m_node->_m_parent;}
		node_type*& leftref() {return _m_node->_m_left;}
		node_type*& rightref() {return _m_node->_m_right;}
		node_type*& parentref() {return _m_node->_m_parent;}
		value_type& valueref() {return _m_node->_m_value;}
		value_type const& valueref() const {return _m_node->_m_value;}
	};

	template <typename noder_tn>
	static noder_tn get_bin_other_sibling(noder_tn noder)
	{
		if (noder.parenter().left() == noder.node())
			return noder.righter();
		return noder.lefter();
	}

	template <typename node_tn,typename value_less_tn=less_default>
	struct rbnoder_default
		: binnoder_default<node_tn,value_less_tn>
	{
		typedef rbnoder_default self;
		typedef node_tn node_type;
		typedef typename node_type::value_type value_type;
		typedef value_less_tn value_less_type;
		typedef rbnoder_default<node_tn const,value_less_type> const_noder_type;
		typedef const_noder_type const_self;
		typedef binnoder_default<node_tn,value_less_tn> base;

		typedef assi_lefter<self> assi_lefter_type;
		typedef assi_righter<self> assi_righter_type;
		typedef assi_parenter<self> assi_parenter_type;
		typedef assi_colorer<self> assi_colorer_type;

		self(node_type* node=0): base(node) {}

		const_self lefter() const {return const_self(left());}
		const_self righter() const {return const_self(right());}
		const_self parenter() const {return const_self(parent());}
		self lefter() {return self(left());}
		self righter() {return self(right());}
		self parenter() {return self(parent());}

		assi_lefter_type left_assi() {return assi_lefter_type(*this);}
		assi_righter_type right_assi() {return assi_righter_type(*this);}
		assi_parenter_type parent_assi() {return assi_parenter_type(*this);}
		assi_colorer_type color_assi() {return assi_colorer_type(*this);}

		self& set_left(node_type* node) {leftref()=node;return *this;}
		self& set_right(node_type* node) {rightref()=node;return *this;}
		self& set_parent(node_type* node) {parentref()=node;return *this;}
		self& set_color(rbcolor_enum color) {colorref()=color;return *this;}

		rbcolor_enum color() const {return _m_node->_m_color;}
		bool is_red_unsafe() const {return color()==__rb_red;}
		bool is_black_unsafe() const {return color()==__rb_black;}
		bool is_red() const {return _m_node && is_red_unsafe();}
		bool is_black() const {return !_m_node || is_black_unsafe();}
		self& operator=(rbcolor_enum color) {if (!is_empty()) colorref() = color;return*this;}
	protected:
		rbcolor_enum& colorref() {return _m_node->_m_color;}
	};

	template <typename noder_tn,typename value_less_tn>
	bool operator==(binnoder_default<noder_tn,value_less_tn> const& a,noder_tn const* p)
	{
		return a.node() == p;
	}
	template <typename noder_tn,typename value_less_tn>
	bool operator!=(binnoder_default<noder_tn,value_less_tn> const& a,noder_tn const* p)
	{
		return a.node() != p;
	}
	template <typename noder_tn,typename value_less_tn>
	bool operator==(binnoder_default<noder_tn,value_less_tn> const& a,binnoder_default<noder_tn,value_less_tn> const& b)
	{
		return a.node() == b.node();
	}
	template <typename noder_tn,typename value_less_tn>
	bool operator!=(binnoder_default<noder_tn,value_less_tn> const& a,binnoder_default<noder_tn,value_less_tn> const& b)
	{
		return a.node() != b.node();
	}

	template <typename noder_tn>
	static bool is_left(noder_tn noder)
	{
		return noder.parenter().left()==noder.node() ?
			true: false;
	}
	template <typename noder_tn>
	static bool is_right(noder_tn noder)
	{
		return noder.parenter().right()==noder.node() ?
			true: false;
	}
	template <typename node_tn,typename value_less_tn>
	static void swap(binnoder_default<node_tn,value_less_tn> a, binnoder_default<node_tn,value_less_tn> b)
	{
		typedef binnoder_default<node_tn,value_less_tn> noder_type;
		noder_type aper = a.parent();
		noder_type aler = a.left();
		noder_type arer = a.right();

		node_tn* bper = b.parent();
		node_tn* bler = b.left();
		node_tn* brer = b.right();

		a.parent_assi() = bper;
		a.left_assi() = bler;
		a.right_assi() = brer;

		b.parent_assi() = aper;
		b.left_assi() = aler;
		b.right_assi() = arer;

		if (aper && aper.left()==a.node()) aper.left_assi() = b;
		if (aler) aler.parent_assi() = b;
		if (arer) arer->parent_assi() = b;

		if (bper && bper.left()==b.node()) bper.left_assi() = a;
		if (bler) bler.parent_assi() = a;
		if (brer) brer.parent_assi() = a;
	}

	template <typename node_tn,typename value_cmp_tn=compare_default,template<typename,typename>class bxtree_noder_tn=bintree_empty_class>
	struct btree_noder_default_tt
	{
		static size_t const __rank = node_tn::__rank;
		typedef btree_noder_default_tt self; ///
		typedef self btree_noder_default;
		typedef btree_noder_default_tt<node_tn const,value_cmp_tn,bxtree_noder_tn> const_self; ///
		typedef node_tn node_type;
		typedef typename node_type::internal_value_type internal_value_type;
		typedef typename node_type::value_type raw_value_type;
		typedef typename met::iff<met::is_con<node_type>::value,const raw_value_type,raw_value_type>::type value_type;
		typedef value_cmp_tn value_cmp_type;

		struct rebind_me_const_self {typedef const_self type;};
		struct rebind_others_const_self {typedef bxtree_noder_tn<node_tn const,value_cmp_tn> type;};
		struct rebind_me_self {typedef self type;};
		struct rebind_others_self {typedef bxtree_noder_tn<node_tn,value_cmp_tn> type;};
		typedef typename met::iff<met::is_same<bxtree_noder_tn<void,void>,bintree_empty_class<void,void>>::value,rebind_me_const_self,rebind_others_const_self>::type::type const_noder_type;
		typedef typename met::iff<met::is_same<bxtree_noder_tn<void,void>,bintree_empty_class<void,void>>::value,rebind_me_self,rebind_others_self>::type::type noder_type;

		typedef assi_dchilder<noder_type> assi_dchilder_type;
		typedef assi_parenter<noder_type> assi_parenter_type;
		typedef assi_dvaluer<noder_type> assi_valuer_type;

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
		assi_dchilder_type child_assi(size_t index) {return assi_dchilder_type(*static_cast<noder_type*>(this),index);}
		assi_valuer_type value_assi(size_t index) {return assi_valuer_type(*static_cast<noder_type*>(this),index);}
		noder_type& set_child(node_type* node,size_t index) {childref(index)=node;return *static_cast<noder_type*>(this);}
		noder_type& set_value(value_type const& value,size_t index) {valueref(index)=value;return *static_cast<noder_type*>(this);}

		const_noder_type childer(size_t index) const {return const_noder_type(child(index));}
		const_noder_type front_childer() const {return child(0);}
		const_noder_type back_childer() const {return child(value_size());}
		noder_type childer(size_t index) {return noder_type(child(index));}
		noder_type front_childer() {return child(0);}
		noder_type back_childer() {return child(value_size());}

		/// get : size,child(i),value(i),parent
		size_t value_size() const {return countref();}

		node_type* child(size_t index) {return childref(index);}
		value_type& value(size_t index) {return valueref(index);}
		node_type const* child(size_t index) const {return childref(index);}
		value_type const& value(size_t index) const {return valueref(index);}
		value_type const& front_value() const {return value(0);}
		value_type const& back_value() const {return value(value_size()-1);}
		value_type& front_value() {return value(0);}
		value_type& back_value() {return value(value_size()-1);}
		node_type* front_child() {return child(0);}
		node_type* back_child() {return child(value_size());}

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
			_m_node->_m_prev = child(1);
			_m_node->_m_data.remove_front();
		}
		void remove_back()
		{
			_m_node->_m_data.remove_back();
		}
		void push_front(value_type const& value,node_type* pnode)
		{
			_m_node->_m_data.push_front(value,front_child());
			child_assi(0) = pnode;
		}
		void push_back(value_type const& value,node_type* pnode)
		{
			_m_node->_m_data.push_back(value,pnode);
		}
		void append(self& noder)
		{
			_m_node->_m_data.back().second = noder.child(0);
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
			newer.child_assi(0)=child(prevoff);
		}
		value_type* split_out(size_t off,self& newer)
		{
			assert(off>=0 && off<value_size());
			split(off+1,newer);
			pair<value_type&,node_type*> last = internal_remove_back();
			return &last.first;
		}
		static void swap(self a,size_t aoff,self b,size_t boff)
		{
			value_type t = a.value(aoff);
			a.value_assi(aoff) = b.value(boff);
			b.value_assi(boff) = t;
		}
	protected:
		node_type const*& childref(size_t index) const {
			node_type* & ret = index==0?_m_node->_m_prev:_m_node->_m_data.at(index-1)->second;
			return const_cast<node_type const*&>(ret);
		}
		node_type*& childref(size_t index) {return index==0?_m_node->_m_prev:_m_node->_m_data.at(index-1)->second;}
		value_type& valueref(size_t index) {return _m_node->_m_data.at(index)->first;}
		value_type const& valueref(size_t index) const {return _m_node->_m_data.at(index)->first;}
		size_t const countref() const {return _m_node->_m_data.size();}
		size_t countref() {return _m_node->_m_data.size();}
	};
	template <typename node_tn,typename value_cmp_tn=compare_default>
	struct bptree_noder_default;
	template <typename node_tn,typename value_cmp_tn>
	struct bptree_noder_default
		: btree_noder_default_tt<node_tn,value_cmp_tn,bptree_noder_default>
	{
		typedef bptree_noder_default self; ///
		typedef btree_noder_default base;
		self(node_type* node=0): base(node) {}
		self& operator=(node_type* node){_m_node = node;return *this;}
		
		value_type* split_leaf_out(size_t off,self& newer)
		{
			assert(off>=0 && off<value_size());
			split(off,newer);
			return &newer.front_value();
		}
		value_type* split_middle_out(size_t off,self& newer)
		{
			assert(off>=0 && off<value_size());
			split(off+1,newer);
			pair<value_type&,node_type*> last = internal_remove_back();
			return &last.first;
		}
		value_type* split_out(size_t off,self& newer)
		{
			if (front_childer().is_empty())
				return split_leaf_out(off,newer);
			return split_middle_out(off,newer);
		}
	};

template <typename value_tn,typename derived_tn=void>
struct binnode_tt
{
	typedef typename met::iff<met::is_same<derived_tn,void>::value,binnode_tt,derived_tn>::type node_type;
	typedef value_tn value_type;

	binnode_tt()
		: _m_left(0),_m_right(0),_m_parent(0)
	{}
	binnode_tt(value_type const& value)
		: _m_left(0),_m_right(0),_m_parent(0)
		, _m_value(value)
	{}
	node_type* _m_left;
	node_type* _m_right;
	node_type* _m_parent;
	value_type _m_value;
};
template <typename value_tn>
struct rbnode_tt
	: binnode_tt<value_tn,rbnode_tt<value_tn>>
{
	typedef value_tn value_type;
	typedef binnode_tt<value_tn,rbnode_tt<value_tn>> base;

	rbnode_tt()
		: _m_color(__rb_red)
	{}
	rbnode_tt(value_type const& value)
		: base(value)
		, _m_color(__rb_red)
	{}
	rbnode_tt(rbcolor_enum color,value_type const& value)
		: base(value)
		, _m_color(color)
	{}
	rbcolor_enum _m_color;
};
template <typename value_tn,size_t rank_tc>
struct bnode_tt
{
	typedef value_tn value_type;
	static size_t const __rank = rank_tc;
	static_assert(__rank>=4, "rank should be >= 4");
	typedef typename static_keyvalue_sorted_podliner_off_tt<value_type,bnode_tt*,__rank-1,size_t>::type static_data_type;
	typedef typename static_data_type::value_type internal_value_type;
	bnode_tt():_m_prev(0) {}
	bnode_tt* _m_prev;
	static_data_type _m_data;
};


/// declare of rbtree_ts
template <typename noder_tn,typename allocator_tn=cppmalloc>
struct rbtree_ts;

template <typename noder_tn,typename allocator_tn=cppmalloc>
struct bintree_ts
	: private allocator_tn
	, make_helper<allocator_tn>
{
	friend rbtree_ts<noder_tn,allocator_tn>;

	typedef noder_tn noder_type;
	typedef typename noder_type::node_type node_type;
	typedef typename noder_type::value_type value_type;
	typedef typename noder_type::value_less_type value_less_type;

	bintree_ts()
		: _m_root(0)
		, make_helper<allocator_type>(this)
	{}

	node_type* insert(value_type const& value)
	{
		if (_m_root)
			return insert(_m_root,value);
		return _m_root = make<node_type>(value);
	}

	node_type* insert(node_type* node, value_type const& value)
	{
		assert(node);
		if (value_less_type()(value,node->_m_value))
		{
			if (!node->_m_left)
			{
				node->_m_left = make<node_type>(value);
				if (!node->_m_left)
				{
					assert(false);
					return 0;
				}
				node->_m_left->_m_parent = node;
				return node->_m_left;
			}
			return insert(node->_m_left,value);
		}
		else
		{
			if (!node->_m_right)
			{
				node->_m_right = make<node_type>(value);
				if (!node->_m_right)
				{
					assert(false);
					return 0;
				}
				node->_m_right->_m_parent = node;
				return node->_m_right;
			}
			return insert(node->_m_right,value);
		}
	}

	node_type* find(value_type const& value)
	{
		return find(_m_root,value);
	}

	node_type* find(node_type* node,value_type const& value)
	{
		if (!node)
			return 0;
		if (value_less_type()(value,node->_m_value))
			return find(node->_m_left,value);
		if (!value_less_type()(node->_m_value,value))
			return node;
		return find(node->_m_right,value);
	}

	/// get prev node
	node_type* remove2(node_type const* node)
	{
		if (!node)
			return 0;
		if (node->_m_left)
		{
			if (node->_m_left->_m_right)
			{
				node_type* a = get_rightest(node->_m_left);
				/// adjust the rightest node
				node_type* parent = a->_m_parent;
				parent->_m_right = a->_m_left;
				a->_m_left->_m_parent = parent;
				/// adjust the victim node
				a->_m_parent = node->_m_parent;
				a->_m_right = node->_m_right;
				node->_m_left->_m_parent = a;
				node->_m_right->_m_parent = a;
				if (a->_m_parent->_m_left==node)
					a->_m_parent->_m_left = a;
				else
					a->_m_parent->_m_right = a;
			}
			else
			{
				node_type* a = node->_m_left;

				a->_m_parent = node->_m_parent;
				a->_m_right = node->_m_right;
				node->_m_right->_m_parent = a;

				/// the same as next
				if (a->_m_parent->_m_left==node)
					a->_m_parent->_m_left = a;
				else
					a->_m_parent->_m_right = a;
			}
		}
		else if (node->_m_parent)
		{
			if (node->_m_parent->_m_left==node)
				node->_m_parent->_m_left = node->_m_right;
			else
				node->_m_parent->_m_right = node->_m_right;
			if (node->_m_right)
				node->_m_right->_m_parent = node->_m_parent;
		}
	}

	node_type* remove(value_type const& value)
	{
		node_type* finded = find(value);
		if (!finded)
			return 0;
		return remove(finded);
	}
	/// removed: the real removed node, for rb fix balance
	node_type* remove(node_type const* node,node_type** removed=0)
	{
		noder_type victimer = (node_type*)node;
		noder_type reter = internal_remove(node,removed);
		if (!victimer.parent()) //&&)!reter || !reter.parent())
			_m_root = reter.node();
		return reter.node();
	}
	/// get next node, independent
	/// return the next node, == the new subroot
	node_type* internal_remove(node_type const* node,node_type** removed=0)
	{
		if (!node) return 0;

		noder_type noder = (node_type*)node;
		node_type* next_node_returned = 0;
		bool is_not_same_node = false;

		/// if has both children
		noder_type nexter = (node_type*)node;
		if (noder.left() && noder.right())
		{
			nexter = get_leftest(noder.right());
			is_not_same_node = true;
		}
		/// get the return node
		if (is_not_same_node)
			next_node_returned = nexter.node();
		else
			noder.left() ? next_node_returned=noder.left() : next_node_returned=noder.right();

		noder_type parenter = nexter.parent();
		bool isleft = parenter && parenter.left()==nexter;

		if (nexter.left())
		{
			if (parenter) isleft? parenter.left_assi()=nexter.left() : parenter.right_assi()=nexter.left();
			nexter.lefter().parent_assi()=parenter;
		}
		else //if (nexter.right())
		{
			if (parenter) isleft? parenter.left_assi()=nexter.right() : parenter.right_assi()=nexter.right();
			if (nexter.right()) nexter.righter().parent_assi()=parenter;
		}

		/// replace the victim with the nexter if they are not the same
		/// we swap the link, so we do not need to exchange the value
		if (is_not_same_node/* && nexter.parent()!=noder*/)
		{
			node_type* nexter_parent;
			node_type* nexter_left;
			node_type* nexter_right;
			if (removed)
			{
				nexter_parent = nexter.parent();
				nexter_left = nexter.left();
				nexter_right = nexter.right();
			}

			nexter.parent_assi() = noder.parent();
			nexter.left_assi() = noder.left();
			nexter.right_assi() = noder.right();

			if (noder.right()) noder.righter().parent_assi() = nexter;
			if (noder.left()) noder.lefter().parent_assi() = nexter;

			noder_type parenter = noder.parenter();
			if (parenter) parenter.right()==noder ? parenter.right_assi()=nexter : parenter.left_assi()=nexter;

			if (removed)
			{
				noder.parent_assi() = nexter_parent;
				noder.left_assi() = nexter_left;
				noder.right_assi() = nexter_right;
			}
		}

		if (removed)
			*removed = noder.node();

		return next_node_returned;
	}

	static node_type* get_prev(node_type* node)
	{
		if (!node)
			return 0;
		if (node->_m_left)
			return get_rightest(node->_m_left);
		return 0;
	}
	static node_type* get_next(node_type* node)
	{
		if (!node)
			return 0;
		if (node->_m_right)
			return get_leftest(node->_m_left);
		return 0;
	}

	static node_type* get_rightest(node_type* node)
	{
		if (!node)
			return 0;
		if (!node->_m_right)
			return node;
		return get_rightest(node->_m_right);
	}

	static node_type* get_leftest(node_type* node)
	{
		if (!node)
			return 0;
		if (!node->_m_left)
			return node;
		return get_leftest(node->_m_left);
	}

	/// n-rightest, the first non-zero node from the rightest node
	static node_type const* get_nrightest(node_type const* node)
	{
		if (!node) return 0;
		while (true)
		{
			if (node->_m_right)
				node = node->_m_right;
			else if (node->_m_left)
				node = node->_m_left;
			else
				return node;
		}
		assert(false && "never should be here");
		return 0;
	}
	static node_type* get_nrightest(node_type* node)
	{
		return const_cast<node_type*>(get_nrightest(const_cast<node_type const*>(node)));
	}
	static node_type const* get_nleftest(node_type const* node)
	{
		if (!node) return 0;
		while (true)
		{
			if (node->_m_left)
				node = node->_m_left;
			else if (node->_m_right)
				node = node->_m_right;
			else
				return node;
		}
		assert(false && "never should be here");
		return 0;
	}
	static node_type* get_nleftest(node_type* node)
	{
		return const_cast<node_type*>(get_nleftest(const_cast<node_type const*>(node)));
	}

	static node_type const* get_nearest_nuncle(node_type const* node,node_type const** top=0)
	{
		assert(node);
		node_type const* p = node;
		while(p->_m_parent)
		{
			if (p->_m_parent->_m_right == p || p->_m_parent->_m_right==0)
			{
				p = p->_m_parent;
				continue;
			}
			p = p->_m_parent->_m_right;
			return p;
		}
		if (top) *top = p;
		return 0;
	}
	static node_type* get_nearest_nuncle(node_type* node,node_type** top=0)
	{
		return const_cast<node_type*>(get_nearest_nuncle(const_cast<node_type const*>(node),top));
	}

	static node_type* get_end(node_type* node)
	{
		return (node_type*)((char*)(node)+sizeof(char*));
	}
	static node_type const* get_end(node_type const* node)
	{
		return (node_type*)((char*)(node)+sizeof(char*));
	}
	/// pre iterate
	struct const_iterator
	{
		const_iterator& operator++()
		{
			assert(_m_p);
			if (_m_p->_m_left)
				_m_p = _m_p->_m_left;
			else if (_m_p->_m_right)
				_m_p = _m_p->_m_right;
			else
			{
				node_type const* p = get_nearest_nuncle(_m_p,&_m_p);
				/// done, get end
				_m_p = p?p:get_end(_m_p);
			}
			return *this;
		}
		const_iterator& operator--()
		{
			assert(_m_p);

			/// if is the end node
			if (_m_p==get_end(_m_root))
				_m_p = get_nrightest(_m_root);

			/// if has no parent
			else if (!_m_p->_m_parent) /// done
				_m_p = get_end(_m_p);

			else /// has parent
				if (_m_p->_m_parent->_m_left==_m_p || _m_p->_m_parent->_m_left==0)
				{
					_m_p = _m_p->_m_parent; /// get it
				}
				else if (true) /// has prev sibling
				{
					_m_p = get_nrightest(_m_p->_m_parent->_m_left);
				}

			return *this;
		}
		value_type const& operator*() const
		{
			return _m_p->_m_value;
		}
		bool operator==(const_iterator const& o)const
		{
			return _m_p==o._m_p;
		}
		bool operator!=(const_iterator const& o)const
		{
			return !operator==(o);
		}
		const_iterator(node_type * root,node_type const* p)
			: _m_root(root), _m_p(p)
		{}
		node_type const* _m_p;
		node_type const* _m_root;
	};

	const_iterator begin() const
	{
		return const_iterator(_m_root,_m_root);
	}
	const_iterator end() const
	{
		return const_iterator(_m_root,get_end(_m_root));
	}

	node_type* root() {return _m_root;}
	node_type const* root() const {return _m_root;}
protected:
	node_type* _m_root;
};

template <typename value_tn,typename less_tn=less_default,typename allocator_tn=cppmalloc>
struct bintree_tt
{
	typedef less_tn less_type;
	typedef binnoder_default<binnode_tt<value_tn>,less_type> noder_type;
	typedef bintree_ts<noder_type,allocator_tn> type;
};

}


namespace ox
{


template <template <typename,typename>class noder_tn=noder_default>
struct get_depth
{
	template <typename node_tn>
	size_t operator()(node_tn* node) const
	{
		size_t depth = 0;
		noder_tn<node_tn> noder(node);
		while(noder.parent())
			noder = noder.parenter(),
			++depth;
		return depth;
	}
};



template <typename noder_tn,typename action_tn>
static bool walk_pre_bin(noder_tn noder,action_tn& action,noder_tn parenter,char role='R',size_t depth=0)
{
	if (!noder.is_empty())
		assert(noder.parenter()==parenter);
	if (!action(noder.node(),parenter.node(),role,depth))
		return false;
	if (!noder.has_children())
		return true;
	if (!walk_pre_bin(noder.lefter(),action,noder,'L',depth+1))
		return false;
	if (!walk_pre_bin(noder.righter(),action,noder,'R',depth+1))
		return false;
	return true;
};

template <typename noder_tn,typename path_tn,typename action_tn>
static bool walk_pre_b(noder_tn noder,path_tn& path,action_tn& action)
{
	//if (!noder.is_empty())
	//	assert(noder.parent()?noder.parent()==path.top().first:path.is_empty());
	if (noder.is_empty())
		return false;
	path.push(noder,0);
	if (noder.child(0))
	{
		action(path,'C');
		walk_pre_b(noder.childer(0),path,action);
	}
	size_t const value_size = noder.value_size();
	for (size_t i=0;i<value_size;++i)
	{
		path.top().second = i;
		action(path,'V');
		if (noder.child(i+1))
		{
			path.top().second = i+1;
			action(path,'C');
			walk_pre_b(noder.childer(i+1),path,action);
		}
	}
	path.pop();
	return true;
};

template <typename node_tn>
struct noder_get_sibling_default
{
	typedef node_tn node_type;
	node_type const* _m_parent;
	noder_get_sibling_default(node_type const* parent=0)
		: _m_parent(parent)
	{}
	node_type const* next_sibling(node_type const* node) const
	{
		if (!node || !_m_parent)
			return 0;
		assert(node?node==node->_m_parent?true);
		if (0==_m_parent->_m_right || _m_parent->_m_right==node)
			return 0; 
		return _m_parent->_m_right;
	}
};

/*
parent
*/
template <typename stringify_value_tn>
struct stringify_binnode
{
	typedef stringify_value_tn stringify_value;
	template <typename binnode_tn>
	string operator()(binnode_tn* node) const
	{
		typedef binnoder_default<binnode_tn> noder_type;
		noder_type noder = node;
		string str;
		str += ": ";
		str += stringify_value()(noder.value());
		return str;
	}
};
template <typename stringify_value_tn>
struct stringify_rbnode
{
	typedef stringify_value_tn stringify_value;
	template <typename rbnode_tn>
	string operator()(rbnode_tn* node) const
	{
		typedef rbnoder_default<rbnode_tn> noder_type;
		noder_type noder = node;
		string str;
		str += noder.is_red() ? "(r)":"(b)";
		str += ": ";
		str += stringify_value()(noder.value());
		return str;
	}
};
template <typename stringify_value_tn>
struct stringify_bnode
{
	typedef stringify_value_tn stringify_value;
	template <typename bnode_tn>
	string operator()(bnode_tn* node,size_t off,char role) const
	{
		typedef btree_noder_default_tt<bnode_tn> noder_type;
		noder_type noder = node;
		string str;
		//str += '(';
		//str += role;
		//str += ')';
		if (role=='V') str.append(" ") += stringify_value()(noder.value(off));
		else if (role=='C') str.append("--.");
		return str;
	}
};
template <typename noder_tn,typename stringify_node_tn>
struct print_pre_bin_ts
{
	typedef typename noder_tn::node_type node_type;
	typedef typename noder_tn::const_noder_type const_noder_type;
	typedef stringify_node_tn stringify_node;
	static bool islast(node_type const* node)
	{
		assert(node);
		const_noder_type& parenter = const_noder_type(node).parenter();
		return parenter.node()?parenter.right()==node:true;
	}
	static void get_pretext(node_type const* node,char role,string& str)
	{
		char c = role=='R'?'`':'+';
		str.append(1,c).append("--").append(1,role);
		str += stringify_node()(node);
		node_type const* p = const_noder_type(node).parent();
		if (!p) return;
		get_pretext_rest(p,str);
	}
	static void get_pretext_rest(node_type const* node,string& str)
	{
		if (islast(node))
			str.insert(0,"      ");
		else
			str.insert(0,"|     ");
		node_type const* p = const_noder_type(node).parent();
		if (p) get_pretext_rest(p,str);
	}
	static void get_null_pretext_rest(node_type const* parent,char role,string& str)
	{
		char c = role=='R'?'`':'+';
		str.append(1,c).append("--").append(1,role);
		str += ": null";
		node_type const* p = parent;
		if (!p) return;
		get_pretext_rest(p,str);
	}
	struct action_print
	{
		bool operator()(node_type const* node,node_type const* parent,char role,size_t depth) const
		{
			string str;
			if (node)
			{
				get_pretext(node,role,str);
				printf("%s\n",str.c_str());
			}
			else 
			{
				get_null_pretext_rest(parent,role,str);
				printf("%s\n",str.c_str());
			}
			return true;

			//if (depth)
			//{
			//	print_tn()(depth*4-1,'-');
			//	print_tn()(' ');
			//}
			//if (noder_default<node_tn>(node).is_empty())
			//	print_tn()("<null>");
			//else
			//	print_tn()(noder_default<node_tn>(node).value());
			//print_tn()('\n');
			//return true;
		}
	};
	bool operator()(noder_tn noder)
	{
		return noder ? walk_pre_bin(noder,action_print(),noder.parenter()) : false;
	}
};

struct stringify_value_basic
{
	template <typename value_tn>
	string operator()(value_tn const& value)
	{
		char buffer[10];
		sprintf(buffer,"%d",value);
		return buffer;
	}
};

template <typename stringify_value_tn=stringify_value_basic>
struct print_pre_binnode
{
	typedef stringify_binnode<stringify_value_tn> stringify_node;

	template <typename node_tn>
	bool operator()(node_tn* node)
	{
		typedef binnoder_default<node_tn> noder_type;
		print_pre_bin_ts<noder_type,stringify_node> printer;
		return printer(noder_type(node));
	}
};
template <typename stringify_value_tn=stringify_value_basic>
struct print_pre_rbnode
{
	typedef stringify_rbnode<stringify_value_tn> stringify_node;

	template <typename node_tn>
	bool operator()(node_tn* node)
	{
		typedef rbnoder_default<node_tn> noder_type;
		print_pre_bin_ts<noder_type,stringify_node> printer;
		return printer(noder_type(node));
	}
};
template <typename stringify_value_tn=stringify_value_basic>
struct print_pre_bnode
{
	typedef stringify_bnode<stringify_value_tn> stringify_node;

	template <typename node_tn>
	bool operator()(node_tn* node)
	{
		typedef btree_noder_default_tt<node_tn> noder_type;
		print_pre_b_ts<noder_type,stringify_node> printer;
		return printer(noder_type(node));
	}
};

template <typename noder_tn,typename stringify_node_tn>
struct print_pre_b_ts
{
	typedef typename stack_tt<pair<noder_tn,size_t>>::type path_type;
	typedef typename path_type::value_type path_value_type;

	typedef noder_tn noder_type;
	typedef typename noder_tn::node_type node_type;
	typedef typename noder_tn::const_noder_type const_noder_type;
	typedef stringify_node_tn stringify_node;
	static bool islast(path_value_type const& parenter)
	{
		bool is_leaf = parenter.first.front_childer().is_empty();
		size_t size = parenter.first.value_size();
		size = is_leaf ? size : size+1;
		size_t off = parenter.second;
		return off+1<size?false:true;
	}
	static bool isfirst(path_value_type const& parenter,char role)
	{
		bool is_leaf = parenter.first.front_childer().is_empty();
		size_t size = parenter.first.value_size();
		size = is_leaf ? size : size+1;
		size_t off = parenter.second;
		return off==0 && is_leaf || off==0&&!is_leaf&&role=='C';
	}
	static void get_pretext(path_type& path,char role,string& str)
	{
		noder_type& noder = path.top().first;
		size_t off = path.top().second;
		str += islast(path.top())||isfirst(path.top(),role) ? '`' : (role=='C'?'+':'|');
		str.append("--");
		str += stringify_node()(noder.node(),off,role);
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
		bool operator()(path_type& path,char role) const
		{
			string str;
			if (!path.top().first.is_empty())
			{
				get_pretext(path,role,str);
				printf("%s\n",str.c_str());
			}
			return true;
		}
	};
	bool operator()(noder_tn noder)
	{
		path_type path;
		return noder ? walk_pre_b(noder,path,action_print()) : false;
	}
};

/// the new root is the noder that will be rotated
template <typename noder_tn>
static noder_tn& noderotate_left(noder_tn& child)
{
	assert(!child.is_empty() && !child.parenter().is_empty());

	noder_tn p = child.parenter();
	noder_tn pp = p.parenter();
	noder_tn l = child.lefter();

	if (!pp.is_empty())
		if (pp.left()==p.node())
			pp.left_assi() = child;
		else
			pp.right_assi() = child;
	child.parent_assi() = pp;

	child.left_assi() = p;
	p.parent_assi() = child;

	p.right_assi() = l;
	if (!l.is_empty())
		l.parent_assi() = p;

	return child;
}

/// the new root is the noder that will be rotated
template <typename noder_tn>
static noder_tn& noderotate_right(noder_tn& child)
{
	assert(!child.is_empty() && !child.parenter().is_empty());

	noder_tn p = child.parenter();
	noder_tn pp = p.parenter();
	noder_tn r = child.righter();

	if (!pp.is_empty())
		if (pp.left()==p.node())
			pp.left_assi() = child;
		else
			pp.right_assi() = child;
	child.parent_assi() = pp;

	child.right_assi() = p;
	p.parent_assi() = child;

	p.left_assi() = r;
	if (!r.is_empty())
		r.parent_assi() = p;

	return child;
}

template <typename noder_tn,typename allocator_tn/*=cppmalloc*/>
struct rbtree_ts
{
	typedef bintree_ts<noder_tn,allocator_tn> bintree_type;

	typedef noder_tn noder_type;
	typedef typename noder_type::node_type node_type;
	typedef typename noder_type::value_type value_type;
	typedef typename noder_type::value_less_type value_less_type;

	rbtree_ts()
	{}

	node_type* root() {return _m_bintree.root();}
	node_type const* root() const {return _m_bintree.root();}

	struct action_null {void operator()(){}};
	node_type* insert(value_type const& value)
	{
		return insert(value,action_null());
	}
	template <typename trace_tn>
	node_type* insert(value_type const& value,trace_tn trace)
	{
		noder_type added =  _m_bintree.insert(value);
		if (!added) return 0;
			
		noder_type rednode = added;

		while (rednode.parent() && rednode.parenter().is_red())
		{
			trace();
			noder_type p = rednode.parenter();
			noder_type pp = p.parenter();
			noder_type ap;
			if (!pp.is_empty())
				ap = pp.left()==p.node() ? pp.righter() : pp.lefter();
			
			/// the uncle is red, recolor and continue
			/// and the root will not be changed
			if (!ap.is_empty() && ap.color()==__rb_red)
			{
				if (!pp.is_empty())
				{
					pp.lefter() = __rb_black;
					pp.righter() = __rb_black;
					pp = __rb_red;
				}
				else
				{
					p = __rb_black; /// we meet root
				}
				rednode = pp;
				continue;
			}
			/// if uncle is black, rotate and finish, the root maybe changed
			if (is_right(rednode))
			{
				if (is_left(rednode.parenter()))
				{
					noderotate_left(rednode);
					noderotate_right(rednode);
					rednode = __rb_black;
					rednode.righter() = __rb_red;
					/// rednode keeps
				}
				else /// right right
				{
					noderotate_left(rednode.parenter());
					p = __rb_black;
					pp = __rb_red;
					/// return the top subroot
					rednode = rednode.parenter();
				}
			}
			else //if (is_left(added))
			{
				if (is_right(rednode.parenter()))
				{
					noderotate_right(rednode);
					noderotate_left(rednode);
					rednode = __rb_black;
					rednode.lefter() = __rb_red;
				}
				else /// left left
				{
					noderotate_right(rednode.parenter());
					p = __rb_black;
					pp = __rb_red;
					/// return the top subroot
					rednode = rednode.parenter();
				}
			}
			/// the rednode must not be null
			break;
		}
		trace();
		/// only the second will change the root
		if (!rednode.parent())
		{
			_m_bintree._m_root = rednode.node();
			/*if (rednode.is_red()) */rednode = __rb_black;
		}
		trace();
		return added.node();
	}

	node_type* find(value_type const& value)
	{
		return find(_m_bintree.root(),value);
	}
	node_type* find(node_type* node,value_type const& value)
	{
		return _m_bintree.find(node,value);
	}
	node_type* remove(value_type const& value)
	{
		node_type* victim = find(value);
		if (!victim) return 0;

		noder_type subrooter = internal_remove(victim);
		if (!subrooter || !subrooter.parent())
			_m_bintree._m_root = subrooter.node();
		return subrooter.node();
	}
	node_type* internal_remove(node_type* victim)
	{
		assert (victim);
		noder_type victimer = victim;
		bool has_both_children = victimer.left() && victimer.right();

		node_type* removed = 0;
		noder_type subrooter = _m_bintree.remove(victim,&removed);
		assert(removed==victim);

		/// make up for the rb node special part of color
		if (has_both_children)
		{
			/// swap the color
			rbcolor_enum c = subrooter.color();
			subrooter = victimer.color();
			victimer = c;
		}
		/// do we need fix up the colors
		noder_type removeder = removed;
		if (removeder.is_red_unsafe())
		{
			return subrooter.node();
		}

		/// yes, we need to fix up the colors

		/// because removeder and fixeder are overlapped,
		/// the difference is the removeder is not referred by the rb-tree
		noder_type fixeder = removeder;
		noder_type fixeder_parent = fixeder.parent();
		noder_type redicted;
		if (removeder.left())
			fixeder = removeder.left();
		else if (removeder.right())
			fixeder = removeder.right();
		else if (!fixeder_parent) /// when the tree just has root, and has been removed
			return 0;
		/// if has no children we construct one first, and then exit at the end of the loop
		/// or we must add many special judgment for each condition
		else if (redicted=fixeder,0==fixeder_parent.left())
			fixeder_parent.left_assi() = fixeder;
		else
			fixeder_parent.right_assi() = fixeder;

		while (fixeder.is_black_unsafe())
		{
			if (fixeder.node()==fixeder_parent.left()) /// fixeder is left node
			{
				noder_type sibling = fixeder_parent.righter();
				if (sibling.is_red_unsafe())
				{
					noderotate_left(sibling);
					sibling = __rb_black;
					sibling.lefter() = __rb_red;
					sibling = sibling.parenter().right();
				}
				if (sibling.is_black())
				{
					if (sibling.lefter().is_black() && sibling.righter().is_black())
					{
						sibling = __rb_red;
						fixeder = fixeder_parent;
						fixeder_parent = fixeder_parent.parenter();
						if (!fixeder_parent) /// fixeder is the root of the tree
						{
							subrooter = fixeder;
							break;
						}
						continue; /// continue, move upward one step
					}
					else if(sibling.lefter().is_red())
					{
						noderotate_right(sibling.lefter());
						sibling = __rb_red;
						sibling.parenter() = __rb_black;
						sibling = sibling.parenter();
					}
					if (sibling.righter().is_red())
					{
						noderotate_left(sibling);
						sibling = fixeder_parent.color();
						sibling.lefter() = __rb_black;
						sibling.righter() = __rb_black;
						subrooter = sibling; /// subrooter is changed
						break;
					}
				}
			} /// end of fixeder is left
			else
			{
				noder_type sibling = fixeder_parent.lefter();
				if (sibling.is_red_unsafe())
				{
					noderotate_right(sibling);
					sibling = __rb_black;
					fixeder.parenter() = __rb_red;
					sibling = fixeder.parenter().left();
				}
				if (sibling.is_black_unsafe())
				{
					if (sibling.lefter().is_black() && sibling.righter().is_black())
					{
						sibling = __rb_red;
						fixeder = fixeder_parent;
						fixeder_parent = fixeder_parent.parenter();
						if (!fixeder_parent) /// fixeder is the root of the tree
						{
							subrooter = fixeder;
							break;
						}
						continue; /// continue, move upward one step
					}
					else if(sibling.righter().is_red())
					{
						noderotate_left(sibling.righter());
						sibling = __rb_red;
						sibling.parenter() = __rb_black;
						sibling = sibling.parenter();
					}
					if (sibling.lefter().is_red())
					{
						noderotate_right(sibling);
						sibling = fixeder_parent.color();
						sibling.lefter() = __rb_black;
						sibling.righter() = __rb_black;
						subrooter = sibling; /// subrooter is changed
						break;
					}
				}
			} /// end of /// fixeder is right
		}
		fixeder = __rb_black;
		///
		if (redicted)
			if (redicted.parenter().left()==redicted.node())
				redicted.parenter().left_assi() = (node_type*)0;
			else
				redicted.parenter().right_assi() = (node_type*)0;
		//// finished
		return subrooter.node();
	}
private:
	bintree_type _m_bintree;
};


template <typename value_tn,typename less_tn=less_default,typename allocator_tn=cppmalloc>
struct rbtree_tt
{
	typedef less_tn less_type;
	typedef rbnoder_default<rbnode_tt<value_tn>,less_type> noder_type;
	typedef rbtree_ts<noder_type,allocator_tn> type;
};



template <typename noder_tn,typename allocator_tn=cppmalloc>
struct btree_rooter
	: common_data_tt<typename noder_tn::node_type*,allocator_tn>
{
	typedef noder_tn noder_type;
	typedef typename noder_type::node_type node_type;
	btree_rooter() {_m_data = 0;}
	void set_root(node_type* node) {_m_data=node;}
	noder_type rooter() {return _m_data;}
	bool is_empty() const {return _m_data==0;}
};

template <typename rooter_tn>
struct btree_ts
{
	typedef btree_ts self;
	typedef rooter_tn rooter_type;
	typedef typename rooter_type::noder_type noder_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename noder_type::value_type value_type;
	typedef typename noder_type::value_cmp_type value_cmp_type;
	typedef typename noder_type::assi_valuer_type valuer_type;
	typedef pair<noder_type,size_t> location_type;
	typedef typename stack_tt<location_type>::type path_type;

	rooter_type _m_rooter;

	//self() {}

	node_type* root() {return _m_rooter.rooter().node();}
	node_type const* root() const {return _m_rooter.rooter().node();}

	location_type find(value_type const& value)
	{
		if (_m_rooter.rooter().is_empty())
			return location_type(_m_rooter.rooter(),-1);
		return find(_m_rooter.rooter(),value);
	}
	bool find(path_type& path,value_type const& value)
	{
		return find(path,_m_rooter.rooter(),value);
	}
	bool find(path_type& path,noder_type noder,value_type const& value)
	{
		if (noder.front_childer().is_empty())
			return false;
		int r = value_cmp_type()(value,noder.front_value());
		if (r<0) return find(path.push(noder,0),noder.front_childer(),value);
		r = value_cmp_type()(value,noder.back_value());
		if (r>0) return find(path.push(noder,noder.value_size()),noder.back_childer(),value);
		pair<size_t,bool> off = noder.try_find(value);
		path.push(noder,off.first);
		if (off.second) return true;
		return find(path,noder.childer(off.first-1),value);
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
		if (/*parent.is_empty() || */parent.is_full())
			return noder_type();
		noder_type new_noder = _m_rooter.make<node_type>();
		if (new_noder.is_empty())
			return noder_type();
		value_type const* new_value = child.split_out(off,new_noder);
		if (!new_value) /// will never happen
			return noder_type();
		parent.unsafe_insert_at(parent_off,*new_value,new_noder);
		//if (is_parent_empty)
		//	parent.child_assi(0) = child;
		return new_noder;
	}
	static size_t find_inserted(noder_type noder,value_type const& value)
	{
		/// if at the left side
		int r = value_cmp_type()(value,noder.front_value());
		if (r<0) return 0;
		/// if at the right side
		r = value_cmp_type()(value,noder.back_value());
		if (r>0) return noder.value_size();
		/// ok, it is inside the current node
		pair<size_t,bool> off = noder.try_find(value);
		//if (!off.second) --off.first;
		return off.first;
	}
	template <typename spliter_tn>
	struct internal_inserter_tt
	{
		internal_inserter_tt(spliter_tn& _spliter) : spliter(_spliter) {}
		spliter_tn& spliter;
		location_type newer;
		location_type toper;

		internal_inserter_tt operator()(noder_type parenter,size_t poff,noder_type noder,value_type const& value)
		{
			assert(parenter.is_empty() || !parenter.is_full());
			/// if noder is full, ok, split first
			if (noder.is_full())
			{
				noder_type new_noder = spliter.split_children(parenter,poff,noder,noder.value_size()/2);
				int r = value_cmp_type()(value,parenter.value(poff));
				if (r>=0) noder=new_noder;
				//++poff;
			}
			assert(!noder.is_full());
			/// if has no children, ok, it the noder, but we should calculate the offset insertion
			if (noder.front_childer().is_empty())
			{
				noder.insert(value);
				return *this;
			}
			size_t off = find_inserted(noder,value);
			return operator()(noder,off,noder.childer(off),value);
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
			_m_rooter.set_root(_m_rooter.make<node_type>());
			pair<noder_type,size_t> ret(0,-1);
			if (_m_rooter.is_empty())
				return ret;
			ret.second = _m_rooter.rooter().insert(value);
			ret.first = _m_rooter.rooter();
			return ret;
		}

		/// the root is full or not full
		noder_type parenter;
		size_t poff = -1;
		noder_type noder = _m_rooter.rooter();
		if (noder.is_full())
		{
			trace();
			_m_rooter.set_root(parenter=_m_rooter.make<node_type>());
			parenter.child_assi(0) = noder;
			poff = 0;
			trace();
		}
		internal_inserter_tt<self> inserter(*this);
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
	static pair<pair<noder_type,size_t>,pair<size_t,size_t>> balance_node(noder_type noder,noder_type parenter,size_t pchildoff)
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

	static pair<location_type,location_type> remove_below_successor(noder_type noder,noder_type parenter,size_t pchildoff,size_t times)
	{
		if (times==0 || noder.front_childer().is_empty())
			return pair<location_type,location_type>(location_type(noder,0),location_type(parenter,pchildoff));
		noder_type noder_front = noder.front_childer();
		pair<pair<noder_type,size_t>,pair<size_t,size_t>> ret = balance_node(noder,parenter,pchildoff);
		noder_type new_noder_right = ret.first.first;
		size_t off_of_noder_right = ret.second.first+0; /// the number of left inserted
		noder_type the_prev_leftest_of_childer_right = noder_front;
		assert (new_noder_right.childer(off_of_noder_right)==the_prev_leftest_of_childer_right);
		return remove_below_successor(the_prev_leftest_of_childer_right,new_noder_right,off_of_noder_right,times-1);
	}
	static pair<location_type,location_type> remove_below_successor(noder_type noder,noder_type parenter,size_t pchildoff)
	{
		return remove_below_successor(noder,parenter,pchildoff,-1);
	}
	struct remove_below_successor_tt
	{
		location_type top;
		pair<location_type,location_type> bottom;
		void operator()(pair<location_type,location_type>& locations)
		{
			noder_type noder = locations.first.first;
			size_t off = locations.first.second;
			noder_type parenter = locations.second.first;
			size_t pchildoff = locations.second.second;

			/// balance the hit node, and the next level maybe change the node position
			top = locations.first;
			bottom = remove_below_successor(noder,parenter,pchildoff,1);
			top.second += bottom.second.second; /// the offset of hit value in the hit node

			size_t child_right_off = top.second + 1;
			noder_type childer_right = top.first.childer(child_right_off);

			size_t size_before = top.first.value_size();
			size_t size_rightest = bottom.first.first.value_size();
			/// the change of the leftest node maybe leads changes of the hit node's position
			bottom = remove_below_successor(childer_right,top.first,child_right_off,1);
			if (size_before!=top.first.value_size())
				top.first=bottom.second.first, top.second=bottom.second.first.value_size()-size_rightest-1;

			/// do the next all is at the leftest node
			bottom = remove_below_successor(bottom.first.first,bottom.second.first,bottom.second.second);
		}
	};
	void remove(value_type const& value)
	{
		remove(noder_type(),0,_m_rooter.rooter(),value);
		if (_m_rooter.rooter().value_size()==0)
			_m_rooter.set_root(_m_rooter.rooter().front_child());
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

		while (!childer_right.is_empty())
		{
			remove_below_successor_tt the_remove_below_successor;
			the_remove_below_successor(finded);
			finded.first = the_remove_below_successor.top;
			successor = the_remove_below_successor.bottom;
			break;
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


template <typename rooter_tn>
struct bptree_ts
{
	typedef bptree_ts self;
	typedef rooter_tn rooter_type;
	typedef typename rooter_type::noder_type noder_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename noder_type::value_type value_type;
	typedef typename noder_type::value_cmp_type value_cmp_type;
	typedef typename noder_type::assi_valuer_type valuer_type;
	typedef pair<noder_type,size_t> location_type;
	typedef typename stack_tt<location_type>::type path_type;

	rooter_type _m_rooter;

	//self() {}

	node_type* root() {return _m_rooter.rooter().node();}
	node_type const* root() const {return _m_rooter.rooter().node();}

	location_type find(value_type const& value)
	{
		if (_m_rooter.rooter().is_empty())
			return location_type(_m_rooter.rooter(),-1);
		return find(_m_rooter.rooter(),value);
	}
	bool find(path_type& path,value_type const& value)
	{
		return find(path,_m_rooter.rooter(),value);
	}
	bool find(path_type& path,noder_type noder,value_type const& value)
	{
		if (noder.front_childer().is_empty())
			return false;
		int r = value_cmp_type()(value,noder.front_value());
		if (r<0) return find(path.push(noder,0),noder.front_childer(),value);
		r = value_cmp_type()(value,noder.back_value());
		if (r>0) return find(path.push(noder,noder.value_size()),noder.back_childer(),value);
		pair<size_t,bool> off = noder.try_find(value);
		path.push(noder,off.first);
		if (off.second) return true;
		return find(path,noder.childer(off.first-1),value);
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
		if (/*parent.is_empty() || */parent.is_full())
			return noder_type();
		noder_type new_noder = _m_rooter.make<node_type>();
		if (new_noder.is_empty())
			return noder_type();
		value_type const* new_value = child.split_out(off,new_noder);
		if (!new_value) /// will never happen
			return noder_type();
		parent.unsafe_insert_at(parent_off,*new_value,new_noder);
		//if (is_parent_empty)
		//	parent.child_assi(0) = child;
		return new_noder;
	}
	static size_t find_inserted(noder_type noder,value_type const& value)
	{
		/// if at the left side
		int r = value_cmp_type()(value,noder.front_value());
		if (r<0) return 0;
		/// if at the right side
		r = value_cmp_type()(value,noder.back_value());
		if (r>0) return noder.value_size();
		/// ok, it is inside the current node
		pair<size_t,bool> off = noder.try_find(value);
		//if (!off.second) --off.first;
		return off.first;
	}
	template <typename spliter_tn>
	struct internal_inserter_tt
	{
		internal_inserter_tt(spliter_tn& _spliter) : spliter(_spliter) {}
		spliter_tn& spliter;
		location_type newer;
		location_type toper;

		internal_inserter_tt operator()(noder_type parenter,size_t poff,noder_type noder,value_type const& value)
		{
			assert(parenter.is_empty() || !parenter.is_full());
			/// if noder is full, ok, split first
			if (noder.is_full())
			{
				noder_type new_noder = spliter.split_children(parenter,poff,noder,noder.value_size()/2);
				int r = value_cmp_type()(value,parenter.value(poff));
				if (r>=0) noder=new_noder;
				//++poff;
			}
			assert(!noder.is_full());
			/// if has no children, ok, it the noder, but we should calculate the offset insertion
			if (noder.front_childer().is_empty())
			{
				noder.insert(value);
				return *this;
			}
			size_t off = find_inserted(noder,value);
			return operator()(noder,off,noder.childer(off),value);
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
			_m_rooter.set_root(_m_rooter.make<node_type>());
			pair<noder_type,size_t> ret(0,-1);
			if (_m_rooter.is_empty())
				return ret;
			ret.second = _m_rooter.rooter().insert(value);
			ret.first = _m_rooter.rooter();
			return ret;
		}

		/// the root is full or not full
		noder_type parenter;
		size_t poff = -1;
		noder_type noder = _m_rooter.rooter();
		if (noder.is_full())
		{
			trace();
			_m_rooter.set_root(parenter=_m_rooter.make<node_type>());
			parenter.child_assi(0) = noder;
			poff = 0;
			trace();
		}
		internal_inserter_tt<self> inserter(*this);
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
	static pair<pair<noder_type,size_t>,pair<size_t,size_t>> balance_node(noder_type noder,noder_type parenter,size_t pchildoff)
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
	/// will generate a new right, and a parent if the parent is empty
	noder_type split_children_of_leaf(noder_type& parent,size_t parent_off,noder_type child,size_t off)
	{
		bool is_parent_empty = parent.is_empty();
		if (is_parent_empty)
			parent = _m_rooter.make<node_type>();
		if (parent.is_empty() || parent.is_full())
			return noder_type();
		noder_type new_noder = _m_rooter.make<node_type>();
		if (new_noder.is_empty())
			return noder_type();
		value_type const* new_value = child.split_out(off,new_noder);
		if (!new_value) /// will never happen
			return noder_type();
		parent.unsafe_insert_at(parent_off,*new_value,new_noder);
		if (is_parent_empty)
			parent.child_assi(0) = child;
		return new_noder;
	}
	static void leaf_borrow_one_from_successor(noder_type noder,noder_type parenter,size_t childoff)
	{
		assert(childoff+1<=parenter.value_size());
		noder_type next_sibling = parenter.child(childoff+1);
		noder.push_back(parenter.value(childoff),next_sibling.front_child());
		next_sibling.remove_front();
		parenter.value_assi(childoff) = next_sibling.front_value();
	}
	static void leaf_borrow_one_from_predecessor(noder_type noder,noder_type parenter,size_t childoff)
	{
		assert(childoff>0);
		noder_type prev_sibling = parenter.child(childoff-1);
		noder.push_front(prev_sibling.back_value(),prev_sibling.back_child());
	}
	/// return the new childer
	static noder_type leaf_merge_two_children(noder_type parenter,size_t index_of_value)
	{
		/// parenter maybe is the root, which is beyond the rule of >1/2
		/// so, we can not add the assertion here
		//assert (parenter.is_greater_than_min());
		assert(index_of_value<=parenter.value_size());
		noder_type lefter = parenter.childer(index_of_value);
		noder_type righter = parenter.childer(index_of_value+1);
		//lefter.push_back(parenter.value(index_of_value),0);
		lefter.append(righter);
		parenter.remove_at(index_of_value);
		return lefter;
	}
	/// poff: child off of noder, return the new childoff
	/// return : <noder,pchildoff>, if merged, noder is the merged node, off is the new, if not, node is noder, off keeping
	pair<pair<noder_type,size_t>,pair<size_t,size_t>> leaf_balance_node(noder_type noder,noder_type parenter,size_t pchildoff)
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
				leaf_borrow_one_from_successor(noder,parenter,pchildoff);
			}
			else if (borrow_index==-1)
			{
				++delta.first;//,--pchildoff,noder=parenter.childer(pchildoff);
				leaf_borrow_one_from_predecessor(noder,parenter,pchildoff);
				//pchildoff;
			}
			else /// merge action
			{
				if (merge_value_pindex<pchildoff)
					delta.first += (1+parenter.childer(merge_value_pindex).value_size());
				else
					delta.second += (1+parenter.childer(merge_value_pindex+1).value_size());
				pchildoff=merge_value_pindex;
				noder = leaf_merge_two_children(parenter,merge_value_pindex);
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

	static pair<location_type,location_type> remove_below_successor(noder_type noder,noder_type parenter,size_t pchildoff,size_t times)
	{
		if (times==0 || noder.front_childer().is_empty())
			return pair<location_type,location_type>(location_type(noder,0),location_type(parenter,pchildoff));
		noder_type noder_front = noder.front_childer();
		pair<pair<noder_type,size_t>,pair<size_t,size_t>> ret = balance_node(noder,parenter,pchildoff);
		noder_type new_noder_right = ret.first.first;
		size_t off_of_noder_right = ret.second.first+0; /// the number of left inserted
		noder_type the_prev_leftest_of_childer_right = noder_front;
		assert (new_noder_right.childer(off_of_noder_right)==the_prev_leftest_of_childer_right);
		return remove_below_successor(the_prev_leftest_of_childer_right,new_noder_right,off_of_noder_right,times-1);
	}
	static pair<location_type,location_type> remove_below_successor(noder_type noder,noder_type parenter,size_t pchildoff)
	{
		return remove_below_successor(noder,parenter,pchildoff,-1);
	}
	struct remove_below_successor_tt
	{
		location_type top;
		pair<location_type,location_type> bottom;
		void operator()(pair<location_type,location_type>& locations)
		{
			noder_type noder = locations.first.first;
			size_t off = locations.first.second;
			noder_type parenter = locations.second.first;
			size_t pchildoff = locations.second.second;

			/// balance the hit node, and the next level maybe change the node position
			top = locations.first;
			bottom = remove_below_successor(noder,parenter,pchildoff,1);
			top.second += bottom.second.second; /// the offset of hit value in the hit node

			size_t child_right_off = top.second + 1;
			noder_type childer_right = top.first.childer(child_right_off);

			size_t size_before = top.first.value_size();
			size_t size_rightest = bottom.first.first.value_size();
			/// the change of the leftest node maybe leads changes of the hit node's position
			bottom = remove_below_successor(childer_right,top.first,child_right_off,1);
			if (size_before!=top.first.value_size())
				top.first=bottom.second.first, top.second=bottom.second.first.value_size()-size_rightest-1;

			/// do the next all is at the leftest node
			bottom = remove_below_successor(bottom.first.first,bottom.second.first,bottom.second.second);
		}
	};
	void remove(value_type const& value)
	{
		remove(noder_type(),0,_m_rooter.rooter(),value);
		if (_m_rooter.rooter().value_size()==0)
			_m_rooter.set_root(_m_rooter.rooter().front_child());
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

		while (!childer_right.is_empty())
		{
			remove_below_successor_tt the_remove_below_successor;
			the_remove_below_successor(finded);
			finded.first = the_remove_below_successor.top;
			successor = the_remove_below_successor.bottom;
			break;
		}
		//}
		location_type& leafed = successor.first;
		location_type& pleafed = successor.second;
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
		pair<pair<noder_type,size_t>,pair<size_t,size_t>> ret = leaf_balance_node(victim,parenter_of_victim,pleafoff);
		/// assign the finded value
		leafed = ret.first; /// ret.frist.second is not the old means
		if (value_off==0 && finded.first.first!=leafed.first)
			finded.first.first.value_assi(finded.first.second) = leafed.first.front_value();
		return;
	}

	noder_type get_leftest_child(noder_type noder) {return noder.child(0);}
	noder_type get_rightest_child(noder_type noder) {return noder.child(0);}
	value_type const& get_leftest_value(noder_type noder) {return noder.value(0);}
	value_type const& get_rightest_value(noder_type noder) {return noder.value(noder.value_size());}
};



template <size_t rank,typename value_tn,typename cmp_tn=compare_default,typename allocator_tn=cppmalloc>
struct btree_tt
{
	typedef cmp_tn cmp_type;
	typedef btree_noder_default_tt<bnode_tt<value_tn,rank>,cmp_type> noder_type;
	typedef btree_rooter<noder_type,allocator_tn> rooter_type;
	typedef btree_ts<rooter_type> type;
};


template <size_t rank,typename value_tn,typename cmp_tn=compare_default,typename allocator_tn=cppmalloc>
struct bptree_tt
{
	typedef cmp_tn cmp_type;
	typedef bptree_noder_default<bnode_tt<value_tn,rank>,cmp_type> noder_type;
	typedef btree_rooter<noder_type,allocator_tn> rooter_type;
	typedef bptree_ts<rooter_type> type;
};


///// free tree, that can stand for the file system, or any form which likes tree structure

template <typename value_tn,typename key_tn,typename keyof_tn,typename keycmp_tn,typename allocator_tn>
struct free_tree_trait
{
	typedef free_tree_trait traiter;

	typedef value_tn value_type;
	typedef key_tn key_type;
	typedef keyof_tn keyof_type;
	typedef keycmp_tn keycmp_type;
	typedef allocator_tn allocator_type;

	struct tree_node_type;
	struct keyof_children : keyof_tn
	{
		key_type const& operator()(tree_node_type const* const& v) const
		{
			return keyof_tn::operator()(v->value());
		}
	};

	typedef typename ox::veclistref_tt<tree_node_type*,key_type,keyof_children,keycmp_type,allocator_type>::type children_type;
	struct tree_node_type
	{
		tree_node_type()
		{
			_m_parent=0;
			_m_vectorhead = 0;
			_m_listhead.set_head(0),_m_listhead.set_tail(0);
		}
		tree_node_type(value_type const& _value)
			: _m_value(_value)
		{
			_m_parent=0;
			_m_vectorhead = 0;
			_m_listhead.set_head(0),_m_listhead.set_tail(0);
		}
		typedef typename children_type::rooter_type::vector_head_t vector_head_t;
		typedef typename children_type::rooter_type::list_head_type list_head_t;
		value_type _m_value;
		tree_node_type* _m_parent;
		vector_head_t* _m_vectorhead;
		list_head_t _m_listhead;
		value_type& value() {return _m_value;}
		value_type const& value() const {return _m_value;}
	};
	//typedef typename children_type::dot_type subdot_type;

	//typedef line_value_container_rooter_ref<value_ptr,vector_malloc> children_ref_rooter;
	//typedef line_value_center<children_ref_rooter> children_line_type;

	template <bool iscon_tc>
	struct node_trait_tt
	{
		typedef typename met::iff<iscon_tc,met::add_con<tree_node_type>,identity<tree_node_type>>::type::type tree_node_type;
		tree_node_type* _m_node;

		node_trait_tt(tree_node_type* _node=0): _m_node(_node) {}
		void set_parent(tree_node_type* _node) {_m_node->_m_parent;}
		tree_node_type* parent() {return _m_node->_m_parent;}
		tree_node_type* const parent() const {return _m_node->_m_parent;}
		children_type as_children(allocator_type* _alloc)
		{
			return children_type(&_m_node->_m_vectorhead,&_m_node->_m_listhead,_alloc);
		}
		//void insert(tree_node_type* node,allocator_type* _alloc)
		//{
		//	as_children(_alloc).push_back(node);
		//}
		tree_node_type* insert(value_type const& value,allocator_type* _alloc)
		{
			tree_node_type* nodenew = (tree_node_type*)_alloc->allocate(sizeof(tree_node_type));
			if (!nodenew) return 0;
			nodenew->set_head(this);
			as_children(_alloc).push_back(nodenew);
			return nodenew;
		}

		//subdot_type fronter(allocator_type* _alloc) {return as_children(_alloc).fronter();}
		//subdot_type backer(allocator_type* _alloc) {return as_children(_alloc).backer();}
		//subdot_type ender(allocator_type* _alloc) {return as_children(_alloc).ender();}
	};

	template <bool iscon_tc>
	struct dot_tt
	{
		typedef typename children_type::vectorref_type::dot_type sdot_type;

		dot_tt(tree_node_type* node=0): _m_node(node), _m_sdot() {}
		tree_node_type* _m_node;
		sdot_type _m_sdot;;
		bool is_empty() const {return _m_node==0;}
		void set_node(tree_node_type* node) {_m_node=node;}
		tree_node_type* node() {return _m_node;}
		tree_node_type const* node() const {return _m_node;}
		operator tree_node_type*() {return _m_node;}
		operator tree_node_type const*() const {return _m_node;}
		bool is_dot_valid() const
		{
			tree_node_type const* p = _m_node->_m_parent;
			if (!p) return true;
			node_trait_tt ntrait(p);
			return ntrait.as_children().as_vector().is_between(p);
		}
	};
	template <>
	struct dot_tt <true>
	{
		typedef typename children_type::vectorref_type::dot_type sdot_type;

		dot_tt(tree_node_type* node=0): _m_node(node), _m_sdot() {}
		dot_tt(tree_node_type const* node=0): _m_node(node), _m_sdot()  {}
		dot_tt(dot_tt<false> const& dot): _m_node((tree_node_type const*)dot.node()), _m_sdot(dot._m_sdot) {}
		tree_node_type const* _m_node;
		sdot_type _m_sdot;;
		bool is_empty() const {return _m_node==0;}
		void set_node(tree_node_type* node) {_m_node=node;}
		void set_node(tree_node_type const* node) {_m_node=node;}
		tree_node_type const* node() const {return _m_node;}
		operator tree_node_type*() {return _m_node;}
		operator tree_node_type const*() const {return _m_node;}
		bool is_dot_valid() const
		{
			tree_node_type const* p = _m_node->_m_parent;
			if (!p) return true;
			node_trait_tt ntrait(p);
			return ntrait.as_children().as_vector().is_between(p);
		}
	};

	struct rooter_tt
		: common_data_tt<tree_node_type*,allocator_type>
	{
		typedef rooter_tt self_type;
		typedef dot_tt<false> dot_type;
		typedef dot_tt<true> cdot_type;
		typedef value_type value_type;
		typedef tree_node_type node_type;
		typedef node_trait_tt<false> node_trait_type;

		self_type() {_m_data=0;}
		void set_head(tree_node_type* node) {_m_data=node;}
		tree_node_type* head() {return _m_data;}
		tree_node_type const* head() const {return _m_data;}

		allocator_type& allocator() {return common_data_tt<tree_node_type*,allocator_type>::allocator();}
		allocator_type const& allocator() const {return common_data_tt<tree_node_type*,allocator_type>::allocator();}

		//node_trait_type as_nodetrait(dot_type dot) {node_trait_type(dot.node());} 
		//ptr_line_type as_vector() {return ptr_line_type(&_m_allocator,_m_data);}
		//value_list_type as_list() {return value_list_type(allocator());}
		//ptr_line_type const as_vector() const {return ptr_line_type((vector_malloc_ref*)0,_m_data);}
	};
};

template <typename rooter_tn>
struct free_tree_center
{
	typedef free_tree_center self_type;
	typedef rooter_tn rooter_type;
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename rooter_type::dot_type dot_type;
	typedef typename rooter_type::cdot_type cdot_type;
	typedef typename rooter_type::node_trait_type node_trait_type;

	private: rooter_type _m_rooter; public:

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

	dot_type insert(dot_type parent,value_type const& value)
	{
		if (parent.is_empty())
		{
			node_type* _root_node = rooter().make<node_type>(value);
			if (!_root_node) return dot_type();
			rooter().set_head(_root_node);
			return dot_type(_root_node);
		}
		node_trait_type ndtrait(parent);
		ndtrait.insert(value,&rooter().allocator());
		return dot_type();
	}
};

template <
	typename value_tn,
	typename key_tn=value_tn,
	typename keyof_tn=keyof_default<key_tn>,
	typename keycmp_tn=key_compare_default<keyof_default<key_tn>>,
	typename allocator_tn=cppmalloc>
struct free_tree_tt
{
	typedef value_tn value_type;
	typedef keyof_tn keyof_type;
	typedef keycmp_tn keycmp_type;
	typedef allocator_tn allocator_type;
	typedef typename free_tree_trait<value_type,key_tn,keyof_type,keycmp_type,allocator_type>::rooter_tt rooter_type;
	typedef free_tree_center<rooter_type> type;
};

} /// end of namespace ox