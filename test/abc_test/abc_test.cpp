// abc_test.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "conio.h"
#include "../../src/cxx/veclist.h"
#include "../../src/io/file.h"
#include "windows.h"
#include <io.h>
//#include <algorithm>
//#include <iostream>
//#include <utility>

struct print_zvector_item
{
	template <typename noder_tn>
	bool operator()(noder_tn noder)
	{
		printf("value:%d\n",noder.value());
		return true;
	}
};
struct print_list_item
{
	template <typename noder_tn>
	bool operator()(noder_tn noder)
	{
		printf("node:%p,chunk-size:%d,meta-size:%d,data-size:%d,data-count:%d,firstdata:%d\n",
			noder.node(),
			noder.size_of_chunk(),
			noder.size_of_meta(),
			noder.size_of_data(),
			noder.size(),
			*noder.begin());
		return true;
	}
};
template <typename list_tn>
void print_list(list_tn& list)
{
	printf ("\n-----------------\n");
	ox::scan_list<typename list_tn::rooter_type>(list.rooter())(print_list_item());
}
template <typename list_tn>
void print_list_reverse(list_tn& list)
{
	printf ("\n-----------------\n");
	ox::scan_list<typename list_tn::rooter_type>(list.rooter())(print_list_item());
}
template <typename zvector_tn>
void print_zvector(zvector_tn& zvec)
{
	printf ("\n-----------------\n");
	ox::scan_zvector<typename zvector_tn::rooter_type>(zvec.rooter())(print_zvector_item());
}
int _tmain(int argc, _TCHAR* argv[])
{
	{
		ox::listref_tt<char>::rootdata_type rootdata = {0,0};
		ox::listref_tt<char>::allocator_type allocator;
		ox::listref_tt<char>::type listref(&allocator,&rootdata);
		listref.is_empty();
		listref.push_back('1');
	}
	{
		ox::string aaa;
		aaa.push_back('2');
		aaa.push_back('3');
		aaa.append("china");
		aaa.insert(1,2,'0');
		char const* str = aaa.c_str();
		aaa.remove_at(1,2);
		char const* str3 = aaa.c_str();
		ox::string bbb(aaa);
		char const* str2 = bbb.c_str();
	}
	//{
	//	ox::sorted_podliner_off_tt<int>::type::rooter_type::head_t* head = 0;
	//	ox::cppmalloc _malloc;
	//	ox::sorted_podliner_off_tt<int>::type liner(&_malloc,&head);
	//	liner.insert(3);
	//	size_t s = liner.size();
	//	liner.insert(2);
	//	liner.insert(9);
	//	liner.insert(9);
	//	liner.insert(7);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//	liner.insert(7);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//}
	//{
	//	ox::static_sorted_podliner_off_tt<int,5>::type liner;
	//	liner.insert(3);
	//	size_t s = liner.size();
	//	liner.insert(2);
	//	liner.insert(9);
	//	liner.insert(9);
	//	liner.insert(7);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//	liner.insert(7);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//}
	{
		typedef ox::veclistref_tt<int>::type indirect;
		indirect::rooter_type::vector_head_t* head = 0;
		indirect::rooter_type::list_head_type listhead = {0,0};
		ox::cppmalloc malloc;
		indirect aaa(&head,&listhead,&malloc);
		aaa.push_back(1);
		aaa.push_front(2);
		print_list(aaa.as_list());
		aaa.remove(aaa.as_list().backer());
		print_list(aaa.as_list());
		aaa.pop_back();
		aaa.pop_front();
		aaa.insert(aaa.as_vector().fronter(),0);
		aaa.insert(aaa.as_list().fronter(),2,1);
		aaa.is_empty();
		aaa.size();
		//aaa.capacity();
		//aaa.cfronter();
		//char b1 = aaa.fronter().value();
		//char b2 = aaa.backer().value();
		//char b3 = aaa.ender().value();

		//indirect::rooter_type::head_t* suffix_head = 0;
		//indirect::rooter_type::head_t* sorted_head = 0;
		//ox::cppmalloc double_malloc;
		//typedef ox::indirect_doublelineref_tt<char>::type doubleindirect;
		//doubleindirect bbb(&suffix_head,&sorted_head,&double_malloc);
		//bbb.push_back('1');

		//ox::veclist_tt<char>::type ccc;
		//ccc.insert(0,'1');
		//ccc.insert(3,2,'2');
		//ccc.insert(1,"abc",3);
		//ccc.remove(0,2);
		//ccc.fronter();
		//ccc.backer();
		//ccc.ender();
		//char a0 = ccc.at(0).value();
		//char a1 = ccc.at(9).value();
		//char a2 = ccc.at(2).value();
		//char a3 = ccc.at(3).value();
		//char a4 = ccc.at(4).value();
		//char a5 = ccc.at(5).value();
		//char a6 = 0;
		//ccc.insert(1,"246",3);
	}
	//{
	//	typedef std::string::iterator si;
	//	ox::file<char>::type file;
	//	ox::file<char>::type file2;
	//	file.open("d:/1.txt",file.mode().as_read().as_update());
	//	file2.open("d:/2.txt",file.mode().as_append());
	//	file.clear();
	//	file.flush();
	//	//file.replace(0,2,"23");
	//	file.replace(0,10,"0123456789");
	//	file.flush();
	//	file.sub(0,3,file2);
	//	file2.close();
	//	file.flush();
	//	file.insert(1,"ox");
	//	file.flush();
	//	file.remove(1,3);
	//	file.close();
	//	file.replace(0,0,0);
	//	file.insert(0,0,0);
	//	file.remove(0,3);
	//	file.sub(0,0,file);
	//	file.copyto(0,si(),1,1);
	//}
	//{
	//	ox::bilateral_space_tt<>::rooter_type rooter2;
	//	ox::bilateral_space_trait<>::bilateral_space_rooter const rooter;
	//	rooter.as_chunk_list();
	//	rooter.as_used_list();
	//	rooter.as_unused_list();
	//	ox::bilateral_space_tt<char>::type memory;
	//	memory.push_back_chunk(48+2);
	//	print_list(memory.as_chunk_list());
	//	char* p = memory.allocate(1);
	//	char* p2 = memory.allocate(1);
	//	memory.deallocate(p);
	//	memory.deallocate(p2);
	//	//memory.as_chunk_list().front();
	//	print_list(memory.as_chunk_list());
	//	print_list(memory.as_used_list());
	//	print_list(memory.as_unused_list());
	//	
	//	ox::same_space_tt<char>::type same_memory;
	//	same_memory.push_back_chunk(3);
	//	char* q = same_memory.allocate(); *q='x';
	//	char* q2 = same_memory.allocate(); *q2='y';
	//	char* q3 = same_memory.allocate(); *q3='z';
	//	same_memory.find(q);
	//	//char* q4 = same_memory.allocate(); *q3='z';
	//	same_memory.as_chunk_list();
	//	same_memory.as_unused_list();
	//	//same_memory.deallocate(q);
	//}
	//{
	//	//ox::mem_cache cache;
	//	//cache.allocate(100);
	//	ox::typetrait_of_zvector<int>::zvector_rooter rooter;
	//	if (0)
	//	{
	//		typedef ox::typetrait_of_zvector<int>::zvector_rooter::node_type node_type;
	//		node_type* p = 0;
	//		node_type const* p2 = 0;
	//		rooter.allocator();
	//		rooter.as_chunk_list();
	//		rooter.as_chunker(p);
	//		rooter.as_vector(p);
	//		rooter.as_chunker(p2);
	//		rooter.as_vector(p2);
	//		rooter.current();
	//		rooter.is_empty();
	//		rooter.make<int>();
	//		rooter.set_current(0);
	//		rooter._m_data;
	//	}
	//	typedef ox::zvector_tt<int>::type zvector;
	//	zvector aaa;
	//	aaa.reserve(1);
	//	aaa.reserve_tegether(2);
	//	aaa.push_back(1);
	//	aaa.push_back(2);
	//	aaa.push_back(3);
	//	aaa.push_back(4);
	//	aaa.push_front(0);
	//	aaa.insert_before(aaa.fronter(),7);
	//	print_zvector(aaa);
	//	print_list(aaa.rooter().as_chunk_list());
	//	printf(", curr:%p",aaa.rooter().current());
	//	aaa.remove(aaa.fronter());
	//	aaa.remove(aaa.fronter());
	//	print_list(aaa.rooter().as_chunk_list());
	//	printf(", curr:%p",aaa.rooter().current());
	//	//print_list_reverse(aaa.rooter().as_chunk_list());
	//	aaa.remove(aaa.backer());
	//	print_list(aaa.rooter().as_chunk_list());
	//	printf(", curr:%p",aaa.rooter().current());
	//	//print_list_reverse(aaa.rooter().as_chunk_list());


	//	////size_t c = aaa.size();
	//	////aaa.size_reserved();
	//	////aaa.push_front(3);
	//	////aaa.push_back(4);
	//	////aaa.insert_before(zvector::dot_type(),4);
	//}


	{
		ox::chunk2_list<char>::type aaa;
		auto r = aaa.push_front(ox::sizable_space_tt<char>(12));
		r = aaa.push_front(3);
		print_list(aaa);
		ox::chunk3_list<char>::type ddd;
		typedef ox::chunk3_list<char>::type::dot_type dot_type;
		ddd.push_front(ox::sizable_space_tt<char>(12));
		ddd.push_back(12);
		ddd.push_back(15);
		ddd.insert_after(dot_type(ddd.fronter().next()),11);
		ddd.insert_before(ddd.backer(),44);
		print_list(ddd);
		ddd.reverse();
		print_list(ddd);
		ox::slist_tt<int>::type bbb;
		bbb.push_front(5);
		bbb.push_front(7);
		bbb.push_front(3);
		printf("\n");
		print_list(bbb);
		ox::list_tt<int>::type ccc;
		ccc.copyfrom(bbb.begin(),bbb.end());
		ccc.push_front(2);
		ccc.push_front(3);
		ccc.push_back(4);
		printf("\n");
		print_list(ccc);
		ccc.reverse();
		printf("\n");
		print_list(ccc);
	}
	//{
	//	//ox::stack_tt<int>::type sss;
	//	//sss.push(1);
	//	//ox::string str;
	//	//str.push_back('a');
	//	//char const* st = str.data();
	//	//char const* st2 = str.c_str();
	//	ox::wstring wstr;
	//	wstr.push_back(L'b');
	//	wchar_t const* ws = wstr.c_str();
	//}
	//{
	//	ox::static_keyvalue_sorted_podliner_off_tt<int,int,5>::type liner;
	//	typedef ox::static_keyvalue_sorted_podliner_off_tt<int,int,5>::type::value_type value_type;
	//	liner.insert(3,4);
	//	value_type const* aa = liner.find(3);
	//	size_t s = liner.size();
	//	liner.insert(2,1);
	//	for (auto i=liner.begin();i<liner.end();++i)
	//		printf ("(%d,%d) ",i->first,i->second);
	//}
	//{
	//	ox::static_podliner_off_tt<int,5>::type liner;
	//	liner.push_back(3);
	//	size_t s = liner.size();
	//	liner.resize(4,1);
	//	//for (int* i=liner.begin();i<liner.end();++i)
	//	//	printf ("%d ",*i);
	//	liner.insert(2,8);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//	liner.insert(2,7);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//	liner.internal_remove_at(3);
	//	//l2 = liner.split(2);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//}
	//{
	//	ox::podliner_tt<int>::type liner;
	//	liner.push_back(3);
	//	size_t s = liner.size();
	//	liner.resize(4,1);
	//	liner.insert(2,8);
	//	liner.insert(2,7);
	//	liner.internal_remove_at(3);
	//	//l2 = liner.split(2);
	//	for (int* i=liner.begin();i<liner.end();++i)
	//		printf ("%d ",*i);
	//}
	//{
	//	ox::veclist<int> intvect;
	//	intvect.push_back(1);
	//	intvect.push_back(2);
	//	intvect.push_back(3);
	//	intvect.push_back(4);
	//	size_t size = intvect.size();
	//	ox::veclist<int>::const_iterator i = intvect.begin();
	//	ox::veclist<int>::const_iterator e = intvect.end();

	//	bool b = e == intvect.of(5);
	//	int index = 0;
	//	for (;i!=e;++i)
	//	{
	//		int v = *i;
	//		printf("%u %u ",v,*intvect.of(index));
	//		++index;
	//	}
	//}
	//{
	//	typedef ox::string_ptrs_tt<wchar_t> stringptr;
	//	stringptr str;
	//	str.push_back(L"a");
	//	str.push_back(L"dt1");
	//	for (
	//		stringptr::iterator i=str.begin();
	//		i!=str.end();
	//		++i)
	//	{
	//		wprintf (L"%c",*i);
	//	}
	//	size_t size = str.size();
	//	size = sizeof(str);
	//	bool b = str==L"adt1";
	//	wchar_t c = str[4];
	//}


	getch();
	return 0;
}

