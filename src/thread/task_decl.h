

#pragma once


#include "../ox/nsab.h"



___namespace2_begin(ox,mos)


template <typename r>
struct run_tt
{
	virtual r run() = 0;
};
struct destruct_interface
{
	virtual void destruct() = 0;
};
template <typename r>
struct object_run_tt : run_tt<r>, destruct_interface
{};

//struct task_free
//{
//	virtual void free() = 0;
//};

template <typename r>
struct task_tt : run_tt<r>
{
	typedef run_tt<r> run_t;
	virtual void destroy() = 0;
	virtual ~task_tt() {}
};

typedef task_tt<void> thread_task_t;
typedef task_tt<bool> timer_task_t;

___namespace2_end()