
/*
 *  @author xuhailong
 *  @date 2009-10-7
 *  @desc construct a concept of guard
 */

#include "../cor/mm_m.h"

#pragma once

namespace ox
{
	namespace alg
	{
		namespace _ns_
		{
			template <typename cell_tn, bool is_self_contained_tc>
			class till_property
			{
			public:
				typedef till_property self;
				self () : _m_begin(0), _m_hit(0)
				{}
				self (cell_tn const * _inptr_begin)
					: _m_begin(_inptr_begin), _m_hit(0)
				{}
				enum { __is_self_contained = is_self_contained_tc?1:0 };
			public:
				void set_begin(cell_tn const * _inptr_begin)
				{
					_m_begin = _inptr_begin;
				}
				size_t get_index() const
				{
					return _m_hit-_m_begin;
				}
				size_t get_hops() const
				{
					return _m_hit-_m_begin;
				}
				cell_tn * get_hit() const
				{
					return _m_hit;
				}
				bool is_hit() const
				{
					return _m_hit!=0;
				}
			protected:
				cell_tn const* get_begin() const
				{
					return _m_begin;
				}
				void set_hit (cell_tn const * _inptr_hit) const
				{
					_m_hit = _inptr_hit;
				}
			private:
				mutable cell_tn const * _m_hit;
				cell_tn const * _m_begin;
			}; /// end of property
		} /// end of namespace _ns_

		/// till an cell, just for a single character, not for 
		/// a set of cells
		template
		<
			typename cell_tn,
			bool is_self_contained_tc,
			typename till_property_tn
				= _ns_::till_property<cell_tn,is_self_contained_tc>
		>
		class till_single_cell
			: public till_property_tn
		{
			typedef till_single_cell self;
			typedef till_property_tn base;
		public:
			self ()
				: base ()
				, _m_cell (0)
			{}
			self(
				cell_tn const * _inptr_begin,
				cell_tn const & _inconref_cell)
				: base (_inptr_begin)
				, _m_cell(&_inconref_cell)
			{}

			self const & operator()() const
			{
				set_hit (
					ox::cor::mm_m::memchr(get_begin(),*_m_cell)
					+ (base::__is_self_contained?1:0));
				return *this;
			}

			self const & operator()(
				cell_tn const & _inconref_cell) const
			{
				set_hit (
					ox::cor::mm_m::memchr(get_begin(),_inconref_cell)
					+ (base::__is_self_contained?1:0));
				return *this;
			}
			self const & operator()(
				cell_tn const * const _inptr_begin,
				cell_tn const & _inconref_cell) const
			{
				set_hit (
					ox::cor::mm_m::memchr(_inptr_begin,_inconref_cell)
					+ (base::__is_self_contained?1:0));
				return *this;
			}
		public:
			void set_cell (cell_tn const & _inconref_cell)
			{
				_m_cell = &_inconref_cell;
			}
		private:
			cell_tn const * _m_cell;
		}; /// end of class till_siggle_cell

		/// till the cell is null
		template
		<
			typename cell_tn,
			bool is_self_contained_tc,
			typename till_property_tn
				= _ns_::till_property<cell_tn,is_self_contained_tc>
		>
		class till_single_null
			: public till_property_tn
		{
			typedef till_single_null self;
			typedef till_property_tn base;
		public:
			self() : base()
			{}
			self(cell_tn const * _inptr_begin)
				: base (_inptr_begin)
			{}

			self const & operator()() const
			{
				set_hit (
					ox::cor::mm_m::memchr(get_begin(),cell_tn(0))
					+ (base::__is_self_contained?1:0));
				return *this;
			}
		}; /// end of class till_sigle_null

	} /// end of alg
} /// end of ox