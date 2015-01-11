
#include "../../src/utl/endian.h"
#include "../../src/test/test_kit.h"

#define  ___TEST___

#pragma once

#ifdef ___TEST___

namespace test
{

	struct test_class_of_endianness
	{
		static void test()
		{
			ox::tst::test_kit<char>::printf_title_of_file(__FILE__);
			test_endianness();
		}
		static void test_endianness()
		{
			ox::tst::test_kit<char>::printf_title_of_class("ox::utl::endian");

			ox::tst::test_kit<char>::printf_outtype_as_string(
				"current system endianness:",
				ox::utl::endianness::test()==ox::utl::endianness::__little_endian?"little":"big",
				"little"
				);
		}
	}; /// end of class test_class_of_endianness

} /// end of namespace test

#endif