// macro_test.cpp : defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <string.h>
#include "../../src/ox/def_macro.h"
#include "../../src/mix/mixture_feature_def.h"




namespace defr_typenames
{
	void test()
	{
		char* str = ___def_to_string(___defr_typenames(t,1,2,));
		char* std = "typename t1,typename t2";
		printf("defr_typenames<from,to>:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
}
namespace def_params
{
	void test()
	{
		char* str = ___def_to_string(___def_params(1,3,t,,p));
		//char* str = ___def_to_string(___mr_ere_ere_1(b,t,1,0,t,p,1,0,p));
		char* std = "t1 p1,t2 p2,t3 p3";
		printf("params:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
}
namespace def_types
{
	void test()
	{
		char* str = ___def_to_string(___def_types(p,0,2,));
		char* std = "p0,p1,p2";
		printf("types:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
}
namespace def_objects
{
	void test()
	{
		char* str = ___def_to_string(___def_objects(p,0,2,));
		char* std = "p0,p1,p2";
		printf("objects:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
}

namespace less
{
	void test_3less2()
	{
		char* str = ___def_to_string(___def_less(3,2));
		char* std = "F";
		printf("less 3<2:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
	void test_0less0()
	{
		char* str = ___def_to_string(___def_less(0,0));
		char* std = "F";
		printf("less 0<0:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
	void test()
	{
		test_3less2();
		test_0less0();
	}
}

namespace stod
{
	void test_stod()
	{
		char* str = ___def_to_string(___def_stod(___def_less(3,2)));
		char* std = "b";
		printf("d 3<2:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
	void test()
	{
		test_stod();
	}
}

namespace join
{
	void test_joinf()
	{
		char* str = ___def_to_string(___def_join(f,a,b));
		char* std = "ab";
		printf("joinf:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
	void test_joinb()
	{
		char* str = ___def_to_string(___def_join(b,a,b));
		char* std = "ba";
		printf("joinb 0<0:\n");
		printf(str);printf("\n");
		printf(std);printf("\n\n");
		assert(strcmp(str,std)==0);
	}
	void test()
	{
		test_joinf();
		test_joinb();
	}
}

#define TOPARAM(i,a) (i,___def_to_many(a))
#define TOFUN(a) CCC##a
#define FF(a) FF2(a)
#define FF2(a) a

#define CC(i,a) CC2(TOPARAM(i,a))
#define CC2(a) FF(TOFUN(a))

#define CCC(a,b,c) { 1=a, 2=b, 3=c }
#define DD(a) ___def_to_many(a)
#define JJ(a,b) a&&b***


#define KK(a,b) OOO##a##[]=b##iiiik

namespace tuple_list
{
	void test()
	{
		//char* str = ___def_to_string(__def_mainclass(item,0,9,int,ox::data));
		//printf ("%s\n",str);

		//char* str2 = ___def_to_string(___def_tulist_3(item,(int),long,char));
		//printf ("%s\n",str2);

		//char* str3 = ___def_to_string(___def_tulistv_1(item));
		//printf ("%s\n",str3);

		char* str4 = ___def_to_string(DD(1(6)));
		printf ("%s\n",str4);
		char* str5 = ___def_to_string(CC(5,(4,ppp)));
		printf ("%s\n",str5);
		char* str6 = ___def_to_string(JJ((9,8)));
		printf ("%s\n",str5);

		char* str7 = ___def_to_string(KK(9,8));
		printf ("%s\n",str5);
	}
}



int main(int argc, char* argv[])
{
	tuple_list::test();
	stod::test();
	join::test();
	less::test();
	defr_typenames::test();
	def_params::test();
	def_types::test();
	def_objects::test();

	getchar();
	return 0;
}

