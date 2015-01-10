// tslist_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/met/static_assert.h"
#include "../../src/met/tslist.h"
#include "../../src/met/bit.h"


using namespace ox;
using namespace ox::met;


template <typename slist_tn>
struct size_of_line
{
	template <typename cur_type_tn, typename suffix_tn, typename last_handled_tn>
	struct handle
	{
		/// necessary, whether the cur_type_tn will be removed
		static bool const __is_removed = false;

		/// your actions
		static size_t const size = last_handled_tn::size+1;
		typedef cur_type_tn type;

		/// necessary, whether the handle is continue
		static bool const __is_continue = true;
	};
	template <typename cur_type_tn, typename last_handled_tn>
	struct handle <cur_type_tn,itot<-1>,last_handled_tn>
	{
		/// necessary, whether the cur_type_tn will be removed
		static bool const __is_removed = false;

		/// your actions
		static size_t const size = 0;
		typedef cur_type_tn type;

		/// necessary, whether the handle is continue
		static bool const __is_continue = true;
	};

	typedef typename apply<slist_tn,lambda<handle<ox::_1,ox::_2,ox::_3>>,true>::type th;
	
	static size_t const value = th::size;
};

namespace modify_test
{
	typedef mkslist<int>::type slist_type;
	typedef tslist<int,void> slist1;

	___static_assert(size<slist1>::value==1);
	size_t j = size<slist1>::value;

	typedef tslist<int*,void> slist2;
	typedef mkslist<int*>::type slist3;

	template <typename t,typename i> struct add_ptr2
	{
		typedef typename t::_t_type* type;
		typedef typename t::_t_syms syms;
		typedef typename t::_t_meta meta;
	};
	typedef modify<slist1,add_ptr2<ox::_1,ox::_2>>::type slist1_added;
	___static_assert((is_same<slist1_added,slist2>::value));
	___static_assert((is_same<slist1_added,slist3>::value));
}


namespace reverse_test
{
	typedef tslist<long,void,void,void> slist1;
	typedef tslist<long,void,void,void> slist1_std;
	typedef reverse<slist1,void>::type slist1_reversed;
	___static_assert_same_type(slist1_std,slist1_reversed);

	typedef tslist<long,void,void,tslist<int,void,void,void>> slist2;
	typedef tslist<int,void,void,tslist<long,void,void,void>> slist2_std;
	typedef reverse<slist2,void>::type slist2_reversed;
	___static_assert_same_type(slist2_std,slist2_reversed);

	typedef tslist<long,long*,long**,tslist<int,int*,int**,void>> slist3;
	typedef tslist<int,int*,int**,tslist<long,long*,long**,void>> slist3_std;
	typedef reverse<slist3,void>::type slist3_reversed;
	___static_assert_same_type(slist3_std,slist3_reversed);

	typedef tslist<long,long*,long**,tslist<int,int*,int**,tslist<char,char*,char**,void>>> slist4;
	typedef tslist<char,char*,char**,tslist<int,int*,int**,tslist<long,long*,long**,void>>> slist4_std;
	typedef reverse<slist4,void>::type slist4_reversed;
	___static_assert_same_type(slist4_std,slist4_reversed);
}

namespace push_front_test
{
	typedef tslist<long,void,void,void> slist1;
	typedef tslist<int,void,void,tslist<long,void,void,void>> slist1_std;
	typedef push_front<slist1,int>::type oslist1;
	___static_assert_same_type(oslist1,slist1_std);

	typedef void slist2;
	typedef tslist<int,void,void,void> slist2_std;
	typedef push_front<slist2,int>::type oslist2;
	___static_assert_same_type(oslist2,slist2_std);

	typedef long slist3;
	typedef tslist<int,void,void,long> slist3_std;
	typedef push_front<slist3,int>::type oslist3;
	___static_assert_same_type(oslist3,slist3_std);
}

namespace remove_test
{
	typedef tslist<long,void,void,tslist<int,void,void,void>> slist1;

	typedef tslist<int,void,void,void> slist1_std;
	template <typename slist_tn, typename suffix_tn,typename prev_tested_tn>
	struct handle
	{
		//typedef typename slist_tn::_t_type _t_type;
		static bool const value = suffix_tn::value==0;
	};
	typedef ox::met::remove<slist1,handle<ox::_1,ox::_2,ox::_3>>::type new_slist;

	___static_assert_same_type(new_slist,slist1_std);

	///
	/// remove the second type

	typedef tslist<long,void,void,tslist<int,void,void,tslist<char,void,void,void>>> slist2;

	typedef tslist<long,void,void,tslist<char,void,void,void>> slist2_std;
	template <typename slist_tn, typename suffix_tn,typename prev_tested_tn>
	struct handle2
	{
		//typedef typename slist_tn::_t_type _t_type;
		static bool const value = suffix_tn::value==1;
	};
	typedef ox::met::remove<slist2,handle2<ox::_1,ox::_2,ox::_3>>::type new_slist2;

	___static_assert_same_type(new_slist2,slist2_std);
}

namespace mk_slist_test
{
	typedef mkslist<int>::type slist1;
	typedef tslist<int,void,void,void> slist1_std;
	___static_assert_same_type(slist1_std,slist1);

	typedef mkslist<int,long>::type slist2;
	typedef tslist<int,void,void,tslist<long,void,void,void>> slist2_std;
	___static_assert_same_type(slist2_std,slist2);

	typedef mkslist<int,long,long*>::type slist3;
	typedef tslist<int,void,void,tslist<long,void,void,tslist<long*,void,void,void>>> slist3_std;
	___static_assert_same_type(slist3_std,slist3);
}

namespace modify2_test
{
	typedef tslist<long,void,void,void> slist1;
	template <typename slist_tn,typename suffix_tn>
	struct handle
	{
		typedef typename slist_tn::_t_type type;
		typedef typename slist_tn::_t_syms syms;
		typedef typename slist_tn::_t_meta meta;
	};
	typedef modify<slist1,handle<ox::_1,ox::_2>>::type new_slist;
	___static_assert_same_type(new_slist,slist1);
}

namespace only_ref_test
{
	typedef tslist<long,void,void,void> slist1;
	typedef only_ref_of_slist<slist1>::type new_slist;
	typedef void ref_result;
	___static_assert_same_type(new_slist,ref_result);

	/// when a type is a ref
	typedef tslist<long,void,void,tslist<char&,void,void,void>> slist2;
	typedef only_ref_of_slist<slist2>::type new_slist2;
	typedef tslist<char&,void,void,void> ref_result2;
	___static_assert_same_type(new_slist2,ref_result2);
}

namespace at_test
{
	typedef mkslist<long,int,short,char>::type slist1;
	typedef at_of_slist<slist1,0>::type t0;
	typedef at_of_slist<slist1,1>::type t1;
	typedef at_of_slist<slist1,4>::type t4;
	___static_assert_same_type(t0,long);
	___static_assert_same_type(t1,int);
	___static_assert_same_type(t4,void);
	typedef at_of_slist<slist1,0>::suffix s0;
	typedef at_of_slist<slist1,1>::suffix s1;
	typedef at_of_slist<slist1,4>::suffix s4;
	typedef at_of_slist<slist1,5>::suffix s5;
	typedef at_of_slist<int,5>::suffix s6;
	___static_assert(s0::value==0);
	___static_assert(s1::value==1);
	___static_assert(s4::value==4);
	___static_assert(s5::value==4);
	___static_assert(s6::value==-1);
	typedef at_of_range<slist1,0>::type at0_type;
	typedef at_of_range<slist1,1>::type at1_type;
	typedef at_of_range<slist1,2>::type at2_type;
	typedef at_of_range<slist1,3> result3;

	___static_assert_same_type(at0_type,long);
	___static_assert_same_type(at1_type,int);
	___static_assert_same_type(at2_type,short);
	___static_assert_same_type(result3::type,char);
	___static_assert(result3::suffix::value==3);
}


namespace push_front_backward_test
{
	template <typename slist_tn,typename index>
	struct test_not_null
	{
		static bool const value = !is_null<typename slist_tn::_t_type>::value;
	};
	typedef test_not_null<ox::_1,ox::_2> test;
	typedef tslist<long,void,void,void> slist1;
	typedef tslist<int,void,void,tslist<ox::null_type,void,void,void>> slist2;
	typedef push_front_backward<slist1,slist2,test>::type slist3;
	typedef tslist<int,void,void,tslist<long,void,void,void>> slist4;
	___static_assert_same_type(slist3,slist4);
}

namespace push_back_batch_test
{
	typedef tslist<long,void,void,void> slist1;
	typedef tslist<int,void,void,tslist<char,void,void,void>> slist2;
	typedef push_back<slist1,slist2>::type slist3;
	typedef tslist<long,void,void,tslist<int,void,void,tslist<char,void,void,void>>> slist4;
	___static_assert_same_type(slist3,slist4);
}


namespace linkall_test
{
	typedef tslist<long,void,void,tslist<int,void,void,void>> slist1;
	typedef tslist<char,void,void,tslist<short,void,void,void>> slist2;
	typedef mkslist<slist1,slist2>::type slist3;
	typedef linkall<slist3>::type slist4;
	typedef mkslist<long,int,char,short>::type slist5;
	___static_assert_same_type(slist4,slist5);
}

namespace linkall_test_test
{
	template <typename slist_tn,typename index>
	struct test
	{
		static bool const value = !is_same<typename slist_tn::_t_type,int>::value;
	};
	typedef tslist<long,void,void,tslist<int,void,void,void>> slist1;
	typedef tslist<char,void,void,tslist<short,void,void,void>> slist2;
	typedef mkslist<slist1,slist2>::type slist3;


	typedef linkall<slist3,test<void,void>>::type slist4;
	typedef mkslist<long,char,short>::type slist5;
	___static_assert_same_type(slist4,slist5);
}

namespace linkall_single_test
{
	typedef mkslist<int,long,char>::type slist1;
	typedef linkall<slist1>::type slist2;
	___static_assert_same_type(slist1,slist2);
}

namespace end_test
{
	typedef mkslist<int>::type slist;
	typedef tslist<int,void,void,void> slist1;
	___static_assert_same_type(slist1,slist);
	typedef end<slist>::type hit;
	___static_assert_same_type(hit,int);
}

namespace find_test
{
	typedef mkslist<int,char,long,char>::type slist;
	typedef find<slist,char> result;
	typedef result::type owner;
	___static_assert_same_type(owner::_t_next::_t_type,long);
	___static_assert(result::index::value==1);
}

namespace find_if_test
{
	template <typename slist_tn,typename index>
	struct test
	{
		static bool const value = is_same<typename slist_tn::_t_type,char>::value;
	};
	typedef mkslist<int,char,long,char>::type slist;
	typedef find_if<slist,test<ox::_1,ox::_2>> result;
	typedef result::type owner;
	___static_assert_same_type(owner::_t_next::_t_type,long);
	___static_assert(result::index::value==1);
}

namespace find_if2_test
{
	template <typename slist_tn,typename index>
	struct test
	{
		static bool const value = is_same<typename slist_tn::_t_type,double>::value;
	};
	typedef mkslist<int,char,long,char>::type slist;
	typedef find_if<slist,test<ox::_1,ox::_2>> result;
	typedef result::type owner;
	___static_assert_same_type(owner,void);
	___static_assert(!result::boolean::value && result::index::value==4);
}

namespace rpush_front_test
{
	typedef rpush_front<void,char,void,void,4>::type slist_type;
	typedef at<slist_type,0>::type t0;
	typedef at<slist_type,1>::type t1;
	typedef at<slist_type,2>::type t2;
	typedef at<slist_type,3>::type t3;
	typedef at<slist_type,4>::type t4;
	___static_assert_same_type(t0,char);
	___static_assert_same_type(t1,char);
	___static_assert_same_type(t2,char);
	___static_assert_same_type(t3,char);
	___static_assert_same_type(t4,void);
	___static_assert(size<slist_type>::value==4);
}

namespace modify_if_test
{
	typedef mkslist<int,short,int,long>::type slist_type;
	template <typename slist_tn, typename index_tn>
	struct mod
	{
		//___static_assert(false);
		static bool const conti = index_tn::value<1;
		/// whether next mod is valid
		typedef boolean<conti> going;
		typedef boolean<false> erased;
		/// if this is valid, the modified type
		typedef typename slist_tn::_t_type* type;
		typedef typename slist_tn::_t_syms syms;
		typedef typename slist_tn::_t_meta meta;
		typedef index_tn index;
	};
	typedef modify_if<slist_type,mod<ox::_1,ox::_2>> out;
	typedef out::modiout modiout;
	typedef mkslist<int*>::type stdout_type;
	___static_assert(out::catched::value==1);
	___static_assert(out::erased::value==0);
	___static_assert(out::taken::value==1);
	___static_assert_same_type(out::type,stdout_type);
}

namespace plurality_test
{
	___static_assert(plurality<int>::value==0);
	typedef mkslist<int>::type slist1;
	___static_assert(plurality<slist1>::value==1);
	typedef mkslist<int,short>::type slist2;
	___static_assert(plurality<slist2>::value==2);
	typedef mkslist<int,short,long>::type slist3;
	___static_assert(plurality<slist3>::value==2);
}

namespace mksl_from_func
{
	namespace A1
	{
	typedef mkslist_from_func<void(int)>::type slist1;
	typedef mkslist<int>::type slist2;
	___static_assert_same_type(slist1,slist2);
	}

	namespace B1
	{
	typedef mkslist_from_func<void(int,int*)>::type slist1;
	typedef mkslist<int,int*>::type slist2;
	___static_assert_same_type(slist1,slist2);
	}
}

namespace mksl_from_2x
{
	typedef binary<1,0,0,1,1> x;
	___static_assert(x::size::value==5);


	typedef boolean<true> t1;
	typedef boolean<false> t0;


	namespace _1t
	{
	typedef mkslist_from_2x<x::value,2,1,t1,t0> result;
	typedef result::type slist1;
	typedef mkslist<t0,t1>::type slist2;
	___static_assert_same_type(slist1,slist2);
	//___static_assert(result::__d=='-');
	//___static_assert(result::__c==2);
	}

	namespace _2t
	{
	typedef mkslist_from_2x<x::value,4,1,t1,t0> result2;
	typedef result2::type slist3;
	typedef mkslist<t1,t0,t0,t1>::type slist4;
	___static_assert_same_type(slist3,slist4);
	//___static_assert(result2::__d=='-');
	//___static_assert(result2::__c==4);
	//___static_assert(result2::__n==9);
	}


	namespace _3t
	{
	typedef mkslist_from_2x<x::value,1,3,t1,t0> result;
	typedef result::type slist1;
	typedef mkslist<t1,t0,t0>::type slist2;
	___static_assert_same_type(slist1,slist2);
	//___static_assert(result::__d=='+');
	//___static_assert(result::__c==3);
	//___static_assert(result::__n==9);
	}

	namespace _4t
	{
	typedef mkslist_from_2x<x::value,3,3,t1,t0> result;
	typedef result::type slist1;
	typedef mkslist<t0>::type slist2;
	___static_assert_same_type(slist1,slist2);
	//___static_assert(result::__d=='0');
	//___static_assert(result::__c==1);
	//___static_assert(result::__n==2);
	}

	namespace _5t
	{
	typedef mkslist_from_2x<x::value,7,7,t1,t0> result;
	typedef result::type slist1;
	typedef void slist2;
	//___static_assert_same_type(slist1,slist2);
	//___static_assert(result::__d=='0');
	//___static_assert(result::__c==0);
	//___static_assert(result::__n==0);
	}
}

namespace indexof_test
{
	typedef mkslist<int,long,char>::type slist;

	static size_t const index1 = indexof<slist,int>::value;
	___static_assert(index1 == 0);

	static size_t const index2 = indexof<slist,long>::value;
	___static_assert(index2 == 1);

	static size_t const index3 = indexof<slist,char>::value;
	___static_assert(index3 == 2);

	static size_t const index4 = indexof<slist,short>::value;
	___static_assert(index4 == -3);
	___static_assert((int)(index4) < 0);

	static size_t const index5 = indexof<int,int>::value;
	___static_assert(index5==-1);
}

namespace anyindexof_test
{
	typedef mkslist<int,long,char>::type slist;

	typedef mkslist<short,char*,int>::type type1;
	static size_t const index1 = anyindexof<slist,type1>::value;
	___static_assert(index1 == 0);

	static size_t const xindex1 = anyxindexof<int,type1>::value;
	___static_assert(xindex1 == 0);
	

	typedef mkslist<short,char*,long>::type type2;
	static size_t const index2 = anyindexof<slist,type2>::value;
	___static_assert(index2 == 1);

	typedef mkslist<short,char*,char>::type type3;
	static size_t const index3 = anyindexof<slist,type3>::value;
	___static_assert(index3 == 2);

	typedef mkslist<short,char*,short>::type type4;
	static size_t const index4 = anyindexof<slist,type4>::value;
	___static_assert(index4 == -3);

	typedef mkslist<short,char*,short>::type type5;
	static size_t const index5 = anyindexof<int,type5>::value;
	___static_assert(index5 == -1);
}

namespace mkslike_test
{
	template <typename type_tn, typename ids_tn=void, typename meta_tn=void, typename next_tn=void>
	struct my
	{
		typedef type_tn _t_type;
		typedef ids_tn _t_syms;
		typedef meta_tn _t_meta;

		typedef next_tn _t_next;
	};
	using namespace ox::met;
	typedef mkslike<my<ox::_1,ox::_2,ox::_3,ox::_4>,int,long>::type mylist;
	/// some thing error happens here
	/// *******************************
	/// why is the mylist void?
	/// warning is too much, i cannot beleve it
}


#if 0
#include "../../src/mix/mixture_feature_def.h"
namespace tul_test
{
	___def_3_tulist_ex(
		list,
		4(char,((char)),void,0),
		4(int,((int)),void,1),
		4(char,((long)),void,1)
		);
	typedef list<0>::type tulist;
	template <typename t>
	struct sel
	{
		static bool const value = t::idiff::value == 1;
	};
	typedef ox::met::lambda1<sel>::type fsel;
	void test()
	{
		static size_t const i = ox::met::indexof_tulist<tulist,char,fsel>::value;
		___static_assert(i==1);
		static size_t const j = ox::met::indexof_tulist<tulist,int>::value;
		___static_assert(j==1);
	}
}

#include "../../src/mix/mixture_feature_def.h"
namespace feature_test
{
	___def_3_features(
		list,
		4(char,((char)),void,1),
		4(int,((int)),void,1),
		4(char,((long)),void,1)
		);
	typedef list<0>::type feature;
	template <typename t>
	struct sel
	{
		static bool const value = t::idiff::value == 1;
	};
	template <typename t,typename t2>
	struct hit
	{
		static bool const value = ox::met::is_same<typename t::_t_type,t2>::value;
	};
	typedef ox::met::lambda<hit<ox::_1,int>> fhit;
	typedef ox::met::lambda1<sel>::type fsel;
	void test()
	{
		static int const i = ox::met::indexof_feature_if<feature,fhit,fsel>::value;
		//___static_assert(i==1);
		int j = i;
	}
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	//feature_test::test();
	int i = anyindexof_test::xindex1;

	//mkslike_test::mylist ddd;

	size_t v = mksl_from_2x::x::value;
	mksl_from_2x::_5t::slist1 dd;

	//size_t vv1 = mksl_from_2x::_5t::result::__d;
	//size_t vv2 = mksl_from_2x::_5t::result::__c;
	//size_t vv3 = mksl_from_2x::_5t::result::__n;


	//begin<modify_if_test::out::type>::type ab;
	//f(reverse_test::dd);
	return 0;
}

