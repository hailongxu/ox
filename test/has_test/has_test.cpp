// has_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/met/has.h"
#include "../../src/ox/def_macro.h"


//
//typedef char yes;
//typedef int no;
//
struct B {};
struct D : B {};

struct host
{
	operator B* () const { return 0; }
	operator D* () { return 0; }
	void f(int) {}
};

template <typename T>
void test(D*, T) /// YES
{
	int o = 1;
}
void test(B*, int) /// NO
{
	int k = 1;
}

___def_has_func(f,void,f,());
___def_has_func(fint,void,f,(int));

int _tmain(int argc, _TCHAR* argv[])
{
	test(host(),int());
	bool b = has_func_f<host>::value;
	bool b2 = has_func_fint<host>::value;
	return 0;
}

