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
	/// Ĭ�Ϲ��캯�����̣��κ�һ������еģ����û��������캯���������е�C++��׼��
	/// �ǲ��ܽ������Ա������
	smartptr():helder_ptr(0){}
	/// �ڲ�ָ�빹�����
	explicit smartptr(type_t *in_ptr):helder_ptr(in_ptr) // initialized by built-in pointers
	{
		
	}
	/// �������캯��
	template <typename any_tt>
	smartptr(smartptr<any_tt> const & sp) : helder_ptr(sp.getp())
	{
		
	}
	/// ʵ�忽�����캯��
	smartptr(smartptr const & sp)
	{

	}
	/// ��������
	~smartptr(){delete [] helder_ptr;}

	/// �û���Ϊɾ���������и�ֵ������ָ�벻�ܴ���
	smartptr & operator = (smartptr const &);

	/// �û���Ϊģ��
	type_t * operator -> () { return helder_ptr; }
	type_t * getp_raw_pointer() { return helder_ptr; }
	type_t * getp_raw_pointer() const { return helder_ptr; };
private:
	type_t * helder_ptr;
};


} // end of sys
} // end of ox
