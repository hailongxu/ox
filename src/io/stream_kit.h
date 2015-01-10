

/**
	����: shell_kit
	����: �캣��
	����: 2005-6-20
	�ļ���: directory.h
	��Ȩ����Ȩ���������У����ļ��������ɴ���
	�Ա�׼�ļ��Ķ��⴦��Ŀǰֻ������getline������õĹ���
*/


#include <fstream>
#include "./file.h"
#include "../utl/array_simple.h"
#include "../cor/allocator.h"
//#include "../cor/mm_line.h"
#include "../cor/mm_m.h"
#pragma warning(push)
#pragma warning(disable:4355)	// �ڹ��캯����ʹ����this


#pragma once

/**
	���ص�������new�����ģ�����Ҫ�ֹ��ͷ�����ڴ��
*/

namespace ox
{
namespace stream
{

//template <typename unit_tn, typename object_tn>
//
//class rstream_tt
//{
//	typedef long position_t;
//	typedef unit_tn unit_t;
//	typedef object_tn object_t;
//
//	position_t tell(); // ��ǰλ��
//	size_t get_units(unit_t * units, size_t units_count); // ȡunits
//	size_t get_unit(unit_t & unit); // ȡһ��unit
//	bool eof(); // �Ƿ����
//};
//
//template <typename unit_tn>
//class rstream_tt<unit_tn,ox::io::file<unit_tn>>
//{
//public:
//
//	typedef long position_t;
//	typedef unit_tn unit_t;
//	typedef ox::io::file<unit_t> object_t;
//	typedef ox::cor::mm_buffer<unit_t> allocator_t;
//	typedef rstream_tt<unit_tn,ox::io::file<unit_tn>> rstream_t;
//
//	static const size_t alloc_init_size = 3;
//
//	~rstream_tt/*<unit_tn,ox::io::file<unit_tn>>*/() {allocator.deallocate();}
//
//	void mount(object_t & object)
//	{
//		file=&object;
//		allocator.allocate(alloc_init_size);
//	}
//	void umount()
//	{
//		file=0;
//	}
//	position_t tell()
//	{
//		assert(file);
//		return refv_object().tell();
//	}
//	size_t get_units(unit_t * units, size_t units_count)
//	{
//		return refv_object().read(units,sizeof(unit_t),units_count);
//	}
//	size_t get_unit(unit_t & unit)
//	{
//		if (eof()) return 0;
//		unit = refv_object().getc();
//		return 1;
//	}
//	// units (out)
//	size_t get_units_till_guard(unit_t * & units,unit_t const & guard)
//	{
//		// ��ǰָ����÷���ջ��ָ���÷�һ����ָ�����һ��Ԫ�ص���һ����ַ
//		//refv_object().lock();
//		unit_t * beappended=allocator.begin();
//		for(;
//			refv_object().read(beappended,sizeof(unit_t),1) /*&&
//			ox::mem::mem_kit<char>::comp_mem(beappended,&guard,sizeof(unit_t))*/;
//			beappended++)
//		{
//			if (beappended-allocator.begin()>allocator.size())
//			{
//				allocator.allocate(allocator.size()<<2);
//				beappended=0;//allocator.more();
//			}
//		}
//		units = allocator.begin();
//		return beappended-allocator.begin();
//	}
//	bool eof()
//	{
//		return refv_object().eof();
//	}
//
//private:
//	object_t & refv_object() {assert(file); return * file;}
//	object_t * file;
//	allocator_t allocator;
//};
//
//template <typename unit_tn>
//class rstream_tt<unit_tn,ox::utl::array_simple_wrapper_tt<unit_tn>>
//{
//public:
//	typedef long position_t;
//	typedef unit_tn unit_t;
//	typedef ox::utl::array_simple_wrapper_tt<unit_t> object_t;
//	typedef ox::cor::mm_buffer<unit_t> allocator_t;
//
//	void mount(object_t & object_)
//	{
//		object=&object_;
//		iterator_element = object.getp_begin();
//	}
//	void umount()
//	{
//		object=iterator_element=0;
//	}
//	position_t tell()
//	{
//		assert(object);
//		return iterator_element-refv_object().getp_begin();
//	}
//	size_t get_units(unit_t * units, size_t units_count)
//	{
//		size_t move_count = ox::mem::mem_kit<char>::move_mem(
//			iterator_element,
//			(refv_object().getp_end()-iterator_element)*sizeof(unit_t),
//			refv_object().getc_count()*
//			units,
//			units_count*sizeof(unit_t));
//		iterator_element += move_count;
//		return move_count;
//	}
//	size_t get_unit(unit_t & unit)
//	{
//		if (eof()) return 0;
//		unit = *iterator_element;
//		iterator_element++;
//		return 1;
//	}
//	size_t get_units_till_guard(unit_t * & units,unit_t const & guard)
//	{
//		assert(iterator_element>object.getp_end()&&iterator_element<object.getp_begin());
//		unit_t * beappended=allocator.memory();
//		for(;
//			iterator_element < refv_object().getp_end();
//			beappended++,iterator_element ++;)
//		{
//			if (ox::mem::mem_kit<char>::comp_mem(iterator_element,&guard,sizeof(unit_t)))
//				break;
//			if (beappended-allocator.memory()>allocator.size())
//			{
//				allocator.allocate(allocator.size()<<2);
//				beappended = allocator.more();
//			}
//			*beappended = *iterator_element;
//		}
//	}
//	bool eof()
//	{
//		return iterator_element>=refv_object().getp_end();
//	}
//private:
//	object_t & refv_object() {assert(object); return * object;}
//	object_t * object;
//	unit_t * iterator_element;
//	allocator_t allocator;
//};
//
//template <typename unit_tn>
//class stream_kit
//{
//
//};

template <typename unit_tn>

class stdstream_kit
{
public:
	typedef typename ox::character::character_trait<unit_tn>::character character;
	typedef ox::string::string_kit_traits<character> skit_trait;
	typedef ox::io::file<character> ox_stdfile;
	typedef ox::cor::allocator_line<character> allocator;
	//typedef ox::cor::mm_buffer<character> allocator;
public:
	stdstream_kit():pthis(this),m_file(0){}
	stdstream_kit(ox_stdfile & file):pthis(this),m_file(&file){}
	
	void attach(ox_stdfile & file) { m_file=&file; }
	void attach(ox_stdfile * file) { m_file=file; }
	void detach() { m_file=0; }
protected:
	class kit_inner_base
	{
	public:
		void attach(stdstream_kit & file_kit) { m_file_kit=&file_kit; }
		void attach(stdstream_kit * file_kit) { m_file_kit=file_kit; }
		void detach() { m_file_kit=0; }
	protected:
		kit_inner_base():m_file_kit(0){}
		kit_inner_base(stdstream_kit *file_kit):m_file_kit(file_kit){}
		kit_inner_base(stdstream_kit &file_kit):m_file_kit(&file_kit){}
		stdstream_kit * getp_stdfile_kit() { return m_file_kit; }
	private:
		stdstream_kit * m_file_kit;
	};

public:
	/// get_line_tt ���������¼�����
	/// ���¼���������У������Ҫ�ı������ڴ�ָ����ʼ��ַ����Ϊ��
	/// �����ڴ棬һ��Ҫ���¼��������ڴ�ĺ�����������
	template <size_t line_tmp_buff_len>
	class get_line_tt : public kit_inner_base
	{
	public:
		static const size_t __buffer_length = line_tmp_buff_len;
		get_line_tt(){}
		get_line_tt(stdstream_kit & file_kit):
		kit_inner_base(file_kit)
		{tmp_line_buffer[line_tmp_buff_len-1]=0;}
		~get_line_tt(){free();}

		void free() { line_allocator.deallocate(); }

	public:
		character * operator ()(size_t * count_unit_getted = 0)
		{
			long start_unit_read = 0, len_unit_read = 0;
			character * presult = line_allocator.allocate(__buffer_length);
			character * p_line_buffer_read = 0;

			assert(presult);
			if (!presult) return presult;

			tmp_line_buffer[0]=0;
			tmp_line_buffer[__buffer_length-1]=skit_trait::linefeed_character; // at current, it is no use

			start_unit_read = getp_stdfile_kit()->m_file->tell();

			for (size_t get_sum_len =0; p_line_buffer_read = getp_stdfile_kit()->m_file->gets(tmp_line_buffer,line_tmp_buff_len);)
			{
				get_sum_len += __buffer_length-1; // Ϊʲô��һ����Ϊ��getsʵ�ʷ��ص����ݱȻ�����Сһ����Ϊ���һ���ֽ�����������
				if (line_allocator.size()<get_sum_len+1)
				{
					presult = (character*)line_allocator.allocate(line_allocator.size()<<2);
					assert(presult);
					if (!presult) break;
				}
				ox::cor::mm_m::memove(
					presult+get_sum_len-(__buffer_length-1),
					tmp_line_buffer,
					__buffer_length-1); // ���һ���ַ����㣬���ÿ������˷�
				if (tmp_line_buffer[line_tmp_buff_len-1]==skit_trait::linefeed_character ||
					tmp_line_buffer[line_tmp_buff_len-2]==skit_trait::linefeed_character) // ok, get line ok
					break;
				tmp_line_buffer[__buffer_length-1]=skit_trait::linefeed_character;
			}
			len_unit_read = getp_stdfile_kit()->m_file->tell()-start_unit_read;
			len_unit_read /= sizeof(character);

			if (len_unit_read>0)
				presult[len_unit_read]=0;
			else
				presult = 0;
			if (count_unit_getted) *count_unit_getted = len_unit_read;
			return presult;
		}

	protected:
		allocator line_allocator;
	private:
		character tmp_line_buffer[line_tmp_buff_len];
	};

	class get_line_default_t : public get_line_tt<128>
	{
	public:
		typedef get_line_tt<4> get_line_base;
		get_line_default_t(){}
		get_line_default_t(stdstream_kit & file_kit):get_line_base(file_kit){}
	};


private:
	ox_stdfile * m_file;
	stdstream_kit * pthis;
};

/**
��ʷ��2007-12-30 �޸��˻������������ ԭ���ģ�tmp_line_buffer[line_stack_buffer_len]=0
*/

} // end of stream
} // end of ox

#pragma warning(pop)