
#include <assert.h>
#include <new.h>
#include "../../utl/intv.h"
//#include "../../ox/tclasses.h"


#pragma once



typedef ox::utl::intv_tt<int> row_intv_t;
//typedef ox::utl::data_tt<interval_t> interval

struct rc_coord;
struct xy_coord;


template <typename t>
struct dim_data_tt
{
	typedef dim_data_tt self;
	typedef t value_type;
	self() {}
	self(value_type d0,value_type d1) {set(d0,d1);}
	value_type _m_d[2];
	value_type& operator[](int i) {assert(i>=0&&i<=1);return _m_d[i];}
	value_type const& operator[](int i) const {assert(i>=0&&i<=1);return _m_d[i];}
	value_type& d0() {return _m_d[0];}
	value_type& d1() {return _m_d[1];}
	value_type const& d0() const {return _m_d[0];}
	value_type const& d1() const {return _m_d[1];}
	template<int i> value_type& d();
	template<> value_type& d<0>() {return d0();}
	template<> value_type& d<1>() {return d1();}
	template<int i> value_type const& d() const;
	template<> value_type const& d<0>() const {return d0();}
	template<> value_type const& d<1>() const {return d1();}
	void set(value_type const& d0,value_type const& d1)
	{
		_m_d[0] = d0;
		_m_d[1] = d1;
	}
	bool operator==(self const& p) const { return d0() == p.d0() && d1() == p.d1(); }
	bool operator!=(self const& p) const { return !operator==(p); }
};

template<int dim_count,typename t,typename coord>
struct size_access_tt;
template<int dim_count,typename t,typename coord>
struct size_tt;


template <int dim_count,typename t>
struct size_access_tt <dim_count,t,rc_coord>
{
	typedef size_access_tt self;
	typedef dim_data_tt<t> data_type;
	typedef t value_type;
	typedef rc_coord coord_type;
	typedef size_tt<dim_count,t,coord_type> mother_type;
	
	value_type& cc() {return data().d0();}
	value_type& rc() {return data().d1();}
	value_type const& cc() const {return data().d0();}
	value_type const& rc() const {return data().d1();}
	template<int i> value_type& d() {return data().d<i>();}
	template<int i> value_type const& d() const {return data().d<i>();}
	void set(value_type const& r,value_type const& c) {data().set(c,r);}
	value_type length() const {return rc()*cc();}
	bool is_empty() const { return rc() == 0 && cc() == 0; }
	data_type const& data() const {return static_cast<mother_type const*>(this)->_m_data;}
	data_type& data() {return static_cast<mother_type*>(this)->_m_data;}
};
template <int dim_count,typename t>
struct size_access_tt <dim_count,t,xy_coord>
{
	typedef size_access_tt self;
	typedef dim_data_tt<t> data_type;
	typedef t value_type;
	typedef xy_coord coord_type;
	typedef size_tt<dim_count,t,coord_type> mother_type;

	value_type& xs() {return data().d0();}
	value_type& ys() {return  data().d1();}
	value_type const& xs() const {return data().d0();}
	value_type const& ys() const {return data().d1();}
	template<int i> value_type& d() {return data().d<i>();}
	template<int i> value_type const& d() const {return data().d<i>();}
	void set(value_type const& x,value_type const& y) {data().set(x,y);}
	value_type length() const {return rc()*cc();}
	bool is_empty() const { return rc() == 0 && cc() == 0; }
	data_type const& data() const {return static_cast<mother_type const*>(this)->_m_data;}
	data_type& data() {return static_cast<mother_type*>(this)->_m_data;}
};

template<int dim_count,typename t,typename coord>
struct size_tt : size_access_tt<dim_count,t,coord>
{
	typedef size_tt self;
	typedef t value_type;
	static size_t const __size = dim_count;
	typedef dim_data_tt<t> data_type;
	self() {set(0,0);}
	self(value_type d0, value_type d1) { set(d0, d1); }
	data_type _m_data;
};

typedef size_tt<2,int,rc_coord> rc_size_t;
typedef size_tt<2,int,xy_coord> xy_size_t;



template<typename t,typename coord_tn>
struct point_tt ;
template <typename t,typename coord_tn>
struct point_access_tt;


template <typename t>
struct point_access_tt <t,xy_coord>
{
	typedef point_access_tt self;
	typedef t value_type;
	typedef dim_data_tt<t> data_type;
	typedef point_tt<t,xy_coord> mother_type;
	value_type& x() {return data().d0();}
	value_type& y() {return data().d1();}
	value_type const& x() const {return data().d0();}
	value_type const& y() const {return data().d1();}
	template<int i> value_type& d() {return data().d<i>();}
	template<int i> value_type const& d() const {return data().d<i>();}
	void set(value_type const& x,value_type const& y) {data().set(x,y);}
	data_type const& data() const {return static_cast<mother_type const*>(this)->_m_data;}
	data_type& data() {return static_cast<mother_type*>(this)->_m_data;}
};

template <typename t>
struct point_access_tt <t,rc_coord>
{
	typedef point_access_tt self;
	typedef t value_type;
	typedef dim_data_tt<t> data_type;
	typedef point_tt<t,rc_coord> mother_type;
	value_type& r() {return data().d0();}
	value_type& c() {return data().d1();}
	value_type const& r() const {return data().d0();}
	value_type const& c() const {return data().d1();}
	template<int i> value_type& d() {return data().d<i>();}
	template<int i> value_type const& d() const {return data().d<i>();}
	void set(value_type const& r,value_type const& c) {data().set(r,c);}
	data_type const& data() const {return static_cast<mother_type const*const>(this)->_m_data;}
	data_type& data() {return static_cast<mother_type*const>(this)->_m_data;}
};

template<typename t,typename coord_tn>
struct point_tt : point_access_tt<t,coord_tn>
{
	typedef point_tt self;
	typedef coord_tn coord_type;
	typedef t value_type;
	typedef dim_data_tt<t> data_type;
	typedef point_access_tt<t,coord_tn> access_t;
	
	self() { set(0,0); }
	self(value_type d0,value_type d1) { access_t::set(d0,d1); }
	data_type _m_data;
	bool operator==(self const& p) const { return _m_data==p._m_data; }
	bool operator!=(self const& p) const { return !operator==(p); }
};
typedef point_tt<int,rc_coord> rc_point_t;
typedef point_tt<int,xy_coord> xy_piont_t;
template<typename t,typename cd>
point_tt<t,cd> operator +(point_tt<t,cd> const& a,point_tt<t,cd> const& b)
{
	return point_tt<t,cd>(a.d<0>()+b.d<0>(),a.d<1>()+b.d<1>());
}
template<typename t,typename cd>
point_tt<t,cd> operator +(point_tt<t,cd> const& a, size_tt<2,t,cd> const& size)
{
	return point_tt<t,cd>(a.d<0>() + size.rc, a.d<1>() + size.cc);
}

typedef point_tt<int,rc_coord> rc_point_t;
typedef point_tt<int,xy_coord> xy_point_t;

template <typename t,typename coord_tn>
struct rect_tt
{
	typedef rect_tt self;
	typedef coord_tn coord_type;
	typedef point_tt<t,coord_type> point_type;
	typedef size_tt<2,t,coord_type> size_type;
	self() {}
	self(point_type const& p): p(p) {}
	self(size_type const& s): s(s) {}
	self(point_type const& p,size_type const& s): p(p),s(s) {}
	self(int v0,int v1,size_type const& s): p(v0,v1),s(s) {}
	self(point_type const& p,int l0,int l1): p(p),s(l0,l1) {}
	self(int v0,int v1,int l0,int l1): p(v0,v1),s(l0,l1) {}
	
	void set(point_type const& p) {this->p=p;}
	void set(size_type const& s) {this->s=s;}
	void set(point_type const& p,size_type const& s) {this->p=p;this->s=s;}
	void set(point_type const& p,size_t d0,size_t d1) {this->p=p;s.set(d0,d1);}
	void set(int v0,int v1,int l0,int l1) {p.set(v0,v1),s.set(l0,l1);}

	int left() const { return p.d<0>(); }
	int right() const { return left()+width(); }
	int top() const { return p.d<1>(); }
	int bottom() const { return top()+height(); }
	size_t width() const { return s.d<0>(); }
	size_t height() const { return s.d<1>(); }
	point_type left_top() const { return p; }
	point_type left_bottom() const { return point_type(left(),bottom()); }
	point_type right_top() const { return point_type(right(),top()); }
	point_type right_bottom() const { return point_type(right(),bottom()); }
	bool is_empty() const { return s.is_empty(); }
	point_type p;
	size_type s;
};

typedef rect_tt<int,rc_coord> rc_rect_t;
typedef rect_tt<int,xy_coord> xy_rect_t;


template <typename t>
struct anti_tt
{
	explicit anti_tt() {}
	explicit anti_tt(t const& v) : _m_value(v) {}
	operator t const& () const { return _m_value; }
	operator t () const { return _m_value; }
	anti_tt& set(t const& v) { _m_value = v; return *this; }
	t _m_value;
};
template <typename t,size_t size>
struct pod_buffer
{
	pod_buffer() {memset((void*)_m_buffer,0,sizeof(_m_buffer));}
	pod_buffer(t v) {fill(v);}
	void resize(size_t size) {assert(size<size);}
	t _m_buffer[size];
	t& operator[](size_t i) {return _m_buffer[i];}
	t const& operator[](size_t i) const {return _m_buffer[i];}
	void fill(t v) {for(size_t i=0;i<size;++i)_m_buffer[i]=v;}
};
template <typename t>
struct mem_buffer
{
	~mem_buffer() {delete[] _m_buffer;_m_buffer=0;_m_size=0;}
	mem_buffer() : _m_buffer(0),_m_size(0) {}
	mem_buffer(mem_buffer const& buf) : _m_buffer(0),_m_size(0) 
	{
		resize(buf._m_size);
		memmove(_m_buffer,buf._m_buffer,buf._m_size);
	}
	t* _m_buffer;
	size_t _m_size;
	void resize(size_t size)
	{
		if (_m_size>=size) return;
		t* pold = _m_buffer;
		size_t size_old = _m_size;
		_m_buffer = new (std::nothrow) t [size];
		_m_size = size;
		assert (_m_buffer);
		memmove(_m_buffer,pold,size_old);
		delete[] pold;
	}
	t& operator[](size_t i) {assert(i<_m_size);return _m_buffer[i];}
	t const& operator[](size_t i) const {assert(i<_m_size);return _m_buffer[i];}
	void fill(t v) {for(size_t i=0;i<size;++i)_m_buffer[i]=v;}
	mem_buffer& operator=(mem_buffer const& buff)
	{
		resize(buff._m_size);
		memmove(_m_buffer,buff._m_buffer,buff._m_size);
		return *this;
	}
};

struct hard_bind_tag;
struct soft_bind_tag;


template <typename t>
struct matrix_trait_helper
{
	typedef size_tt<2,int,rc_coord> ssize_t;
	typedef point_tt<int,rc_coord> point_t;
	typedef rect_tt<int,rc_coord> rect_t;
	typedef t value_type;
};

template <typename t,typename coord>
struct coord_conv_null
{
	typedef point_tt<t,coord> point_t;
	typedef rect_tt<t,coord> rect_t;
	point_t const& get(point_t const& p) {return p;}
	rect_t const& get(rect_t const& r) {return r;}
	point_t& get(point_t& p) {return p;}
	rect_t& get(rect_t& r) {return r;}
};

template <typename t,typename coord>
struct coord_conv_translational
{
	typedef coord_conv_translational self;
	typedef point_tt<t,coord> point_t;
	typedef rect_tt<t,coord> rect_t;
	self() {}
	self(point_t const& o) : _m_origin(o) {}
	point_t get(point_t const& p) {return _m_origin+p;}
	rect_t get(rect_t const& r) {return rect_t(get(r.p),r.s);}
	point_t& get(point_t& p) {p+=_m_origin;return p;}
	rect_t& get(rect_t& r) {r.p+=_m_origin;return r;}
	point_t _m_origin;
};

template <typename t,typename buffer=mem_buffer<t>>
struct matrix_tt;

template <typename t,typename buffer,typename tag>
struct matrix_access_method_tt;

template <typename matrix_tn>
struct matrix_access_tt;


template <typename t,typename buffer,typename tag>
struct get_derived_type;

template <typename t,typename buffer>
struct get_derived_type <t,buffer,hard_bind_tag>
{
	typedef matrix_tt<t,buffer> type;
};

template <typename t,typename buffer>
struct get_derived_type <t,buffer,soft_bind_tag>
{
	typedef matrix_access_tt<matrix_tt<t,buffer>> type;
};


template <typename t,typename buffer>
matrix_tt<t,buffer>* get_matrix(matrix_access_method_tt<t,buffer,hard_bind_tag>* access)
{
	typedef matrix_tt<t,buffer> matrix_type;
	matrix_type* m = static_cast<matrix_type*>(access);
	return m;
}
template <typename t,typename buffer>
matrix_tt<t,buffer> const* get_matrix(matrix_access_method_tt<t,buffer,hard_bind_tag> const* access)
{
	typedef matrix_tt<t,buffer> matrix_type;
	matrix_type const* m = static_cast<matrix_type const*>(access);
	return m;
}
template <typename t,typename buffer>
matrix_tt<t,buffer>* get_matrix(matrix_access_method_tt<t,buffer,soft_bind_tag>* assess_method)
{
	typedef matrix_tt<t,buffer> matrix_type;
	typedef matrix_access_tt<matrix_type> matrix_access_t;
	matrix_access_t* access = static_cast<matrix_access_t*>(assess_method);
	return access->_m_matrix;
}
template <typename t,typename buffer>
matrix_tt<t,buffer> const* get_matrix(matrix_access_method_tt<t,buffer,soft_bind_tag> const* assess_method)
{
	typedef matrix_tt<t,buffer> matrix_type;
	typedef matrix_access_tt<matrix_type> matrix_access_t;
	matrix_access_t const* access = static_cast<matrix_access_t const*>(assess_method);
	return access->_m_matrix;
}
template <typename t,typename buffer>
typename matrix_tt<t,buffer>::coord_conv_t get_coord(matrix_access_method_tt<t,buffer,hard_bind_tag>* access)
{
	return get_matrix(access)->coord_conv();
}
template <typename t,typename buffer>
typename matrix_tt<t,buffer>::coord_conv_t get_coord(matrix_access_method_tt<t,buffer,hard_bind_tag> const* access)
{
	return get_matrix(access)->coord_conv();
}
template <typename t,typename buffer>
typename matrix_access_tt<matrix_tt<t,buffer>>::coord_conv_t get_coord(matrix_access_method_tt<t,buffer,soft_bind_tag>* access_method)
{
	typedef matrix_tt<t,buffer> matrix_type;
	typedef matrix_access_tt<matrix_type> matrix_access_t;
	matrix_access_t* access = static_cast<matrix_access_t*>(access_method);
	return access->coord_conv();
}
template <typename t,typename buffer>
typename matrix_access_tt<matrix_tt<t,buffer>>::coord_conv_t get_coord(matrix_access_method_tt<t,buffer,soft_bind_tag> const* access_method)
{
	typedef matrix_tt<t,buffer> matrix_type;
	typedef matrix_access_tt<matrix_type> matrix_access_t;
	matrix_access_t const* access = static_cast<matrix_access_t const*>(access_method);
	return access->coord_conv();
}



template <typename t,typename buffer,typename bind_tag>
struct matrix_access_method_tt
{
	typedef matrix_tt<t,buffer> matrix_type;
	typedef bind_tag access_tag;
	typedef typename matrix_trait_helper<t>::value_type value_type;
	typedef typename matrix_trait_helper<t>::rect_t rect_t;
	typedef typename matrix_trait_helper<t>::ssize_t ssize_t;
	typedef typename matrix_trait_helper<t>::point_t point_t;

	/// no coord
	ssize_t size() const {matrix_type const* m = get_matrix(this);return m->_m_size;}
	size_t rc() const {return size().rc();}
	size_t cc() const {return size().cc();}
	void set_rc(size_t rc) {matrix_type* m = get_matrix(this);m->_m_size.rc()=rc;m->_m_buffer.resize(m->_m_size.length());}
	void set_cc(size_t cc) {matrix_type* m = get_matrix(this);m->_m_size.cc()=cc;m->_m_buffer.resize(m->_m_size.length());}
	void set_rcc(size_t rc,size_t cc) {resize(rc,cc);}
	size_t width() const {return cc();}
	size_t height() const {return rc();}
	void resize(size_t rc,size_t cc)
	{
		matrix_type* m = get_matrix(this);
		m->_m_size.set(rc,cc);
		m->_m_buffer.resize(m->_m_size.length());
	}

	point_t pointed(point_t const& p) const {return get_coord(this).get(p);}
	rect_t rected(rect_t const& rect) const { return get_coord(this).get(rect); }
	point_t pointed(int r,int c) { return get_coord(this).get(point_t(r,c)); }

	void fill(value_type const& v) {do_fill(v);}
	void fill(rect_t const& r,value_type const& v) {do_fill(rected(r),v);}
	void fill_row_by_row(value_type const* v,size_t size) {do_fill_row_by_row(v,size);}
	matrix_type get(rect_t const& rect) const { return do_get(rected(rect)); }
	value_type get(point_t const& p) const { return do_ref(pointed(p)); }
	value_type get(size_t r,size_t c) const { return get(point_t(r,c)); }
	void set(int r,int c,value_type v) { set(point_t(r,c),v); }
	void set(point_t const& p,value_type v) { do_ref(pointed(p)) = v; }

	void set_h(size_t r,size_t c,size_t size,value_type const& v) { set_h(point_t(r,c),size,v); }
	void set_h(point_t const& p,size_t size,value_type const& v) { do_set_h(pointed(p),size,v); }
	void set_h(size_t r,size_t c,size_t size,value_type const* p) { set_h(point_t(r,c),size,p); }
	void set_h(point_t const& p,size_t size,value_type const* v) { do_set_h(pointed(p),size,v); }
	void set_v(size_t r,size_t c,size_t size,value_type const& v) {set_v(point_t(r,c),size,v);}
	void set_v(point_t const& p,size_t size,value_type const& v) {do_set_v(pointed(p),size,v);}
	void set(rect_t const& rect,value_type const& v) { do_set(rected(rect),v); }
	value_type& ref(point_t const& p) { return do_ref(pointed(p)); }
	value_type& ref(size_t r,size_t c) { return ref(point_t(r,c)); }
	value_type const& ref(point_t const& p) const { return do_ref(pointed(p)); }
	value_type const& ref(size_t r,size_t c) const { return ref(point_t(r,c)); }
	/// actual there has 2 directions (r,c)
	void forward_copy(point_t const& from,point_t const& to,ssize_t const& size) { do_forward_copy(pointed(from),pointed(to),size); }
	void revert_copy(point_t const& from,point_t const& to,ssize_t const& size) { do_revert_copy(pointed(from),pointed(to),size); }
	void forward_copy_hor(point_t const& from,point_t const& to,int size) { do_forward_copy_hor(pointed(from),pointed(to),size); }
	void revert_copy_hor(point_t const& from,point_t const& to,int size) { do_revert_copy_hor(pointed(from),pointed(to),size); }


private:
	void do_fill(value_type const& v)
	{
		matrix_type* m = get_matrix(this);
		typedef typename matrix_type::size_type size_type;
		ssize_t const& size = m->_m_size;
		for (size_t i=0;i<size.rc();++i)
			for (size_t j=0;j<size.cc();++j)
				do_set(i,j,v);
	}
	void do_fill_row_by_row(value_type const* v,size_t size)
	{
		matrix_type* m = get_matrix<matrix_type>(this);
		typedef typename matrix_type::size_type size_type;
		ssize_t const& msize = m->_m_size;
		size_t n = 0;
		for (size_t i=0;i<msize.rc();++i)
			for (size_t j=0;j<msize.cc();++j)
			{
				if (++n>size) return;
				do_set(i,j,*v++);
			}
	}
	void do_fill(rect_t const& r, value_type const& v)
	{
		int bottom = r.bottom();
		int top = r.top();
		int right = r.right();
		int left = r.left();
		for (size_t i=top;i<bottom;++i)
			for (size_t j=left;j<right;++j)
				do_set(i,j,v);
	}
	matrix_type do_get(rect_t const& r) const
	{
		point_t p = r.p;
		point_t q;
		matrix_type m(r.s.rc(),r.s.cc());
		for (;q.r()<r.s.rc();++q.r(),++p.r())
		{
			q.c()=0; p.c()=r.p.c();
			for (;q.c()<r.s.cc();++q.c(),++p.c())
				m.set(q,do_get(p));
		}
		return m;
	}

	void do_set_h(size_t r,size_t c,size_t size, value_type const& v) {do_set_h(point_t(r,c),size,v);}
	void do_set_h(point_t const& p, size_t size, value_type const& v)
	{
		for (size_t i=0;i<size;++i) set(p+point_t(0,i),v);
	}
	void do_set_h(size_t r, size_t c, size_t size, value_type const* p) {do_set_h(point_t(r,c),size,p);}
	void do_set_h(point_t const& p, size_t size, value_type const* v)
	{
		for (size_t i=0;i<size;++i) do_set(p+point_t(0,i),*v++);
	}

	void do_set_v(size_t r, size_t c, size_t size, value_type const& v)
	{
		do_set_v(point_t(r,c),size,v);
	}
	void do_set_v(point_t const& p, size_t size, value_type const& v)
	{
		for (size_t i=0;i<size;++i) do_set(p+point_t(i,0),v);
	}
	void do_set(rect_t const& rect,value_type const& v)
	{
		for (size_t i=0;i<rect.s.rc();++i)
			do_set_h(rect.p+point_t(i,0),rect.s.cc(),v);
	}

	/// actual there has 2 directions (r,c)
	void do_forward_copy(point_t const& from, point_t const& to,ssize_t const& size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		point_t fe(from.r()+size.rc(), from.c() + size.cc());
		for (; f.r() < fe.r(); ++f.r(), ++t.r())
			do_forward_copy_hor(f, t, size.cc());
	}
	void do_revert_copy(point_t const& from, point_t const& to,ssize_t const& size)
	{
		if (from == to) return;
		point_t f (from.r() + size.rc() - 1,from.c());
		point_t t (to.r()+size.rc()-1,to.c());
		int rend = from.r()-1;
		for (; f.r() > rend; --f.r(), --t.r())
			do_revert_copy_hor(f, t, size.cc());
	}
	void do_forward_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		int cend = from.c() + size;
		for (; f.c()<cend; ++f.c(),++t.c())
			do_ref(t) = do_ref(f);
	}
	void do_revert_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f (from.r(),from.c()+size-1);
		point_t t (to.r(), to.c() + size - 1);
		point_t fe(from.r(), from.c()-1);
		for (; f.c()>fe.c(); --f.c(), --t.c())
			do_ref(t) = do_ref(f);
	}

	value_type do_get(point_t const& p) const {return do_ref(p);}
	value_type do_get(size_t r,size_t c) const {return do_ref(r,c);}
	void do_set(size_t r,size_t c,value_type v) {do_ref(r,c) = v;}
	void do_set(point_t const& p,value_type v) {do_ref(p) = v;}
	value_type& do_ref(point_t const& p)
	{
		matrix_type* m = get_matrix(this);
		ssize_t const& size = m->_m_size;
		assert(p.r()<size.rc() && p.c()<size.cc());
		return m->_m_buffer[p.r()*size.cc()+p.c()];
	}
	value_type const& do_ref(point_t const& p) const
	{
		matrix_type const* m = get_matrix(this);
		ssize_t const& size = m->_m_size;
		assert(p.r()<size.rc() && p.c()<size.cc());
		return m->_m_buffer[p.r()*size.cc()+p.c()];
	}
	value_type& do_ref(size_t r,size_t c) { return do_ref(point_t(r,c)); }
	value_type const& do_ref(size_t r,size_t c) const { return do_ref(point_t(r,c)); }
};



template <typename t,typename buffer>
struct matrix_tt : matrix_access_method_tt<t,buffer,hard_bind_tag>
{
	friend matrix_access_method_tt<t,buffer,hard_bind_tag>;
	friend matrix_access_method_tt<t,buffer,soft_bind_tag>;

	typedef matrix_tt self;
	typedef buffer buffer_type;
	typedef coord_conv_null<int,rc_coord> coord_conv_t;
	typedef typename matrix_trait_helper<t>::value_type value_type;
	typedef typename matrix_trait_helper<t>::rect_t rect_t;
	typedef typename matrix_trait_helper<t>::ssize_t ssize_t;
	typedef typename matrix_trait_helper<t>::point_t point_t;
	typedef size_t size_type;

	self() {}
	self(size_t rc,size_t cc)
	{
		resize(rc,cc);
	}
	coord_conv_t coord_conv() const {return coord_conv_t();}
private:
	buffer_type _m_buffer;
	ssize_t _m_size;
};




template <typename t,typename buffer>
struct matrix_access_tt <matrix_tt<t,buffer>>
	: matrix_access_method_tt<t,buffer,soft_bind_tag>
{
	friend matrix_access_method_tt<t,buffer,soft_bind_tag>;

	typedef matrix_access_tt self;
	typedef matrix_tt<t,buffer> matrix_t;
	typedef t value_type;
	typedef point_tt<int,rc_coord> point_t;
	typedef rect_tt<int,rc_coord> rect_t;
	typedef size_tt<2,int,rc_coord> ssize_t;
	typedef coord_conv_translational<int,rc_coord> coord_conv_t;
	matrix_t* _m_matrix;
	coord_conv_t _m_coord_conv;
	self(): _m_matrix(0) {}
	self(matrix_t* matrix): _m_matrix(matrix) {}
	self(matrix_t& matrix): _m_matrix(&matrix) {}
	self(matrix_t* matrix,point_t const& origin): _m_matrix(matrix),_m_origin(origin) {}
	self(matrix_t& matrix,point_t const& origin): _m_matrix(&matrix),_m_origin(origin) {}
	coord_conv_t coord_conv() const {return _m_coord_conv;}
	void set(matrix_t* m,point_t const& origin) { _m_matrix=m;_m_coord_conv._m_origin=origin; }
	void set(matrix_t& m,point_t const& origin) { _m_matrix=&m;_m_coord_conv._m_origin=origin; }
	using matrix_access_method_tt<t,buffer,soft_bind_tag>::set;
};


typedef matrix_tt<char,mem_buffer<char>> matrix2_t;

template <typename matrix>
matrix get_transposed(matrix const& m)
{
	matrix rb(m.cc(),m.rc());
	for (size_t c=0;c<m.cc();c++)
	{
		for (size_t r=0;r<m.rc();r++)
		{
			rb.set(c,m.rc()-r-1,m.get(r,c));
		}
	}
	return rb;
}