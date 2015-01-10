// vecbuff_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/utl/vecbuff.h"
//#include "../../src/kernel/ipc_helper_internal.h"


namespace vecbuff_test
{
	typedef ox::utl::cdata_t cdata_t;
	typedef ox::utl::vecbuff_t vecbuff_t;
	typedef ox::utl::vecbuff_helper vecbuff_helper;
	void test()
	{
		std::string vbuff("--------");
		vecbuff_helper helper(vbuff);
		helper.init();
		helper.add("12345",5);
		helper.add("abcdef",6);
		char const* data = vbuff.data();
		vecbuff_t& vecbuff = helper.as_vecbuff();
		vecbuff.convert_to_absolute();



		cdata_t datas[2] = {cdata_t("123",3),cdata_t("abc",3)};
		char buffer[128];
		vecbuff_t* vecbuff2 = vecbuff_t::unsafe_as_vecbuff(buffer);
		vecbuff2->init_rel();
		vecbuff2->unsafe_add_index_and_data(datas,2);
		vecbuff2->unsafe_add_index_and_data(datas[0]);
		vecbuff2->convert_to_absolute();
	}
}

namespace kvbuffer_test
{
	typedef ox::utl::vecbuff_t vecbuff_t;
	typedef ox::utl::kvbuff_helper<std::string> kvbuff_t;
	void test()
	{
		std::string buf;
		kvbuff_t kv(buf);
		kv.init();
		kv.add_str0("1","11");
		kv.add_str0("2","22");
		vecbuff_t& vb = kv.as_vecbuff();
		std::string str;
		ox::utl::to_kvstring(str,vb);
	}
}

//namespace ipcbuff_test
//{
//	typedef ox::ipc_protocol::ipc_protocol_buff_helper ipc_protocol_buff_helper;
//	typedef ox::communication_define::cdata_t cdata_t;
//	void test()
//	{
//		std::string buf;
//		cdata_t source("s",1);
//		cdata_t data("ABCDEFG",7);
//		std::string dest;
//		ox::utl::vecbuff_t const& d = ox::utl::vecbuff_helper(dest).addstr("d").as_vecbuff();
//		ipc_protocol_buff_helper(buf).add(source,cdata_t(),d.data(),1,data);
//	}
//}


#include "../../src/pkg/string_simple_kit.h"
namespace get_line_test
{
	static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
	static ox::character::is_case_sensitive_c const incasive = ox::character::__case_insensitive;
	typedef ox::string::astring_kit strkit;
	void test()
	{
		char const* str = "1,2,,";
		char const* sep [] = {","};
		strkit::get_line<casive> get(str,strkit::end(str));
		get.find.set_seps(sep);
		char const* line = get(0);
		line = get(0);
		line = get(0);
		line = get(0);
		//strkit::foreach_line<casive> foreach();
	}
};

#include "../../src/utl/seplist.h"
namespace seplist_test
{
	typedef ox::utl::cdata_t cdata_t;
	typedef ox::utl::seplist<char> seplist;


	void test()
	{
		std::string str;
		seplist list (str);
		list.add("1");
		list.add("2");
		list.add("3");
		list.add("45");
		cdata_t d1 = list.next(cdata_t());
		cdata_t d2 = list.next(d1);
		list.erase("4");
		list.erase("2");
		list.erase("1");
		list.erase("3");
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	do {
		kvbuffer_test::test(); break;
		seplist_test::test(); break;
		vecbuff_test::test(); break;
		get_line_test::test(); break;
		//ipcbuff_test::test(); break;
	}
	while(0);
	return 0;
}

