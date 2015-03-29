

#include "data_t.h"


#pragma once


namespace ox
{
namespace utl
{

	struct bin_conv
	{
		template <char first>
		static char hex_low_to_char(char c)
		{
			assert(first=='A'||first=='a');
			c &= 0x0f;
			return c>9 ? (c-10)+first : c+'0';
		}
		template <char first>
		static char hex_high_to_char(char c)
		{
			c &= 0xf0;
			c >>= 4;
			return hex_low_to_char<first>(c);
		}

		static char char_to_hex(char c)
		{
			assert(c>='0' && c<='9' || c>='a' && c<='f' || c>='A' && c<='F');
			return c>='0' && c<='9' ? c-'0' :
				(c>='a' && c<='f' ? c-'a'+10 : 
				(c>='A' && c<='F' ? c-'A'+10 : 0));
		}

		template <char first>
		static void single_to_char(char c,char out[2])
		{
			out[0] = hex_high_to_char<first>(c);
			out[1] = hex_low_to_char<first>(c);
		}
		template <char first>
		static void single_to_char(char c,wchar_t out[2])
		{
			char c0 = hex_high_to_char<first>(c);
			out[1] = hex_low_to_char<first>(c);
		}
		static unsigned char char_to_single(char const src[2])
		{
			char c0 = char_to_hex(src[0]);
			char c1 = char_to_hex(src[1]);
			return (c0<<4) | c1;
		}
		static unsigned char char_to_single(wchar_t const src[2])
		{
			char c0 = char_to_hex(src[0]);
			char c1 = char_to_hex(src[1]);
			return (c0<<4) | c1;
		}

		template <char first>
		static int buff_to_str(cdata_t src,data_t out,int append='\0')
		{
			if (out.size<src.size*2)
			{
				assert (false);
				return -1;
			}
			data_t::value_type* p = out.begin;
			for(size_t i=0;i<src.size;++i,p+=2)
				single_to_char<first>(src.begin[i],p);
			if (append!=-1) *p=append;
			return src.size<<1;
		}

		template <char first>
		static int buff_to_str(cdata_t src,wdata_t out,int append='\0')
		{
			if (out.size<src.size*2 || out.begin==0)
			{
				assert (false);
				return -1;
			}
			wdata_t::value_type* p = out.begin;
			for(size_t i=0;i<src.size;++i,p+=2)
				single_to_char<first>(src.begin[i],p);
			if (append!=-1) *p=append;
			return src.size<<1;
		}

		static bool str_to_buff(cdata_t src,data_t out)
		{
			assert(src.size%2==0);
			if (out.size<(src.size>>1))
			{
				assert (false);
				return false;
			}
			data_t::value_type* p = out.begin;
			for(size_t i=0;i<src.size;i+=2,p++)
				*p = char_to_single(src.begin+i);
			return true;
		}
	};



}
}