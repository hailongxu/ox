

#include <vector>
#include "../ox/nsab.h"
#include "gui_define.h"



#pragma once


___namespace2_begin(ox,gui)



struct draw_interface
{
	virtual void draw(drawer_t const& drawer,xy_rect_t const& rect) = 0;
};
//struct str_interface
//{
//	virtual char const* get_str() const = 0;
//};
//struct xy_point_interface
//{
//	virtual xy_point_t get_point() const = 0;
//};

struct item_interface : draw_interface
{};

struct overlapped_layout_t
{
	typedef std::vector<item_interface*> item_set;
	item_set _m_items;
	xy_size_t _m_size;
	void add_back(item_interface* item)
	{
		_m_items.insert(_m_items.end(),item);
	}
	void add_front(item_interface* item)
	{
		_m_items.insert(_m_items.begin(),item);
	}
	xy_rect_t get_layout_rect(xy_point_t const& o,size_t i) const
	{
		return xy_rect_t(o,_m_size);
	}
	void draw(drawer_t const& drawer,xy_rect_t const& rect)
	{
		for (int i=0;i<_m_items.size();++i)
		{
			_m_items[i]->draw(drawer,rect);
		}
	}
};



struct item_text_t : item_interface
{
	virtual void draw(drawer_t const& drawer,xy_rect_t const& r)
	{
		drawer.draw_text(_m_str,r.p);
	}
	char const* _m_str = "hello world";
};

struct item_rectangle : item_interface
{
	virtual void draw(drawer_t const& drawer,xy_rect_t const& r)
	{
		drawer.draw_rectangle(r);
	}
};

struct win_label : item_interface
{
	typedef win_label self;
	self()
	{
		_m_layout.add_front(&_m_text);
		_m_layout.add_front(&_m_rect_angle);
		_m_text._m_str = _m_str;
	}
	virtual void draw(drawer_t const& drawer,xy_rect_t const& r)
	{
		do_draw(drawer,r);
	}
	void do_draw(drawer_t const& drawer,xy_rect_t const& r)
	{
		_m_layout.draw(drawer,r);
	}
	item_text_t _m_text;
	item_rectangle _m_rect_angle;
	char const* _m_str = "hello world";
	overlapped_layout_t _m_layout;
};



___namespace2_end()