/*
	author: 徐海龙 xuhailong
	date: 2006-6-20
	msn: hailongxu@msn.com
	Email: hailongxu@msn.com
	copyright：本代码属开源代码，版权归属作者所有
 */
//
//////////////////////////////////////////////////////////////////////////
// author : xuhailong
// desc : the frequent function & base common function in the socket programming
// history : 2006-11-22 17:55



#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../pkg/string_simple_kit.h"
#include "data_t.h"


#pragma once



/*
	author: 徐海龙 xuhailong
	date: 2006-6-20
	msn: hailongxu@msn.com
	Email: hailongxu@msn.com
	copyright：本代码属开源代码，版权归属作者所有
 */
//
//////////////////////////////////////////////////////////////////////////
// author : xuhailong
// desc : the common function of base64
// history : 2006-11-22 17:55




#include "data_t.h"


#pragma once


namespace ox
{
namespace utl
{


struct base64
{
	static size_t encode_size(size_t size) {return (size+3-1)/3*4;}
	static size_t b1(unsigned char c1) {return c1>>2;}
	static size_t b2(unsigned char c1,unsigned char c2) {return ((c1&3)<<4) | (c2>>4);}
	static size_t b3(unsigned char c2,unsigned char c3) {return ((c2&0x0f)<<2) | (c3>>6);}
	static size_t b4(unsigned char c3) {return c3&0x3f;}
	template <typename out>
	static size_t encode(cdata_t data,out& o)
	{
		static char const table[] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};
		size_t const len = encode_size(data.size);
		size_t t = data.size%3;
		size_t n = data.size-t;
		char const* p = data.begin;
		char const* e = p+n;
		unsigned char c1,c2,c3;
		for(;p<e;p+=3)
		{
			c1 = (char)*(p+0);
			c2 = (char)*(p+1);
			c3 = (char)*(p+2);
			o.push_back(table[b1(c1)]);
			o.push_back(table[b2(c1,c2)]);
			o.push_back(table[b3(c2,c3)]);
			o.push_back(table[b4(c3)]);
		}
		/// do with tail
		if (!t) return len;
		c1 = t>0 ? *e : 0;
		c2 = t>1 ? *(e+1) : 0;
		c3 = 0;
		o.push_back(table[b1(c1)]);
		o.push_back(table[b2(c1,c2)]);
		o.push_back(t==2?table[b3(c2,c3)]:'=');
		o.push_back('=');
		return len;
	}

	static size_t c1(unsigned char b1,unsigned char b2) {return b1<<2 | b2>>4;}
	static size_t c2(unsigned char b2,unsigned char b3) {return b2<<4 | b3>>2;}
	static size_t c3(unsigned char b3,unsigned char b4) {return b3<<6 | b4;}

	template <typename out>
	static void decode(cdata_t data,out& o)
	{
		static unsigned char const table[128] =
		{  
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,0xf0,0xff,0xff,0xf1,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xe0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3e,0xff,0xf2,0xff,0x3f,
			0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0xff,0xff,0xff,0x00,0xff,0xff,
			0xff,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,
			0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xff,0xff,0xff,0xff,0xff,
			0xff,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
			0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0xff,0xff,0xff,0xff,0xff 
		};
		assert (data.size>=2);
		if (data.size && data.begin[data.size-1]=='=') --data.size;
		if (data.size && data.begin[data.size-1]=='=') --data.size;
		size_t t = data.size%4;
		size_t n = data.size-t;
		char const* p = data.begin;
		char const* e = p+n;
		unsigned char b1,b2,b3,b4;
		for(;p<e;p+=4)
		{
			b1 = (char)*(p+0);
			b2 = (char)*(p+1);
			b3 = (char)*(p+2);
			b4 = (char)*(p+3);
			o.push_back(c1(table[b1],table[b2]));
			o.push_back(c2(table[b2],table[b3]));
			o.push_back(c3(table[b3],table[b4]));
		}
		if (!t) return;
		assert(t>1);
		b1 = e[0];
		b2 = t>1 ? e[1] : 0;
		b3 = t>2 ? e[2] : 0;
		b4 = '=';
		o.push_back(c1(table[b1],table[b2]));
		if (t==3) o.push_back(c2(table[b2],table[b3]));
	}
};


}
}