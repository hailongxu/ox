
#include <string>
#include <map>
#include "../cxx/delegate.h"
#include "../pkg/string_simple_kit.h"


#pragma once


struct interaction_t
{
	typedef interaction_t self;
	typedef delegate<void(char const* argv[],size_t argc,void*,size_t* control)> action_t;
	struct action2_t
	{
		action_t action;
		void* bind;
		std::string desc;
	};
	typedef std::map<std::string,action2_t> actions_t;
	actions_t _m_actions;
	typedef ox::str::astring_kit::split3<ox::character::__case_sensitive> splitter;
	splitter _m_splitter;
	std::vector<char const*> _m_argv;
	char _m_input[64];
	
	void add(char const* cmd,action_t const& action,void* bind,char const* desc)
	{
		action2_t item = {action,bind,desc};
		_m_actions[cmd] = item;
	}
	void list(char const* argv[],size_t argc,void*,size_t*)
	{
		printf("\n==========\n");
		for (actions_t::iterator i=_m_actions.begin();i!=_m_actions.end();++i)\
		{
			printf ("%s: %s\n",i->first.c_str(),i->second.desc.c_str());
		}
	}
	void start()
	{
		char buffer_last[64] = {0};
		add("l",action_t(this,&self::list),0,"list the commands");
		while(1)
		{
			char buffer_back[64] = {0};
			char buffer[64] = {0};
			gets(buffer);
			strcpy(buffer_back,buffer);
			if (strlen(buffer)==0)
			{
				strcpy(buffer_back,buffer_last);
				strcpy(buffer,buffer_last);
				printf ("%s",buffer);
			}
			split_commander_param(buffer);
			actions_t::iterator i = _m_actions.find(buffer);
			if (i==_m_actions.end())
				continue;
			strcpy(buffer_last,buffer_back);
			action2_t& action = i->second;
			size_t control = 0;
			if (_m_argv.empty()) _m_argv.push_back("");
			action.action(&_m_argv[0],_m_argv.size(),action.bind,&control);
			if (control!=0) break;
		}
	}
	void split_commander_param(char* buffer)
	{
		static char const* seps[1] = {" "};
		_m_splitter.find.set_seps(seps);
		_m_splitter.handle.assign(this,&self::on_split_commander_param);
		char* b = buffer;
		char* e = ox::str::astring_kit::end(buffer);
		_m_argv.clear();
		_m_splitter(b,e);
	}
	bool on_split_commander_param(char const*begin,char const*finded,char const*end)
	{
		if (begin==finded) return true;
		*(char*)finded = 0;
		_m_argv.push_back(begin);
		return true;
	}
};
