

/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

//#include "../../cxx/delegate_one.h"

#include <vector>
#include "delegate.h"


#pragma once


template <typename function_tn>
struct multi_delegate
{
	typedef multi_delegate self;
	typedef delegate<function_tn> delegate_t;
	typedef std::vector<delegate_t> delegate_vector;
	typedef typename delegate_t::return_type return_type;

	self()
	{
	}
	self(self const& md)
	{
		_m_delegates = md._m_delegates;
	}
	bool is_empty() const
	{
		return _m_delegates.empty();
	}
	void clear()
	{
		_m_delegates.clear();
	}

	self& add(delegate_t const& d)
	{
		_m_delegates.push_back(d);
		return *this;
	}

	template <typename c, typename f>
	self& add(c* _c, f _f)
	{
		_m_delegates.push_back(delegate_t(_c,_f));
		return *this;
	}

	template <typename c>
	self& add(c* _c)
	{
		_m_delegates.push_back(delegate_t(_c));
		return *this;
	}

	self& remove(delegate_t const& d)
	{
		typedef delegate_vector::iterator I;
		for (I i=_m_delegates.begin();i!=_m_delegates.end();++i)
		{
			if (*i==d)
			{
				_m_delegates.erase(i);
				break;
			}
		}
		return *this;
	}
	self& remove_all(delegate_t const& d)
	{
		typedef delegate_vector::iterator I;
		for (I i=_m_delegates.begin();i!=_m_delegates.end();)
		{
			if (*i==d)
				i = _m_delegates.erase(i);
			else
				++i;
		}
		return *this;
	}

	template <typename rt>
	struct run
	{
		rt r;
		rt& return_value()
		{
			return r;
		}
		void operator()(delegate_t& d)
		{
			r = d();
		}
		template <typename p1>
		void operator()(delegate_t& d,p1 _p1)
		{
			r = d(_p1);
		}
		template <typename p1,typename p2>
		void operator()(delegate_t& d,p1 _p1,p2 _p2)
		{
			r = d(_p1,_p2);
		}
		template <typename p1,typename p2,typename p3>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3)
		{
			r = d(_p1,_p2,_p3);
		}
		template <typename p1,typename p2,typename p3,typename p4>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
		{
			r = d(_p1,_p2,_p3,_p4);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
		{
			r = d(_p1,_p2,_p3,_p4,_p5);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6)
		{
			r = d(_p1,_p2,_p3,_p4,_p5,_p6);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7)
		{
			r = d(_p1,_p2,_p3,_p4,_p5,_p6,_p7);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7,p8 _p8)
		{
			r = d(_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8);
		}
	};
	template <>
	struct run <void>
	{
		void return_value()
		{
		}
		void operator()(delegate_t& d)
		{
			d();
		}
		template <typename p1>
		void operator()(delegate_t& d,p1 _p1)
		{
			d(_p1);
		}
		template <typename p1,typename p2>
		void operator()(delegate_t& d,p1 _p1,p2 _p2)
		{
			d(_p1,_p2);
		}
		template <typename p1,typename p2,typename p3>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3)
		{
			d(_p1,_p2,_p3);
		}
		template <typename p1,typename p2,typename p3,typename p4>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
		{
			d(_p1,_p2,_p3,_p4);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
		{
			d(_p1,_p2,_p3,_p4,_p5);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6)
		{
			d(_p1,_p2,_p3,_p4,_p5,_p6);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7)
		{
			d(_p1,_p2,_p3,_p4,_p5,_p6,_p7);
		}
		template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
		void operator()(delegate_t& d,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7,p8 _p8)
		{
			d(_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8);
		}
	};
	return_type operator()()
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i);
		return run.return_value();
	}
	template <typename p1>
	return_type operator()(p1 _p1)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1);
		return run.return_value();
	}
	template <typename p1,typename p2>
	return_type operator()(p1 _p1,p2 _p2)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3,typename p4>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3,p4 _p4)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3,_p4);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3,typename p4,typename p5>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3,_p4,_p5);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3,_p4,_p5,_p6);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3,_p4,_p5,_p6,_p7);
		return run.return_value();
	}
	template <typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
	return_type operator()(p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5,p6 _p6,p7 _p7,p8 _p8)
	{
		typedef run<return_type> run_t;
		typedef delegate_vector::iterator I;
		run_t run;
		for(I i=_m_delegates.begin();i!=_m_delegates.end();++i)
			run(*i,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8);
		return run.return_value();
	}

private:
	delegate_vector _m_delegates;
};

