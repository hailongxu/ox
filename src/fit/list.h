

#include <assert.h>
#include "../ox/nsab.h"
#include "allocator_default.h"




___namespace2_begin(ox,fit)


template <typename node_tn>
struct slist_head_tt
{
	slist_head_tt():_m_first(0),_m_last(0) {}

	node_tn* _m_first;
	node_tn* _m_last;
	void set_first(node_tn* node)
	{
		_m_first = node;
	}
	node_tn const* get_first() const
	{
		return _m_first;
	}
	node_tn* get_first()
	{
		return _m_first;
	}
	void set_last(node_tn* node)
	{
		_m_last = node;
	}
	node_tn const* get_last() const
	{
		return _m_last;
	}
	node_tn* get_last()
	{
		return _m_last;
	}
	bool is_empty() const
	{
		assert (_m_first && _m_last || !_m_first && !_m_last)
		return _m_first == 0;
	}
};

template <typename value_tn>
struct slist_node_tt
{
	slist_node_tt() : _m_next(0) {}
	slist_node_tt* _m_next;
	value_tn _m_value;
	/// necessary
	set_next(slist_node_tt* next) {_m_next=next;}
	slist_node_tt* get_next() {return _m_next;}
	slist_node_tt const* get_next() {return _m_next;}
	value_tn& operator*() {return _m_value;}
	value_tn const& operator*() const {return _m_value;}
};

template <typename value_tn,typename head_tn,typename node_tn,typename allocator_tn>
struct slist_rooter : head_tn
{
	typedef head_tn head_type;
	typedef allocator_tn allocator_type;
	head_type& head() {return *this;}
	head_type const& head() const {return *this;}

	typedef value_tn value_type;
	typedef node_tn node_type;
	node_type* allocate_node(size_t data_bytes)
	{
		node_type* pnode = allocator_type::allocate(sizeof(node_type));
		return new (pnode) node_type ();
	}
	node_type* deallocate_node(node_type* pnode)
	{
		pnode->~node_type();
		allocator_type::deallocate(pnode);
	}
};


template <typename rooter_tn>
struct slist : rooter_tn
{
	typedef rooter_tn rooter_type;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::node_type node_type;
	node_type* add_front(value_type const& value)
	{
		node_type* node_new = rooter().allocate_node();
		*node_new = value;
		return add_front(node_new);
	}
	node_type* add_front(node_type* node_new)
	{
		node_new->set_next(rooter().head().get_first());
		/// update header
		rooter().head().set_first(node_new);
		if (!rooter().head()->get_first()->get_next()) rooter().head().set_last(node_new);
		return node_new;
	}
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
	void add_after(value_type const& value,node_type* node)
	{
		node_type* node_new = rooter().allocate_node();
		*node_new = value;
		if (!node) add_front(node_new);
		node_new->set_next(node->get_next());
		node->set_next(node_new);
		/// update header.last
		if (!node_new->get_next()) rooter().head().set_last(node_new);
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
struct list : rooter_tn
{
	typedef rooter_tn rooter_type;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::node_type node_type;
	void add_front(value_type const& value)
	{
		node_type* node_new = rooter().allocate_node();
		*node_new = value;
		node_type* first = rooter().head().get_first();
		node_new->set_next(first);
		node_new->set_prev(0);
		first->set_prev(node_new);
		rooter().head().set_next(node);
	}
	node_type* remove_front()
	{
		if (rooter().head().is_empty())
			return;
		node_type* first = rooter().head().get_first();
		rooter().head().set_first(first->get_next());
		return first;
	}
	void erase_front()
	{
		node_type* victim = remove_front();
		deallocate_node(victim);
	}
	void add_after(value_type const& value,node_type* node)
	{
		node_type* node_new = rooter().allocate_node();
		*node_new = value;
		node_new->set_next(node->get_next());
		node->set_next(node_new);
	}
	node_type* remove_after(node_type* node)
	{
		if (!node) return remove_front();
		node_type* victim = node->get_next();
		if (!victim) return; /// last
		node->set_next(victim->get_next());
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


___namespace2_end()