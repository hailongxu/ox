

#pragma once


___namespace2_begin(ox,mos)

template <typename t>
struct free_ptr
{
	typedef t value_type;
	free_ptr(t* p=0):_m_p(p) {}
	t* operator->() {return _m_p;}
	t& operator*() {return *_m_p;}
	t* pointer() {return _m_p;}
	t const* pointer() const {return _m_p;}
	free_ptr& operator=(t* p) {_m_p=p;return *this;}
	bool is_empty() const {return _m_p==0;}
	t& operator()() {return *_m_p;}
private:
	t* _m_p;
};
	
___namespace2_end()