// work.cpp : Defines the entry point for the console application.
//

/**
	author: xu hailong 
	date: 2008-8-10
	desc: 算法，实现了给定一组数字和一个目标数，只能用+-×/()组成一个表达式，
	使其这个表达式的结果等于这个目标数
	history: 2008-8-15
		更新了一些求解过程，并进行了相应的优化，能够搜索到所有解，并打印所有匹配的结果
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


/// type的简单的内存分配器
template <typename type_tn>
struct type_allocator
{
	typedef std::vector<type_tn*> type_ptr_list;
	/// 插入一个新的data_set，并且返回一个新的data_set的引用
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
	对每一个操作数进行包装，用number-rpn的形式进行包装，
	以便我们能知道number对应的表达式
	提供了，两个操作数的运算，包括，+ - × /
*/
struct number
{
	typedef std::vector<unsigned char> rpn_t;
	typedef double value_t;
	/// 输入的original_index_t是从0开始的
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
	@brief 存储我们计算过的数据，key_value，以备以后调用
		我没有存储单纯两个数的四则运算结果，因为觉得这个结果得不偿失？
		我用两次乘法，来换取一次乘法的结果，
		所以，我只保存大于等于两次运算以上的结果，这样比较划算
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
	对任意两个元素计算的结果，会形成一个集合，这个集合经作为一个单独的集合，放在工作集合中，
	实现了，两个集合的四则运算，利用了data_t的四则运算
	atom_set有一个元素时，可当成成了一个数了，
	atom_set代表了临时结果结合
	操作数：是指可以用来进行+-×/运算的因子，任意两个操作数的计算结果还是操作数
*/
struct operand : public std::vector<number>
{
	/**
		@brief 每次找到一个匹配项时，就会触发这个事件

		＠para [in] object
			外部可以传给事件处理的对象指针
		＠para [in] hit_number
			表示被匹配的数字
		＠para [in] is_equal_number
			true : 表示匹配的数是完全相等的
			false : 表示匹配的数近似的
		@return [ret]
			true : 表示继续
			false ：表示不再继续
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
		@brief 对两个操作数进行+-×/操作，并产生一个新的operand，
			 这里会从保存的数中，查找，如果有，就用原来的，就不再
			 重新计算了
	*/
	static void melt(
		operand const & ref_one_operand,
		operand const & ref_another_operand,
		operand * & result_operand_ptr)
	{
		/// 生成这个operand的rpn_id
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
			/// 是简单的两个数相乘，进行特别处理优化
			db_value<operand>::operand_ext * p  = db_value<operand>::refv_db().getp_operand_ext(
				number_one._m_rpn[0]*db_value<operand>::refv_db().get_number_count()+number_another._m_rpn[0]);
			result_operand_ptr = & p->_m_operand;
			if (p->_m_is_initialized)
				result_operand_ptr = &p->_m_operand;
			/// 如果没有，重新计算，并保存数据
			else if (number_another._m_value)
			{
				db_value<operand>::refv_db().put_operand(
					*p,
					number_one._m_value + number_another._m_value,
					number_one._m_value - number_another._m_value,
					number_one._m_value * number_another._m_value,
					number_one._m_value / number_another._m_value,
					false); /// 表示除数不为零
			}
			else
			{
				db_value<operand>::refv_db().put_operand(
					*p,
					number_one._m_value + number_another._m_value,
					number_one._m_value - number_another._m_value,
					number_one._m_value * number_another._m_value,
					number_one._m_value / number_another._m_value,
					true);/// 表示除数为零
			}
		}
		else if (rpn.size()>3)
			result_operand_ptr = db_value<operand>::refv_db().getp_operand(rpn.size(),rpn);

		if (result_operand_ptr)
			return ;
		else
			result_operand_ptr = operand_allocator::new_instance();

		operand & result_operand = * result_operand_ptr;

		/// 当前operand_rpn进行赋值
		result_operand._m_operand_rpn.assign(rpn.begin(),rpn.end());
			
		value_type tmp;

		/**
			对两个操作数没有进行过运算的，重新开始运行
		*/
		for (const_iterator i_one = ref_one_operand.begin(); i_one!=ref_one_operand.end(); i_one++)
			for (const_iterator i_another=ref_another_operand.begin(); i_another!=ref_another_operand.end(); i_another++)
			{
				const value_type & number_one = *i_one;
				const value_type & number_another = *i_another;
				unsigned char one_size = number_one._m_rpn.size();
				unsigned char another_size = number_another._m_rpn.size();
				unsigned char two_size = one_size + another_size;

				//// 做下面的运算，是为了进行防止多余的内存拷贝，加减乘除，在当前的表达中，只有最有一位不一样
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
				/// 防止除数为零
				if (number_another._m_value!=0)
				{
					tmp._m_rpn[two_size]=operator_id_util::__div;
					tmp._m_value = number_one._m_value / number_another._m_value;
					result_operand.push_back(tmp);
				}
			} // end of inner for loop
	} // end of melt function

	/**
	＠brief 给定一个值，找出最近的那个iterator
	@param [in] is_equal_matched : 在这个集合中，匹配项是相等的，还是近似的
		这两个结果的关系是互斥的
	@return [ret] 就是match_handle事件的返回结果
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
	操作数集合：去往表达式路径上的一个中间集合，
	直到这个集合中只剩下一个操作数了，
	这就说明，对于这个集合的操作全部完成

	取这个集合的任意两个操作数，进行运算，结果会出现
	P[n,2]个新的操作数集合
*/

struct operand_set : public std::vector<operand*>
{
	/// 定义个一个事件类型
	typedef void (*gen_new_data_set_d)(void * object, operand_set * new_data_set);
	typedef std::vector<operand*> operand_vec;
	typedef operand_vec::value_type operand_ptr;
	typedef operand_vec::iterator iterator;
	typedef type_allocator<operand_set> operand_set_allocator;

	/// 完成一步，任意取两个操作数，然后会生成P[n,2]个data_set集合
	/// 这个会产生一个时间，就是每生成一新的集合，就会触发一个事件
	/// 外部程序，会接管这个事件进行相应的处理
	bool do_one_operation(void * object, gen_new_data_set_d action)
	{
		if (size()<=1 || action==0) return false;
		/// 除去本元素之间的运算
		for (iterator i=begin(); i!=end(); i++)
		{
			/// 当前元素的前半部分
			for (iterator p=begin(); p!=i; p++)
				action(object, make_a_new_operand_set(i,p));
			/// 当前元素的后半部分
			for (iterator q=i+1; q!=end(); q++)
				action(object, make_a_new_operand_set(i,q));
		}
		return true;
	}

	/// index : 用户输入原有参数的排序，从0开始编号
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
		/// 增加没有参加运算的操作数
		for (iterator i=begin(); i!=end(); i++)
		{
			if (i!=left && i!=right)
				new_operand_set->push_back(*i);
		}
		/// 增加运算操作数的结果
		new_operand_set->push_back(result_operand_ptr);
		return new_operand_set;
	}
};

typedef operand_set::operand_set_allocator operand_set_allocator;

/**
	处理任意排列组合的情况
	这里存放所有可能的结果，
	每一个结果都是一个完整的操作数集合
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
			/// 判断这一次的计算结果
			if (!is_done) // 没有算完
			{
				_m_current_muti_data_set->clear(),
				_m_current_muti_data_set=getp_the_other_multi_data_set(_m_current_muti_data_set);
			}
			else /// 如果都算完了，就退出
			{
				break;
			}
		}
	}
	/**
	@brief 从这个所有的集合中获取所需要的结果，就是和目标数相匹配的
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
				/// 获取每个集合中的第一个操作数
				operand & first_operand = *((*i)->front());
				/// 找这个集合中的所有匹配的结果
				unsigned int this_matched_count = 0;
				ret_value=first_operand.find_value(
					match_event, /// 响应事件
					object, /// 事件输入数据
					dest_number, /// 目标数
					this_matched_count, /// 这个集合中匹配的个数
					has_met_matched_equal /// 匹配结果的模式
					); 
				matched_count += this_matched_count;
				if (!ret_value)
					break;
			}
			else /// 不是最后的结果，不进行寻找结果
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

/// 操作rpn（逆波兰式）的实用程序
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

/// 把一个个数据包装成一个最原始的，用户输入集合
class input
{
public:
	input():_m_next_sequence(0),_m_dest_value(0)
	{}

	/// 输入一个操作数
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

	/// 设置一个目标数
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

/// 组织实际的运算，并保存结果
class work
{
public:
	work():_m_input(0){}
	work(input & input_set)
	{
		set_input_set(input_set);
	}

	/// 输入初始集合
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
	/// 输出所有的结果
	bool get_all_matched_exp_list(std::vector<std::string> & out_string_list)
	{
		unsigned int matched_count = 0;
		bool find_event_ret = refv_work_set().find_result(
			process_find_matched_number_handle,
			this,
			refv_input().get_dest_value(),
			matched_count);

		bool ret_value = true;
		if (_m_matched_number_list.size()>0) /// 有完全匹配的情况
		{
			/// 遍历每一个结果，并把逆波兰表达式转化成正常的表达式显示出来
			for(operand::iterator i=_m_matched_number_list.begin(); i!=_m_matched_number_list.end(); i++)
				out_string_list.push_back(rpn_util::translate(i->_m_rpn,refv_input().refv_original_set()));
		}
		else /// 没有完全匹配的情况，只有一个最近似的值
		{
			/// 遍历每一个结果，并把逆波兰表达式转化成正常的表达式显示出来
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
		/// 判断参数有效性，并获取额外对象
		if (object==0 || !calculate_ptr || !hit_number)
		{
			assert(false);
			return false;
		}
		if (is_equal_number)
		{
			/// 如果值完成相等，就加入到指定的列表中
			calculate_ptr->_m_matched_number_list.push_back(*hit_number);
		}
		/// 如果是近似，则换成最接近的那个，当然是，从来没有相等的情况下
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

		return true; /// 表示继续寻找下一个匹配值
	}
	static number::value_t abs(number::value_t const & value)
	{
		if (value<0.0)
			return -value;
		return value;
	}
};

/**
	参数部分：
		第1个，就是第1个数字
		第2个，就是第2个数字
		....
		最后一个是，目标数
*/
int main(int argc, char * argv[])
{
	input original_input;
	/// 获取输入数据
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

	/// 计算，并得出结果
	work calculate(original_input);
	calculate.start();
	std::vector<std::string> exp_list;
	calculate.get_all_matched_exp_list(exp_list);

	/// 打印出运算结果
	printf ("\n>>>>>>>>>\n");
	calculate.print_exp_list(exp_list);

	// 按键退出
	printf("\n\n press any key to exit");
	getch();

	return 0;
}