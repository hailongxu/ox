

#include "../utl/data_t.h"
#include "../cxx/delegate.h"
#include "../mos/task_decl.h"


#pragma once


___namespace2_begin(ox,comm)


struct communication_public
{
	typedef ox::utl::cdata_t cdata_t;
	typedef delegate<void(void const*,cdata_t)> send_back_d;
	typedef delegate<void(ox::mos::task_tt<void>*,size_t)> post_task_d;
	typedef delegate<void(char const*)> logf_d;
};


___namespace2_end()