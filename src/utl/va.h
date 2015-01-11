

#include "../sys/gp_typetrait.h"
#include <stdarg.h>

#pragma once


/// 可变参数都是值传递的

namespace ox
{
	namespace utl
	{

		enum va_over_condition_t
		{
			__counter,
			__sentinel,
		};

		template <typename va_type_tn, va_over_condition_t type>
		class va
		{
		public:
			typedef va_over_condition_t over_condition_t;
			typedef va_type_tn va_type_t;


			template <over_condition_t type>
			struct condition_tt{};

			template <>
			struct condition_tt<__counter>
			{
				typedef size_t condition_t;

				size_t counter;
				size_t current;
				bool is_eof()
				{
					return current>=counter;
				}
				void operator ++()
				{
					current ++;
				}
				void assign_condition(va_type_t * current, condition_t max_counter)
				{
					counter = max_counter;
					this->current = 0;
				}
			};
			template <>
			struct condition_tt<__sentinel>
			{
				typedef va_type_t condition_t;
				va_type_t over_sentinel;
				va_type_t * current;
				bool is_eof()
				{
					return *current == over_sentinel;
				}
				void operator ++()
				{}
				void assign_condition(va_type_t * current, condition_t sentinel)
				{
					this->current = current;
					over_sentinel = sentinel;
				}
			};

			typedef condition_tt<type> condition_whole_t;
			typedef typename condition_whole_t::condition_t condition_t;


			va() {}
			va(va_type_t const & para_start, condition_t condition) { mount(para_start,condition); }
			~va () { over(); }

			void mount(va_type_t const & para_start,  condition_t condition)
			{
				va_start(argp, para_start);
				para = para_start;
				vcondition.assign_condition(const_cast<va_type_t*>(&para),condition);
			}

			bool is_eof()
			{
				return vcondition.is_eof();
			};

			va & operator ++ ()
			{
				para = va_arg(argp,va_type_t);
				vcondition ++;
				return *this;
			}
			va_type_t & operator * ()
			{
				return para;
			}

			void over()
			{
				va_end(argp);
			}
			
		private:
			va_list argp;
			va_type_t para;
			condition_whole_t vcondition;
		};

		template <typename va_type_tn>
		class va_counter : public va<va_type_tn,__counter>
		{
		public:
			typedef va<va_type_tn,__counter> va_inner_t;
			typedef typename va_inner_t::condition_t condition_t;
			va_counter():va_inner_t(){}
			va_counter(va_type_tn const & para_start, condition_t max_counter):va_inner_t(para_start,max_counter){}
		};

		template <typename va_type_tn>
		class va_sentinel : public va<va_type_tn,__sentinel>
		{
		public:
			typedef va<va_type_tn,__sentinel> va_inner_t;
			typedef typename va_inner_t::condition_t condition_t;
			va_sentinel():va_inner_t(){}
			va_sentinel(va_type_tn const & para_start, condition_t sentinel):va_inner_t(para_start,sentinel){}
		};


	} /// end of utl
} /// end of ox