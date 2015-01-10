

#pragma once


namespace ox
{

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

}