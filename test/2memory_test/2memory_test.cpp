// 2memory_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/cxx/bintree.h"


int _tmain(int argc, _TCHAR* argv[])
{
	{
		typedef ox::free_tree_tt<long>::type free_tree_t;
		free_tree_t tree;
		tree.insert(free_tree_t::dot_type(),1);
		
		/////typedef ox::free_tree_center<>
		//typedef ox::free_tree_trait<int,ox::cppmalloc>::node_trait_tt node_t;
		//typedef ox::free_tree_trait<int,ox::cppmalloc>::internal_node internal_node;
		//node_t node;
		//node.as_children(0);
		//node.insert(0,internal_node(),0);
		//node.fronter(0);
		//node.backer(0);
		//node.ender(0);
	}
	typedef ox::bptree_tt<4,int>::type bptree;
	bptree eee;
	eee.insert(100);
	eee.insert(110);
	eee.insert(120);
	eee.insert(111);
	eee.insert(140);
	eee.insert(150);
	eee.insert(160);
	eee.insert(170);
	eee.insert(151);
	eee.insert(152);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(140);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(152);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(150);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(170);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(160);
	//ox::print_pre_bnode<>()(eee.root());
	eee.remove(120);
	ox::print_pre_bnode<>()(eee.root());
	eee.remove(111);
	ox::print_pre_bnode<>()(eee.root());

	//struct printbplus
	//{
	//	printbplus(ox::btree_tt<4,int>::type& tr):bt(tr){}
	//	ox::btree_tt<4,int>::type& bt;
	//	void operator()() {ox::print_pre_bplusnode<>()(bt.root());}
	//};
	//ox::bplustree_tt<4,int>::type ddd;
	//ddd.insert(100);
	//ddd.insert(110);
	//ddd.insert(120);
	////ox::print_pre_bplusnode<>()(ddd.root());
	//ddd.insert(111);
	////ox::print_pre_bplusnode<>()(ddd.root());
	//ddd.insert(140);
	//ddd.insert(150);
	//ddd.insert(160);
	//ddd.insert(170);
	//ddd.insert(151);
	////ox::print_pre_bplusnode<>()(ddd.root());
	//ddd.insert(152);
	//ox::print_pre_bplusnode<>()(ddd.root());
	//ddd.test_find(152);
	//ddd.remove(152);



	struct printb
	{
		printb(ox::btree_tt<4,int>::type& tr):bt(tr){}
		ox::btree_tt<4,int>::type& bt;
		void operator()() {ox::print_pre_bnode<>()(bt.root());}
	};

	ox::btree_tt<4,int>::type ccc;
	ccc.insert(50);
	ccc.insert(60);
	ccc.insert(70);
	ccc.insert(80);
	ccc.insert(90);
	ccc.insert(100);//,printb(ccc));
	ccc.insert(120);//,printb(ccc));
	ccc.insert(160);//,printb(ccc));
	ccc.insert(180);//,printb(ccc));
	ccc.insert(181);//,printb(ccc));
	ccc.insert(182);//,printb(ccc));
	ccc.insert(183);//,printb(ccc));
	ccc.insert(184);//,printb(ccc));
	ccc.insert(185);//,printb(ccc));

	ccc.insert(89);//,printb(ccc));
	ccc.insert(88);//,printb(ccc));
	ccc.insert(87);//,printb(ccc));
	ccc.insert(86);//,printb(ccc));
	ccc.insert(85);//,printb(ccc));
	ccc.insert(84);//,printb(ccc));
	ccc.insert(83);//,printb(ccc));
	ccc.insert(82);//,printb(ccc));
	ccc.insert(81);//,printb(ccc));
	//ccc.insert(17);//,printb(ccc));
	ox::print_pre_bnode<>()(ccc.root());
	ccc.find(5);
	ccc.remove(89);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(84);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(86);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(160);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(120);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(182);
	//ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(50);
	ox::print_pre_bnode<>()(ccc.root());
	ccc.remove(80);
	ox::print_pre_bnode<>()(ccc.root());
	
	ox::bintree_tt<int>::type aaa;
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
		printrb(ox::rbtree_tt<int>::type& tr):rbt(tr){}
		ox::rbtree_tt<int>::type const& rbt;
		void operator()() const {ox::print_pre_rbnode<>()(rbt.root());}
	};

	ox::rbtree_tt<int>::type bbb;
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

	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(10);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(4);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(6);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(8);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(100);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(3);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(5);
	ox::print_pre_rbnode<>()(bbb.root());
	bbb.remove(9);
	ox::print_pre_rbnode<>()(bbb.root());

	//ox::bintree_tt<int>::const_iterator i = aaa.begin();
	//for(;i!=aaa.end();++i)
	//{
	//	printf("%d ",*i);
	//}
	//printf("\n");
	//ox::bintree_tt<int>::const_iterator j = --aaa.end();
	//for(;j!=aaa.begin();--j)
	//{
	//	printf("%d ",*j);
	//}
	return 0;
}

