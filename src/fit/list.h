

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
	bool is_empty() const
	{
		assert (_m_first && _m_last || !_m_first && !_m_last);
		return _m_first == 0;
	}
};

template <typename value_tn>
struct slist_node_tt
{
	slist_node_tt() : _m_next(0) {}
	slist_node_tt(value_tn const& value): _m_value(value) {}
	slist_node_tt* _m_next;
	value_tn _m_value;
	/// necessary
	void set_next(slist_node_tt* next)
	{
		_m_next=next;
	}
	slist_node_tt* get_next()
	{
		return _m_next;
	}
	slist_node_tt const* get_next() const
	{
		return _m_next;
	}
	value_tn& operator*()
	{
		return _m_value;
	}
	value_tn const& operator*() const
	{
		return _m_value;
	}
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
	list_node_tt const* get_prev() const {return _m_prev;}
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
	node_type* add_node_after(node_type* node_new,node_type* node)
	{
		return add_node_after(node_new,node,rooter().head());
	}
	node_type* add_after(value_type const& value,node_type* node)
	{
		node_type* node_new = rooter().allocate_node(value);
		return add_node_after(node_new,node);
	}
	node_type* add_front(value_type const& value)
	{
		return add_after(value,0);
	}
	node_type* add_back(value_type const& value)
	{
		node_type* node_new = rooter().allocate_node(value);
		return add_node_after(node_new,rooter().head().get_last());
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
	struct revert_action
	{
		bool operator()(node_type* curr)
		{
			next = curr->get_next();
			curr->set_next(head);
			head = curr;
		}
		node_type* head;
		node_type* next;
	};
	void revert()
	{
		revert_slist(rooter().head());
	}
	template <typename value_action>
	struct each_node_action
	{
		value_action& _m_value_action;
		each_node_action(value_action& va)
			: _m_value_action(va)
		{}
		bool operator()(node_type* node)
		{
			assert (node);
			return _m_value_action(**node);
		}
	};
	template <typename action>
	void foreach(action& act)
	{
		each_node_action<action> node_action(act);
		foreach_node(node_action);
	}
	template <typename action>
	void foreach_node(action& act)
	{
		node_type* p=rooter().head().get_first();
		while (p)
		{
			if (!act(p)) break;
			p = p->get_next();
		}
	}
	/// static functions
	template <typename head_tn>
	static void revert_slist(head_tn& head)
	{
		node_type* h=0;
		node_type* c=head.get_first();
		node_type* n=0;
		while (c)
		{
			n=c->get_next();
			c->set_next(h);
			h=c;
			c=n;
		}
		/// swap first<-->last
		node_type* t=head.get_first();
		head.set_first(head.get_last());
		head.set_last(t);
	}
	/// if node is null, node new is added as front node
	template <typename head_tn>
	static node_type* add_node_after(node_type* node_new,node_type* node,head_tn& head)
	{
		node_type* next=head.get_first();
		if (node)
		{
			next=node->get_next();
			node->set_next(node_new);
		}
		node_new->set_next(next);
		/// update header.last
		if (!node) head.set_first(node_new);
		if (!next) head.set_last(node_new);
		return node_new;
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
		return insert(value,rooter().head().get_first());
	}
	node_type* add_back(value_type const& value)
	{
		return insert(value,0);
	}
	node_type* insert(value_type const& value,node_type* node)
	{
		/// make a new node
		node_type* node_new = rooter().allocate_node(value);
		return insert_node(node_new,node);
	}
	node_type* insert_node(node_type* node_new,node_type* node)
	{
		/// get prev node
		node_type* prev = rooter().head().get_last();
		if (node)
			prev = node->get_prev();
		/// update new node
		node_new->set_next(node);
		node_new->set_prev(prev);
		/// update node
		if (node)
			node->set_prev(node_new);
		/// update node-prev
		if (prev)
			prev->set_next(node_new);
		/// update head
		if (node==rooter().head().get_first()) /// update first
			rooter().head().set_first(node_new);
		if (!node) /// update last
			rooter().head().set_last(node_new);
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
	template <typename value_action>
	struct each_node_action
	{
		value_action& _m_value_action;
		each_node_action(value_action& va)
			: _m_value_action(va)
		{}
		bool operator()(node_type* node)
		{
			assert (node);
			return _m_value_action(**node);
		}
	};
	template <typename action>
	void foreach(action& act)
	{
		each_node_action<action> node_action(act);
		foreach_node(node_action);
	}
	template <typename action>
	void foreach_node(action& act)
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
	void revert()
	{
		revert_list(rooter().head());
	}
	template <typename head_tn>
	static void revert_list(head_tn& head)
	{
		node_type* h = 0;
		node_type* n = 0;
		node_type* c = head.get_first();
		while(c)
		{
			n = c->get_next();
			c->set_next(h);
			c->set_prev(0);
			if (h) h->set_prev(c);
			h = c;
			c = n;
		}
		/// swap first<-->last
		node_type* t = head.get_first();
		head.set_first(head.get_last());
		head.set_last(t);
	}
};

template <typename value_tn,typename allocator_tn=cppmalloc>
struct slist : slist_tt<xlist_rooter<value_tn,slist_node_tt<value_tn>,xlist_head_tt<slist_node_tt<value_tn>>,allocator_tn>>
{};
	
template <typename value_tn,typename allocator_tn=cppmalloc>
struct list : list_tt<xlist_rooter<value_tn,list_node_tt<value_tn>,xlist_head_tt<list_node_tt<value_tn>>,allocator_tn>>
{};


___namespace2_end()