
#include "../../src/utl/intv.h"


#pragma once



typedef ox::utl::intv_tt<int> row_intv_t;
//typedef ox::utl::data_tt<interval_t> interval

template<typename t>
struct size2_t
{
	typedef t integer_type;
	static size_t const __dim_size = 2;
	size2_t() {set(0,0);}
	size2_t(integer_type rc, integer_type cc) { set(rc, cc); }
	void set(integer_type rc, integer_type cc) { this->rc = rc, this->cc = cc; }
	integer_type length() const {return rc*cc;}
	bool is_empty() const { return rc == 0 && cc == 0; }
	integer_type rc, cc;
};
typedef size2_t<int> ssize2_t;
template<typename t>
struct point_tt
{
	typedef point_tt self;
	typedef t integer_type;
	self() { set(0,0); }
	self(integer_type r,integer_type c) { set(r,c); }
	self& set(integer_type r,integer_type c) { this->r=r,this->c=c;return *this; }
	integer_type r,c;
	bool operator==(self const& p) const { return r == p.r && c == p.c; }
	bool operator!=(self const& p) const { return !operator==(p); }
};
typedef point_tt<int> point_t;
point_t operator +(point_t const& a,point_t const& b)
{
	return point_t(a.r+b.r,a.c+b.c);
}
point_t operator +(point_t const& a, ssize2_t const& size)
{
	return point_t(a.r + size.rc, a.c + size.cc);
}
struct rect_t
{
	rect_t() {}
	rect_t(point_t const& p): p(p) {}
	rect_t(ssize2_t const& s): s(s) {}
	rect_t(point_t const& p,ssize2_t const& s): p(p),s(s) {}
	rect_t(point_t const& p,size_t rc,size_t cc): p(p),s(rc,cc) {}
	rect_t(size_t r,size_t c,size_t rc,size_t cc): p(r,c),s(rc,cc) {}
	
	struct rc_t
	{
		rect_t const& rect;
		rc_t(rect_t const& r) :rect(r) {}
		point_t left_top() const { rect.p; }
		point_t left_bottom() const { return point_t(rect.p.r + rect.s.rc, rect.p.c); }
		point_t right_top() const { return point_t(rect.p.r, rect.p.c + rect.s.cc); }
		point_t right_bottom() const { return point_t(rect.p.r+rect.s.rc, rect.p.c + rect.s.cc); }
	};
	void set(point_t const& p) {this->p=p;}
	void set(ssize2_t const& s) {this->s=s;}
	void set(point_t const& p,ssize2_t const& s) {this->p=p;this->s=s;}
	void set(point_t const& p,size_t rc,size_t cc) {this->p=p;s.set(rc,cc);}
	void set(size_t r,size_t c,size_t rc,size_t cc) {p.set(r,c),s.set(rc,cc);}

	int left() const { return p.c; }
	int right() const { return p.c+s.cc; }
	int top() const { return p.r; }
	int bottom() const { return p.r+s.rc; }
	size_t width() const { return s.cc; }
	size_t height() const { return s.rc; }
	rc_t rc() const { return rc_t(*this); }
	point_t left_top() const { return p; }
	point_t left_bottom() const { return point_t(p.r+s.rc,p.c); }
	point_t right_top() const { return point_t(p.r, p.c + s.cc); }
	point_t right_bottom() const { return point_t(p.r+s.rc, p.c + s.cc); }
	bool is_empty() const { return s.is_empty(); }
	point_t p;
	ssize2_t s;
};


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
template <typename t,typename buffer>
struct matrix_tt
{
	typedef t value_type;
	matrix_tt() {}
	matrix_tt(size_t rc,size_t cc)
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
		matrix_tt m(rect.s.rc,rect.s.cc);
		for (;q.r<rect.s.rc;++q.r,++p.r)
		{
			q.c=0; p.c=rect.p.c;
			for (;q.c<rect.s.cc;++q.c,++p.c)
				m.set(q,get(p));
		}
		return m;
	}
	ssize2_t size() const {return _m_size;}
	size_t rc() const {return _m_size.rc;}
	size_t cc() const {return _m_size.cc;}
	void set_rc(size_t rc) {_m_size.rc=rc;_m_buffer.resize(_m_size.length());}
	void set_cc(size_t cc) {_m_size.cc=cc;_m_buffer.resize(_m_size.length());}
	void set_rcc(size_t rc,size_t cc) {_m_size.set(rc,cc);_m_buffer.resize(_m_size.length());}
	size_t width() const {return _m_size.cc;}
	size_t height() const {return _m_size.rc;}
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
		assert(p.r<_m_size.rc && p.c<_m_size.cc);
		return _m_buffer[p.r*_m_size.cc+p.c];
	}
	value_type const& ref(point_t const& p) const
	{
		assert(p.r<_m_size.rc && p.c<_m_size.cc);
		return _m_buffer[p.r*_m_size.cc+p.c];
	}
	value_type& ref(size_t r,size_t c) { return ref(point_t(r,c)); }
	value_type const& ref(size_t r,size_t c) const { return ref(point_t(r,c)); }
	/// actual there has 2 directions (r,c)
	void forward_copy(point_t const& from, point_t const& to,ssize2_t const& size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		point_t fe(from.r+size.rc, from.c + size.cc);
		for (; f.r < fe.r; ++f.r, ++t.r)
			forward_copy_hor(f, t, size.cc);
	}
	void revert_copy(point_t const& from, point_t const& to,ssize2_t const& size)
	{
		if (from == to) return;
		point_t f (from.r + size.rc - 1,from.c);
		point_t t (to.r+size.rc-1,to.c);
		int rend = from.r-1;
		for (; f.r > rend; --f.r, --t.r)
			revert_copy_hor(f, t, size.cc);
	}
	void forward_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		int cend = from.c + size;
		for (; f.c<fe.c; ++f.c,++t.c)
			ref(t) = ref(f);
	}
	void revert_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f {from.r,from.c+size-1};
		point_t t {to.r, to.c + size - 1};
		point_t fe(from.r, from.c-1);
		for (; f.c>fe.c; --f.c, --t.c)
			ref(t) = ref(f);
	}
private:
	buffer _m_buffer;
	ssize2_t _m_size;
};

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