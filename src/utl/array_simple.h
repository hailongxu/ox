
/*
* @author xu hailong
* @version 1.0
* @date 2007-5-29
* 版权：版权归作者所有，本文件可以自由传播
*/

#pragma once

namespace ox
{
namespace utl
{
	template <typename unit_tn>
	struct array_simple_wrapper_tt
	{
	public:
		typedef unit_tn unit_t;
		static const size_t __unit_byte_size = sizeof(unit_t);

		array_simple_wrapper_tt():units(0),units_count(0){}
		template<size_t units_count_tc>
		array_simple_wrapper_tt (unit_t const (&units_) [units_count_tc]):
		units(const_cast<unit_t*>(units_)),
			units_count(units_count_tc)
		{}
		template<size_t units_count_tc>
		array_simple_wrapper_tt (unit_t (&units_) [units_count_tc]):
		units(units_),
			units_count(units_count_tc)
		{}
		array_simple_wrapper_tt(unit_t const * units_, size_t units_count_):
		units(const_cast<unit_t*>(units_)),
			units_count(units_count_)
		{}

		template<size_t units_count_tc>
		void init(unit_t const (&units_) [units_count_tc])
		{
			units = const_cast<unit_t*>(units_);
			units_count = units_count_tc;
		}
		template<size_t units_count_tc>
		void init(unit_t (&units_) [units_count_tc])
		{
			units = units_;
			units_count = units_count_tc;
		}
		unit_t * getp_begin(){return units;}
		unit_t * getp_end(){return units+units_count;}
		unit_t & refv_unit(size_t index)
		{
			assert(index<units_count);
			return units[index];
		}
		unit_t & operator [] (size_t index)
		{
			assert(index<units_count);
			return units[index];
		}
		size_t getc_count(){return units_count;}
		size_t getc_bytes(){return units_count*__unit_byte_size;}
	private:
		unit_t * units;
		size_t units_count;
	};
}
}


