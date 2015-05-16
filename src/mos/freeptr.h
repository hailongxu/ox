

#pragma once


___namespace2_begin(ox,mos)


template <typename t>
struct free_ptr
{
	typedef t value_type;
	~free_ptr() {clear();}
	explicit free_ptr(value_type* p=0):_m_p(p) {}
	value_type* operator->() {return _m_p;}
	value_type& operator*() {return *_m_p;}
	value_type& value() {return *_m_p;}
	value_type const& value() const {return *_m_p;}
	value_type& value_nonconst() const {return *_m_p;}
	value_type* pointer() {return _m_p;}
	value_type const* pointer() const {return _m_p;}
	free_ptr& operator=(value_type* p) {_m_p=p;return *this;}
	bool is_empty() const {return _m_p==0;}
	//value_type& operator()() {return *_m_p;}
	operator bool() const {return !is_empty();}
	void clear() {_m_p=0;}
private:
	value_type* _m_p;
};

template <typename t>
struct free_ptr<t const>
{
	typedef t const value_type;
	~free_ptr() {clear();}
	explicit free_ptr(value_type* p=0):_m_p(p) {}
	value_type* operator->() {return _m_p;}
	value_type& operator*() {return *_m_p;}
	value_type& value() const {return *_m_p;}
	value_type& value_nonconst() const {return *_m_p;}
	value_type* pointer() {return _m_p;}
	free_ptr& operator=(value_type* p) {_m_p=p;return *this;}
	bool is_empty() const {return _m_p==0;}
	value_type& operator()() {return *_m_p;}
	operator bool() const {return !is_empty();}
	void clear() {_m_p=0;}
private:
	value_type* _m_p;
};
	
___namespace2_end()