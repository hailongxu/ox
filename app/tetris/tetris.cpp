// tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory.h>
#include <assert.h>
#include <vector>
#include "math.h"
#include "win_console.h"


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
	static value_type const __front_char = '@';
	static value_type back_char() {return __back_char;}
	static value_type front_char() { return __front_char; }
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
		_m_user_rect.set(4,4,20,10);
		_m_board.resize(28,18);
		_m_access.set(_m_board,_m_user_rect.p);
	}
	rect_t _m_user_rect;
	matrix_t _m_board;
	access_t _m_access;
	//matrix_t const& matrix() const {return _m_board;}
	//matrix_t& matrix() {return _m_board;}
	access_t& access() {return _m_access;}
	point_t const& o() const { return _m_user_rect.p; }
	void init()
	{
		_m_board.set(user_rect(),'.');
		_m_board.set(rect_t(point_t(0,4),ssize2_t(4,14)),'.');
		_m_board.set(rect_t(point_t(0,0),ssize2_t(28,4)),'@');
		_m_board.set(rect_t(point_t(0,14),ssize2_t(28,4)),'@');
		_m_board.set(rect_t(point_t(24,4),ssize2_t(4,10)),'@');
	}
	rect_t user_rect() const {return rect_t(point_t(4,4),ssize2_t(20,10));}
	void revert_copy_row(size_t r1begin,size_t r2begin,size_t size)
	{
		_m_access.revert_copy(point_t(r1begin,0), point_t(r2begin,0), ssize2_t(size, 10));
	}
	void remove_row_with_top_down(int row)
	{
		revert_copy_row(0,1,row);
		_m_access.set_h(0,0,10,'.');
	}
	void remove_row_with_top_down(int rbegin,int rend)
	{
		revert_copy_row(0,rend-rbegin,rbegin);
		_m_access.set(rect_t(0,0,rend-rbegin,10),'.');
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
			if (is_row_full(r+rect.p.r,rect.left(),rect.right()))
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
		return intv_count;
	}
	bool is_row_full(int r,int c1,int c2)
	{
		for (int c=c1;c<c2;++c)
			if (box_t::is_value_null(access().get(r,c))) return false;
		return true;
	}
	size_t get_each_row_full_list(int r1,int r2,row_intv_t* out,int n)
	{
		return get_each_row_full_list(rect_t(r1,0,r2-r1,_m_user_rect.s.cc),out,n);
	}
	rect_t get_row_rect(int row)
	{
		return rect_t(row,0,1,_m_user_rect.s.cc);
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
			box1.rotate_check_point[0].set(0,0);
			box1.rotate_check_point[1].set(0,1);
			box_t& box2 = _m_boxes.box(ibox_t{i,2});
			box2 = get_transposed(box1);
			box2.next.set(-1,0);
			box2.rotate_check_point[0].set(-1,0);
			box2.rotate_check_point[1].set(-1,1);
			box2.rotate_check_point[2].set(-1,2);
			box2.rotate_check_point[3].set(0,2);
			box_t& box3 = _m_boxes.box(ibox_t{i,3});
			box3 = get_transposed(box2);
			box3.next.set(1,0);
			box3.rotate_check_point[0].set(0,0);
			box3.rotate_check_point[1].set(0,1);
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
			box1.rotate_check_point[0].set(0,1);
			box1.rotate_check_point[1].set(1,1);
			box1.rotate_check_point[2].set(3,-2);
			box1.rotate_check_point[3].set(3,-1);
			box_t& box2 = _m_boxes.box(ibox_t{i,2});
			box2 = get_transposed(box1);
			box2.next.set(-2,2);
			box2.rotate_check_point[0].set(-2,0);
			box2.rotate_check_point[1].set(-2,1);
			box2.rotate_check_point[2].set(-1,0);
			box2.rotate_check_point[3].set(-1,1);
			box2.rotate_check_point[4].set(1,3);
			box_t& box3 = _m_boxes.box(ibox_t{i,3});
			box3 = get_transposed(box2);
			box3.next.set(2,-2);
			box3.rotate_check_point[0].set(0,1);
			box3.rotate_check_point[1].set(1,1);
			box3.rotate_check_point[2].set(3,-2);
			box3.rotate_check_point[3].set(3,-1);
		}
		_m_board.init();
	}
	boxes_t& boxes() {return _m_boxes;}
	box_t const& box(ibox_t const& i) const { return _m_boxes.box(i); }
	rect_t user_rect() const {return _m_board.user_rect();}
	template <typename t,typename bu1,typename bu2>
	static void clear(matrix_tt<t,bu1>& md,point_t pd,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		set(md,pd,box_t::back_char(),ms,ps);
	}
	template <typename m,typename t,typename bu2>
	static void clear(matrix_access<m>& md,point_t pd,matrix_tt<t,bu2> const& ms,point_t const& ps)
	{
		set(md,pd,box_t::back_char(),ms,ps);
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
		box_t const& box = _m_boxes.box(i);
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
	void clear_board()
	{
		rect_t rect(point_t(0,0),_m_board.user_rect().s);
		clear_board(rect);
	}
	void clear_board(rect_t const& rect)
	{
		_m_board.access().fill(rect,box_t::back_char());
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
	bool _m_is_finished;
	typedef delegate<void(rect_t const* board_invalid,size_t size)> data_changed_d;
	typedef delegate<void(box_trace_t const& from,box_trace_t const& to)> trace_changed_d;
	typedef delegate<void()> finished_d;
	data_changed_d on_data_changed;
	trace_changed_d on_trace_changed;
	finished_d on_finished;
	
	rect_t const& user_rect() const
	{
		return _m_tetris.user_rect();
	}
	point_t const& area_origin() const
	{
		return _m_tetris._m_board.o();
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
		_m_pos.set(-1,-1);
		_m_is_finished = true;
	}
	bool is_finished() const {return _m_is_finished;}
	void on_start_game()
	{
		_m_is_finished = false;
		_m_tetris.clear_board();
	}
	void on_move_down()
	{
		if (is_finished()) return;
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
		/// if top is above zero, game over
		if (to.rect.top()<0)
		{
			_m_is_finished = true;
			if (!on_finished.is_empty()) on_finished();
			return;
		}
		/// scan the rows, remove it if full, finally trigger the data-changed event
		_m_tetris.set_by(from.ibox,from.rect.p);
		static int const __max_box_height = 4;
		row_intv_t row_full_list[__max_box_height] = {row_intv_t(),row_intv_t(),row_intv_t(),row_intv_t()};
		size_t count = _m_tetris._m_board.get_each_row_full_list(from.rect.top(),from.rect.bottom(),row_full_list,__max_box_height);
		assert(count<=__max_box_height);
		_m_tetris.remove_rows(row_full_list,count);
		rect_t rect_full_list[1+__max_box_height] = {from.rect};
		for (size_t i=0;i<count;++i)
		{
			row_intv_t const& intv = row_full_list[i];
			rect_full_list[i+1] = _m_tetris._m_board.get_row_rect(intv.begin,intv.end);
		}
		if (!on_data_changed.is_empty()) on_data_changed(rect_full_list,count);
	}
	void on_move_left()
	{
		if (is_finished()) return;
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
		if (is_finished()) return;
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
		if (is_finished()) return;
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to = from;
		to.rect.p = _m_tetris.move_rotate(_m_index,_m_pos,to.ibox);
		if (from.rect.p==to.rect.p && from.ibox==to.ibox) return;
		_m_pos = to.rect.p;
		_m_index = to.ibox;
		box_t const& box = _m_tetris.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void set_ibox_current(ibox_t const& index)
	{
		_m_index = index;
		ssize2_t size = active_box().size();
		_m_pos.set(-size.rc,0);
	}
};
struct ibox_generator
{
	random _m_random;
	int _m_box_count;
	void init(int box_count)
	{
		_m_box_count = box_count;
		_m_random.init();
	}
	ibox_t operator()() const
	{
		ibox_t index;
		int i = _m_random.get()%(_m_box_count);
		int si = _m_random.get()%4;
		index.i = i;
		index.j = si;
		return index;
	}
};

struct draw_view_t
{
	draw_view_t(win_console& console) : _m_console(console) {}
	point_t _m_origin;
	win_console& _m_console;
	template <typename cond_tn>
	void draw(data_view_t const& data_view, point_t const& p,cond_tn const& cond)
	{
		ssize2_t size = data_view.size();
		for (int r = 0; r < size.rc; r++)
			for (int c = 0; c < size.cc; c++)
			{
				char v = data_view.value(point_t(r, c));
				point_t board_point = p+point_t(r, c);
				if (!cond(v,board_point)) continue;
				_m_console.draw_point(_m_origin+board_point, v);
			}
	}
};

struct ui_board
{
	struct always_true
	{
		bool operator()(char,point_t const&) const {return true;}
	};
	struct rect_view_t
	{
		typedef rect_view_t self;
		self(ssize2_t const& size,char c) : _m_size(size),_m_value(c) {}
		char _m_value;
		ssize2_t _m_size;
		char get_value(point_t const& p)
		{
			return _m_value;
		}
		ssize2_t get_size()
		{
			return _m_size;
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};
	ui_board(win_console& console) : _m_console(console),_m_draw_view(_m_console) {}
	win_console& _m_console;
	point_t _m_origin;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_origin.set(2,2);
		_m_draw_view._m_origin = _m_origin;
	}
	void draw_box(data_view_t const& box,point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,point_t const& p){return !box_t::is_value_null(v)&&p.r>=0;});
	}
	void draw_board(data_view_t const& box_board)
	{
		draw_view(box_board,point_t());
	}
	void draw_board(rect_t const& rect,char v)
	{
		rect_t r = rect;
		r.p = _m_origin+r.p;
		_m_console.fill_rect(r,v);
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
		_m_draw_view.draw(data_view,p,always_true());
	}
	void clear_view(data_view_t const& data_view, point_t const& p)
	{
		_m_console.fill_rect(rect_t(_m_origin+p, data_view.size()), '.');
	}
};

struct ui_preview
{
	ui_preview(win_console& console): _m_console(console), _m_draw_view(console)
	{}
	win_console& _m_console;
	rect_t _m_rect;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_rect.p.set(2,14);
		_m_rect.s.set(4,4);
		_m_draw_view._m_origin = _m_rect.p;
	}
	void draw(data_view_t const& box,point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,point_t const& p){return true;});
	}
	void clear()
	{
		_m_console.fill_rect(_m_rect,box_t::back_char());
	}
};

#include "../../src/cxx/delegate.h"
#include "../../src/thread/win_thread.h"
struct input_event_source
{
	typedef input_event_source self;
	typedef delegate<void()> action_d;
	typedef ox::win_thread<unsigned()> thread_t;
	
	action_d on_start_game;
	action_d on_moved_left;
	action_d on_moved_rotate;
	action_d on_moved_right;
	action_d on_moved_down;
	action_d on_quit;
	thread_t _m_thread;
	win_console* _m_console;
	void init() {}
	void start()
	{
		_m_thread.on_run().assign(this,&self::read_keyboard);
		_m_thread.start();
	}
	void stop()
	{
		ungetch('q');
	}
	static size_t const __arrow_left = 37;
	static size_t const __arrow_up = 38;
	static size_t const __arrow_right = 39;
	static size_t const __arrow_down = 40;
	static size_t const __start_game_s = 's';
	static size_t const __start_game_S = 'S';
	static size_t const __char_q = 'q';
	static size_t const __char_Q = 'Q';

	unsigned read_keyboard()
	{
		bool bquit = false;
		while (!bquit)
		{
			size_t c; bool isdown;
			_m_console->read_keyboard_input(c,isdown);
			if (!isdown) continue;
			switch (c)
			{
			case __arrow_left: if (!on_moved_left.is_empty()) on_moved_left(); break;
			case __arrow_up: if (!on_moved_rotate.is_empty()) on_moved_rotate(); break;
			case __arrow_right: if (!on_moved_right.is_empty()) on_moved_right(); break;
			case __arrow_down: if (!on_moved_down.is_empty()) on_moved_down(); break;
			case __start_game_s: case __start_game_S: if (!on_start_game.is_empty()) on_start_game(); break;
			case __char_q: case __char_Q: bquit = true; if (!on_quit.is_empty()) on_quit(); break;
			}
		}
		return 0;
	}
};

struct ui_drive
{
	ui_board _m_ui_board;
	input_event_source _m_event_source;
	void init()
	{
		_m_ui_board.init();
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
		rect_t const& rect() const {return _m_rect;}
		void set(rect_t const& rect)
		{
			_m_rect=rect;
			if (_m_rect.p.r<0)
			{
				_m_rect.s.rc += _m_rect.p.r;
				_m_rect.p.r = 0;
			}
		}
		char get_value(point_t const& p)
		{
			return _m_access.get(_m_rect.p+p);
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

	win_console _m_console;
	ui_board _m_ui_board;
	ui_preview _m_ui_preview;
	drive _m_drive;
	input_event_source _m_event_source;
	data_view_t _m_board_view;
	box_view_list _m_box_view_list;
	board_part_view _m_board_part_view;
	ibox_t _m_ibox_next;
	ibox_generator _m_ibox_generator;


	self(): _m_box_view_list(_m_drive), _m_board_part_view(_m_drive._m_tetris._m_board.access())
		, _m_ui_board(_m_console), _m_ui_preview(_m_console)
	{}

	void init()
	{
		_m_console.enable_keyboard_input(true);
		_m_drive.on_init();
		_m_drive.on_trace_changed.assign(this,&self::on_trace_changed);
		_m_drive.on_data_changed.assign(this,&self::on_data_changed);
		_m_drive.on_finished.assign(this,&self::on_finished);
		_m_ui_board.init();
		_m_ui_preview.init();
		_m_event_source._m_console = &_m_ui_board._m_console;
		_m_event_source.init();
		_m_event_source.on_start_game.assign(this,&self::on_start_game);
		_m_event_source.on_moved_rotate.assign(this, &self::on_moved_rotate);
		_m_event_source.on_moved_down.assign(this, &self::on_move_down);
		_m_event_source.on_moved_right.assign(this, &self::on_move_right);
		_m_event_source.on_moved_left.assign(this, &self::on_move_left);
		_m_board_view = get_board_view();
		_m_box_view_list.init();
		_m_ibox_next.i=_m_ibox_next.j = -1;
		_m_ibox_generator.init(_m_drive._m_tetris.boxes().size());
	}
	void on_data_changed(rect_t const* invalid,size_t size)
	{
		draw_board();
		start_round();
	}
	void on_trace_changed(box_trace_t const& from,box_trace_t const& to)
	{
		draw_board_part(from.rect);
		draw_box(to.ibox,to.rect.p);
	}
	void on_finished()
	{
		rect_t user_rect = _m_drive.user_rect();
		for (int r=0;r<user_rect.s.rc;++r)
		{
			Sleep(50);
			_m_ui_board.draw_board(rect_t(r,0,1,user_rect.s.cc),box_t::front_char());
		}
		Sleep(100);
		for (int r=0;r<user_rect.s.rc;++r)
		{
			Sleep(50);
			_m_ui_board.draw_board(rect_t(r,0,1,user_rect.s.cc),box_t::back_char());
		}
		clear_preview();
	}
	void on_start_game()
	{
		_m_drive.on_start_game();
		retrieve_ibox_next();
		clear_preview();
		draw_active_in_preview();
		start_round();
	}
	void on_move_down()
	{
		_m_drive.on_move_down();
	}
	void on_move_left()
	{
		_m_drive.on_move_left();
	}
	void on_move_right()
	{
		_m_drive.on_move_right();
	}
	void on_moved_rotate()
	{
		_m_drive.on_move_rotate();
	}
	void on_quit()
	{
		_m_event_source.start();
	}
	void start()
	{
		_m_event_source.start();
		draw_board();
		clear_preview();
	}
	void start_round()
	{
		_m_drive.set_ibox_current(_m_ibox_next);
		retrieve_ibox_next();
		clear_preview();
		draw_active_in_preview();
	}
	void retrieve_ibox_next()
	{
		_m_ibox_next = _m_ibox_generator();
	}
	void draw_board()
	{
		_m_ui_board.draw_board(_m_board_view);
	}
	void draw_board_part(rect_t const& rect)
	{
		/// the rect maybe chipped by the user rect border
		_m_board_part_view.set(rect);
		_m_ui_board.draw_view(_m_board_part_view.as_view(),_m_board_part_view.rect().p);
	}
	void draw_box(ibox_t const& ibox,point_t const& p)
	{
		data_view_t view = _m_box_view_list.get(ibox);
		_m_ui_board.draw_box(view,p);
	}
	void clear_preview()
	{
		_m_ui_preview.clear();
	}
	void draw_active_in_preview()
	{
		data_view_t view = _m_box_view_list.get(_m_ibox_next);
		_m_ui_preview.draw(view,point_t(0,0));
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

