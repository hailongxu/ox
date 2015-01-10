/**
	���ߣ��캣��
	2008-1-1
	�����ַ�����ͬ�ַ��汾��ת���õ�
	��Ȩ����Ȩ���������У����ļ��������ɴ���
*/

#include <limits>
#include "./string_simple_kit.h"
#include "./character_trait.h"
#include "../pkg/il.h"
//#include "../alg/till.h"
//#include "../cor/mm_m.h"

#undef max
#undef min

#pragma once

namespace ox
{
	namespace string
	{


		struct conv_const_t
		{
			static size_t getv_multibyte_current_max(){return MB_CUR_MAX;}
			static size_t getv_multibyte_max(){return MB_LEN_MAX;}
		};


		///
		/// �κ�ת������Ĭ��locale����ƽ̨locale
		///
		struct conv : conv_const_t
		{
		public:

			/// ref-string (ref-string, ref-string)
			template <typename character_from_tn, typename character_to_tn>
			static std::basic_string<character_to_tn> &
			to_string(
				std::basic_string<character_from_tn> const & string_from,
				std::basic_string<character_to_tn> & string_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				return to_string<character_from_tn,character_to_tn>(string_from.c_str(),string_to,locale);
			}
			/// value-string (ref-string)
			template <typename character_from_tn, typename character_to_tn>
			static std::basic_string<character_to_tn>
			to_string(
				std::basic_string<character_from_tn> const & string_from,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				std::basic_string<character_to_tn> string_to;
				return to_string<character_from_tn,character_to_tn>(string_from,string_to,locale);
			}
			/// value-string (c-string)
			template <typename character_from_tn, typename character_to_tn>
			static std::basic_string<character_to_tn>
			to_string(
				character_from_tn const * string_from,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				int string_to_len = to_string<character_from_tn,character_to_tn>(string_from,0,0,locale);
				if (string_to_len<0)
					return std::basic_string<character_to_tn>();
				std::basic_string<character_to_tn> string_to(string_to_len,character_to_tn());
				to_string<character_from_tn,character_to_tn>(string_from,const_cast<character_to_tn*>(string_to.c_str()),string_to_len,locale);
				string_to.resize(string_to_len);
				return string_to;
			}
			/// ref-string (c-string,ref-string)
			template <typename character_from_tn, typename character_to_tn>
			static std::basic_string<character_to_tn> &
			to_string(
				character_from_tn const * string_from,
				std::basic_string<character_to_tn> & string_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				int string_to_len = to_string<character_from_tn,character_to_tn>(string_from,0,0);
				static std::basic_string<character_to_tn> emptyString;
				if (string_to_len<0)
					return emptyString;
				string_to.assign(string_to_len,character_to_tn());
				to_string<character_from_tn,character_to_tn>(string_from,const_cast<character_to_tn*>(string_to.c_str()),string_to_len,locale);
				string_to.resize(string_to_len);
				return string_to;
			}

			/// void (__in__ c-string,  __out__ c-string)
			/// ת��������ָ��length
			template <typename character_from_tn, typename character_to_tn>
			static int to_string(
				character_from_tn const * string_from,
				character_to_tn * string_to,
				size_t string_to_len,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				typedef character_from_tn character_from_t;
				typedef character_to_tn character_to_t;
				return convert<character_from_t,character_to_t>(
					string_from,
					string_to,
					string_to_len,
					locale);
			}
			/// ת������������ԭ��length
			template <typename character_from_tn, typename character_to_tn>
			static int to_string(
				character_from_tn const * string_from,
				character_to_tn * string_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				typedef character_from_tn character_from_t;
				typedef character_to_tn character_to_t;
				return convert<character_from_t,character_to_t>(
					string_from,
					string_to,
					std::numeric_limits<int>::max()/2-1,
					locale);
			}

			/// ת���ַ��ʹ�
			template<typename character_from_tn, typename character_to_tn>
			static int to_string(
				character_from_tn const * character_from,
				character_to_tn & character_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				return convert<character_from_tn,character_to_tn>(
					character_from,
					character_to,
					locale);
			}
			template<typename character_from_tn, typename character_to_tn>
			static int to_string(
				character_from_tn const & character_from,
				character_to_tn * character_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				return convert<character_from_tn,character_to_tn>(
					character_from,
					character_to,
					locale);
			}

			// ת���ַ����ַ�
			template<typename character_from_tn, typename character_to_tn>
			static int to_string(
				character_from_tn const & character_from,
				character_to_tn & character_to,
				ox::il::locale_t const & locale=ox::il::locale_t::get_sys())
			{
				return convert<character_from_tn,character_to_tn>(
					character_from,
					character_to,
					locale);
			}
			template<typename character_tn>
			static int to_string(
				character_tn const & character_from,
				character_tn & character_to)
			{
				return convert<character_tn>(
					character_from,
					character_to);
			}
		private:
			/// ����ת��
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const * string_from,
				character_to_tn * string_to,
				size_t string_to_len,
				ox::il::locale_t const & locale)
			{
				typedef character_from_tn character;
				assert(string_from);
				if(string_to!=string_from && string_to)
					return ox::cor::mm_m::memove(
						string_to,
						string_to_len,
						string_from,
						ox::alg::till_single_null<character,true>(string_from));
				else
					return string_simple_kit<character>::length(string_from);
			}
			template<>
			static int convert<char,wchar_t>(
				char const * string_from,
				wchar_t * string_to,
				size_t string_to_len,
				ox::il::locale_t const & locale)
			{
				assert(string_from);
				return (int)_mbstowcs_l(string_to,string_from,string_to_len,*locale);
			}
			template<>
			static int convert<wchar_t,char>(
				wchar_t const * string_from,
				char * string_to,
				size_t string_to_len,
				ox::il::locale_t const & locale)
			{
				return (int)_wcstombs_l(string_to,string_from,string_to_len,*locale);
			}

			/// the second type
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const * string_from,
				character_to_tn * string_to,
				size_t string_to_len)
			{
				typedef character_from_tn character;
				assert(string_from);
				if(string_to!=string_from && string_to)
					return ox::mem::mem_kit<character>::copy_string(
					string_from,
					string_to,
					string_to_len);
				else
					return string_simple_kit<character>::length(string_from);
			}
			/// �ַ��������ַ�ת��
			/// ���ַ�
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const * character_from,
				character_to_tn & character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_from);
				if (character_from!=&character_to)
					character_to=*character_from;
				return 1;
			}
			template<>
			static int convert<char,wchar_t>(
				char const * character_from,
				wchar_t & character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_from);
				return _mbtowc_l(
					&character_to,
					character_from,
					getv_multibyte_current_max(),
					*locale);
			}
			template<>
			static int convert<wchar_t,char>(
				wchar_t const * character_from,
				char & character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_from);
				if (character_from[0]>127)
				{
					character_to='?';
					return -1;
				}
				return _wctomb_l(
					&character_to,
					character_from[0],
					*locale);
			}
			/// �ַ�-��
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const & character_from,
				character_to_tn * character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_to);
				if (&character_from!=character_to)
					*character_to=character_from;
				return 1;
			}
			template<>
			static int convert<wchar_t,char>(
				wchar_t const & character_from,
				char * character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_to);
				return _wctomb_l(character_to,character_from,*locale);
			}
			template<>
			static int convert<char,wchar_t>(
				char const & character_from,
				wchar_t * character_to,
				ox::il::locale_t const & locale)
			{
				assert(character_from);
				return _mbtowc_l(
					character_to,
					&character_from,
					getv_multibyte_current_max(),
					*locale);
			}
			////
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const * character_from,
				character_to_tn & character_to)
			{
				assert(character_from);
				if (character_from!=&character_to)
					character_to=*character_from;
				return 1;
			}
			template<> static int convert<char,wchar_t>(
				char const * character_from,
				wchar_t & character_to)
			{
				assert(character_from);
				return _mbtowc_l(
					&character_to,
					character_from,
					getv_multibyte_current_max(),
					*ox::il::locale_t::get_sys()
					);
			}
			//
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const & character_from,
				character_to_tn * character_to)
			{
				assert(character_to);
				if (&character_from!=character_to)
					*character_to=character_from;
				return 1;
			}
			template<>
			static int convert<wchar_t,char>(
				wchar_t const & character_from,
				char * character_to)
			{
				assert(character_to);
				return _wctomb_l(
					character_to,
					character_from,
					*ox::il::locale_t::get_sys());
			}
			/// �ַ�-�ַ�
			template<typename character_from_tn, typename character_to_tn>
			static int convert(
				character_from_tn const & character_from,
				character_to_tn & character_to,
				ox::il::locale_t const & locale)
			{
				if (&character_from!=&character_to)
					character_to=character_from;
				return 1;
			}
			template<>
			static int convert<char,wchar_t>(
				char const & character_from,
				wchar_t & character_to,
				ox::il::locale_t const & locale)
			{
				return _mbtowc_l(
					&character_to,
					&character_from,
					getv_multibyte_current_max(),
					*locale);
			}
			template<>
			static int convert<wchar_t,char>(
				wchar_t const & character_from,
				char & character_to,
				ox::il::locale_t const & locale)
			{
				if (character_from>127)
				{
					character_to='?';
					return -1;
				}
				return _wctomb_l(
					&character_to,
					character_from,
					*locale);
			}
			///
			template<typename character_tn>
			static int convert(
				character_tn const & character_from,
				character_tn & character_to)
			{
				if (&character_from!=&character_to)
					character_to=character_from;
				return 1;
			}

		};


	} /// end of string
} /// end of ox


/**
	2008-11-5, ��ɶ�std::string�Ĵ�����֤�ˣ�ʹ���������ӷ���
			��stl::string���ַ����ڴ��ʹ�ã����˸���һ�����˽⣬��Ҫ�õ�resize,reserve,assign,contructor.
*/