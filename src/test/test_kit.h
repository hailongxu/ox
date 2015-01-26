

#pragma once

#include "../pkg/string_convert.h"
#include "../pkg/string_simple_kit.h"
#include "../io/console.h"


namespace ox
{
	namespace tst
	{

		template<typename character_tn>
		struct test_kit
		{
			typedef character_tn character;
			typedef ox::str::string_simple_kit<character> string_kit;

			static void printf_title_of_file(const character * file_title)
			{
				character format[256] = {0};
				if (format[0]==0)
				ox::str::conv::to_string(
					"\r\n"							"\r\n\r\n"
					">>>>> >>>>>test file -- name"	"\r\n"
					"============================"	"\r\n"
					"\t## %s"						"\r\n\r\n",
					format);
				ox::io::console::out<character>::printf(
					format,
					file_title);
			}

			static void printf_title_of_class(const character * class_title)
			{
				character format[256] = {0};
				if (format[0]==0)
				ox::str::conv::to_string(
					"\r\n"							"\r\n"
					">>>>> test -- class"			"\r\n"
					"-------------------"			"\r\n"
					"\t## %s"						"\r\n\r\n",
					format);
				ox::io::console::out<character>::printf(
					format,
					class_title);
			}

			static void printf_title_of_method(const character * method_title)
			{
				ox::io::console::out<character>::printf(
					"\r\n"
					">>>-- method"		"\r\n"
					"````````````"		"\r\n"
					"\t$$ %s"			"\r\n",
					method_title);
			}
			static character * get_result_desc(bool compare_result)
			{
				static character result_success[10] = {0};
				static character result_fail[10] = {0};

				if (result_success[0]==0)
					ox::str::conv::to_string("success",result_success);
				if (result_fail[0]==0)
					ox::str::conv::to_string("xxxxxxx",result_fail);

				return const_cast<character*>(compare_result?result_success:result_fail);
			}

			static void printf_outtype_as_bool(
				const character * formular_desc,
				bool std_result,
				bool run_result)
			{
				static character format[200] = {0};
				if (format[0]==0)
					ox::str::conv::to_string(
						"\r\n"
						"==>run:%s!!"			"\r\n"
						"\t%s"					"\r\n"
						"\tpredict result: %s"	"\r\n"
						"\tactual  return: %s"	"\r\n",
						format);
				
				ox::io::console::out<character>::printf(
					format,
					get_result_desc(!!std_result==!!run_result),
					formular_desc,
					string_kit::to_string(std_result),
					string_kit::to_string(run_result)
					);
			}
			static void printf_outtype_as_integer(
				const character * formular_desc,
				int std_result,
				int run_result)
			{
				static character format[200] = {0};
				character std_result_buffer[32] = {0};
				character run_result_buffer[32] = {0};
				if (format[0]==0)
					ox::str::conv::to_string(
					"\r\n"
					"==>run:%s!!"			"\r\n"
					"\t%s"					"\r\n"
					"\tpredict result: %s"	"\r\n"
					"\tactual  return: %s"	"\r\n",
					format);

				ox::io::console::out<character>::printf(
					format,
					get_result_desc(std_result==run_result),
					formular_desc,
					string_kit::to_string(std_result,std_result_buffer,10),
					string_kit::to_string(run_result,run_result_buffer,10)
					);
			}
			static void printf_outtype_as_string(
				const character * formular_desc,
				const character * std_result,
				const character * run_result)
			{
				static character format[200] = {0};
				if (format[0]==0)
					ox::str::conv::to_string(
					"\r\n"
					"==>run:%s!!"			"\r\n"
					"\t%s"					"\r\n"
					"\tpredict result: %s"	"\r\n"
					"\tactual  return: %s"	"\r\n",
					format);

				ox::io::console::out<character>::printf(
					format,
					get_result_desc(
						ox::str::string_simple_kit<character>::
							strcmp<ox::character::__case_sensitive>(
								std_result,run_result)==0),
					formular_desc,
					std_result,
					run_result);
			}

			static void printf_outtype_as_address(
				const character * formular_desc,
				const character * std_address,
				const character * run_address)
			{
				static character format[200] = {0};
				if (format[0]==0)
					ox::str::conv::to_string(
													"\r\n"
					"==>run:%s!!"					"\r\n"
					"\t%s"							"\r\n"
					"\t""predict result: %p"		"\r\n"
					"\t""actual  return: %p"		"\r\n",
					format);

				ox::io::console::out<character>::printf(
					format,
					get_result_desc(std_address==run_address),
					formular_desc,
					std_address,
					run_address);
			}
		};


	} // end of tst
} // end of ox

