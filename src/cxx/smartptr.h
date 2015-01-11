//
//////////////////////////////////////////////////////////////////////////
// file : smartptr.h
// author : xuhailong
// date: 2007-7-30
// history: 


namespace ox
{
namespace cxx
{

template <typename type_tn>
class smartptr
{
public:
	/// smart pointers are typically
	typedef type_tn type_t;

public:
	/// 默认构造函数过程，任何一个类必有的，如果没有这个构造函数，在现有的C++标准中
	/// 是不能进行类成员声明的
	smartptr():helder_ptr(0){}
	/// 内部指针构造过程
	explicit smartptr(type_t *in_ptr):helder_ptr(in_ptr) // initialized by built-in pointers
	{
		
	}
	/// 泛化构造函数
	template <typename any_tt>
	smartptr(smartptr<any_tt> const & sp) : helder_ptr(sp.getp())
	{
		
	}
	/// 实体拷贝构造函数
	smartptr(smartptr const & sp)
	{

	}
	/// 析构函数
	~smartptr(){delete [] helder_ptr;}

	/// 用户行为删除，不能有赋值函数，指针不能传递
	smartptr & operator = (smartptr const &);

	/// 用户行为模仿
	type_t * operator -> () { return helder_ptr; }
	type_t * getp_raw_pointer() { return helder_ptr; }
	type_t * getp_raw_pointer() const { return helder_ptr; };
private:
	type_t * helder_ptr;
};


} // end of sys
} // end of ox
