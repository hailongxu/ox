
#include <assert.h>
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
	void set(value_type const& x,value_type const& y) {data().set(x,y);}
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
	size_t width() const { return s.d0(); }
	size_t height() const { return s.d1(); }
	point_type left_top() const { return p; }
	point_type left_bottom() const { return point_type(left(),bottom()); }
	point_type right_top() const { return point_type(right(),top()); }
	point_type right_bottom() const { return point_type(right(),bottom()); }
	bool is_empty() const { return s.is_empty(); }
	point_type p;
	size_type s;
};

typedef rect_tt<int,rc_coord> rc_rect_t;


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

template <typename t,typename buffer=mem_buffer<t>>
struct matrix_tt
{
	typedef matrix_tt self;
	typedef size_tt<2,int,rc_coord> ssize_t;
	typedef point_tt<int,rc_coord> point_t;
	typedef rect_tt<int,rc_coord> rect_t;

	typedef t value_type;
	self() {}
	self(size_t rc,size_t cc)
	{
		resize(rc,cc);
	}
	void resize(size_t rc,size_t cc)
	{
		_m_size.set(rc,cc);
		_m_buffer.resize(_m_size.length());
	}
	void fill(t const& v)
	{
		for (size_t i=0;i<_m_size.rc;++i)
			for (size_t j=0;j<_m_size.cc;++j)
				set(i,j,v);
	}
	void fill_row_by_row(t const* v,size_t size)
	{
		size_t n = 0;
		for (size_t i=0;i<_m_size.rc;++i)
			for (size_t j=0;j<_m_size.cc;++j)
			{
				if (++n>size) return;
				set(i,j,*v++);
			}
	}
	void fill(rect_t const& r, t const& v)
	{
		int bottom = r.bottom();
		int top = r.top();
		int right = r.right();
		int left = r.left();
		for (size_t i=top;i<bottom;++i)
			for (size_t j=left;j<right;++j)
				set(i,j,v);
	}
	matrix_tt get(rect_t const& rect) const
	{
		point_t p = rect.p;
		point_t q;
		matrix_tt m(rect.s.rc(),rect.s.cc());
		for (;q.r<rect.s.rc;++q.r,++p.r)
		{
			q.c=0; p.c=rect.p.c;
			for (;q.c<rect.s.cc;++q.c,++p.c)
				m.set(q,get(p));
		}
		return m;
	}
	ssize_t size() const {return _m_size;}
	size_t rc() const {return _m_size.rc();}
	size_t cc() const {return _m_size.cc();}
	void set_rc(size_t rc) {_m_size.rc()=rc;_m_buffer.resize(_m_size.length());}
	void set_cc(size_t cc) {_m_size.cc()=cc;_m_buffer.resize(_m_size.length());}
	void set_rcc(size_t rc,size_t cc) {_m_size.set(rc,cc);_m_buffer.resize(_m_size.length());}
	size_t width() const {return _m_size.cc();}
	size_t height() const {return _m_size.rc();}
	value_type get(point_t const& p) const {return ref(p);}
	value_type get(size_t r,size_t c) const {return ref(r,c);}
	void set(size_t r,size_t c,value_type v) {ref(r,c) = v;}
	void set(point_t const& p,value_type v) {ref(p) = v;}

	void set_h(size_t r,size_t c,size_t size, value_type const& v) {set_h(point_t(r,c),size,v);}
	void set_h(point_t const& p, size_t size, value_type const& v)
	{
		for (size_t i=0;i<size;++i) set(p+point_t(0,i),v);
	}
	void set_h(size_t r, size_t c, size_t size, value_type const* p) {set_h(point_t(r,c),size,p);}
	void set_h(point_t const& p, size_t size, value_type const* v)	{
		for (size_t i=0;i<size;++i) set(p+point_t(0,i),*v++);
	}

	void set_v(size_t r, size_t c, size_t size, value_type const& v)
	{
		set_v(point_t(r,c),size,v);
	}
	void set_v(point_t const& p, size_t size, value_type const& v)
	{
		for (size_t i=0;i<size;++i) set(p+point_t(i,0),v);
	}
	void set(rect_t const& rect,value_type const& v)
	{
		for (size_t i=0;i<rect.s.rc;++i)
			set_h(rect.p+point_t(i,0),rect.s.cc,v);
	}
	value_type& ref(point_t const& p)
	{
		assert(p.r()<_m_size.rc() && p.c()<_m_size.cc());
		return _m_buffer[p.r()*_m_size.cc()+p.c()];
	}
	value_type const& ref(point_t const& p) const
	{
		assert(p.r()<_m_size.rc() && p.c()<_m_size.cc());
		return _m_buffer[p.r()*_m_size.cc()+p.c()];
	}
	value_type& ref(size_t r,size_t c) { return ref(point_t(r,c)); }
	value_type const& ref(size_t r,size_t c) const { return ref(point_t(r,c)); }
	/// actual there has 2 directions (r,c)
	void forward_copy(point_t const& from, point_t const& to,ssize_t const& size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		point_t fe(from.r()+size.rc(), from.c() + size.cc());
		for (; f.r() < fe.r(); ++f.r(), ++t.r())
			forward_copy_hor(f, t, size.cc());
	}
	void revert_copy(point_t const& from, point_t const& to,ssize_t const& size)
	{
		if (from == to) return;
		point_t f (from.r() + size.rc() - 1,from.c());
		point_t t (to.r()+size.rc()-1,to.c());
		int rend = from.r()-1;
		for (; f.r() > rend; --f.r(), --t.r())
			revert_copy_hor(f, t, size.cc());
	}
	void forward_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		int cend = from.c() + size;
		for (; f.c()<cend; ++f.c(),++t.c())
			ref(t) = ref(f);
	}
	void revert_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f (from.r(),from.c()+size-1);
		point_t t (to.r(), to.c() + size - 1);
		point_t fe(from.r(), from.c()-1);
		for (; f.c()>fe.c(); --f.c(), --t.c())
			ref(t) = ref(f);
	}
private:
	buffer _m_buffer;
	ssize_t _m_size;
};
#if 0

template <typename matrix_tn>
struct matrix_access;

enum struct direction_enum
{
	__negative=-1, /// relative to the original coordination
	__positive=1, /// relative to the original coordination
};
template <typename t,typename buffer>
struct matrix_access <matrix_tt<t,buffer>>
{
	typedef matrix_access self;
	typedef matrix_tt<t,buffer> matrix_t;
	typedef t value_type;
	matrix_t* _m_matrix;
	point_t _m_origin;
	direction_enum _m_row_d; /// not using so far
	direction_enum _m_col_d; /// not using so far
	self(): _m_matrix(0) { _m_col_d=_m_row_d=direction_enum::__positive; }
	self(matrix_t* matrix): _m_matrix(matrix) { _m_col_d=_m_row_d=direction_enum::__positive; }
	self(matrix_t& matrix): _m_matrix(&matrix) { _m_col_d=_m_row_d=direction_enum::__positive; }
	self(matrix_t* matrix,point_t const& origin): _m_matrix(matrix),_m_origin(origin) { _m_col_d=_m_row_d=direction_enum::__positive; }
	self(matrix_t& matrix,point_t const& origin): _m_matrix(&matrix),_m_origin(origin) { _m_col_d=_m_row_d=direction_enum::__positive; }
	void set(matrix_t* m,point_t const& origin) { _m_matrix=m;_m_origin=origin; }
	void set(matrix_t& m,point_t const& origin) { _m_matrix=&m;_m_origin=origin; }
	matrix_t& matrix() { return *_m_matrix; }
	matrix_t const& matrix() const { return *_m_matrix; }
	point_t pointed(point_t const& p) const { return p+_m_origin; }
	rect_t rected(rect_t const& rect) const { return rect_t(pointed(rect.p),rect.s); }
	point_t pointed(int r,int c) { return point_t(r,c)+_m_origin; }

	size_t rc() const { return _m_matrix->rc(); }
	size_t cc() const { return _m_matrix->cc(); }
	point_t const& offset() const { return _m_origin; }
	//int offset_r() const { return _m_origin.r; }
	//int offset_c() const { return _m_origin.c; }

	void fill(value_type const& v) {_m_matrix->fill(v);}
	void fill(rect_t const& r,value_type const& v) {_m_matrix->fill(rected(r),v);}
	void fill_row_by_row(value_type const* v,size_t size) {_m_matrix->fill_row_by_row(v,size);}
	matrix_t get(rect_t const& rect) const { return _m_matrix->get(rected(rect)); }
	void set_rc(size_t rc) { _m_matrix->set_rc(rc); }
	void set_cc(size_t cc) { _m_matrix->set_cc(cc); }
	void set_rcc(size_t rc,size_t cc) { _m_matrix->set_rcc(rc,cc); }
	size_t width() const { return _m_matrix->width(); }
	size_t height() const { return _m_matrix->height(); }
	value_type get(point_t const& p) const { return _m_matrix->ref(pointed(p)); }
	value_type get(size_t r,size_t c) const { return get(point_t(r,c)); }
	void set(int r,int c,value_type v) { set(point_t(r,c),v); }
	void set(point_t const& p,value_type v) { _m_matrix->ref(pointed(p)) = v; }

	void set_h(size_t r,size_t c,size_t size,value_type const& v) { set_h(point_t(r,c),size,v); }
	void set_h(point_t const& p,size_t size,value_type const& v) { _m_matrix->set_h(pointed(p),size,v); }
	void set_h(size_t r,size_t c,size_t size,value_type const* p) { set_h(point_t(r,c),size,p); }
	void set_h(point_t const& p,size_t size,value_type const* v)	{ _m_matrix->set_h(pointed(p),size,v); }
	void set_v(size_t r,size_t c,size_t size,value_type const& v) {set_v(point_t(r,c),size,v);}
	void set_v(point_t const& p,size_t size,value_type const& v) {_m_matrix->set_v(pointed(p),size,v);}
	void set(rect_t const& rect,value_type const& v) { _m_matrix->set(rected(rect),v); }
	value_type& ref(point_t const& p) { return _m_matrix->ref(pointed(p)); }
	value_type& ref(size_t r,size_t c) { return ref(point_t(r,c)); }
	value_type const& ref(point_t const& p) const { return _m_matrix->ref(pointed(p)); }
	value_type const& ref(size_t r,size_t c) const { return ref(point_t(r,c)); }
	/// actual there has 2 directions (r,c)
	void forward_copy(point_t const& from,point_t const& to,ssize2_t const& size) { _m_matrix->forward_copy(pointed(from),pointed(to),size); }
	void revert_copy(point_t const& from,point_t const& to,ssize2_t const& size) { _m_matrix->revert_copy(pointed(from),pointed(to),size); }
	void forward_copy_hor(point_t const& from,point_t const& to,int size) { _m_matrix->forward_copy_hor(pointed(from),pointed(to),size); }
	void revert_copy_hor(point_t const& from,point_t const& to,int size) { _m_matrix->revert_copy_hor(pointed(from),pointed(to),size); }
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
#endif