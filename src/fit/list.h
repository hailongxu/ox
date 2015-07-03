

#include <assert.h>
#include "../ox/nsab.h"
#include "allocator_default.h"




___namespace2_begin(ox,fit)


template <typename node_tn>
struct xlist_head_tt
{
	xlist_head_tt():_m_first(0),_m_last(0) {}

	node_tn* _m_first;
	node_tn* _m_last;
	void set_first(node_tn* node) {_m_first = node;}
	node_tn const* get_first() const {return _m_first;}
	node_tn* get_first() {return _m_first;}
	void set_last(node_tn* node) {_m_last = node;}
	node_tn const* get_last() const {return _m_last;}
	node_tn* get_last() {return _m_last;}
	bool is_empty() const {assert (_m_first && _m_last || !_m_first && !_m_last);return _m_first == 0;}
};

template <typename value_tn>
struct slist_node_tt
{
	slist_node_tt() : _m_next(0) {}
	slist_node_tt(value_tn const& value): _m_value(value) {}
	slist_node_tt* _m_next;
	value_tn _m_value;
	/// necessary
	void set_next(slist_node_tt* next) {_m_next=next;}
	slist_node_tt* get_next() {return _m_next;}
	slist_node_tt const* get_next() const {return _m_next;}
	value_tn& operator*() {return _m_value;}
	value_tn const& operator*() const {return _m_value;}
};
template <typename value_tn>
struct list_node_tt
{
	list_node_tt() : _m_next(0) {}
	list_node_tt(value_tn const& value): _m_value(value) {}
	list_node_tt* _m_next;
	list_node_tt* _m_prev;
	value_tn _m_value;
	/// necessary
	void set_next(list_node_tt* next) {_m_next=next;}
	list_node_tt* get_next() {return _m_next;}
	list_node_tt const* get_next() const {return _m_next;}
	void set_prev(list_node_tt* prev) {_m_prev=prev;}
	list_node_tt* get_prev() {return _m_prev;}
	list_node_tt const* get_prev() {return _m_prev;}
	value_tn& operator*() {return _m_value;}
	value_tn const& operator*() const {return _m_value;}
};

template <typename value_tn,typename node_tn,typename head_tn,typename allocator_tn>
struct xlist_rooter : head_tn
{
	typedef head_tn head_type;
	typedef allocator_tn allocator_type;
	head_type& head() {return *this;}
	head_type const& head() const {return *this;}

	typedef value_tn value_type;
	typedef node_tn node_type;
	node_type* allocate_node(value_type const& value)
	{
		node_type* pnode = (node_type*)allocator_type::allocate(sizeof(node_type));
		return new (pnode) node_type (value);
	}
	node_type* deallocate_node(node_type* pnode)
	{
		pnode->~node_type();
		allocator_type::deallocate(pnode);
	}
};

template <typename rooter_tn>
struct slist_tt : rooter_tn
{
	typedef rooter_tn rooter_type;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::node_type node_type;
	node_type* remove_front()
	{
		if (rooter().head().is_empty())
			return;
		node_type* first = rooter().head().get_first();
		/// update header
		rooter().head().set_first(first->get_next());
		if (!rooter().head().get_first()) rooter().head().set_last(0);
		return first;
	}
	void erase_front()
	{
		node_type* victim = remove_front();
		deallocate_node(victim);
	}
	node_type* add_after(value_type const& value,node_type* node)
	{
		node_type* node_new = rooter().allocate_node(value);
		node_type* next = node?node->get_next():0;
		node_new->set_next(node->get_next());
		node->set_next(node_new);
		/// update header.last
		if (!node) rooter().head().set_first(node_new);
		if (!next) rooter().head().set_last(node_new);
		return node_new;
	}
	node_type* add_front(value_type const& value)
	{
		return add_after(value,0);
	}
	node_type* remove_after(node_type* node)
	{
		if (!node) return remove_front();
		node_type* victim = node->get_next();
		if (!victim) return; /// last
		node->set_next(victim->get_next());
		/// update header.last
		if (!node->get_next()) rooter().head().set_last(node);
		return victim;
	}
	void erase_after(node_type* node)
	{
		node_type* victim = remove_after(node);
		deallocate_node(victim);
	}
	template <typename action>
	void foreach(action& act)
	{
		node_type* p=rooter().head().get_first();
		while (p)
		{
			if (!act(p)) break;
			p = p->get_next();
		}
	}
};


template <typename rooter_tn>
struct list_tt : rooter_tn
{
	typedef rooter_tn rooter_type;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::node_type node_type;
	node_type* add_front(value_type const& value)
	{
		node_type* node_new = rooter().allocate_node(value);
		return add_front(node_new);
	}
	node_type* add_front(node_type* node_new)
	{
		node_type* first = rooter().head().get_first();
		node_new->set_next(first);
		node_new->set_prev(0);
		if (first) first->set_prev(node_new);
		rooter().head().set_first(node);
		if (!first) rooter().head().set_last(node_new);
	}
	node_type* add_back(node_type* node_new)
	{
		node_type* first = rooter().head().get_first();
		node_type* last = rooter().head().get_last();
		node_new->set_next(0);
		node_new->set_prev(last);
		last->set_next(node_new);
		rooter().head().set_last(node);
		if (!first) rooter().head().set_first(node_new);
	}
	node_type* insert(value_type const& value,node_type* node)
	{
		/// make a new node
		node_type* node_new = rooter().allocate_node(value);
		/// if append to end
		if (!node) return add_back(node_new);
		/// 
		node_type* prev = node->get_prev();
		/// update new node
		node_new->set_next(node->get_next());
		node_new->set_prev(prev);
		/// update node
		node->set_prev(node_new);
		/// update node-prev
		node->set_next(node_new);
		/// update head
		if (node==rooter().head().get_first()) rooter().head().set_first(node);
		return node_new;
	}
	node_type* remove(node_type* victim)
	{
		assert(node);
		node_type* next = victim->get_next();
		node_type* prev = victim->get_prev();
		if (prev) prev->set_next(next);
		if (next) next->set_prev(prev);
		if (!prev) rooter().head().set_first(next);
		if (!next) rooter().head().set_last(prev);
		return victim;
	}
	node_type* remove_front()
	{
		return remove(rooter().head().get_first());
	}
	node_type* remove_back()
	{
		return remove(rooter().head().get_last());
	}
	void erase_front()
	{
		node_type* victim = remove_front();
		deallocate_node(victim);
	}
	void erase_back()
	{
		node_type* victim = remove_back(node);
		deallocate_node(victim);
	}
	template <typename action>
	void foreach(action& act)
	{
		node_type* p=rooter().head().get_first();
		while (p)
		{
			if (!act(p)) break;
			p = p->get_next();
		}
	}
	template <typename action>
	void revert_foreach(action& act)
	{
		node_type* p=rooter().head().get_last();
		while (p)
		{
			if (!act(p)) break;
			p = p->get_prev();
		}
	}
};

template <typename value_tn,typename allocator_tn=cppmalloc>
struct slist : slist_tt<xlist_rooter<value_tn,slist_node_tt<value_tn>,xlist_head_tt<slist_node_tt<value_tn>>,allocator_tn>>
{};
	
template <typename value_tn,typename allocator_tn=cppmalloc>
struct list : list_tt<xlist_rooter<value_tn,list_node_tt<value_tn>,xlist_head_tt<list_node_tt<value_tn>>,allocator_tn>>
{};


___namespace2_end()