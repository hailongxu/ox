// 2memory_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../src/bintree.h"



int _tmain(int argc, _TCHAR* argv[])
{

	int ints[] = {1,3,5,7,8,10,13,19};
	int count = sizeof(ints)/sizeof(int);
	int* x = abc::binary_search(&ints[0],&ints[count],10);

	{
		abc::btree_tt<3,int>::type aaa;
		aaa.search_in_node(aaa.root(),3);
		aaa.insert(5);
	}
	
	abc::bintree_tt<int>::type aaa;
	aaa.insert(5);
	aaa.insert(8);
	aaa.insert(6);
	aaa.insert(3);
	aaa.insert(4);
	aaa.insert(7);
	aaa.remove(5);
	aaa.remove(4);
	aaa.remove(8);
	aaa.remove(6);
	aaa.remove(7);
	aaa.remove(3);

	struct printrb
	{
		printrb(abc::rbtree_tt<int>::type& tr):rbt(tr){}
		abc::rbtree_tt<int>::type const& rbt;
		void operator()() const {abc::print_pre_rbnode<>()(rbt.root());}
	};

	abc::rbtree_tt<int>::type bbb;
	bbb.insert(5);
	bbb.insert(8);
	bbb.insert(6);
	bbb.insert(3);
	bbb.insert(4);
	bbb.insert(9);
	bbb.insert(10);
	bbb.insert(100);
	//bbb.insert(50);
	//bbb.insert(30);

	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(10);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(4);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(6);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(8);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(100);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(3);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(5);
	abc::print_pre_rbnode<>()(bbb.root());
	bbb.remove(9);
	abc::print_pre_rbnode<>()(bbb.root());

	//abc::bintree_tt<int>::const_iterator i = aaa.begin();
	//for(;i!=aaa.end();++i)
	//{
	//	printf("%d ",*i);
	//}
	//printf("\n");
	//abc::bintree_tt<int>::const_iterator j = --aaa.end();
	//for(;j!=aaa.begin();--j)
	//{
	//	printf("%d ",*j);
	//}
	return 0;
}

