// tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory.h>
#include <assert.h>
#include <vector>
#include "../../src/utl/intv.h"


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
	void fill(rect_t const& r, t const& v)
	{
		int bottom = r.bottom();
		int right = r.right();
		for (size_t i=r.r;i<bottom;++i)
			for (size_t j=0;j<right;++j)
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
		point_t fe(from.r-1, from.c-1);
		for (; f.r > fe.r; --f.r, --t.r)
			revert_copy_hor(f, t, size.cc);
	}
	void forward_copy_hor(point_t const& from, point_t const& to, int size)
	{
		if (from == to) return;
		point_t f = from;
		point_t t = to;
		point_t fe(from.r, from.c + size);
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
	//size_t _m_rc,_m_cc;
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

struct box_t : matrix2_t
{
	typedef char value_type;
	typedef matrix2_t matrix_t;
	box_t()
	{
		memset(rotate_check_point,-1,sizeof(rotate_check_point));
	}
	box_t(size_t rc,size_t cc): matrix_t(rc,cc)
	{
		memset(rotate_check_point,-1,sizeof(rotate_check_point));
	}
	static value_type const __back_char = '.';
	static value_type const __front_char = '$';
	static value_type null_char() {return __back_char;}
	static value_type real_char() { return __front_char; }
	void fill() {matrix_t::fill(__back_char);}
	matrix_t const& matrix() const {return *this;}
	point_t next;
	point_t rotate_check_point[4*4];
	static bool is_value_null(char v) {return v==__back_char;}
	bool is_null(point_t const& p) const {return is_value_null(get(p));}
	bool is_null(size_t r,size_t c) const {return is_value_null(get(r,c));}
};

struct box_board
{
	typedef matrix2_t matrix_t;
	typedef matrix_access<matrix_t> access_t;

	box_board()
	{
		_m_user_size.set(20,10);
		_m_origin.set(4,4);
		_m_board.resize(28,18);
		_m_access.set(_m_board,_m_origin);
	}
	ssize2_t _m_user_size;
	matrix_t _m_board;
	point_t _m_origin;
	access_t _m_access;
	//matrix_t const& matrix() const {return _m_board;}
	//matrix_t& matrix() {return _m_board;}
	access_t& access() {return _m_access;}
	point_t const& o() const { return _m_origin; }
	void init()
	{
		_m_board.set(user_rect(),'.');
		_m_board.set(rect_t(point_t(0,0),ssize2_t(4,18)),'.');
		_m_board.set(rect_t(point_t(4,0),ssize2_t(24,4)),'@');
		_m_board.set(rect_t(point_t(4,14),ssize2_t(24,4)),'@');
		_m_board.set(rect_t(point_t(24,4),ssize2_t(4,10)),'@');
	}
	rect_t user_rect() const {return rect_t(point_t(4,4),ssize2_t(20,10));}
	void revert_copy_row(size_t r1begin,size_t r2begin,size_t size)
	{
		_m_access.revert_copy(point_t(r1begin,0), point_t(r2begin,0), ssize2_t(size, 10));
	}
	//void remove_row_with_top_down(size_t rbegin, size_t rend)
	//{
	//	copy_row(rbegin,rend);
	//	_m_board.set(rect_t(o(),ssize2_t(rend-rbegin,10)),'.');
	//}
	void remove_row_with_top_down(int row)
	{
		revert_copy_row(0,1,row);
		_m_access.set_h(0,0,10,'.');
	}
	void remove_row_with_top_down(int rbegin,int rend)
	{
		revert_copy_row(0,rend-rbegin,rbegin);
		_m_access.set(rect_t(rbegin,0,rend-rbegin,10),'.');
	}
	void remove_row_with_top_down(row_intv_t const* row,int n)
	{
		assert(n>=0);
		for (int i=0;i<n;++i)
			remove_row_with_top_down((*row).begin,(*row).end);
	}
	size_t get_each_row_full_list(rect_t const& rect,row_intv_t* out,int n)
	{
		bool is_last_full = false;
		size_t count = 0, intv_count=0;
		for (int r=0;r<rect.s.rc;++r)
		{
			if (is_row_full(r,rect.left(),rect.right()))
			{
				assert(intv_count<n);
				count ++;
				row_intv_t& row = *out;
				if (!is_last_full)
				{
					intv_count ++;
					row.begin = r+rect.p.r, count++;
					row.end = row.begin+1;
				}
				else
					row.end++;
				is_last_full = true;
			}
			else // the end of successful intv end
			{
				if (is_last_full) out++;
				is_last_full = false;
			}
		}
		return count;
	}
	bool is_row_full(int r,int c1,int c2)
	{
		for (int c=c1;c<c2;++c)
			if (box_t::is_value_null(access().get(r,c))) return false;
		return true;
	}
	size_t get_each_row_full_list(int r1,int r2,row_intv_t* out,int n)
	{
		return get_each_row_full_list(rect_t(r1,0,r2,_m_user_size.cc),out,n);
	}
	rect_t get_row_rect(int row)
	{
		return rect_t(row,0,1,_m_user_size.cc);
	}
	rect_t get_row_rect(int row_begin,int row_end)
	{
		assert(row_end>=row_begin);
		return rect_t(row_begin,0,row_end-row_begin,10);
	}
};

struct ibox_t
{
	ibox_t():i(0),j(0) {}
	ibox_t(int _i,int _j): i(_i),j(_j) {}
	int i;
	int j;
	int index() const {return i*4+j;}
	bool operator==(ibox_t const& other) const {return i==other.i && j==other.j;}
	bool operator!=(ibox_t const& other) const {return !operator==(other);}
};
struct boxes_t
{
	typedef std::vector<box_t> box_vector;
	box_vector _m_boxes;
	void resize(size_t size)
	{
		_m_boxes.resize(size*4);
	}
	size_t size() const
	{
		assert((_m_boxes.size()&3)==0);
		return _m_boxes.size()/4;
	}
	//box_t& box(size_t i,size_t j)
	//{
	//	assert(4*i+j < _m_boxes.size());
	//	return _m_boxes[4*i+j];
	//}
	//box_t const& box(size_t i,size_t j) const
	//{
	//	return _m_boxes[4*i+j];
	//}
	box_t& box(ibox_t const& i)
	{
		assert(i.index() < _m_boxes.size());
		return _m_boxes[i.index()];
	}
	box_t const& box(ibox_t const& i) const
	{
		assert(i.index() < _m_boxes.size());
		return _m_boxes[i.index()];
	}
	static int get_next_subi(int subi)
	{
		assert(subi>=0);
		return (subi+1)%4;
	}
	static ibox_t get_next(ibox_t const& i)
	{
		return ibox_t{i.i,get_next_subi(i.j)};
	}
};

struct tetris_t
{
	typedef box_board::matrix_t matrix_t;
	typedef box_board::access_t access_t;
	boxes_t _m_boxes;
	box_board _m_board;
	void init()
	{
		_m_boxes.resize(2);
		//#0
		/*
			**
			 **
		*/
		{
			static int const i = 0;
			box_t& box0 = _m_boxes.box(ibox_t{i,0});
			box0.set_rcc(2,3);
			box0.fill();
			box0.set_h(0,0,2,'@');
			box0.set_h(1,1,2,'@');
			box0.next.set(-1,0);
			box0.rotate_check_point[0].set(-1,0);
			box0.rotate_check_point[1].set(-1,1);
			box0.rotate_check_point[2].set(-1,2);
			box0.rotate_check_point[3].set(0,2);
			box_t& box1 = _m_boxes.box(ibox_t{i,1});
			box1 = get_transposed(box0);
			box1.next.set(1,0);
			box0.rotate_check_point[0].set(0,0);
			box0.rotate_check_point[1].set(0,1);
			box_t& box2 = _m_boxes.box(ibox_t{i,2});
			box2 = get_transposed(box1);
			box2.next.set(-1,0);
			box0.rotate_check_point[0].set(-1,0);
			box0.rotate_check_point[1].set(-1,1);
			box0.rotate_check_point[2].set(-1,2);
			box0.rotate_check_point[3].set(0,2);
			box_t& box3 = _m_boxes.box(ibox_t{i,3});
			box3 = get_transposed(box2);
			box3.next.set(1,0);
			box0.rotate_check_point[0].set(0,0);
			box0.rotate_check_point[1].set(0,1);
		}
		/// # 1
		{
			static int const i = 1;
			box_t& box0 = _m_boxes.box(ibox_t{i,0});
			box0.set_rcc(1,4);
			box0.fill();
			box0.set_h(0,0,4,'@');
			box0.next.set(-2,2);
			box0.rotate_check_point[0].set(-2,0);
			box0.rotate_check_point[1].set(-2,1);
			box0.rotate_check_point[2].set(-1,0);
			box0.rotate_check_point[3].set(-1,1);
			box0.rotate_check_point[4].set(1,3);
			box_t& box1 = _m_boxes.box(ibox_t{i,1});
			box1 = get_transposed(box0);
			box1.next.set(2,-2);
			box0.rotate_check_point[0].set(0,1);
			box0.rotate_check_point[1].set(1,1);
			box0.rotate_check_point[2].set(3,-2);
			box0.rotate_check_point[3].set(3,-1);
			box_t& box2 = _m_boxes.box(ibox_t{i,2});
			box2 = get_transposed(box1);
			box2.next.set(-2,2);
			box0.rotate_check_point[0].set(-2,0);
			box0.rotate_check_point[1].set(-2,1);
			box0.rotate_check_point[2].set(-1,0);
			box0.rotate_check_point[3].set(-1,1);
			box0.rotate_check_point[4].set(1,3);
			box_t& box3 = _m_boxes.box(ibox_t{i,3});
			box3 = get_transposed(box2);
			box3.next.set(2,-2);
			box0.rotate_check_point[0].set(0,1);
			box0.rotate_check_point[1].set(1,1);
			box0.rotate_check_point[2].set(3,-2);
			box0.rotate_check_point[3].set(3,-1);
		}
		_m_board.init();
	}
	boxes_t& boxes() {return _m_boxes;}
	box_t const& box(ibox_t const& i) const { return _m_boxes.box(i); }
	rect_t user_rect() const {return _m_board.user_rect();}
	template <typename t,typename bu1,typename bu2>
	static void clear(matrix_tt<t,bu1>& md,point_t pd,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		set(md,pd,box_t::null_char(),ms,ps);
	}
	template <typename m,typename t,typename bu2>
	static void clear(matrix_access<m>& md,point_t pd,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		set(md,pd,box_t::null_char(),ms,ps);
	}
	template <typename t,typename bu1,typename bu2>
	static void set(matrix_tt<t,bu1>& md,point_t pd,char const& v,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		point_t s = ps;
		point_t d = pd;
		for (;s.r<ms.rc();++s.r,++d.r)
		{
			s.c = ps.c;
			d.c = pd.c;
			for (;s.c<ms.cc();++s.c,++d.c)
				if (!box_t::is_value_null(ms.get(s)))
					md.set(d,v);
		}
	}
	template <typename m,typename t,typename bu2>
	static void set(matrix_access<m>& md,point_t pd,char const& v,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		point_t s = ps;
		point_t d = pd;
		for (;s.r<ms.rc();++s.r,++d.r)
		{
			s.c = ps.c;
			d.c = pd.c;
			for (;s.c<ms.cc();++s.c,++d.c)
				if (!box_t::is_value_null(ms.get(s)))
					md.set(d,v);
		}
	}
	template <typename m,typename t,typename bu2>
	static void set(matrix_access<m>& md,point_t pd,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		typedef typename matrix_access<m>::value_type value_type;
		point_t s = ps;
		point_t d = pd;
		for (;s.r<ms.rc();++s.r,++d.r)
		{
			s.c = ps.c;
			d.c = pd.c;
			for (;s.c<ms.cc();++s.c,++d.c)
			{
				value_type v = ms.get(s);
				if (box_t::is_value_null(v)) continue;
				md.set(d,v);
			}
		}
	}
	static bool is_enabled(box_t const& box,matrix_t const& matrix,point_t const& p)
	{
		bool ret = true;
		point_t s = p;
		point_t b;
		for (;b.r<box.rc();++b.r,++s.r)
		{
			b.c=0; s.c=p.c;
			for (;b.c<box.cc();++b.c,++s.c)
			{
				char v = box.get(b);
				if (box_t::is_value_null(v)) continue;
				if (box_t::is_value_null(matrix.get(s))) continue;
				ret = false;
				break;
			}
		}
		return ret;
	}
	bool is_down_enabled(box_t const& box,point_t const& p)
	{
		matrix_t m = _m_board.access().get(rect_t(p,box.rc()+1,box.cc()));
		clear(m,point_t(0,0),box.matrix(),point_t(0,0));
		return is_enabled(box,m,point_t(1,0));
	}
	bool is_down_enabled(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_down_enabled(box,p);
	}
	bool is_left_enabled(box_t const& box,point_t const& p)
	{
		matrix_t m = _m_board.access().get(rect_t(point_t(p.r,p.c-1),ssize2_t(box.rc(),box.cc()+1)));
		clear(m,point_t(0,1),box.matrix(),point_t(0,0));
		return is_enabled(box,m,point_t(0,0));
	}
	bool is_left_enabled(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_left_enabled(box,p);
	}
	bool is_right_enabled(box_t const& box,point_t const& p)
	{
		matrix_t m = _m_board.access().get(rect_t(p,ssize2_t(box.rc(),box.cc()+1)));
		clear(m,point_t(0,0),box.matrix(),point_t(0,0));
		return is_enabled(box,m,point_t(0,1));
	}
	bool is_right_enabled(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_right_enabled(box,p);
	}

	static bool is_rotate_enabled(box_t const& box,access_t const& matrix,point_t const& p)
	{
		bool bret = true;
		point_t const* check = box.rotate_check_point;
		for (int i=0;check[i]!=point_t(-1,-1);++i)
		{
			if (!box_t::is_value_null(matrix.get(p+check[i])))
			{
				bret = false;
				break;
			}
		}
		return bret;
	}
	bool is_rotate_enabled(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(boxes_t::get_next(i));
		return is_rotate_enabled(box,_m_board.access(),p);
	}
	point_t move_rotate(ibox_t const& i,point_t const& p,ibox_t& in)
	{
		if (!is_rotate_enabled(i,p))
			return p;
		box_t const& box = _m_boxes.box(i);
		in = boxes_t::get_next(i);
		box_t const& box_next = _m_boxes.box(in);
		point_t r=p;
		point_t n=p+box.next;
		return n;
	}
	point_t move_down(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_down_enabled(box, p)) return p;
		point_t r = p; r.r++;
		return r;
	}
	point_t move_left(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_left_enabled(box, p))
			return p;
		point_t r = p; r.c--;
		return r;
	}
	point_t move_right(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_right_enabled(box, p))
			return p;
		point_t r = p; r.c++;
		return r;
	}
	void set_by(ibox_t const& i,point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		set(_m_board.access(), p, box.matrix(), point_t(0, 0));
	}
	void remove_rows(row_intv_t const* rows,size_t n)
	{
		_m_board.remove_row_with_top_down(rows,n);
	}
};

#include <time.h>

struct random
{
	void init()
	{
		srand(time(0));
	}
	int get() const
	{
		return rand();
	}
};
#include "../../src/cxx/delegate.h"
struct data_view_t
{
	typedef delegate<char(point_t const&)> get_value_d;
	typedef delegate<ssize2_t()> get_size_d;
	get_value_d value;
	get_size_d size;
};


struct box_trace_t
{
	ibox_t ibox;
	rect_t rect;
	//point_t pos;
};
struct drive
{
	typedef drive self;
	tetris_t _m_tetris;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	ibox_t _m_index;
	point_t _m_pos;
	random _m_random;
	typedef delegate<void(rect_t const* board_invalid,size_t size)> data_changed_d;
	typedef delegate<void(box_trace_t const& from,box_trace_t const& to)> trace_changed_d;
	data_changed_d on_data_changed;
	trace_changed_d on_trace_changed;

	point_t const& area_origin() const
	{
		return _m_tetris._m_board._m_origin;
	}
	ibox_t active_ibox() const
	{
		return _m_index;
	}
	box_t const& active_box() const
	{
		return _m_tetris.box(_m_index);
	}
	point_t const& active_point() const
	{
		return _m_pos;
	}
	void on_init()
	{
		_m_tetris.init();
		_m_random.init();
	}
	void on_move_down()
	{
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris.move_down(_m_index, _m_pos);
		box_t const& box = _m_tetris.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (from.rect.p!=to.rect.p)
		{
			_m_pos = to.rect.p;
			if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
			return;
		}
		/// reach the bottom
		/// scan the rows, remove it if full, finally trigger the data-changed event
		_m_tetris.set_by(from.ibox,from.rect.p);
		static int const __max_box_height = 4;
		row_intv_t row_full_list[__max_box_height] = {row_intv_t(),row_intv_t(),row_intv_t(),row_intv_t()};
		size_t count = _m_tetris._m_board.get_each_row_full_list(from.rect.top(),from.rect.bottom(),row_full_list,__max_box_height);
		assert(count<=__max_box_height);
		_m_tetris.remove_rows(row_full_list,count);
		//rect_t row_rect_full_list[__max_box_height];
		rect_t rect_full_list[1+__max_box_height] = {from.rect};
		for (size_t i=0;i<count;++i)
		{
			row_intv_t const& intv = row_full_list[i];
			rect_full_list[i-1] = _m_tetris._m_board.get_row_rect(intv.begin,intv.end);
		}
		if (on_data_changed.is_empty()) on_data_changed(rect_full_list,count);
		_m_pos.set(0,0);
	}
	void on_move_left()
	{
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris.move_left(_m_index, _m_pos);
		if (from.rect.p==to.rect.p) return;
		_m_pos = to.rect.p;
		box_t const& box = _m_tetris.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void on_move_right()
	{
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris.move_right(_m_index, _m_pos);
		if (from.rect.p==to.rect.p) return;
		_m_pos = to.rect.p;
		box_t const& box = _m_tetris.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void on_move_rotate()
	{
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to;
		to.rect.p = _m_tetris.move_rotate(_m_index, _m_pos,to.ibox);
		if (from.rect.p==to.rect.p && from.ibox==to.ibox) return;
		_m_pos = to.rect.p;
		_m_index = to.ibox;
		box_t const& box = _m_tetris.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void on_get_another()
	{
		ibox_t i;
		get_random_next(i);
		_m_pos.set(0,0);
	}
	void get_random_next(ibox_t& index)
	{
		int i = _m_random.get()%(_m_tetris._m_boxes.size());
		int si = _m_random.get()%4;
		index.i = 0;
		index.j = 0;
	}
};

#include <conio.h>
#include <Windows.h>
struct console
{
	HANDLE _m_hout;
	HANDLE _m_hin;
	console()
	{
		_m_hout = GetStdHandle(STD_OUTPUT_HANDLE);
		_m_hin = GetStdHandle(STD_INPUT_HANDLE);
		assert(_m_hin != INVALID_HANDLE_VALUE);
		assert(_m_hout != INVALID_HANDLE_VALUE);
	}
	~console()
	{
		CloseHandle(_m_hout);
		CloseHandle(_m_hin);
	}
	void draw_box(rect_t const& r, char c)
	{
		if (r.height() != 0)
		{
			draw_hor(r.left_top(), r.width(), c);
			draw_hor(point_t(r.p.r+r.height()-1,r.p.c), r.width(), c);
		}
		if (r.width() != 0)
		{
			draw_ver(r.left_top(), r.height(), c);
			draw_ver(point_t(r.p.r,r.p.c+r.width()-1), r.height(), c);
		}
	}
	void draw_hor(point_t const& p, size_t len, char c)
	{
		for (int i = 0; i < len;++i)
			WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{ p.c+i, p.r }, 0);
	}
	void draw_ver(point_t const& p, size_t len, char c)
	{
		size_t bottom = p.r + len;
		for (size_t r=p.r; r < bottom;++r)
			WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{ p.c,r }, 0);
	}
	void fill_box(rect_t const& rect, char c)
	{
		size_t bottom = rect.p.r + rect.height();
		for (size_t r = rect.p.r; r < bottom; ++r)
			draw_hor(point_t(r, rect.p.c), rect.width(), c);
	}
	void draw_point(point_t const& p, char c)
	{
		DWORD l;
		WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{ p.c, p.r }, &l);
	}
	void diable_except_keyboard(bool b)
	{

	}
	void enable_keyboard_input(bool b)
	{
		DWORD mode;
		BOOL br = GetConsoleMode(_m_hin, &mode);
		assert(br);
		if (b) mode |= ENABLE_WINDOW_INPUT; else mode &= (~ENABLE_WINDOW_INPUT);
		br = SetConsoleMode(_m_hin, mode);
		assert(br);
	}
	enum event_type_enum
	{
		__mouse_event=1,
		__keyboard_event
	};
	void read_keyboard_input(size_t& c,bool& isdown)
	{
		INPUT_RECORD keyrec;
		DWORD state = 0, res;
		while (1)
		{
			ReadConsoleInput(_m_hin,&keyrec,1,&res);
			if (keyrec.EventType != KEY_EVENT) continue;
			c = keyrec.Event.KeyEvent.wVirtualKeyCode;
			isdown = keyrec.Event.KeyEvent.bKeyDown?true:false;
			break;
		}
	}
	void peek_keyboard_input(size_t& c)
	{
		INPUT_RECORD keyrec;
		DWORD state = 0, res;
		PeekConsoleInput(_m_hin, &keyrec, 1, &res);
		assert(keyrec.EventType == KEY_EVENT);
		c = keyrec.Event.KeyEvent.wVirtualKeyCode;
	}
	void start()
	{
		draw_box(rect_t(2, 3,4,6), '1');
		fill_box(rect_t(10, 2, 4, 4), '0');
	}
};

struct ui
{
	console _m_console;
	point_t _m_origin;
	void init()
	{
		_m_origin.set(2,2);
		_m_console.enable_keyboard_input(true);
	}


	void draw_box(data_view_t const& box,point_t const& p)
	{
		draw_view(box,p);
	}
	void draw_board(data_view_t const& box_board)
	{
		draw_view(box_board,point_t());
	}
	void clear_box(data_view_t const& box,point_t const& p)
	{
		clear_view(box,p);
	}
	void clear_board(data_view_t const& box_board)
	{
		clear_view(box_board,point_t());
	}
	void draw_view(data_view_t const& data_view, point_t const& p)
	{
		ssize2_t size = data_view.size();
		for (int r = 0; r < size.rc; r++)
			for (int c = 0; c < size.cc; c++)
			{
				char v = data_view.value(point_t(r, c));
				_m_console.draw_point(_m_origin+p+point_t(r, c), v);
			}
	}
	void clear_view(data_view_t const& data_view, point_t const& p)
	{
		_m_console.fill_box(rect_t(_m_origin+p, data_view.size()), '.');
	}


	//void draw_matrix(matrix2_t const& matrix, point_t const& p)
	//{
	//	draw_matrix_screen(matrix,_m_origin+p);
	//}
	//void clear_matrix(matrix2_t const& matrix, point_t const& p)
	//{
	//	clear_matrix_screen(matrix,p+_m_origin+p);
	//}
	//void draw_matrix_screen(matrix2_t const& matrix, point_t const& p)
	//{
	//	for (int r = 0; r < matrix.rc(); r++)
	//		for (int c = 0; c < matrix.cc(); c++)
	//		{
	//			char v = matrix.get(r, c);
	//			_m_console.draw_point(p+point_t(r, c), v);
	//		}
	//}
	//void clear_matrix_screen(matrix2_t const& matrix, point_t const& p)
	//{
	//	_m_console.fill_box(rect_t(p, matrix.rc(),matrix.cc()), '.');
	//}
	//void draw_box_screen(box_t const& box, point_t const& p)
	//{
	//	draw_matrix_screen(box, p);
	//}
	//void draw_board_screen(box_board const& box_board, point_t const& p)
	//{
	//	draw_matrix_screen(box_board.matrix(), p);
	//}
	//void clear_box_screen(box_t const& box, point_t const& p)
	//{
	//	clear_matrix_screen(box, p);
	//}
	//void clear_board_screen(box_board const& box_board, point_t const& p)
	//{
	//	clear_matrix_screen(box_board.matrix(), p);
	//}
};

#include "../../src/cxx/delegate.h"
#include "../../src/thread/win_thread.h"
struct input_event_source
{
	typedef input_event_source self;
	typedef delegate<size_t()> moved_d;
	typedef ox::win_thread<unsigned()> thread_t;
	moved_d on_moved_left;
	moved_d on_moved_rotate;
	moved_d on_moved_right;
	moved_d on_moved_down;
	moved_d on_quit;
	thread_t _m_thread;
	console* _m_console;
	void init() {}
	void start()
	{
		_m_thread.on_run().assign(this,&self::read_keyboard);
		_m_thread.start();
	}
	static size_t const __arrow_left = 37;
	static size_t const __arrow_up = 38;
	static size_t const __arrow_right = 39;
	static size_t const __arrow_down = 40;
	static size_t const __char_q = 'q';
	static size_t const __char_Q = 'Q';

	unsigned read_keyboard()
	{
		bool bquit = false;
		while (!bquit)
		{
			///int c = getch();
			size_t c; bool isdown;
			_m_console->read_keyboard_input(c,isdown);
			if (!isdown) continue;
			switch (c)
			{
			case __arrow_left: if (!on_moved_left.is_empty()) on_moved_left(); break;
			case __arrow_up: if (!on_moved_rotate.is_empty()) on_moved_rotate(); break;
			case __arrow_right: if (!on_moved_right.is_empty()) on_moved_right(); break;
			case __arrow_down:if (!on_moved_down.is_empty()) on_moved_down(); break;
			case __char_q: case __char_Q: bquit = true; if (!on_quit.is_empty()) on_quit(); break;
			}
		}
		return 0;
	}
};

struct ui_drive
{
	ui _m_ui;
	input_event_source _m_event_source;
	void init()
	{
		_m_ui.init();
	}
	void start()
	{
		//_m_ui
	}
	void on_move_down()
	{

	}
};

#include <vector>
struct app
{
	typedef app self;

	struct box_view_t
	{
		typedef box_view_t self;
		self(box_t const* box) : _m_box(box) {}
		self(box_t const& box) : _m_box(&box) {}
		box_t const* _m_box;
		char get_value(point_t const& p)
		{
			return _m_box->get(p);
		}
		ssize2_t get_size()
		{
			return ssize2_t(_m_box->rc(),_m_box->cc());
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};
	struct box_view_unit
	{
		box_t const* box;
		box_view_t box_view;
	};
	struct box_view_list
	{
		~box_view_list()
		{
			for(auto i:_m_box_view_list)
			{
				assert(false);
				//delete *i;
			}
		}
		box_view_list(drive& dr) : _m_drive(dr) {}
		drive& _m_drive;
		std::vector<box_view_t*> _m_box_view_list;
		void init()
		{
			int n = _m_drive._m_tetris.boxes().size();
			for(ibox_t index;index.i<n;++index.i)
			{
				for (index.j=0;index.j<4;++index.j)
				{
					box_t const& box2 = _m_drive._m_tetris.box(index);
					box_view_t* box_view = new box_view_t {box2};
					_m_box_view_list.push_back(box_view);
				}
			}
		}
		data_view_t get(ibox_t i)
		{
			return _m_box_view_list[i.index()]->as_view();
		}
	};
	struct board_part_view
	{
		typedef board_part_view self;
		self(box_board::access_t& access): _m_access(access)
		{}
		rect_t _m_rect;
		box_board::access_t& _m_access;
		void set(rect_t const& rect) {_m_rect=rect;}
		char get_value(point_t const& p)
		{
			return _m_access.get(p);
		}
		ssize2_t get_size()
		{
			return _m_rect.s;
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};

	ui _m_ui;
	drive _m_drive;
	input_event_source _m_event_source;
	data_view_t _m_board_view;
	box_view_list _m_box_view_list;
	board_part_view _m_board_part_view;

	self(): _m_box_view_list(_m_drive), _m_board_part_view(_m_drive._m_tetris._m_board.access())
	{}

	void init()
	{
		_m_drive.on_init();
		_m_drive.on_trace_changed.assign(this,&self::on_trace_changed);
		_m_drive.on_data_changed.assign(this,&self::on_data_changed);
		_m_ui.init();
		_m_event_source._m_console = &_m_ui._m_console;
		_m_event_source.init();
		_m_event_source.on_moved_rotate.assign(this, &self::on_moved_rotate);
		_m_event_source.on_moved_down.assign(this, &self::on_move_down);
		_m_event_source.on_moved_right.assign(this, &self::on_move_right);
		_m_event_source.on_moved_left.assign(this, &self::on_move_left);
		_m_board_view = get_board_view();
		_m_box_view_list.init();
	}
	void on_data_changed(rect_t const* invalid,size_t size)
	{
		draw_board();
		//_m_board_part_view._m_rect = active;
		//_m_ui.clear_view(_m_board_part_view.as_view(),active.p);
		//draw_active_box();

		/// if erased is not empty, brush all the board
		/// or brush the active from box board
		/// finally draw the active box
	}
	void on_trace_changed(box_trace_t const& from,box_trace_t const& to)
	{
		draw_board_part(from.rect);
		draw_box(to.ibox,to.rect.p);
	}
	size_t on_move_down()
	{
		_m_drive.on_move_down();
		return 0;
	}
	size_t on_move_left()
	{
		_m_drive.on_move_left();
		return 0;
	}
	size_t on_move_right()
	{
		_m_drive.on_move_right();
		return 0;
	}
	size_t on_moved_rotate()
	{
		_m_drive.on_move_rotate();
		return 0;
	}
	size_t on_get_another()
	{
		_m_drive.on_get_another();
	}
	void start()
	{
		_m_event_source.start();
		draw_board();
		_m_drive.on_get_another();
		draw_active_box();
	}
	void draw_board()
	{
		_m_ui.draw_board(_m_board_view);
	}
	void draw_board_part(rect_t const& rect)
	{
		_m_board_part_view.set(rect);
		_m_ui.clear_view(_m_board_part_view.as_view(),rect.p);
	}
	void draw_box(ibox_t const& ibox,point_t const& p)
	{
		data_view_t view = _m_box_view_list.get(ibox);
		_m_ui.draw_box(view,p);
	}
	void draw_active_box()
	{
		draw_box(_m_drive.active_ibox(),_m_drive.active_point());
	}
	data_view_t get_board_view()
	{
		data_view_t view;
		view.size.assign(this,&self::get_board_size);
		view.value.assign(this,&self::get_board_value);
		return view;
	}
	char get_board_value(point_t const& p)
	{
		return _m_drive._m_tetris._m_board.access().get(p);
	}
	ssize2_t get_board_size()
	{
		return _m_drive._m_tetris._m_board.user_rect().rc().rect.s;
	}
};

#include <string>
template <typename obj>
std::string to_string(obj const& b)
{
	std::string s;
	for (size_t i=0;i<b.height();++i)
	{
		for (size_t j=0;j<b.width();++j)
		{
			s.push_back(b.get(i,j));
		}
		s.push_back('\n');
	}
	return s;
}

#include "../../src/thread/win_queue_thread.h"

int _tmain(int argc, _TCHAR* argv[])
{
	app _l_app;
	_l_app.init();
	_l_app.start();
	ox::win_queue_thread th;
	th.start_here();
	getchar();
	return 0;
}

