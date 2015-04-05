

#pragma once


___namespace2_begin(ox,cxx)


template <typename t>
struct reference_tt
{
	typedef reference_tt self;
	typedef t value_type;
	self() : _m_obj(0) {}
	self(value_type* obj) : _m_obj(obj) {}
	self(value_type& obj) : _m_obj(&obj) {}
	void bind(value_type* obj) {_m_obj=obj;}
	void bind(value_type& obj) {_m_obj=&obj;}
	value_type* _m_obj;
	value_type& operator()() {return *_m_obj;}
	value_type const& operator()() const {return *_m_obj;}
	self& operator=(value_type* obj) {bind(obj);return *this;}
	self& operator=(value_type& obj) {bind(obj);return *this;}
};


___namespace2_end()