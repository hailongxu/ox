// watch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../src/ox/io/watch.h"
#include "../src/ox/io/filekit.h"


namespace watch_test
{
	ox::io::watch watch;
	void on_changed(char const* path,size_t off,size_t length)
	{
		ox::io::file<char> file(path,"rb");
		size_t len = file.length();
		if (len==0) return;
		if (off+1>len) off=len-1;
		if (off+length>len) length = len-off;
		std::string buff;
		buff.resize(length);
		file.seek(ox::io::file_helper::__seek_begin,off);
		file.read(buff.data(),1,length);
		static int i = 0;
		//printf ("%d [%s] changed\n",i++,path);
		printf ("%s",buff.c_str());
	}
	void test()
	{
		watch.add("d:\\abc.txt",ox::io::watch::file_changed_d(on_changed),500);
		watch.start();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	watch_test::test();
	abc::win_queue_thread th;
	th.start_here();
	return 0;
}

