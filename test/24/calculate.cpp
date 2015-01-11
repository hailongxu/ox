// work.cpp : Defines the entry point for the console application.
//

/**
	author: xu hailong 
	date: 2008-8-10
	desc: �㷨��ʵ���˸���һ�����ֺ�һ��Ŀ������ֻ����+-��/()���һ�����ʽ��
	ʹ��������ʽ�Ľ���������Ŀ����
	history: 2008-8-15
		������һЩ�����̣�����������Ӧ���Ż����ܹ����������н⣬����ӡ����ƥ��Ľ��
*/

#include "stdafx.h"
#include "assert.h"
#include "conio.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <limits>
#include <stack>


/// type�ļ򵥵��ڴ������
template <typename type_tn>
struct type_allocator
{
	typedef std::vector<type_tn*> type_ptr_list;
	/// ����һ���µ�data_set�����ҷ���һ���µ�data_set������
	static type_tn * new_instance()
	{
		return new type_tn();
	}
	~type_allocator()
	{
		for (type_ptr_list::iterator i=refv_data_set_ptr_list().begin(); i!=refv_data_set_ptr_list().end(); i++)
			if (*i) delete *i;
	}
private:
	static type_ptr_list & refv_data_set_ptr_list()
	{
		static type_ptr_list static_data_set_ptr_list;
		return static_data_set_ptr_list;
	}
};


struct operator_id_util
{
public:
	enum operator_id
	{
		__add=-1,
		__sub=-2,
		__mul=-3,
		__div=-4,
		__xxx=-5,
	};
	static char get_operator_char(operator_id id)
	{
		return *(getp_ids()-id);
	}
	static char get_operator_char(char id)
	{
		return *(getp_ids()-id);
	}
	static bool is_operator_id(char id)
	{
		return id<=__add && id>=__div;
	}
private:
	static const char * getp_ids()
	{
		static const char _m_id[5] = {'@','+','-','*','/'};
		return _m_id;
	}
};

/**
	��ÿһ�����������а�װ����number-rpn����ʽ���а�װ��
	�Ա�������֪��number��Ӧ�ı��ʽ
	�ṩ�ˣ����������������㣬������+ - �� /
*/
struct number
{
	typedef std::vector<unsigned char> rpn_t;
	typedef double value_t;
	/// �����original_index_t�Ǵ�0��ʼ��
	typedef rpn_t::value_type original_index_t;

	number(){}

	number(value_t value, char index):_m_value(value)
	{
		_m_rpn.push_back(index);
	}
	rpn_t _m_rpn;
	value_t _m_value;
};

struct operand;

/**
	@brief �洢���Ǽ���������ݣ�key_value���Ա��Ժ����
		��û�д洢��������������������������Ϊ�����������ò���ʧ��
		�������γ˷�������ȡһ�γ˷��Ľ����
		���ԣ���ֻ������ڵ��������������ϵĽ���������Ƚϻ���
*/
template <typename operand_tn>
class db_value
{
public:
	typedef operand_tn operand_t;
	db_value():_m_array_rpn_operand(0),_m_number_count(0)
	{}
	db_value(unsigned char number_count)
	{
		set_number_count(number_count);
	}
	~db_value()
	{
		delete [] _m_array_rpn_operand;
		delete [] _m_operand_for_2_operands;
		_m_array_rpn_operand = 0;
		_m_operand_for_2_operands = 0;
	}
public:
	typedef std::map<number::rpn_t, operand_t*> rpn_operand_ptr_t;
	struct operand_ext
	{
		operand_ext():_m_is_initialized(false) {}
		bool _m_is_initialized;
		operand_t _m_operand;
	};
	void set_number_count(unsigned char number_count)
	{
		_m_number_count = number_count;
		_m_array_rpn_operand = new rpn_operand_ptr_t [number_count*2];
		_m_operand_for_2_operands = new operand_ext[number_count*number_count];
		number tmp_number;
		tmp_number._m_rpn.push_back(0);
		tmp_number._m_rpn.push_back(0);
		tmp_number._m_rpn.push_back(0);
		for (size_t i=0; i<number_count; i++)
		{
			tmp_number._m_rpn[0] = i;
			for (size_t j = 0; j<number_count; j++)
			{
				size_t index = i*number_count+j;
				tmp_number._m_rpn[1] = j;
				tmp_number._m_rpn[2] = operator_id_util::__add;
				_m_operand_for_2_operands[index]._m_operand.push_back(tmp_number);
				tmp_number._m_rpn[2] = operator_id_util::__sub;
				_m_operand_for_2_operands[index]._m_operand.push_back(tmp_number);
				tmp_number._m_rpn[2] = operator_id_util::__mul;
				_m_operand_for_2_operands[index]._m_operand.push_back(tmp_number);
				tmp_number._m_rpn[2] = operator_id_util::__div;
				_m_operand_for_2_operands[index]._m_operand.push_back(tmp_number);
				tmp_number._m_rpn[2] = operator_id_util::__xxx;
				_m_operand_for_2_operands[index]._m_operand.refv_rpn().assign(
					tmp_number._m_rpn.begin(),
					tmp_number._m_rpn.end());
			}
		}
	}
	operand_t * getp_operand(
		unsigned char key_size,
		number::rpn_t const & key)
	{
		db_value::rpn_operand_ptr_t::iterator i= _m_array_rpn_operand[key_size].find(key);
		return i!=_m_array_rpn_operand[key_size].end() ? i->second : 0;
	}

	bool getp_operand(unsigned char index, operand * & refp_operand)
	{
		refp_operand = &_m_operand_for_2_operands[index]._m_operand;
		return _m_operand_for_2_operands[index]._m_is_initialized;
	}
	operand_ext * getp_operand_ext(unsigned char index)
	{
		return &_m_operand_for_2_operands[index];
	}
	void put_operand(unsigned char key_size, number::rpn_t const & rpn, operand * an_operand_ptr)
	{
		_m_array_rpn_operand[key_size][rpn] = an_operand_ptr;
	}
	void put_operand(
		operand_ext & refv_operand_ext,
		number::value_t const & add_value,
		number::value_t const & sub_value,
		number::value_t const & mul_value,
		number::value_t const & div_value,
		bool div_by_zero = false)
	{
		typename operand_t::iterator i = refv_operand_ext._m_operand.begin();
		i->_m_value = add_value; i++;
		i->_m_value = sub_value; i++;
		i->_m_value = mul_value; i++;
		if (div_by_zero)
			refv_operand_ext._m_operand.pop_back();
		else
			i->_m_value = div_value; i++;
		refv_operand_ext._m_is_initialized=true;
	}
	unsigned int get_number_count() { return _m_number_count; }
private:
	rpn_operand_ptr_t * _m_array_rpn_operand;
	unsigned int _m_number_count;
	operand_ext * _m_operand_for_2_operands;
public:
	static db_value & refv_db()
	{
		static db_value static_db_value;
		return static_db_value;
	}
};

/**
	����������Ԫ�ؼ���Ľ�������γ�һ�����ϣ�������Ͼ���Ϊһ�������ļ��ϣ����ڹ��������У�
	ʵ���ˣ��������ϵ��������㣬������data_t����������
	atom_set��һ��Ԫ��ʱ���ɵ��ɳ���һ�����ˣ�
	atom_set��������ʱ������
	����������ָ������������+-��/��������ӣ����������������ļ��������ǲ�����
*/
struct operand : public std::vector<number>
{
	/**
		@brief ÿ���ҵ�һ��ƥ����ʱ���ͻᴥ������¼�

		��para [in] object
			�ⲿ���Դ����¼�����Ķ���ָ��
		��para [in] hit_number
			��ʾ��ƥ�������
		��para [in] is_equal_number
			true : ��ʾƥ���������ȫ��ȵ�
			false : ��ʾƥ��������Ƶ�
		@return [ret]
			true : ��ʾ����
			false ����ʾ���ټ���
	*/
	typedef bool (*match_one_number_d)(
		void * object, 
		number * hit_number, 
		bool is_equal_number);

	typedef std::vector<number> base_type;
	typedef base_type::iterator iterator;
	typedef base_type::value_type value_type;
	typedef value_type::rpn_t operand_rpn_t;
	typedef type_allocator<operand> operand_allocator;

	/**
		@brief ����������������+-��/������������һ���µ�operand��
			 �����ӱ�������У����ң�����У�����ԭ���ģ��Ͳ���
			 ���¼�����
	*/
	static void melt(
		operand const & ref_one_operand,
		operand const & ref_another_operand,
		operand * & result_operand_ptr)
	{
		/// �������operand��rpn_id
		operand::operand_rpn_t rpn;
		rpn.insert(	rpn.end(),
			ref_one_operand._m_operand_rpn.begin(),
			ref_one_operand._m_operand_rpn.end());
		rpn.insert(rpn.end(),
			ref_another_operand._m_operand_rpn.begin(),
			ref_another_operand._m_operand_rpn.end());
		rpn.push_back(operator_id_util::__xxx);

		if (rpn.size()<3)
		{
			assert(false);
			return ;
		}

		result_operand_ptr = 0;

		if (rpn.size()==3)
		{
			value_type tmp;
			const value_type & number_one = ref_one_operand.front();
			const value_type & number_another = ref_another_operand.front();
			/// �Ǽ򵥵���������ˣ������ر����Ż�
			db_value<operand>::operand_ext * p  = db_value<operand>::refv_db().getp_operand_ext(
				number_one._m_rpn[0]*db_value<operand>::refv_db().get_number_count()+number_another._m_rpn[0]);
			result_operand_ptr = & p->_m_operand;
			if (p->_m_is_initialized)
				result_operand_ptr = &p->_m_operand;
			/// ���û�У����¼��㣬����������
			else if (number_another._m_value)
			{
				db_value<operand>::refv_db().put_operand(
					*p,
					number_one._m_value + number_another._m_value,
					number_one._m_value - number_another._m_value,
					number_one._m_value * number_another._m_value,
					number_one._m_value / number_another._m_value,
					false); /// ��ʾ������Ϊ��
			}
			else
			{
				db_value<operand>::refv_db().put_operand(
					*p,
					number_one._m_value + number_another._m_value,
					number_one._m_value - number_another._m_value,
					number_one._m_value * number_another._m_value,
					number_one._m_value / number_another._m_value,
					true);/// ��ʾ����Ϊ��
			}
		}
		else if (rpn.size()>3)
			result_operand_ptr = db_value<operand>::refv_db().getp_operand(rpn.size(),rpn);

		if (result_operand_ptr)
			return ;
		else
			result_operand_ptr = operand_allocator::new_instance();

		operand & result_operand = * result_operand_ptr;

		/// ��ǰoperand_rpn���и�ֵ
		result_operand._m_operand_rpn.assign(rpn.begin(),rpn.end());
			
		value_type tmp;

		/**
			������������û�н��й�����ģ����¿�ʼ����
		*/
		for (const_iterator i_one = ref_one_operand.begin(); i_one!=ref_one_operand.end(); i_one++)
			for (const_iterator i_another=ref_another_operand.begin(); i_another!=ref_another_operand.end(); i_another++)
			{
				const value_type & number_one = *i_one;
				const value_type & number_another = *i_another;
				unsigned char one_size = number_one._m_rpn.size();
				unsigned char another_size = number_another._m_rpn.size();
				unsigned char two_size = one_size + another_size;

				//// ����������㣬��Ϊ�˽��з�ֹ������ڴ濽�����Ӽ��˳����ڵ�ǰ�ı���У�ֻ������һλ��һ��
				tmp._m_rpn.assign(number_one._m_rpn.begin(),number_one._m_rpn.end());
				tmp._m_rpn.insert(tmp._m_rpn.end(),number_another._m_rpn.begin(),number_another._m_rpn.end());
			
				tmp._m_rpn.push_back(operator_id_util::__add);
				tmp._m_value = number_one._m_value + number_another._m_value;
				result_operand.push_back(tmp);
				tmp._m_rpn[two_size]=operator_id_util::__sub;
				tmp._m_value = number_one._m_value - number_another._m_value;
				result_operand.push_back(tmp);
				tmp._m_rpn[two_size]=operator_id_util::__mul;
				tmp._m_value = number_one._m_value * number_another._m_value;
				result_operand.push_back(tmp);
				/// ��ֹ����Ϊ��
				if (number_another._m_value!=0)
				{
					tmp._m_rpn[two_size]=operator_id_util::__div;
					tmp._m_value = number_one._m_value / number_another._m_value;
					result_operand.push_back(tmp);
				}
			} // end of inner for loop
	} // end of melt function

	/**
	��brief ����һ��ֵ���ҳ�������Ǹ�iterator
	@param [in] is_equal_matched : ����������У�ƥ��������ȵģ����ǽ��Ƶ�
		����������Ĺ�ϵ�ǻ����
	@return [ret] ����match_handle�¼��ķ��ؽ��
	*/
	bool find_value(
		match_one_number_d match_event,
		void * object,
		value_type::value_t des_value,
		unsigned int & matched_count,
		bool & is_equal_matched)
	{
		value_type::value_t sub_value_prev = std::numeric_limits<double>::max();
		value_type::value_t sub_value_curr = value_type::value_t();
		iterator proximal_iterator = this->end();

		bool ret_value = true;
		matched_count = 0;
		bool has_met_equal_value = false;
		for (iterator i=this->begin(); i!=end(); i++)
		{
			sub_value_curr = i->_m_value-des_value;
			if (sub_value_curr<0)
				sub_value_curr = -sub_value_curr;
			if (sub_value_curr==0.0)
			{
				has_met_equal_value = true;
				matched_count ++;
				if (match_event)
					if (!match_event(object,&(*i),true))
					{
						ret_value = false;
						break;
					}
			}
			else if (!has_met_equal_value && sub_value_curr<sub_value_prev)
			{
				sub_value_prev = sub_value_curr;
				proximal_iterator = i;
			}
		}
		if (!has_met_equal_value)
			if (match_event)
				matched_count++,
				ret_value=match_event(object,&(*proximal_iterator),false);
		return ret_value;
	}

public:
	operand_rpn_t & refv_rpn() { return _m_operand_rpn; }

	void push_back_to_rpn(operand_rpn_t const & rpn)
	{
		_m_operand_rpn.insert(_m_operand_rpn.end(), rpn.begin(), rpn.end());
	}
private:
	operand_rpn_t _m_operand_rpn;
};

typedef operand::operand_allocator operand_allocator;

/**
	@description
	���������ϣ�ȥ�����ʽ·���ϵ�һ���м伯�ϣ�
	ֱ�����������ֻʣ��һ���������ˣ�
	���˵��������������ϵĲ���ȫ�����

	ȡ������ϵ������������������������㣬��������
	P[n,2]���µĲ���������
*/

struct operand_set : public std::vector<operand*>
{
	/// �����һ���¼�����
	typedef void (*gen_new_data_set_d)(void * object, operand_set * new_data_set);
	typedef std::vector<operand*> operand_vec;
	typedef operand_vec::value_type operand_ptr;
	typedef operand_vec::iterator iterator;
	typedef type_allocator<operand_set> operand_set_allocator;

	/// ���һ��������ȡ������������Ȼ�������P[n,2]��data_set����
	/// ��������һ��ʱ�䣬����ÿ����һ�µļ��ϣ��ͻᴥ��һ���¼�
	/// �ⲿ���򣬻�ӹ�����¼�������Ӧ�Ĵ���
	bool do_one_operation(void * object, gen_new_data_set_d action)
	{
		if (size()<=1 || action==0) return false;
		/// ��ȥ��Ԫ��֮�������
		for (iterator i=begin(); i!=end(); i++)
		{
			/// ��ǰԪ�ص�ǰ�벿��
			for (iterator p=begin(); p!=i; p++)
				action(object, make_a_new_operand_set(i,p));
			/// ��ǰԪ�صĺ�벿��
			for (iterator q=i+1; q!=end(); q++)
				action(object, make_a_new_operand_set(i,q));
		}
		return true;
	}

	/// index : �û�����ԭ�в��������򣬴�0��ʼ���
	number::value_t get_original_value(number::original_index_t index) const
	{
		assert(index>=0 && index<this->size());
		return at(index)->front()._m_value;
	}

protected:
	operand_set * make_a_new_operand_set(iterator & left, iterator & right)
	{
		operand_ptr result_operand_ptr = 0;
		operand::melt(**left,**right,result_operand_ptr);
		operand_set * new_operand_set = operand_set_allocator::new_instance();
		/// ����û�вμ�����Ĳ�����
		for (iterator i=begin(); i!=end(); i++)
		{
			if (i!=left && i!=right)
				new_operand_set->push_back(*i);
		}
		/// ��������������Ľ��
		new_operand_set->push_back(result_operand_ptr);
		return new_operand_set;
	}
};

typedef operand_set::operand_set_allocator operand_set_allocator;

/**
	��������������ϵ����
	���������п��ܵĽ����
	ÿһ���������һ�������Ĳ���������
*/
class work_set
{
	typedef std::list<operand_set*> muti_operand_ptr_set;

public:
	work_set() : _m_current_muti_data_set(0){}
	void start (operand_set const & refv_input_data_set)
	{
		operand_set * new_data_set = operand_set_allocator::new_instance();
		new_data_set->insert(new_data_set->end(), refv_input_data_set.begin(), refv_input_data_set.end());
		init_current_multi_data_set();
		_m_current_muti_data_set->push_back(new_data_set);

		while(true)
		{
			bool is_done = false;

			for (muti_operand_ptr_set::iterator i=_m_current_muti_data_set->begin(); i!=_m_current_muti_data_set->end(); i++)
			{
				if (!(*i)->do_one_operation(getp_the_other_multi_data_set(_m_current_muti_data_set),new_data_set_gen_handle))
				{
					is_done = true;
					break;
				}
			}
			/// �ж���һ�εļ�����
			if (!is_done) // û������
			{
				_m_current_muti_data_set->clear(),
				_m_current_muti_data_set=getp_the_other_multi_data_set(_m_current_muti_data_set);
			}
			else /// ����������ˣ����˳�
			{
				break;
			}
		}
	}
	/**
	@brief ��������еļ����л�ȡ����Ҫ�Ľ�������Ǻ�Ŀ������ƥ���
	*/
	bool find_result(
		operand::match_one_number_d match_event,
		void * object,
		number::value_t dest_number,
		unsigned int & matched_count)
	{
		if (!getp_current_multi_data_set())
		{
			assert(false);
			return 0;
		}
		bool ret_value = true;
		bool has_met_matched_equal = false;
		unsigned int match_count = 0;
		for (muti_operand_ptr_set::iterator i=getp_current_multi_data_set()->begin();
			i!=getp_current_multi_data_set()->end();
			i++)
		{
			if ((*i)->size()==1) /// it is a final set
			{
				/// ��ȡÿ�������еĵ�һ��������
				operand & first_operand = *((*i)->front());
				/// ����������е�����ƥ��Ľ��
				unsigned int this_matched_count = 0;
				ret_value=first_operand.find_value(
					match_event, /// ��Ӧ�¼�
					object, /// �¼���������
					dest_number, /// Ŀ����
					this_matched_count, /// ���������ƥ��ĸ���
					has_met_matched_equal /// ƥ������ģʽ
					); 
				matched_count += this_matched_count;
				if (!ret_value)
					break;
			}
			else /// �������Ľ����������Ѱ�ҽ��
			{
				assert(false);
				continue;
			}
		}
		return ret_value;
	}
private:
	void init_current_multi_data_set()
	{
		_m_current_muti_data_set = &_m_final_data_set_array[0];
	}
	muti_operand_ptr_set * getp_current_multi_data_set()
	{
		return _m_current_muti_data_set;
	}
	muti_operand_ptr_set * getp_the_other_multi_data_set(muti_operand_ptr_set * p)
	{
		if (p==_m_final_data_set_array)
			return &_m_final_data_set_array[1];
		else
			return &_m_final_data_set_array[0];
	}
	muti_operand_ptr_set * _m_current_muti_data_set;
	muti_operand_ptr_set _m_final_data_set_array[2];

private:
	static void new_data_set_gen_handle(void * object, operand_set * new_operand_set_ptr)
	{
		muti_operand_ptr_set * p = (reinterpret_cast<muti_operand_ptr_set*>(object));
		p->push_back(new_operand_set_ptr);
	}
};

/// ����rpn���沨��ʽ����ʵ�ó���
struct rpn_util
{
	typedef number::rpn_t rpn_t;
	typedef std::stack<rpn_t> rpn_stack_t;

	static std::string translate(rpn_t const & ref_rpn, operand_set const & ref_original)
	{
		rpn_stack_t _m_rpn_stack;
		for (rpn_t::const_iterator i=ref_rpn.begin(); i!=ref_rpn.end(); i++)
		{
			rpn_t trpn;
			if (operator_id_util::is_operator_id(*i))
			{
				rpn_t right_opn = _m_rpn_stack.top(); _m_rpn_stack.pop();
				rpn_t left_opn = _m_rpn_stack.top(); _m_rpn_stack.pop();
				combian(trpn,left_opn,*i,right_opn,ref_original);
				_m_rpn_stack.push(trpn);
			}
			else
			{
				trpn.push_back(*i);
				_m_rpn_stack.push(trpn);
			}
		}
		std::string string_exp;
		return to_string(_m_rpn_stack,string_exp);
	}
private:
	static std::string & to_string(rpn_stack_t const & rpn_stack, std::string & string_exp)
	{
		typedef rpn_stack_t::value_type rpn_t;
		string_exp.clear();
		rpn_t const & rpn = rpn_stack.top();
		for(rpn_t::const_iterator i = rpn.begin(); i!=rpn.end(); i++)
			string_exp.push_back(*i);
		return string_exp;
	}
	static void combian(rpn_t & result_rpn, rpn_t & left_opn, int id, rpn_t & right_opn, operand_set const & ref_original)
	{
		if (left_opn.size()==1)
		{
			char index = left_opn.front();
			char buf[128] = { 0 };
			sprintf(buf,"%d",static_cast<long>(ref_original.get_original_value(index)));
			std::string s(buf);
			left_opn.clear();
			left_opn.insert(left_opn.end(),s.begin(),s.end());
		}
		if (right_opn.size()==1)
		{
			char index = right_opn.front();
			char buf[128] = { 0 };
			sprintf(buf,"%d",static_cast<long>(ref_original.get_original_value(index)));
			std::string s(buf);
			right_opn.clear();
			right_opn.insert(right_opn.end(),s.begin(),s.end());
		}
		result_rpn.insert(result_rpn.end(),'(');
		result_rpn.insert(result_rpn.end(),left_opn.begin(),left_opn.end());
		result_rpn.insert(result_rpn.end(),operator_id_util::get_operator_char(id));
		result_rpn.insert(result_rpn.end(),right_opn.begin(),right_opn.end());
		result_rpn.insert(result_rpn.end(),')');
	}
};

/// ��һ�������ݰ�װ��һ����ԭʼ�ģ��û����뼯��
class input
{
public:
	input():_m_next_sequence(0),_m_dest_value(0)
	{}

	/// ����һ��������
	void append_number(long x)
	{
		operand * atom_operand_ptr = operand_allocator::new_instance();
		operand & atom_operand = * atom_operand_ptr;
		atom_operand.push_back_to_rpn(operand::operand_rpn_t(1,_m_next_sequence));
		atom_operand.insert(atom_operand.end(),number(x,_m_next_sequence++));
		_m_original_set.push_back(atom_operand_ptr);
	}
	void append_number(char * x_string)
	{
		assert(x_string);
		if (x_string)
			append_number(atoi(x_string));
	}

	/// ����һ��Ŀ����
	void set_dest_number(long dest_value)
	{
		_m_dest_value = dest_value;
	}
	void set_dest_number(char * str_dest_value)
	{
		assert(str_dest_value);
		if (str_dest_value)
			set_dest_number(atoi(str_dest_value));
	}
	
	long get_dest_value()  { return _m_dest_value; }

	operand_set & refv_original_set()
	{
		return _m_original_set;
	}

private:
	operand_set _m_original_set;
	unsigned int _m_next_sequence;
	long _m_dest_value;
};

/// ��֯ʵ�ʵ����㣬��������
class work
{
public:
	work():_m_input(0){}
	work(input & input_set)
	{
		set_input_set(input_set);
	}

	/// �����ʼ����
	void set_input_set(input & input_set)
	{
		_m_input = & input_set;
		db_value<operand>::refv_db().set_number_count(input_set.refv_original_set().size());
	}
	void start()
	{
		number tmp;
		tmp._m_value = std::numeric_limits<double>::max();
		_m_proximal_number_list.push_back(tmp);
		refv_work_set().start(refv_input().refv_original_set());
	}
	/// ������еĽ��
	bool get_all_matched_exp_list(std::vector<std::string> & out_string_list)
	{
		unsigned int matched_count = 0;
		bool find_event_ret = refv_work_set().find_result(
			process_find_matched_number_handle,
			this,
			refv_input().get_dest_value(),
			matched_count);

		bool ret_value = true;
		if (_m_matched_number_list.size()>0) /// ����ȫƥ������
		{
			/// ����ÿһ������������沨�����ʽת���������ı��ʽ��ʾ����
			for(operand::iterator i=_m_matched_number_list.begin(); i!=_m_matched_number_list.end(); i++)
				out_string_list.push_back(rpn_util::translate(i->_m_rpn,refv_input().refv_original_set()));
		}
		else /// û����ȫƥ��������ֻ��һ������Ƶ�ֵ
		{
			/// ����ÿһ������������沨�����ʽת���������ı��ʽ��ʾ����
			for(operand::iterator i=_m_proximal_number_list.begin(); i!=_m_proximal_number_list.end(); i++)
				out_string_list.push_back(rpn_util::translate(i->_m_rpn,refv_input().refv_original_set()));
			ret_value = false;
		}

		return ret_value;
	}
	void print_exp_list(std::vector<std::string> & exp_list)
	{
		char dest_value_show[32] = {0};
		printf("Inputed Numbers : ");
		for (operand_set::iterator k=refv_input().refv_original_set().begin();
			k!=refv_input().refv_original_set().end();
			k++)
			printf("%0.0lf  ",(*k)->front()._m_value);
		sprintf(dest_value_show,"\nInputed Destinated Number : %ld",refv_input().get_dest_value());
		printf(dest_value_show);
		if (_m_matched_number_list.size()>0)
			printf("\n\nHere, totoally found %d expressions matched\n", exp_list.size());
		else
			printf("\n\nHere, no expressions exactly matched, but find proximally %d matched\n", exp_list.size());

		typedef std::vector<std::string> exp_list_t;
		unsigned int index = 1;
		for (exp_list_t::iterator i=exp_list.begin(); i!=exp_list.end(); i++)
		{
			printf("\n [ %i ] = ", index++);
			printf(i->c_str());
		}
	}
private:
	input & refv_input() { assert(_m_input); return *_m_input; }
	work_set & refv_work_set() { return _m_work_set; }
	work_set _m_work_set;
	input * _m_input;
	operand _m_matched_number_list;
	operand _m_proximal_number_list;
private:
	static bool process_find_matched_number_handle(void * object, number * hit_number, bool is_equal_number)
	{
		work * calculate_ptr = static_cast<work*>(object);
		/// �жϲ�����Ч�ԣ�����ȡ�������
		if (object==0 || !calculate_ptr || !hit_number)
		{
			assert(false);
			return false;
		}
		if (is_equal_number)
		{
			/// ���ֵ�����ȣ��ͼ��뵽ָ�����б���
			calculate_ptr->_m_matched_number_list.push_back(*hit_number);
		}
		/// ����ǽ��ƣ��򻻳���ӽ����Ǹ�����Ȼ�ǣ�����û����ȵ������
		else if (!is_equal_number)
		{
			double curr_value = abs(hit_number->_m_value-calculate_ptr->refv_input().get_dest_value());
			double prev_value = abs(calculate_ptr->_m_proximal_number_list.front()._m_value-calculate_ptr->refv_input().get_dest_value());
			if (curr_value<prev_value)
			{
				calculate_ptr->_m_proximal_number_list.clear();
				calculate_ptr->_m_proximal_number_list.push_back(*hit_number);
			}
			else if (curr_value==prev_value)
			{
				calculate_ptr->_m_proximal_number_list.push_back(*hit_number);
			}
		}

		return true; /// ��ʾ����Ѱ����һ��ƥ��ֵ
	}
	static number::value_t abs(number::value_t const & value)
	{
		if (value<0.0)
			return -value;
		return value;
	}
};

/**
	�������֣�
		��1�������ǵ�1������
		��2�������ǵ�2������
		....
		���һ���ǣ�Ŀ����
*/
int main(int argc, char * argv[])
{
	input original_input;
	/// ��ȡ��������
	if (argc>=3)
	{
		printf("input number : ");
		int i = 0;
		for (i=1; i<argc-1; i++)
		{
			original_input.append_number(argv[i]);
			printf(" %ld ", atoi(argv[i]));
		}
		original_input.set_dest_number(argv[i]);
		printf("\ndest number : %ld\n",atoi(argv[i]));
	}
	else
	{
		printf(
			"yeah, you do not input number\n"
			"usage : the last one is dest number, others is operand number !! \n"
			"\teg. work 2 2 3 4 8 \n\n"
			"we input the default number: 1 2 3 4 \n"
			"default dest number: 7\n");
		original_input.append_number(1);
		original_input.append_number(2);
		original_input.append_number(3);
		original_input.append_number(4);
		original_input.set_dest_number(7);
	}

	/// ���㣬���ó����
	work calculate(original_input);
	calculate.start();
	std::vector<std::string> exp_list;
	calculate.get_all_matched_exp_list(exp_list);

	/// ��ӡ��������
	printf ("\n>>>>>>>>>\n");
	calculate.print_exp_list(exp_list);

	// �����˳�
	printf("\n\n press any key to exit");
	getch();

	return 0;
}