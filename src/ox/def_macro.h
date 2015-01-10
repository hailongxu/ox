
/*
 *  @author£ºxuhailong
 *  @date£º2007-7-10
 *      implements the general macro operation that can generate the
 *		repeated codes
 */


#pragma once




/// unfold macro and output the result as string

#define ___def_to_string(s) ___def_to_string2(s)
#define ___def_to_string2(s) #s

#define ___def_unfold(e) ___def_unfold2(e)
#define ___def_unfold2(e) e

/// join two symbol
#define ___def_joinf(x,y) ___def_joinf2(x,y)
#define ___def_joinf2(x,y) x##y

#define ___def_joinb(x,y) ___def_joinb2(x,y)
#define ___def_joinb2(x,y) y##x

#define ___def_join(d,x,y) ___def_join##d(x,y)


/// join three symbol
#define ___def_3joinf(x,y,z) ___def_3joinf2(x,y,z)
#define ___def_3joinf2(x,y,z) x##y##z

#define ___def_3joinb(x,y,z) ___def_3joinb2(x,y,z)
#define ___def_3joinb2(x,y,z) z##y##x

#define ___def_3join(d,x,y,z) ___def_3join##d(x,y,z)


/// join four symbol
#define ___def_4_join(a,b,c,d) ___def_4_join2(a,b,c,d)
#define ___def_4_join2(a,b,c,d) a##b##c##d

/// join five symbol
#define ___def_5_join(a,b,c,d,e) ___def_5_join2(a,b,c,d,e)
#define ___def_5_join2(a,b,c,d,e) a##b##c##d##e

/// join six symbol
#define ___def_6_join(a,b,c,d,e,f) ___def_6_join2(a,b,c,d,e,f)
#define ___def_6_join2(a,b,c,d,e,f) a##b##c##d##e##f

/// join six symbol
#define ___def_7_join(a,b,c,d,e,f,g) ___def_7_join2(a,b,c,d,e,f,g)
#define ___def_7_join2(a,b,c,d,e,f,g) a##b##c##d##e##f##g

/// join two symbol with whitespace
#define ___def_joinf_with_ws(a,b) ___def_joinf_with_ws2(a,b) 
#define ___def_joinf_with_ws2(a,b) a b


#define ___def_join_lbrace(a) ___def_join_lbrace2(a)
#define ___def_join_lbrace2(a) a##(


#define ___def_comma ,
#define ___def_lbrace (
#define ___def_rbrace )


/// down a number

//// amount to 32 items

#define ___def_dec31 30
#define ___def_dec30 29
#define ___def_dec29 28
#define ___def_dec28 27
#define ___def_dec27 26
#define ___def_dec26 25
#define ___def_dec25 24
#define ___def_dec24 23
#define ___def_dec23 22
#define ___def_dec22 21
#define ___def_dec21 20
#define ___def_dec20 19
#define ___def_dec19 18
#define ___def_dec18 17
#define ___def_dec17 16
#define ___def_dec16 15
#define ___def_dec15 14
#define ___def_dec14 13
#define ___def_dec13 12
#define ___def_dec12 11
#define ___def_dec11 10
#define ___def_dec10 9
#define ___def_dec9 8
#define ___def_dec8 7
#define ___def_dec7 6
#define ___def_dec6 5
#define ___def_dec5 4
#define ___def_dec4 3
#define ___def_dec3 2
#define ___def_dec2 1
#define ___def_dec1 0
#define ___def_dec0 _1
#define ___def_dec_1 _2
#define ___def_dec_2 _3
#define ___def_dec_3 _4
#define ___def_dec_4 _5
#define ___def_dec_5 _6
#define ___def_dec_6 _7
#define ___def_dec_7 _8
#define ___def_dec_8 _9
#define ___def_dec_9 _10
#define ___def_dec_10 _11
#define ___def_dec_11 _12
#define ___def_dec_12 _13
#define ___def_dec_13 _14
#define ___def_dec_14 _15
#define ___def_dec_15 _16
#define ___def_dec_16 _17
#define ___def_dec_17 _18
#define ___def_dec_18 _19
#define ___def_dec_19 _20
#define ___def_dec(n) ___def_dec##n

#define ___def_inc8 9
#define ___def_inc7 8
#define ___def_inc6 7
#define ___def_inc5 6
#define ___def_inc4 5
#define ___def_inc3 4
#define ___def_inc2 3
#define ___def_inc1 2
#define ___def_inc0 1
#define ___def_inc(n) ___def_inc##n


/// ensure the n <=9

#define ___def_sub0(n) n
#define ___def_sub1(n) ___def_dec(n)
#define ___def_sub2(n) ___def_sub1(___def_dec(n))
#define ___def_sub3(n) ___def_sub2(___def_dec(n))
#define ___def_sub4(n) ___def_sub3(___def_dec(n))
#define ___def_sub5(n) ___def_sub4(___def_dec(n))
#define ___def_sub6(n) ___def_sub5(___def_dec(n))
#define ___def_sub7(n) ___def_sub6(___def_dec(n))
#define ___def_sub8(n) ___def_sub7(___def_dec(n))
#define ___def_sub9(n) ___def_sub8(___def_dec(n))
#define ___def_sub10(n) ___def_sub9(___def_dec(n))
#define ___def_sub11(n) ___def_sub10(___def_dec(n))
#define ___def_sub12(n) ___def_sub11(___def_dec(n))
#define ___def_sub13(n) ___def_sub12(___def_dec(n))
#define ___def_sub14(n) ___def_sub13(___def_dec(n))
#define ___def_sub15(n) ___def_sub14(___def_dec(n))
#define ___def_sub16(n) ___def_sub15(___def_dec(n))
#define ___def_sub17(n) ___def_sub16(___def_dec(n))
#define ___def_sub18(n) ___def_sub17(___def_dec(n))
#define ___def_sub19(n) ___def_sub18(___def_dec(n))
#define ___def_sub20(n) ___def_sub19(___def_dec(n))
#define ___def_sub21(n) ___def_sub20(___def_dec(n))
#define ___def_sub22(n) ___def_sub21(___def_dec(n))
#define ___def_sub23(n) ___def_sub22(___def_dec(n))
#define ___def_sub24(n) ___def_sub23(___def_dec(n))
#define ___def_sub25(n) ___def_sub24(___def_dec(n))
#define ___def_sub26(n) ___def_sub25(___def_dec(n))
#define ___def_sub27(n) ___def_sub26(___def_dec(n))
#define ___def_sub28(n) ___def_sub27(___def_dec(n))
#define ___def_sub29(n) ___def_sub28(___def_dec(n))
#define ___def_sub30(n) ___def_sub29(___def_dec(n))
#define ___def_sub31(n) ___def_sub30(___def_dec(n))
#define ___def_sub(n1,n2) ___def_sub##n2(n1)


/// ensure the n <=9
/// T : True
/// F : False

#define ___def_less_0__31 F
#define ___def_less_0__30 F
#define ___def_less_0__29 F
#define ___def_less_0__28 F
#define ___def_less_0__27 F
#define ___def_less_0__26 F
#define ___def_less_0__25 F
#define ___def_less_0__24 F
#define ___def_less_0__23 F
#define ___def_less_0__22 F
#define ___def_less_0__21 F
#define ___def_less_0__20 F
#define ___def_less_0__19 F
#define ___def_less_0__18 F
#define ___def_less_0__17 F
#define ___def_less_0__16 F
#define ___def_less_0__15 F
#define ___def_less_0__14 F
#define ___def_less_0__13 F
#define ___def_less_0__12 F
#define ___def_less_0__11 F
#define ___def_less_0__10 F
#define ___def_less_0__9 F
#define ___def_less_0__8 F
#define ___def_less_0__7 F
#define ___def_less_0__6 F
#define ___def_less_0__5 F
#define ___def_less_0__4 F
#define ___def_less_0__3 F
#define ___def_less_0__2 F
#define ___def_less_0__1 F
#define ___def_less_0_0 F
#define ___def_less_0_1 T
#define ___def_less_0_2 T
#define ___def_less_0_3 T
#define ___def_less_0_4 T
#define ___def_less_0_5 T
#define ___def_less_0_6 T
#define ___def_less_0_7 T
#define ___def_less_0_8 T
#define ___def_less_0_9 T
#define ___def_less_0_10 F
#define ___def_less_0_11 T
#define ___def_less_0_12 T
#define ___def_less_0_13 T
#define ___def_less_0_14 T
#define ___def_less_0_15 T
#define ___def_less_0_16 T
#define ___def_less_0_17 T
#define ___def_less_0_18 T
#define ___def_less_0_19 T
#define ___def_less_0_20 T
#define ___def_less_0_21 T
#define ___def_less_0_22 T
#define ___def_less_0_23 T
#define ___def_less_0_24 T
#define ___def_less_0_25 T
#define ___def_less_0_26 T
#define ___def_less_0_27 T
#define ___def_less_0_28 T
#define ___def_less_0_29 T
#define ___def_less_0_30 T
#define ___def_less_0_31 T

#define ___def_less(n,m) ___def_joinf(___def_less_0_,___def_sub(m,n))

/// sign to direction
/// F(false) --> b (backword)
/// T(true) --> f (foreward)
#define ___def_stod_F b
#define ___def_stod_T f
#define ___def_stod(s) ___def_stod_pre(s)
#define ___def_stod_pre(s) ___def_stod_##s



///mr : marco recursively

#define ___mr_e_0(e,n) e
#define ___mr_e_1(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_2(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_3(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_4(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_5(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_6(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_7(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_8(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_9(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_10(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_11(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_12(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_13(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_14(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_15(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_16(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_17(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_18(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_19(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_20(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_21(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_22(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_23(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_24(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_25(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_26(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_27(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_28(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_29(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_30(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))
#define ___mr_e_31(e,n) ___def_joinf(e,___def_joinf(___mr_e_,___def_dec(n))(e,n))

#define ___defr_e(e,n) ___mr_e_##n(e,n)

/// iterate (e,n)
/// (p,3) ==> p3,p2,p1
#define ___mrr_en_0(e,n) ___def_joinf(e,n)
#define ___mrr_en_1(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_2(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_3(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_4(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_5(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_6(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_7(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_8(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_9(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_10(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_11(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_12(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_13(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_14(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_15(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_16(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_17(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_18(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_19(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_20(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_21(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_22(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_23(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_24(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_25(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_26(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_27(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_28(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_29(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_30(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))
#define ___mrr_en_31(e,n) ___def_joinf(e,n),___def_joinf(___mrr_en_,___def_dec(n))(e,___def_dec(n))

#define ___defrr_en(e,n) ___mrr_en_##n(e,n)


/// iterate (e,n,e)
/// (p,3,_tn) ==> p3_tn,p2_tn,p1_tn
#define ___mrr_ene_0(e1,n,e2) ___def_3joinf(e1,n,e2)
#define ___mrr_ene_1(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_2(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_3(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_4(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_5(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_6(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_7(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_8(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_9(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_10(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_11(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_12(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_13(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_14(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_15(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_16(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_17(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_18(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_19(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_20(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_21(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_22(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_23(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_24(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_25(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_26(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_27(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_28(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_29(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_30(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)
#define ___mrr_ene_31(e1,n,e2) ___def_3joinf(e1,n,e2),___def_joinf(___mrr_ene_,___def_dec(n))(e1,___def_dec(n),e2)

#define ___defrr_ene(e1,n,e2) ___mrr_ene_##n(e1,n,e2)



/// iterate (e,n)
/// (p,3) ==> p0,p1,p2,p3

#define ___mr_en_0(d,e,n) ___def_joinf(e,n)
#define ___mr_en_1(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_2(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_3(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_4(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_5(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_6(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_7(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_8(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_9(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_10(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_11(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_12(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_13(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_14(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_15(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_16(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_17(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_18(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_19(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_20(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_21(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_22(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_23(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_24(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_25(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_26(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_27(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_28(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_29(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_30(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))
#define ___mr_en_31(d,e,n) ___def_3join(d,___def_joinf(___mr_en_,___def_dec(n))(d,e,___def_dec(n)),___def_comma,___def_joinf(e,n))

#define ___defr_en(d,e,n) ___mr_en_##n(d,e,n)


/// iterate (e,n,e,e,n,e)
/// (p,3,_tn, _p,3) ==> p3_tn _p3,p2_tn _p2,p1_tn _p1
#define ___mr_ene_ene_0(e1,n,e2,e3,n2,e4) ___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_1(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_2(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_3(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_4(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_5(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_6(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_7(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_8(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_9(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_10(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_11(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_12(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_13(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_14(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_15(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_16(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_17(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_18(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_19(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_20(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_21(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_22(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_23(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_24(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_25(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_26(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_27(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_28(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_29(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_30(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))
#define ___mr_ene_ene_31(e1,n,e2,e3,n2,e4) ___def_joinf(___mr_ene_ene_,___def_dec(n))(e1,___def_dec(n),e2,e3,___def_dec(n2),e4),___def_joinf_with_ws(___def_3_join(e1,n,e2),___def_3_join(e3,n2,e4))

#define ___defr_ene_ene(e1,n1,e2,e3,n2,e4) ___mr_ene_ene_##n1(e1,n1,e2,e3,n2,e4)



/// the minium is 0
/// n>=n2, e.g. 4~2
/// d==f ===> 2,3,4
/// d==b ===> 4,3,2
#define ___mr_ere_0(d,e,n,n2,e2) ___def_3joinf(e,n,e2)
#define ___mr_ere_1(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_2(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_3(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_4(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_5(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_6(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_7(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_8(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_9(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_10(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_11(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_12(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_13(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_14(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_15(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_16(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_17(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_18(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_19(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_20(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_21(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_22(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_23(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_24(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_25(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_26(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_27(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_28(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_29(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_30(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))
#define ___mr_ere_31(d,e,n,n2,e2) ___def_3join(d,___def_joinf(___mr_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2),___def_comma,___def_3joinf(e,n,e2))

/// n>=n2
#define ___defr_ere(d,e,n,n2,e2) ___def_joinf(___mr_ere_,___def_sub(n,n2))(d,e,n,n2,e2)




#define ___mr_ere_ere_0(d,e,n,n2,e2,e3,m,m2,e4) ___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4))
#define ___mr_ere_ere_1(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_2(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_3(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_4(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_5(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_6(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_7(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_8(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_9(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_10(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_11(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_12(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_13(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_14(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_15(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_16(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_17(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_18(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_19(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_20(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_21(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_22(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_23(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_24(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_25(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_26(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_27(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_28(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_29(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_30(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))
#define ___mr_ere_ere_31(d,e,n,n2,e2,e3,m,m2,e4) ___def_3join(d,___def_joinf(___mr_ere_ere_,___def_sub(___def_dec(n),n2))(d,e,___def_dec(n),n2,e2,e3,___def_dec(m),m2,e4),___def_comma,___def_joinf_with_ws(___def_3joinf(e,n,e2),___def_3joinf(e3,m,e4)))

/// <p,3,2,_t, p,3,2,q> == > (p3_t p3q,p2_t p2q)
#define ___defr_ere_ere(d,e,n,n2,e2,e3,m,m2,e4) ___def_joinf(___mr_ere_ere_,___def_sub(n,n2))(d,e,n,n2,e2,e3,m,m2,e4)




//// f(p,r), p: param, r: recurse point

#define ___def_mkrf(f,p,r) ___def_mkrf2(f,p,r)
#define ___def_mkrf2(f,p,r) f##(##p##,##r##)

#define ___defre_0(f,t0) t0
#define ___defre_1(f,t1,t0) ___def_mkrf(f,t1,___defre_0(f,t0))
#define ___defre_2(f,t1,t2,t0) ___def_mkrf(f,t1,___defre_1(f,t2,t0))
#define ___defre_3(f,t1,t2,t3,t0) ___def_mkrf(f,t1,___defre_2(f,t2,t3,t0))
#define ___defre_4(f,t1,t2,t3,t4,t0) ___def_mkrf(f,t1,___defre_3(f,t2,t3,t4,t0))
#define ___defre_5(f,t1,t2,t3,t4,t5,t0) ___def_mkrf(f,t1,___defre_4(f,t2,t3,t4,t5,t0))
#define ___defre_6(f,t1,t2,t3,t4,t5,t6,t0) ___def_mkrf(f,t1,___defre_5(f,t2,t3,t4,t5,t6,t0))
#define ___defre_7(f,t1,t2,t3,t4,t5,t6,t7,t0) ___def_mkrf(f,t1,___defre_6(f,t2,t3,t4,t5,t6,t7,t0))
#define ___defre_8(f,t1,t2,t3,t4,t5,t6,t7,t8,t0) ___def_mkrf(f,t1,___defre_7(f,t2,t3,t4,t5,t6,t7,t8,t0))
#define ___defre_9(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t0) ___def_mkrf(f,t1,___defre_8(f,t2,t3,t4,t5,t6,t7,t8,t9,t0))
#define ___defre_10(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t0) ___def_mkrf(f,t1,___defre_9(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t0))
#define ___defre_11(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t0) ___def_mkrf(f,t1,___defre_10(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t0))
#define ___defre_12(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t0) ___def_mkrf(f,t1,___defre_11(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t0))
#define ___defre_13(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t0) ___def_mkrf(f,t1,___defre_12(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t0))
#define ___defre_14(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t0) ___def_mkrf(f,t1,___defre_13(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t0))
#define ___defre_15(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t0) ___def_mkrf(f,t1,___defre_14(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t0))
#define ___defre_16(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t0) ___def_mkrf(f,t1,___defre_15(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t0))
#define ___defre_17(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t0) ___def_mkrf(f,t1,___defre_16(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t0))
#define ___defre_18(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t0) ___def_mkrf(f,t1,___defre_17(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t0))
#define ___defre_19(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t0) ___def_mkrf(f,t2,___defre_18(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t0))
#define ___defre_20(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t0) ___def_mkrf(f,t2,___defre_19(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t0))
#define ___defre_21(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t0) ___def_mkrf(f,t1,___defre_20(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t0))
#define ___defre_22(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t0) ___def_mkrf(f,t1,___defre_21(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t0))
#define ___defre_23(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t0) ___def_mkrf(f,t1,___defre_22(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t0))
#define ___defre_24(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t0) ___def_mkrf(f,t1,___defre_23(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t0))
#define ___defre_25(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t0) ___def_mkrf(f,t1,___defre_24(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t0))
#define ___defre_26(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t0) ___def_mkrf(f,t1,___defre_25(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t0))
#define ___defre_27(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t0) ___def_mkrf(f,t1,___defre_26(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t0))
#define ___defre_28(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t0) ___def_mkrf(f,t1,___defre_27(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t0))
#define ___defre_29(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t0) ___def_mkrf(f,t1,___defre_28(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,,t27,t28,t29,t0))
#define ___defre_30(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t0) ___def_mkrf(f,t1,___defre_29(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,,t27,t28,t29,t30,t0))
#define ___defre_31(f,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t0) ___def_mkrf(f,t1,___defre_30(f,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,,t27,t28,t28,t29,t30,t31,t0))







//#define ___def_typenames(t,n) ___defr_en(f,typename t,n)

/// n<m ---> T ---> f, but ___defr_er(n,m) {n>=m}, so we exchange the n and m
#define ___defr_typenames_f(t,n,m,ta) ___defr_ere(f,typename t,m,n,ta)
/// n>m ---> F ---> b, but ___defr_er(n,m) {n>=m}, so we keep the order
#define ___defr_typenames_b(t,n,m,ta) ___defr_ere(b,typename t,n,m,ta)
/// n<m or n>m
#define ___defr_typenames(t,n,m,ta) ___def_joinf(___defr_typenames_,___def_stod(___def_less(n,m)))(t,n,m,ta)
#define ___defr_typenames_default(n,m) ___defr_typenames(t,n,m,)

/// n<m ---> T ---> f, but ___defr_er(n,m) {n>=m}, so we exchange the n and m
#define ___defr_params_f(n,m,t,ta,p) ___defr_ere_ere(f,t,m,n,ta,p,m,n,)
/// n>m ---> F ---> b, but ___defr_er(n,m) {n>=m}, so we keep the order
#define ___defr_params_b(n,m,t,ta,p) ___defr_ere_ere(b,t,n,m,ta,p,n,m,)
/// n<m or n>m
#define ___def_params(n,m,t,ta,p) ___def_joinf(___defr_params_,___def_stod(___def_less(n,m)))(n,m,t,ta,p)
#define ___def_params_default(n,m) ___def_params(n,m,t,,p)



/// n<m ---> T ---> f, but ___defr_er(n,m) {n>=m}, so we exchange the n and m
#define ___def_types_f(t,n,m,a) ___defr_ere(f,t,m,n,a)
/// n>m ---> F ---> b, but ___defr_er(n,m) {n>=m}, so we keep the order
#define ___def_types_b(t,n,m,a) ___defr_ere(b,t,n,m,a)
/// n<m or n>m
#define ___def_types(t,n,m,a) ___def_joinf(___def_types_,___def_stod(___def_less(n,m)))(t,n,m,a)
#define ___def_types_default(t,n,m,a) ___def_types(t,n,m,)



/// n<m ---> T ---> f, but ___defr_er(n,m) {n>=m}, so we exchange the n and m
#define ___def_objects_f(t,n,m,a) ___defr_ere(f,t,m,n,a)
/// n>m ---> F ---> b, but ___defr_er(n,m) {n>=m}, so we keep the order
#define ___def_objects_b(t,n,m,a) ___defr_ere(b,t,n,m,a)
/// n<m or n>m
#define ___def_objects(t,n,m,a) ___def_joinf(___def_objects_,___def_stod(___def_less(n,m)))(t,n,m,a)
#define ___def_objects_default(n,m) ___def_objects(p,n,m,)

/*
	history: 
	2011-4-25: add the sign and the number order
 */