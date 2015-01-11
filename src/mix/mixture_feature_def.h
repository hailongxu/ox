
#include "../ox/tclasses.h"
#include "../met/mod.h"
#include "../met/type_for_param_passed.h"



#pragma once




#if 0

	/// example of the usage of ___def_x_tulist_ex

	___def_3_tulist_ex(
		list4, /// the list name
		2(char,((int*))), /// 2(the data-type, method-tag-list)
		2(char const,2(d(char*),(long const*))),
		2(void,2((char*const),(int const*)))
	);

#endif


#define ___def_tul_zero_class(name) \
	template <size_t> \
	struct name { \
		typedef ox::itot<0> idiff; \
		static size_t const i = 0; \
		static size_t const n = 0; \
		typedef void _t_type; \
		typedef void _t_syms; \
		typedef void _t_meta; \
		typedef void unit; \
		typedef ox::identity<ox::empty_type> wp0; \
		typedef name<0> type; \
		static char const* to_string() { return "void"; } \
	};


#define ___def_tul_mainclass_decl(name) \
	template <size_t iii> \
	struct name


#define ___def_tul_mainclass(name,nn) \
	template <size_t iii> \
	struct name \
	{ \
		___def_tul_zero_class(m##name); \
		typedef ox::itot<0> idiff; \
		static size_t const i = 0; \
		static size_t const n = nn; \
		typedef void _t_type; \
		typedef m##name<0> _t_syms; \
		typedef void _t_meta; \
		typedef void unit; \
		typedef ox::identity<ox::empty_type> wp0; \
		typedef name<n> type; \
		static char const* to_string() { return "void"; } \
	}

#define ___def_tul_partclass(name,ii,t,s,u,idi) \
	template <> \
	struct name <ii> \
	{ \
		typedef ox::itot<idi==-1?(ox::met::is_same<t,void>::value?0:1):idi> idiff; \
		static size_t const i = ii; \
		typedef t _t_type; \
		typedef s _t_syms; \
		typedef void _t_meta; \
		typedef u unit; \
		typedef ox::met::param_type_for_init<_t_type> wp0; \
		static char const* to_string() { return ___def_to_string(t); } \
	}




#define ___def_tul_partclass_ex(name,ii,t,s,u,idi) \
	template <> \
	struct name <ii> \
	{ \
		___def_inner_method_class(name,s); \
		typedef ox::itot<idi==-1?(ox::met::is_same<t,void>::value?0:1):idi> idiff; \
		static size_t const i = ii; \
		typedef t _t_type; \
		typedef ___def_inner_method_class_first(name) _t_syms; \
		typedef void _t_meta; \
		typedef u unit; \
		typedef ox::met::param_type_for_init<_t_type> wp0; \
		static char const* to_string() { return ___def_to_string(t); } \
	}





/*
	t1,t2,t3,t4:
	(a), like this, should be encluded by breackets
*/

#define ___def_remove_bracket(one) ___def_remove_bracket_do(___def_joinf(___def_param_,one))
#define ___def_remove_bracket_do(one) ___def_remove_bracket_do_again(one)
#define ___def_remove_bracket_do_again(one) one

#define ___def_param_(p1) p1 
#define ___def_param_2(p1,p2) p1,p2
#define ___def_param_3(p1,p2,p3) p1,p2,p3
#define ___def_param_4(p1,p2,p3,p4) p1,p2,p3,p4
#define ___def_param_5(p1,p2,p3,p4,p5) p1,p2,p3,p4,p5
#define ___def_param_6(p1,p2,p3,p4,p5,p6) p1,p2,p3,p4,p5,p6
#define ___def_param_7(p1,p2,p3,p4,p5,p6,p7) p1,p2,p3,p4,p5,p6,p7
#define ___def_param_8(p1,p2,p3,p4,p5,p6,p7,p8) p1,p2,p3,p4,p5,p6,p7,p8


#define ___def_param_count_void 0
#define ___def_param_count_(t1) 1
#define ___def_param_count_2(t1,t2) 2
#define ___def_param_count_3(t1,t2,t3) 3
#define ___def_param_count_4(t1,t2,t3,t4) 4
#define ___def_param_count_5(t1,t2,t3,t4,t5) 5
#define ___def_param_count_6(t1,t2,t3,t4,t5,t6) 6
#define ___def_param_count_7(t1,t2,t3,t4,t5,t6,t7) 7
#define ___def_param_count_8(t1,t2,t3,t4,t5,t6,t7,t8) 8

#define ___def_get_param_count(one) ___def_get_param_count_do(___def_joinf(___def_param_count_,one))
#define ___def_get_param_count_do(one) ___def_get_param_count_do_again(one)
#define ___def_get_param_count_do_again(one) one

#define ___def_inner_method_class(name,p) ___def_inner_method_class_do(___def_3joinf(___def_inner_tul_class_name_,___def_get_param_count(p),(___def_joinf(m,name),p)))
#define ___def_inner_method_class_do(pp) ___def_inner_method_class_do_again(pp)
#define ___def_inner_method_class_do_again(pp) pp

#define ___def_inner_tul_class_name_0(name,p) ___def_inner_tul_class_name_do(___def_0method_tulist(name))
#define ___def_inner_tul_class_name_1(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_1method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_2(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_2method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_3(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_3method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_4(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_4method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_5(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_5method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_6(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_6method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_7(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_7method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_8(name,p) ___def_inner_tul_class_name_do(___def_6_join(___def_8method_tulist,___def_lbrace,name,___def_comma,___def_remove_bracket(p),___def_rbrace))
#define ___def_inner_tul_class_name_do(pp) ___def_inner_tul_class_name_do_again(pp)
#define ___def_inner_tul_class_name_do_again(pp) pp


#define ___def_inner_method_class_first(name) ___def_3joinf(m,name<m,name<0>::n>)





#define ___def_tul_part_list_full_1(name,ii,t1,s1,ulmd1,idi1) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1)

#define ___def_tul_part_list_full_2(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_1(name,ii-1,t2,s2,ulmd2,idi2)

#define ___def_tul_part_list_full_3(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_2(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3)

#define ___def_tul_part_list_full_4(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_3(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4)

#define ___def_tul_part_list_full_5(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_4(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5)

#define ___def_tul_part_list_full_6(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_5(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6)

#define ___def_tul_part_list_full_7(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_6(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7)

#define ___def_tul_part_list_full_8(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7,t8,s8,ulmd8,idi8) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_7(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7,t8,s8,ulmd8,idi8)


//// ex

#define ___def_tul_part_list_full_ex_1(name,ii,t1,s1,ulmd1,idi1) \
	___def_tul_partclass_ex(name,ii,t1,s1,ulmd1,idi1)

#define ___def_tul_part_list_full_ex_2(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2) \
	___def_tul_partclass_ex(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_1(name,ii-1,t2,s2,ulmd2,idi2)

#define ___def_tul_part_list_full_ex_3(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3) \
	___def_tul_partclass_ex(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_2(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3)

#define ___def_tul_part_list_full_ex_4(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4) \
	___def_tul_partclass_ex(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_3(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4)

#define ___def_tul_part_list_full_ex_5(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_4(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5)

#define ___def_tul_part_list_full_ex_6(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_5(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6)

#define ___def_tul_part_list_full_ex_7(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_6(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7)

#define ___def_tul_part_list_full_ex_8(name,ii,t1,s1,ulmd1,idi1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7,t8,s8,ulmd8,idi8) \
	___def_tul_partclass(name,ii,t1,s1,ulmd1,idi1); \
	___def_tul_part_list_full_ex_7(name,ii-1,t2,s2,ulmd2,idi2,t3,s3,ulmd3,idi3,t4,s4,ulmd4,idi4,t5,s5,ulmd5,idi5,t6,s6,ulmd6,idi6,t7,s7,ulmd7,idi7,t8,s8,ulmd8,idi8)



/// (a) ==> a,void
/// 2(a,b) ==> a,b
/// 3(a,b,c) ==> a,b,c
/// {{
#define ___def_tslist_datap4(t,s,u,idi) t,s,u,idi
#define ___def_tslist_datap3(t,s,u) t,s,u,-1
#define ___def_tslist_datap2(t,s) t,s,void,-1
#define ___def_tslist_datap(t) t,void,void,-1
#define ___def_tslist_datapd(t) t,void,void,1

#define ___def_tslist_methodp(s) void,s,void,0
#define ___def_tslist_methodp2(s,u) void,s,u,0
#define ___def_tslist_methodpd(s) void,s,void,1
#define ___def_tslist_methodpx(s,t,idi) t,s,void,idi


#define ___def_to_data_many(a) ___def_to_many2(___def_joinf(___def_tslist_datap,a))
#define ___def_to_method_many(a) ___def_to_many2(___def_joinf(___def_tslist_methodp,a))
#define ___def_to_many2(a) ___def_to_many3(a)
#define ___def_to_many3(a) a
/// }}}


#define ___def_func_call(a) ___def_func2_call(a)
#define ___def_func2_call(a) a

#define ___def_funcex_call(a) ___def_funcex2_call(a)
#define ___def_funcex2_call(a) a


/// last one param unfolded
#define ___def_to_data_param1(name,i,p1) (name,i,___def_to_data_many(p1))
#define ___def_to_method_param1(name,i,p1) (name,i,___def_to_method_many(p1))
#define ___def_to_func1(a) ___def_tul_part_list_full_1##a
#define ___def_func1_make(a) ___def_func_call(___def_to_func1(a))
#define ___def_to_func_ex1(a) ___def_tul_part_list_full_ex_1##a
#define ___def_funcex1_make(a) ___def_funcex_call(___def_to_func_ex1(a))
/// redefine the part list
#define ___def_tul_data_part_list_1(name,i,t1) ___def_func1_make(___def_to_data_param1(name,i,t1))
#define ___def_tul_method_part_list_1(name,i,t1) ___def_func1_make(___def_to_method_param1(name,i,t1))
#define ___def_tul_data_part_list_ex_1(name,i,t1) ___def_funcex1_make(___def_to_data_param1(name,i,t1))


/// last two param unfolded
#define ___def_to_data_param2(name,i,p1,p2) (name,i,___def_to_data_many(p1),___def_to_data_many(p2))
#define ___def_to_method_param2(name,i,p1,p2) (name,i,___def_to_method_many(p1),___def_to_method_many(p2))
#define ___def_to_func2(a) ___def_tul_part_list_full_2##a
#define ___def_func2_make(a) ___def_func_call(___def_to_func2(a))
#define ___def_to_func_ex2(a) ___def_tul_part_list_full_ex_2##a
#define ___def_funcex2_make(a) ___def_funcex_call(___def_to_func_ex2(a))
/// redefine the part list
#define ___def_tul_data_part_list_2(name,i,t1,t2) ___def_func2_make(___def_to_data_param2(name,i,t1,t2))
#define ___def_tul_method_part_list_2(name,i,t1,t2) ___def_func2_make(___def_to_method_param2(name,i,t1,t2))
#define ___def_tul_data_part_list_ex_2(name,i,t1,t2) ___def_funcex2_make(___def_to_data_param2(name,i,t1,t2))

/// last three param unfolded
#define ___def_to_data_param3(name,i,p1,p2,p3) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3))
#define ___def_to_method_param3(name,i,p1,p2,p3) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3))
#define ___def_to_func3(a) ___def_tul_part_list_full_3##a
#define ___def_func3_make(a) ___def_func_call(___def_to_func3(a))
#define ___def_to_func_ex3(a) ___def_tul_part_list_full_ex_3##a
#define ___def_funcex3_make(a) ___def_funcex_call(___def_to_func_ex3(a))
/// redefine the part list
#define ___def_tul_data_part_list_3(name,i,t1,t2,t3) ___def_func3_make(___def_to_data_param3(name,i,t1,t2,t3))
#define ___def_tul_method_part_list_3(name,i,t1,t2,t3) ___def_func3_make(___def_to_method_param3(name,i,t1,t2,t3))
#define ___def_tul_data_part_list_ex_3(name,i,t1,t2,t3) ___def_funcex3_make(___def_to_data_param3(name,i,t1,t2,t3))

/// last four param unfolded
#define ___def_to_data_param4(name,i,p1,p2,p3,p4) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3),___def_to_data_many(p4))
#define ___def_to_method_param4(name,i,p1,p2,p3,p4) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3),___def_to_method_many(p4))
#define ___def_to_func4(a) ___def_tul_part_list_full_4##a
#define ___def_func4_make(a) ___def_func_call(___def_to_func4(a))
#define ___def_to_func_ex4(a) ___def_tul_part_list_full_ex_4##a
#define ___def_funcex4_make(a) ___def_funcex_call(___def_to_func_ex4(a))
/// redefine the part list
#define ___def_tul_data_part_list_4(name,i,t1,t2,t3,t4) ___def_func4_make(___def_to_data_param4(name,i,t1,t2,t3,t4))
#define ___def_tul_method_part_list_4(name,i,t1,t2,t3,t4) ___def_func4_make(___def_to_method_param4(name,i,t1,t2,t3,t4))
#define ___def_tul_data_part_list_ex_4(name,i,t1,t2,t3,t4) ___def_funcex4_make(___def_to_data_param4(name,i,t1,t2,t3,t4))

/// last five param unfolded
#define ___def_to_data_param5(name,i,p1,p2,p3,p4,p5) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3),___def_to_data_many(p4),___def_to_data_many(p5))
#define ___def_to_method_param5(name,i,p1,p2,p3,p4,p5) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3),___def_to_method_many(p4),___def_to_method_many(p5))
#define ___def_to_func5(a) ___def_tul_part_list_full_5##a
#define ___def_func5_make(a) ___def_func_call(___def_to_func5(a))
#define ___def_to_func_ex5(a) ___def_tul_part_list_full_ex_5##a
#define ___def_funcex5_make(a) ___def_funcex_call(___def_to_func_ex5(a))
/// redefine the part list
#define ___def_tul_data_part_list_5(name,i,t1,t2,t3,t4,t5) ___def_func5_make(___def_to_data_param5(name,i,t1,t2,t3,t4,t5))
#define ___def_tul_method_part_list_5(name,i,t1,t2,t3,t4,t5) ___def_func5_make(___def_to_method_param5(name,i,t1,t2,t3,t4,t5))
#define ___def_tul_data_part_list_ex_5(name,i,t1,t2,t3,t4,t5) ___def_funcex5_make(___def_to_data_param5(name,i,t1,t2,t3,t4,t5))

/// last six param unfolded
#define ___def_to_data_param6(name,i,p1,p2,p3,p4,p5,p6) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3),___def_to_data_many(p4),___def_to_data_many(p5),___def_to_data_many(p6))
#define ___def_to_method_param6(name,i,p1,p2,p3,p4,p5,p6) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3),___def_to_method_many(p4),___def_to_method_many(p5),___def_to_data_many(p6))
#define ___def_to_func6(a) ___def_tul_part_list_full_6##a
#define ___def_func6_make(a) ___def_func_call(___def_to_func6(a))
#define ___def_to_func_ex6(a) ___def_tul_part_list_full_ex_6##a
#define ___def_funcex6_make(a) ___def_funcex_call(___def_to_func_ex6(a))
/// redefine the part list
#define ___def_tul_data_part_list_6(name,i,t1,t2,t3,t4,t5,t6) ___def_func6_make(___def_to_data_param6(name,i,t1,t2,t3,t4,t5,t6))
#define ___def_tul_method_part_list_6(name,i,t1,t2,t3,t4,t5,t6) ___def_func6_make(___def_to_method_param6(name,i,t1,t2,t3,t4,t5,t6))
#define ___def_tul_data_part_list_ex_6(name,i,t1,t2,t3,t4,t5,t6) ___def_funcex6_make(___def_to_data_param6(name,i,t1,t2,t3,t4,t5,t6))

/// last seven param unfolded
#define ___def_to_data_param7(name,i,p1,p2,p3,p4,p5,p6,p7) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3),___def_to_data_many(p4),___def_to_data_many(p5),___def_to_data_many(p6),___def_to_data_many(p6))
#define ___def_to_method_param7(name,i,p1,p2,p3,p4,p5,p6,p7) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3),___def_to_method_many(p4),___def_to_method_many(p5),___def_to_data_many(p6),___def_to_data_many(p6))
#define ___def_to_func7(a) ___def_tul_part_list_full_7##a
#define ___def_func7_make(a) ___def_func_call(___def_to_func7(a))
#define ___def_to_func_ex7(a) ___def_tul_part_list_full_ex_7##a
#define ___def_funcex7_make(a) ___def_funcex_call(___def_to_func_ex7(a))
/// redefine the part list
#define ___def_tul_data_part_list_7(name,i,t1,t2,t3,t4,t5,t6,t7) ___def_func7_make(___def_to_data_param7(name,i,t1,t2,t3,t4,t5,t6,t7))
#define ___def_tul_method_part_list_7(name,i,t1,t2,t3,t4,t5,t6,t7) ___def_func7_make(___def_to_method_param7(name,i,t1,t2,t3,t4,t5,t6,t7))
#define ___def_tul_data_part_list_ex_7(name,i,t1,t2,t3,t4,t5,t6,t7) ___def_funcex7_make(___def_to_data_param7(name,i,t1,t2,t3,t4,t5,t6,t7))

/// last eight param unfolded
#define ___def_to_data_param8(name,i,p1,p2,p3,p4,p5,p6,p7,p8) (name,i,___def_to_data_many(p1),___def_to_data_many(p2),___def_to_data_many(p3),___def_to_data_many(p4),___def_to_data_many(p5),___def_to_data_many(p6),___def_to_data_many(p6),___def_to_data_many(p6))
#define ___def_to_method_param8(name,i,p1,p2,p3,p4,p5,p6,p7,p8) (name,i,___def_to_method_many(p1),___def_to_method_many(p2),___def_to_method_many(p3),___def_to_method_many(p4),___def_to_method_many(p5),___def_to_data_many(p6),___def_to_data_many(p6),___def_to_data_many(p6))
#define ___def_to_func8(a) ___def_tul_part_list_full_8##a
#define ___def_func8_make(a) ___def_func_call(___def_to_func8(a))
#define ___def_to_func_ex8(a) ___def_tul_part_list_full_ex_8##a
#define ___def_funcex8_make(a) ___def_funcex_call(___def_to_func_ex8(a))
/// redefine the part list
#define ___def_tul_data_part_list_8(name,i,t1,t2,t3,t4,t5,t6,t7,t8) ___def_func8_make(___def_to_data_param8(name,i,t1,t2,t3,t4,t5,t6,t7,t8))
#define ___def_tul_method_part_list_8(name,i,t1,t2,t3,t4,t5,t6,t7,t8) ___def_func8_make(___def_to_method_param8(name,i,t1,t2,t3,t4,t5,t6,t7,t8))
#define ___def_tul_data_part_list_ex_8(name,i,t1,t2,t3,t4,t5,t6,t7,t8) ___def_funcex8_make(___def_to_data_param8(name,i,t1,t2,t3,t4,t5,t6,t7,t8))



//// the tulist you can use

/*
	t1 = (t) --> t,void
	t1 = (t,s) --> t,s
	t1 = (t,s,m) --> t,s,m
*/

/// 0 type list
#define ___def_0_tulist(name) \
	___def_tul_mainclass(name,0)

#define ___def_0method_tulist(name) \
	___def_tul_mainclass(name,0)

#define ___def_0_tulist_ex(name) \
	___def_tul_mainclass(name,0)


/// 1 type list
#define ___def_1_tulist(name,t1) \
	___def_tul_mainclass(name,1); \
	___def_tul_data_part_list_1(name,1,t1)

#define ___def_1method_tulist(name,t1) \
	___def_tul_mainclass(name,1); \
	___def_tul_method_part_list_1(name,1,t1)

#define ___def_1_tulist_ex(name,t1) \
	___def_tul_mainclass(name,1); \
	___def_tul_data_part_list_ex_1(name,1,t1)


/// 2 type list
#define ___def_2_tulist(name,t1,t2) \
	___def_tul_mainclass(name,2); \
	___def_tul_data_part_list_2(name,2,t1,t2)

#define ___def_2method_tulist(name,t1,t2) \
	___def_tul_mainclass(name,2); \
	___def_tul_method_part_list_2(name,2,t1,t2)

#define ___def_2_tulist_ex(name,t1,t2) \
	___def_tul_mainclass(name,2); \
	___def_tul_data_part_list_ex_2(name,2,t1,t2)


/// 3 type list
#define ___def_3_tulist(name,t1,t2,t3) \
	___def_tul_mainclass(name,3); \
	___def_tul_data_part_list_3(name,3,t1,t2,t3)

#define ___def_3method_tulist(name,t1,t2,t3) \
	___def_tul_mainclass(name,3); \
	___def_tul_method_part_list_3(name,3,t1,t2,t3)

#define ___def_3_tulist_ex(name,t1,t2,t3) \
	___def_tul_mainclass(name,3); \
	___def_tul_data_part_list_ex_3(name,3,t1,t2,t3)


/// 4 type list
#define ___def_4_tulist(name,t1,t2,t3,t4) \
	___def_tul_mainclass(name,4); \
	___def_tul_data_part_list_4(name,4,t1,t2,t3,t4)

#define ___def_4method_tulist(name,t1,t2,t3,t4) \
	___def_tul_mainclass(name,4); \
	___def_tul_method_part_list_4(name,4,t1,t2,t3,t4)

#define ___def_4_tulist_ex(name,t1,t2,t3,t4) \
	___def_tul_mainclass(name,4); \
	___def_tul_data_part_list_ex_4(name,4,t1,t2,t3,t4)


/// 5 type list
#define ___def_5_tulist(name,t1,t2,t3,t4,t5) \
	___def_tul_mainclass(name,5); \
	___def_tul_data_part_list_5(name,5,t1,t2,t3,t4,t5)

#define ___def_5method_tulist(name,t1,t2,t3,t4,t5) \
	___def_tul_mainclass(name,5); \
	___def_tul_method_part_list_5(name,5,t1,t2,t3,t4,t5)

#define ___def_5_tulist_ex(name,t1,t2,t3,t4,t5) \
	___def_tul_mainclass(name,5); \
	___def_tul_data_part_list_ex_5(name,5,t1,t2,t3,t4,t5)

/// 6 type list
#define ___def_6_tulist(name,t1,t2,t3,t4,t5,t6) \
	___def_tul_mainclass(name,6); \
	___def_tul_data_part_list_6(name,6,t1,t2,t3,t4,t5,t6)

#define ___def_6method_tulist(name,t1,t2,t3,t4,t5,t6) \
	___def_tul_mainclass(name,6); \
	___def_tul_method_part_list_6(name,6,t1,t2,t3,t4,t5,t6)

#define ___def_6_tulist_ex(name,t1,t2,t3,t4,t5,t6) \
	___def_tul_mainclass(name,6); \
	___def_tul_data_part_list_ex_6(name,6,t1,t2,t3,t4,t5,t6)

/// 7 type list
#define ___def_7_tulist(name,t1,t2,t3,t4,t5,t6,t7) \
	___def_tul_mainclass(name,7); \
	___def_tul_data_part_list_7(name,7,t1,t2,t3,t4,t5,t6,t7)

#define ___def_7method_tulist(name,t1,t2,t3,t4,t5,t6,t7) \
	___def_tul_mainclass(name,7); \
	___def_tul_method_part_list_7(name,7,t1,t2,t3,t4,t5,t6,t7)

#define ___def_7_tulist_ex(name,t1,t2,t3,t4,t5,t6,t7) \
	___def_tul_mainclass(name,7); \
	___def_tul_data_part_list_ex_7(name,7,t1,t2,t3,t4,t5,t6,t7)

/// 8 type list
#define ___def_8_tulist(name,t1,t2,t3,t4,t5,t6,t7,t8) \
	___def_tul_mainclass(name,8); \
	___def_tul_data_part_list_8(name,8,t1,t2,t3,t4,t5,t6,t7,t8)

#define ___def_8method_tulist(name,t1,t2,t3,t4,t5,t6,t7,t8) \
	___def_tul_mainclass(name,8); \
	___def_tul_method_part_list_8(name,8,t1,t2,t3,t4,t5,t6,t7,t8)

#define ___def_8_tulist_ex(name,t1,t2,t3,t4,t5,t6,t7,t8) \
	___def_tul_mainclass(name,8); \
	___def_tul_data_part_list_ex_8(name,8,t1,t2,t3,t4,t5,t6,t7,t8)




#define ___def_features_decl(name) ___def_tul_mainclass_decl(name)

#define ___def_0_features(name) ___def_0_tulist_ex(name)
#define ___def_1_features(name,t1) ___def_1_tulist_ex(name,t1)
#define ___def_2_features(name,t1,t2) ___def_2_tulist_ex(name,t1,t2)
#define ___def_3_features(name,t1,t2,t3) ___def_3_tulist_ex(name,t1,t2,t3)
#define ___def_4_features(name,t1,t2,t3,t4) ___def_4_tulist_ex(name,t1,t2,t3,t4)
#define ___def_5_features(name,t1,t2,t3,t4,t5) ___def_5_tulist_ex(name,t1,t2,t3,t4,t5)
#define ___def_6_features(name,t1,t2,t3,t4,t5,t6) ___def_6_tulist_ex(name,t1,t2,t3,t4,t5,t6)
#define ___def_7_features(name,t1,t2,t3,t4,t5,t6,t7) ___def_7_tulist_ex(name,t1,t2,t3,t4,t5,t6,t7)
#define ___def_8_features(name,t1,t2,t3,t4,t5,t6,t7,t8) ___def_8_tulist_ex(name,t1,t2,t3,t4,t5,t6,t7,t8)

#define ___def_0data_features(name) ___def_0_tulist(name)
#define ___def_1data_features(name,t1) ___def_1_tulist(name,t1)
#define ___def_2data_features(name,t1,t2) ___def_2_tulist(name,t1,t2)
#define ___def_3data_features(name,t1,t2,t3) ___def_3_tulist(name,t1,t2,t3)
#define ___def_4data_features(name,t1,t2,t3,t4) ___def_4_tulist(name,t1,t2,t3,t4)
#define ___def_5data_features(name,t1,t2,t3,t4,t5) ___def_5_tulist(name,t1,t2,t3,t4,t5)
#define ___def_6data_features(name,t1,t2,t3,t4,t5,t6) ___def_6_tulist(name,t1,t2,t3,t4,t5,t6)
#define ___def_7data_features(name,t1,t2,t3,t4,t5,t6,t7) ___def_7_tulist(name,t1,t2,t3,t4,t5,t6,t7)
#define ___def_8data_features(name,t1,t2,t3,t4,t5,t6,t7,t8) ___def_8_tulist(name,t1,t2,t3,t4,t5,t6,t7,t8)

#define ___def_0method_features(name) ___def_0method_tulist(name)
#define ___def_1method_features(name,t1) ___def_1method_tulist(name,t1)
#define ___def_2method_features(name,t1,t2) ___def_2method_tulist(name,t1,t2)
#define ___def_3method_features(name,t1,t2,t3) ___def_3method_tulist(name,t1,t2,t3)
#define ___def_4method_features(name,t1,t2,t3,t4) ___def_4method_tulist(name,t1,t2,t3,t4)
#define ___def_5method_features(name,t1,t2,t3,t4,t5) ___def_5method_tulist(name,t1,t2,t3,t4,t5)
#define ___def_6method_features(name,t1,t2,t3,t4,t5,t6) ___def_6method_tulist(name,t1,t2,t3,t4,t5,t6)
#define ___def_7method_features(name,t1,t2,t3,t4,t5,t6,t7) ___def_7method_tulist(name,t1,t2,t3,t4,t5,t6,t7)
#define ___def_8method_features(name,t1,t2,t3,t4,t5,t6,t7,t8) ___def_8method_tulist(name,t1,t2,t3,t4,t5,t6,t7,t8)


#define ___def_1v_tulist(name) \
	template <typename t1> \
	struct name \
	{ \
		___def_1_tulist(tl,(t1)); \
		typedef tl<1> type; \
	}

#define ___def_1vmethod_tulist(name) \
	template <typename t1> \
	struct name \
	{ \
		___def_1method_tulist(tl,(t1)); \
		typedef tl<1> type; \
	}


#define ___def_2v_tulist(name) \
	template <typename t1,typename t2> \
	struct name \
	{ \
		___def_2_tulist(tl,(t1),(t2)); \
		typedef tl<2> type; \
	}

#define ___def_2vmethod_tulist(name) \
	template <typename t1,typename t2> \
	struct name \
	{ \
		___def_2method_tulist(tl,(t1),(t2)); \
		typedef tl<2> type; \
	}


#define ___def_3v_tulist(name) \
	template <typename t1,typename t2,typename t3> \
	struct name \
	{ \
		___def_3_tulist(tl,(t1),(t2),(t3)); \
		typedef tl<3> type; \
	}

#define ___def_3vmethod_tulist(name) \
	template <typename t1,typename t2,typename t3> \
	struct name \
	{ \
		___def_3method_tulist(tl,(t1),(t2),(t3)); \
		typedef tl<3> type; \
	}

#define ___def_4v_tulist(name) \
	template <typename t1,typename t2,typename t3,typename t4> \
	struct name \
	{ \
		___def_4_tulist(tl,(t1),(t2),(t3),(t4)); \
		typedef tl<3> type; \
	}

#define ___def_4vmethod_tulist(name) \
	template <typename t1,typename t2,typename t3,typename t4> \
	struct name \
	{ \
		___def_4method_tulist(tl,(t1),(t2),(t3),(t4)); \
		typedef tl<3> type; \
	}





#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "../met/tslist_decl.h"
#include "../cxx/tags.h"

#pragma once


___namespace2_begin(ox,mix)


template <typename t1,typename t2=ox::null_type, typename t3=ox::null_type,typename t4=ox::null_type>
struct tulist;

template <typename t1>
struct tulist <t1,ox::null_type,ox::null_type,ox::null_type>
{
	typedef typename ox::met::ts::tof<t1>::t t1t;
	typedef typename ox::met::ts::sof<t1>::t t1s;

	___def_1_tulist_ex(tl,2(t1t,(d(t1s))));
	typedef typename tl<0>::type type;
};


template <typename t1,typename t2>
struct tulist <t1,t2,ox::null_type,ox::null_type>
{
	typedef typename ox::met::ts::tof<t1>::t t1t;
	typedef typename ox::met::ts::sof<t1>::t t1s;
	typedef typename ox::met::ts::tof<t2>::t t2t;
	typedef typename ox::met::ts::sof<t2>::t t2s;
	___def_2_tulist_ex(tl,2(t1t,(d(t1s))),2(t2t,(d(t2s))));
	typedef typename tl<0>::type type;
};

template <typename t1,typename t2,typename t3>
struct tulist <t1,t2,t3,ox::null_type>
{
	typedef typename ox::met::ts::tof<t1>::t t1t;
	typedef typename ox::met::ts::sof<t1>::t t1s;
	typedef typename ox::met::ts::tof<t2>::t t2t;
	typedef typename ox::met::ts::sof<t2>::t t2s;
	typedef typename ox::met::ts::tof<t3>::t t3t;
	typedef typename ox::met::ts::sof<t3>::t t3s;
	___def_3_tulist_ex(tl,2(t1t,(d(t1s))),2(t2t,(d(t2s))),2(t3t,(d(t3s))));
	typedef typename tl<0>::type type;
};

template <typename t1,typename t2,typename t3,typename t4>
struct tulist
{
	typedef typename ox::met::ts::tof<t1>::t t1t;
	typedef typename ox::met::ts::sof<t1>::t t1s;
	typedef typename ox::met::ts::tof<t2>::t t2t;
	typedef typename ox::met::ts::sof<t2>::t t2s;
	typedef typename ox::met::ts::tof<t3>::t t3t;
	typedef typename ox::met::ts::sof<t3>::t t3s;
	typedef typename ox::met::ts::tof<t4>::t t4t;
	typedef typename ox::met::ts::sof<t4>::t t4s;
	___def_4_tulist_ex(tl,2(t1t,(d(t1s))),2(t2t,(d(t2s))),2(t3t,(d(t3s))),2(t4t,(d(t4s))));
	typedef typename tl<0>::type type;
};


template <size_t n> struct mix;

template <> struct mix <1> { ___def_1v_tulist(list); };
template <> struct mix <2> { ___def_2v_tulist(list); };
template <> struct mix <3> { ___def_3v_tulist(list); };
template <> struct mix <4> { ___def_4v_tulist(list); };





___namespace2_end()





#include "../cxx/tags.h"

#pragma once

//// null item

___namespace2_begin(ox,mix)


template <size_t i>
struct tulist_null
{
	typedef ox::itot<0> idiff;
	static size_t const n = 0;
	typedef void _t_type;
	typedef void _t_syms;
	typedef void _t_meta;
	typedef void lambda;
	typedef ox::identity<ox::empty_type> wp0;
};
template <size_t i>
struct tulist_sole_null
{
	typedef ox::itot<0> idiff;
	static size_t const n = 0;
	typedef void _t_type;
	typedef void _t_syms;
	typedef void _t_meta;
	typedef void lambda;
	typedef ox::identity<ox::empty_type> wp0;
};


//template <typename t>
//struct tulist_1method
//{
//	template <size_t i>
//	struct list
//	{
//		//typedef ox::tags::data idiff;
//		static size_t const n = 1;
//		//typedef void unit;
//	};
//	template <>
//	struct list <1>
//	{
//		typedef ox::tags::method idiff;
//		static size_t const i = 1;
//		typedef t _t_type;
//		typedef void _t_syms;
//		typedef void _t_meta;
//		typedef void unit;
//		typedef ox::identity<ox::empty_type> wp0;
//	};
//	typedef list<1> type;
//};

template <typename t>
struct tulist_1method
{
	template <size_t i>
	struct list
	{
		//typedef ox::tags::data idiff;
		static size_t const n = 1;
		//typedef void unit;
	};
	template <>
	struct list <1>
	{
		typedef ox::itot<0> idiff;
		static size_t const i = 1;
		typedef t _t_type;
		typedef void _t_syms;
		typedef void _t_meta;
		typedef void unit;
		typedef ox::identity<ox::empty_type> wp0;
	};
	typedef list<1> type;
};

___namespace2_end()
