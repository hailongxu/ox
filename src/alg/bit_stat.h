
/*
 *  @author xuhailong
 *  @date 2009-10-8
 *  @desc the smart algorithm collected in internet
 *      these alg show the great abstraction of the parallel thought
 */

#include "../met/static_assert.h"

#pragma once

namespace ox
{
	namespace alg
	{
		template <typename uint_tn>
		static uint_tn get_reverse(uint_tn n)
		{
			enum { __integer_width = sizeof (uint_tn) };
			___static_assert(__integer_width<=4);

			if (__integer_width>=1)
			{
				n = ( n & 0x55555555UL ) << 1 | ( n & 0xAAAAAAAA ) >> 1;
				n = ( n & 0x33333333UL ) << 2 | ( n & 0xCCCCCCCC ) >> 2;
				n = ( n & 0x0f0f0f0fUL ) << 4 | ( n & 0xF0F0F0F0 ) >> 4;
			}
			if (__integer_width>=2)
				n = ( n & 0x00ff00ffUL ) << 8 | ( n & 0xff00ff00UL ) >> 8;
			if (__integer_width>=4)
				n = ( n & 0x0000ffffUL ) << 16 | ( n & 0xffff0000UL ) >> 16;

			return n;
		}

		template <typename uint_tn>
		static size_t get_1_count(uint_tn n)
		{
			enum { __integer_width = sizeof (uint_tn) };
			___static_assert(__integer_width<=4);

			if (__integer_width>=1)
			{
				n = (n & 0x55555555UL) + ((n >> 1) & 0x55555555UL); // 0-2 in 2 bits
				n = (n & 0x33333333UL) + ((n >> 2) & 0x33333333UL); // 0-4 in 4 bits
				n = (n & 0x0f0f0f0fUL) + ((n >> 4) & 0x0f0f0f0fUL); // 0-8 in 8 bits
			}

			if (__integer_width>=2)
				n = (n & 0x00ff00ffUL) + ((n >> 8) & 0x00ff00ffUL); // 0-16 in 16 bits

			if (__integer_width>=4)
				n = (n & 0x0000ffffUL) + ((n >> 16) & 0x0000ffffUL); // 0-31 in 32 bits

			return n;
		}

		template <typename uint_tn>
		static size_t get2_1_count(uint_tn n)
		{
			enum { __integer_width = sizeof (uint_tn) };
			___static_assert(__integer_width<=4);

			if (__integer_width>=1)
			{
				n = (n & 0x55555555UL) + ((n >> 1) & 0x55555555UL); // 0-2 in 2 bits
				n = (n & 0x33333333UL) + ((n >> 2) & 0x33333333UL); // 0-4 in 4 bits
				n = (n + (n >> 4)) & 0x0f0f0f0fUL; // 0-8 in 4 bits
			}
			if (__integer_width>=2)
				n += n >> 8; // 0-16 in 8 bits

			if (__integer_width>=4)
				n += n >> 16; // 0-32 in 8 bits

			return n;
		}

		static bool is_2n(unsigned int n)
		{
			return (n& (n-1)) == 0;
		}

		static unsigned get_1index_of_2n(unsigned int n)
		{
			assert(is_2n(n) || n==0);
			return get_1_count((unsigned int)(n-1));
		}

	} /// end of namespace alg
} /// end of namespace ox