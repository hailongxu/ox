
#include "zlib.h"

#pragma once

struct zlib_compressor
{
	bool compress(
		char* _out_dst, unsigned long&_out_dst_len,
		char const* _in_src, unsigned long _inval_src_len)
	{
		long nResult = ::compress(
			(unsigned char*)_out_dst, &_out_dst_len,
			(unsigned char*)_in_src, _inval_src_len);

		return nResult == Z_OK;
	}

	unsigned long decompress(
		char* _out_dst, unsigned long&_out_dst_len,
		char const* _in_src, unsigned long _in_src_len)
	{
		long nResult = ::uncompress(
			(unsigned char*)_out_dst, &_out_dst_len,
			(unsigned char*)_in_src, _in_src_len);

		return nResult == Z_OK;
	}
};
