// tetris.cpp : Defines the entry point for the console application.
//

#include <memory.h>
#include <assert.h>
#include <vector>
#include "../../ox/nsab.h"
#include "math.h"
#include "tetris_define.h"




#pragma once



___namespace3_begin(ox,app,tetris)


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

	void fill() {matrix_t::fill(tetris_define::__back_char);}
	matrix_t const& matrix() const {return *this;}
	rc_point_t next;
	rc_point_t rotate_check_point[4*4];
	
	bool is_null(rc_point_t const& p) const {return tetris_define::is_value_null(get(p));}
	bool is_null(size_t r,size_t c) const {return tetris_define::is_value_null(get(r,c));}
};

struct box_board
{
	typedef matrix2_t matrix_t;
	typedef matrix_access_tt<matrix_t> access_t;

	box_board()
	{
		_m_user_rect.set(4,4,20,10);
		_m_board.resize(28,18);
		_m_access.set(_m_board,_m_user_rect.p);
	}
	rc_rect_t _m_user_rect;
	matrix_t _m_board;
	access_t _m_access;
	//matrix_t const& matrix() const {return _m_board;}
	//matrix_t& matrix() {return _m_board;}
	access_t& access() {return _m_access;}
	rc_point_t const& o() const { return _m_user_rect.p; }
	void init()
	{
		_m_board.set(user_rect(),'.');
		_m_board.set(rc_rect_t(rc_point_t(0,4),rc_size_t(4,14)),'.');
		_m_board.set(rc_rect_t(rc_point_t(0,0),rc_size_t(28,4)),'@');
		_m_board.set(rc_rect_t(rc_point_t(0,14),rc_size_t(28,4)),'@');
		_m_board.set(rc_rect_t(rc_point_t(24,4),rc_size_t(4,10)),'@');
	}
	rc_rect_t user_rect() const {return rc_rect_t(rc_point_t(4,4),rc_size_t(20,10));}
	void revert_copy_row(size_t r1begin,size_t r2begin,size_t size)
	{
		_m_access.revert_copy(rc_point_t(r1begin,0), rc_point_t(r2begin,0), rc_size_t(size, 10));
	}
	void remove_row_with_top_down(int row)
	{
		revert_copy_row(0,1,row);
		_m_access.set_h(0,0,10,'.');
	}
	void remove_row_with_top_down(int rbegin,int rend)
	{
		revert_copy_row(0,rend-rbegin,rbegin);
		_m_access.set(rc_rect_t(0,0,rend-rbegin,10),'.');
	}
	void remove_row_with_top_down(row_intv_t const* row,int n)
	{
		assert(n>=0);
		for (int i=0;i<n;++i)
			remove_row_with_top_down((*row).begin,(*row).end);
	}
	size_t get_each_row_full_list(rc_rect_t const& rect,row_intv_t* out,int n)
	{
		bool is_last_full = false;
		size_t count = 0, intv_count=0;
		for (int r=0;r<rect.s.rc();++r)
		{
			if (is_row_full(r+rect.p.r(),rect.left(),rect.right()))
			{
				assert(intv_count<n);
				count ++;
				row_intv_t& row = *out;
				if (!is_last_full)
				{
					intv_count ++;
					row.begin = r+rect.p.r(), count++;
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
			if (tetris_define::is_value_null(access().get(r,c))) return false;
		return true;
	}
	size_t get_each_row_full_list(int r1,int r2,row_intv_t* out,int n)
	{
		return get_each_row_full_list(rc_rect_t(r1,0,r2-r1,_m_user_rect.s.cc()),out,n);
	}
	rc_rect_t get_row_rect(int row)
	{
		return rc_rect_t(row,0,1,_m_user_rect.s.cc());
	}
	rc_rect_t get_row_rect(int row_begin,int row_end)
	{
		assert(row_end>=row_begin);
		return rc_rect_t(row_begin,0,row_end-row_begin,10);
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

struct tetris_core_data
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
			box1.rotate_check_point[0].set(0,2);
			box1.rotate_check_point[1].set(1,2);
			box1.rotate_check_point[2].set(2,1);
			box1.rotate_check_point[3].set(2,2);
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
			box1.rotate_check_point[0].set(0,2);
			box1.rotate_check_point[1].set(1,2);
			box1.rotate_check_point[2].set(2,1);
			box1.rotate_check_point[3].set(2,2);
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
	rc_rect_t user_rect() const {return _m_board.user_rect();}
	template <typename t,typename bu1,typename bu2>
	static void clear(matrix_tt<t,bu1>& md,rc_point_t pd,matrix_tt<t,bu2> const& ms,rc_point_t const& ps)
	{
		set(md,pd,tetris_define::back_char(),ms,ps);
	}
	template <typename m,typename t,typename bu2>
	static void clear(matrix_access_tt<m>& md,rc_point_t pd,matrix_tt<t,bu2> const& ms,rc_point_t const& ps)
	{
		set(md,pd,tetris_define::back_char(),ms,ps);
	}
	template <typename t,typename bu1,typename bu2>
	static void set(matrix_tt<t,bu1>& md,rc_point_t pd,char const& v,matrix_tt<t,bu2> const& ms,rc_point_t const& ps)
	{
		rc_point_t s = ps;
		rc_point_t d = pd;
		for (;s.r()<ms.rc();++s.r(),++d.r())
		{
			s.c() = ps.c();
			d.c() = pd.c();
			for (;s.c()<ms.cc();++s.c(),++d.c())
				if (!tetris_define::is_value_null(ms.get(s)))
					md.set(d,v);
		}
	}
	template <typename m,typename t,typename bu2>
	static void set(matrix_access_tt<m>& md,rc_point_t pd,char const& v,matrix_tt<t,bu2> const& ms,rc_point_t const& ps)
	{
		rc_point_t s = ps;
		rc_point_t d = pd;
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
	static void set(matrix_access_tt<m>& md,rc_point_t pd,matrix_tt<t,bu2> const& ms,rc_point_t const& ps)
	{
		typedef typename matrix_access_tt<m>::value_type value_type;
		rc_point_t s = ps;
		rc_point_t d = pd;
		for (;s.r()<ms.rc();++s.r(),++d.r())
		{
			s.c() = ps.c();
			d.c() = pd.c();
			for (;s.c()<ms.cc();++s.c(),++d.c())
			{
				value_type v = ms.get(s);
				if (tetris_define::is_value_null(v)) continue;
				md.set(d,v);
			}
		}
	}
	static bool is_enabled(box_t const& box,matrix_t const& matrix,rc_point_t const& p)
	{
		bool ret = true;
		rc_point_t s = p;
		rc_point_t b;
		for (;b.r()<box.rc();++b.r(),++s.r())
		{
			b.c()=0; s.c()=p.c();
			for (;b.c()<box.cc();++b.c(),++s.c())
			{
				char v = box.get(b);
				if (tetris_define::is_value_null(v)) continue;
				if (tetris_define::is_value_null(matrix.get(s))) continue;
				ret = false;
				break;
			}
		}
		return ret;
	}
	bool is_down_enabled(box_t const& box,rc_point_t const& p)
	{
		matrix_t m = _m_board.access().get(rc_rect_t(p,box.rc()+1,box.cc()));
		clear(m,rc_point_t(0,0),box.matrix(),rc_point_t(0,0));
		return is_enabled(box,m,rc_point_t(1,0));
	}
	bool is_down_enabled(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_down_enabled(box,p);
	}
	bool is_left_enabled(box_t const& box,rc_point_t const& p)
	{
		matrix_t m = _m_board.access().get(rc_rect_t(rc_point_t(p.r(),p.c()-1),rc_size_t(box.rc(),box.cc()+1)));
		clear(m,rc_point_t(0,1),box.matrix(),rc_point_t(0,0));
		return is_enabled(box,m,rc_point_t(0,0));
	}
	bool is_left_enabled(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_left_enabled(box,p);
	}
	bool is_right_enabled(box_t const& box,rc_point_t const& p)
	{
		matrix_t m = _m_board.access().get(rc_rect_t(p,rc_size_t(box.rc(),box.cc()+1)));
		clear(m,rc_point_t(0,0),box.matrix(),rc_point_t(0,0));
		return is_enabled(box,m,rc_point_t(0,1));
	}
	bool is_right_enabled(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_right_enabled(box,p);
	}

	static bool is_rotate_enabled(box_t const& box,access_t const& matrix,rc_point_t const& p)
	{
		bool bret = true;
		rc_point_t const* check = box.rotate_check_point;
		for (int i=0;check[i]!=rc_point_t(-1,-1);++i)
		{
			if (!tetris_define::is_value_null(matrix.get(p+check[i])))
			{
				bret = false;
				break;
			}
		}
		return bret;
	}
	bool is_rotate_enabled(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		return is_rotate_enabled(box,_m_board.access(),p);
	}
	rc_point_t move_rotate(ibox_t const& i,rc_point_t const& p,ibox_t& in)
	{
		if (!is_rotate_enabled(i,p))
			return p;
		box_t const& box = _m_boxes.box(i);
		in = boxes_t::get_next(i);
		box_t const& box_next = _m_boxes.box(in);
		rc_point_t r=p;
		rc_point_t n=p+box.next;
		return n;
	}
	rc_point_t move_down(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_down_enabled(box, p)) return p;
		rc_point_t r = p; r.r()++;
		return r;
	}
	rc_point_t move_left(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_left_enabled(box, p))
			return p;
		rc_point_t r = p; r.c()--;
		return r;
	}
	rc_point_t move_right(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		if (!is_right_enabled(box, p))
			return p;
		rc_point_t r = p; r.c()++;
		return r;
	}
	void set_by(ibox_t const& i,rc_point_t const& p)
	{
		box_t const& box = _m_boxes.box(i);
		set(_m_board.access(), p, box.matrix(), rc_point_t(0, 0));
	}
	void remove_rows(row_intv_t const* rows,size_t n)
	{
		_m_board.remove_row_with_top_down(rows,n);
	}
	void clear_board()
	{
		rc_rect_t rect(rc_point_t(0,0),_m_board.user_rect().s);
		clear_board(rect);
	}
	void clear_board(rc_rect_t const& rect)
	{
		_m_board.access().fill(rect,tetris_define::back_char());
	}
};

struct box_trace_t
{
	ibox_t ibox;
	rc_rect_t rect;
	//rc_point_t pos;
};
struct tetris_drive
{
	typedef tetris_drive self;
	tetris_core_data _m_tetris_core_data;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	ibox_t _m_index;
	rc_point_t _m_pos;
	bool _m_is_finished;
	typedef delegate<void(rc_rect_t const* board_invalid,size_t size)> data_changed_d;
	typedef delegate<void(box_trace_t const& from,box_trace_t const& to)> trace_changed_d;
	typedef delegate<void()> finished_d;
	data_changed_d on_data_changed;
	trace_changed_d on_trace_changed;
	finished_d on_finished;
	
	rc_rect_t const& user_rect() const
	{
		return _m_tetris_core_data.user_rect();
	}
	rc_point_t const& area_origin() const
	{
		return _m_tetris_core_data._m_board.o();
	}
	ibox_t active_ibox() const
	{
		return _m_index;
	}
	box_t const& active_box() const
	{
		return _m_tetris_core_data.box(_m_index);
	}
	rc_point_t const& active_point() const
	{
		return _m_pos;
	}
	void on_init()
	{
		_m_tetris_core_data.init();
		_m_pos.set(-1,-1);
		_m_is_finished = true;
	}
	bool is_finished() const {return _m_is_finished;}
	void start_game()
	{
		_m_is_finished = false;
		_m_tetris_core_data.clear_board();
	}
	void move_down()
	{
		if (is_finished()) return;
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris_core_data.move_down(_m_index, _m_pos);
		box_t const& box = _m_tetris_core_data.box(from.ibox);
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
		_m_tetris_core_data.set_by(from.ibox,from.rect.p);
		static int const __max_box_height = 4;
		row_intv_t row_full_list[__max_box_height] = {row_intv_t(),row_intv_t(),row_intv_t(),row_intv_t()};
		size_t count = _m_tetris_core_data._m_board.get_each_row_full_list(from.rect.top(),from.rect.bottom(),row_full_list,__max_box_height);
		assert(count<=__max_box_height);
		_m_tetris_core_data.remove_rows(row_full_list,count);
		rc_rect_t rect_full_list[1+__max_box_height] = {from.rect};
		for (size_t i=0;i<count;++i)
		{
			row_intv_t const& intv = row_full_list[i];
			rect_full_list[i+1] = _m_tetris_core_data._m_board.get_row_rect(intv.begin,intv.end);
		}
		if (!on_data_changed.is_empty()) on_data_changed(rect_full_list,1+count);
	}
	void move_left()
	{
		if (is_finished()) return;
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris_core_data.move_left(_m_index, _m_pos);
		if (from.rect.p==to.rect.p) return;
		_m_pos = to.rect.p;
		box_t const& box = _m_tetris_core_data.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void move_right()
	{
		if (is_finished()) return;
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to {_m_index};
		to.rect.p = _m_tetris_core_data.move_right(_m_index, _m_pos);
		if (from.rect.p==to.rect.p) return;
		_m_pos = to.rect.p;
		box_t const& box = _m_tetris_core_data.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void move_rotate()
	{
		if (is_finished()) return;
		box_trace_t from {_m_index,_m_pos};
		box_trace_t to = from;
		to.rect.p = _m_tetris_core_data.move_rotate(_m_index,_m_pos,to.ibox);
		if (from.rect.p==to.rect.p && from.ibox==to.ibox) return;
		_m_pos = to.rect.p;
		_m_index = to.ibox;
		box_t const& box = _m_tetris_core_data.box(from.ibox);
		from.rect.s = box.size();
		to.rect.s = from.rect.s;
		if (!on_trace_changed.is_empty()) on_trace_changed(from,to);
	}
	void set_ibox_current(ibox_t const& index)
	{
		_m_index = index;
		rc_size_t size = active_box().size();
		_m_pos.set(-size.rc(),0);
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


___namespace3_end()