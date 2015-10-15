

#include <set>
#include <map>
#include "../utl/data_t.h"
#include "../utl/seplist.h"
#include "../utl/data_helper.h"
#include "../mos/scope_raii.h"
#include "../mos/atomic_number.h"
#include "../comm/communication_define.h"
#include "../comm/communication_const.h"
#include "../comm/log_proxy.h"
#include "share_file.h"
#include "ipcnode_define.h"


#pragma once


___namespace2_begin(ox,ipc)


struct channelid_list
{
	typedef ox::str::astring_kit strkit;
	typedef ipcnode_define::channelid channelid;

	typedef std::set<size_t> channel_set;
	channel_set _m_channels;

	static size_t seq()
	{
		static ox::mos::atomic_long _static_seq(0);
		return ++_static_seq;
	}
	static void make_index(channelid& chid)
	{
		char name[sizeof(chid.name)];
		memmove(name,chid.name,sizeof(chid.name));
		sprintf (chid.name,"chid_%u/%s",chid.seq,name);
	}
	static void keep_top(std::string& idlist)
	{
		size_t pos = idlist.find(',');
		if (pos==std::string::npos) return;
		idlist.erase(pos);
	}
	bool find(size_t seq)
	{
		typedef channel_set::iterator I;
		I i = _m_channels.find(seq);
		return i!=_m_channels.end();
	}
	void update(size_t seq)
	{
		_m_channels.insert(seq);
	}
	void erase(size_t seq)
	{
		_m_channels.erase(seq);
	}
	void clear()
	{
		_m_channels.clear();
	}
};

struct ipc_id_list
{
	typedef ox::utl::cdata_t cdata_t;

	static std::string ipc_pipe_name(cdata_t const& ipc_node_name)
	{
		typedef ox::str::astring_kit strkit;
		size_t pid = GetCurrentProcessId();
		char bufpid [32] = {0};
		strkit::to_string(pid,bufpid,10);
		std::string pipe_name(ipc_node_name.begin,ipc_node_name.size);
		pipe_name.push_back('_');
		pipe_name.append(bufpid);
		return pipe_name;
	}
	static std::string ipc_mutex_name(cdata_t const& ipc_node_name)
	{
		size_t pid = GetCurrentProcessId();
		return ipc_mutex_name(ipc_node_name,pid);
	}
	static std::string ipc_mutex_name(cdata_t const& ipc_node_name,size_t pid)
	{
		typedef ox::str::astring_kit strkit;
		char bufpid [32] = {0};
		strkit::to_string(pid,bufpid,10);
		std::string mutex_name("Global\\ipcmutex_");
		mutex_name.append(ipc_node_name.begin,ipc_node_name.size);
		mutex_name.append("_");
		mutex_name.append(bufpid);
		return mutex_name;
	}
	static bool is_ipc_node_existing(char const* ipc_node_name,size_t pid)
	{
		ox::mos::win_mutex_slave mutex_slave(ipc_node_name);
		return !mutex_slave.is_invalid();
	}

	ipc_id_list()
	{}
	ipc_id_list(cdata_t const& id_name)
		: _m_name(id_name.begin,id_name.size)
	{}
	void init(cdata_t const& id_name)
	{
		_m_name.assign(id_name.begin,id_name.size);
	}
	/// unique
	size_t add(char const* id)
	{
		size_t errorid = 0;
		do {
			std::string content;
			if (!_m_file.is_opened()) errorid = _m_file.open_or_create(_m_name.c_str(),128);
			if (errorid!=0) break;
			errorid = _m_file.read_all(content);
			log_proxy::instance().printf("ipc.file.add before [%s]:[%s]\r\n",_m_name.c_str(),content.c_str());
			if (errorid!=0) break;
			typedef ox::utl::seplist<char> seplist;
			seplist list(content);
			list.add_unique(id);
			log_proxy::instance().printf("ipc.file.add after [%s]:[%s]\r\n",_m_name.c_str(),content.c_str());
			errorid = _m_file.write_all(content.data(),content.size());
			if (errorid!=0) break;
			_m_content = content;
		} while (0);
		return errorid;
	}
	size_t erase(char const* id)
	{
		return erase<int>(cdata_t(id,strlen(id)),0);
	}
	template <typename cmp_tn>
	size_t erase(cdata_t const& id,cmp_tn cmp)
	{
		log_proxy::instance().printf("ipc.file.erase called\r\n");
		size_t errorid = 0;
		do {
			std::string content;
			if (!_m_file.is_opened()) errorid = _m_file.open(_m_name.c_str());
			if (errorid!=0) break;
			errorid = _m_file.read_all(content);
			if (errorid!=0) break;
			log_proxy::instance().printf("ipc.file.erase before [%s]:[%s]\r\n",_m_name.c_str(),content.c_str());
			typedef ox::utl::seplist<char> seplist;
			seplist list(content);
			list.erase<cmp_tn>(id,cmp);
			log_proxy::instance().printf("ipc.file.erase after [%s]:[%s]\r\n",_m_name.c_str(),content.c_str());
			errorid = _m_file.write_all(content.data(),content.size());
			//update_group_list(group_name,node_name);
		} while (0);
		return errorid;
	}
	size_t refresh()
	{
		size_t errorid = 0;
		while (1)
		{
			if (!_m_file.is_opened()) errorid = _m_file.open(_m_name.c_str());
			if (errorid!=0)
			{
				log_proxy::instance().printf("ipc.file.read [%s] error[%u]\r\n",_m_name.c_str(),errorid);
				break;
			}
			errorid = _m_file.read_all(_m_content);
			log_proxy::instance().printf("ipc.file.read ok, and content[%s]\r\n",_m_name.c_str(),_m_content.c_str());
			break;
		}
		return errorid;
	}
	shared_memory _m_file;
	std::string _m_content;
	std::string _m_name;
};

struct ipc_all_list
{
	typedef ox::utl::cdata_t cdata_t;
	typedef ox::str::astring_kit strkit;
	typedef std::map<std::string,ipc_id_list> group_id_list;
	struct cmp_pid_only
	{
		bool operator()(cdata_t const& a,cdata_t const& b)
		{
			cdata_t ida = get_pid(a);
			cdata_t idb = get_pid(b);
			if (ida.size!=a.size) ida.assign(a.begin,a.size-ida.size-1);
			if (idb.size!=b.size) idb.assign(b.begin,b.size-idb.size-1);
			if (ida.size!=idb.size) return false;
			return 0==strkit::strncmp<ox::character::__case_sensitive>(ida.begin,idb.begin,ida.size);
		}
	};

	void init(char const* node_name)
	{
		_m_this._m_name = node_name;
		add_pid();
	}

	void get_self_nidpid(char* nidpid) const
	{
		get_current_nidpid(nidpid,ox::utl::to_data(_m_this._m_name));
	}
	void get_pid_list(std::string& list,cdata_t const& node_name)
	{
		ipc_id_list get_pid_list;
		get_pid_list._m_name.assign(node_name.begin,node_name.size);
		get_pid_list.refresh();
		list = get_pid_list._m_content;
	}
	void get_pid_list(std::string& list)
	{
		_m_this.refresh();
		list = _m_this._m_content;
	}
	void get_nodeid_list(std::string& list,cdata_t const& group_name)
	{
		typedef group_id_list::iterator I;
		std::string key (group_name.begin,group_name.size);
		I i = _m_group_list.find(key);
		if (i==_m_group_list.end())
			return;
		ipc_id_list& gnamelist = i->second;
		gnamelist.refresh();
		list = gnamelist._m_content;
	}
	void add_pid()
	{
		char bufpid [32] = {0};
		get_current_pid_sid(bufpid);
		log_proxy::instance().printf("ipc add self [%s]\r\n",bufpid);
		_m_this.add(bufpid);
	}
	void remove_pid()
	{
		char bufpid [32] = {0};
		get_current_pid_sid(bufpid);
		log_proxy::instance().printf("ipc remove pid[%s]\r\n",bufpid);
		_m_this.erase(bufpid);
	}
	void remove_pid(cdata_t const& pid)
	{
		_m_this.erase<cmp_pid_only>(pid,cmp_pid_only());
	}
	void add_into_group(char const* group_name)
	{
		typedef group_id_list::iterator I;
		I i = _m_group_list.find(group_name);
		if (i==_m_group_list.end())
		{
			ipc_id_list& list = (_m_group_list[group_name] = ipc_id_list());
			list._m_name = group_name;
			list.add(_m_this._m_name.c_str());
			return;
		}
		ipc_id_list& list = i->second;
		list.add(_m_this._m_name.c_str());
	}
	void remove_from_group(char const* group_name)
	{
		typedef group_id_list::iterator I;
		I i = _m_group_list.find(group_name);
		if (i==_m_group_list.end())
			return;
		ipc_id_list& list = i->second;
		list.erase(_m_this._m_name.c_str());
	}
	void remove_from_all_groups()
	{
		log_proxy::instance().printf("ipc [%s] remove_from_all_groups\r\n",_m_this._m_name.c_str());
		remove_pid();
		typedef group_id_list::iterator I;
		for (I i=_m_group_list.begin();i!=_m_group_list.end();++i)
		{
			ipc_id_list& list = i->second;
			list.erase(_m_this._m_name.c_str());
		}
	}
	//void update_group_list(std::string const& gid,std::string const& content)
	//{
	//	typedef group_id_list::iterator I;
	//	if (content.empty()) _m_group_list.erase(gid);
	//	_m_group_list[gid] = content;
	//}
	ipc_id_list _m_this;
	group_id_list _m_group_list;
	/// bufpidsid's size should be larger than (11+1)*2+1
	static void get_current_pid_sid(char* bufpidsid)
	{
		assert (bufpidsid);
		DWORD sid = 0;
		DWORD pid = GetCurrentProcessId();
		BOOL b = ProcessIdToSessionId(pid,&sid);
		assert (b);
		//char bufpid [32] = {0};
		strkit::to_string(pid,bufpidsid,10);
		char bufsid [16] = {0};
		strkit::to_string(sid,bufsid,10);
		char* p = strkit::append(bufpidsid,"_");
		p = strkit::append(p,bufsid);
	}
	static cdata_t get_lastid(cdata_t const& id,char const& sep='_')
	{
		char const* p = strkit::strrch<true>(id.end()-1,id.size,sep);
		p ? p++ : p=id.begin;
		return cdata_t(p,id.end()-p);
	}
	static cdata_t get_pid(cdata_t const& id)
	{
		return get_lastid(id,'_');
	}
	static cdata_t get_nidpid(cdata_t const& id)
	{
		return get_lastid(id,'/');
	}
	/// nidpid.size > 64
	static void get_current_nidpid(char* nidpid,cdata_t const& id)
	{
		size_t pid = GetCurrentProcessId();
		char bufpid [16] = {0};
		strkit::to_string(pid,bufpid,10);
		char* p = strkit::append(nidpid,id.begin,id.size);
		p = strkit::append(p,"_");
		p = strkit::append(p,bufpid);
	}
};

struct safe_ipc_all_list
{
	typedef ox::utl::cdata_t cdata_t;

	safe_ipc_all_list()
		: _m_mutex("Global\\bds_ipc_all_list_mutex")
	{
		DWORD pid = GetCurrentProcessId();
		DWORD sid = 0;
		ProcessIdToSessionId(pid,&sid);
		_m_sessionid = sid;
	}
	
	void init(char const* node_name)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.init(node_name);
	}
	/*static*/ void get_pid_list(std::string& list,cdata_t const& node_name)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.get_pid_list(list,node_name);
	}

	void get_pid_list(std::string& list)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.get_pid_list(list);
	}
	void get_nodeid_list(std::string& list,cdata_t const& group_name)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.get_nodeid_list(list,group_name);
	}
	void add_pid()
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.add_pid();
	}
	void remove_pid()
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.remove_pid();
	}
	void remove_pid(cdata_t const& pid)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.remove_pid(pid);
	}
	void remove_pid(size_t pid)
	{
		typedef ox::str::astring_kit strkit;
		char buffpid[32] = {0};
		strkit::to_string(pid,buffpid,10);
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.remove_pid(cdata_t(buffpid,strkit::length(buffpid)));
	}
	void add_into_group(char const* group_name)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.add_into_group(group_name);
	}
	void remove_from_group(char const* group_name)
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.remove_from_group(group_name);
	}
	void remove_from_all_groups()
	{
		ox::mos::scope_mutex lock(_m_mutex);
		_m_ipc_all_list.remove_from_all_groups();
	}
	size_t _m_sessionid;
	ipc_all_list _m_ipc_all_list;
	ox::mos::win_mutex _m_mutex;
};


struct ipc_list_make_helper
{
	typedef ipc_list_make_helper self;
	typedef ox::utl::cdata_t cdata_t;
	struct add_binded_param
	{
		add_binded_param(): is_session_cross_okay(false), count(0) {}
		add_binded_param(cdata_t _data,bool is_cross,size_t cnt=0)
			: data(_data),is_session_cross_okay(is_cross), count(cnt) {}
		cdata_t data;
		bool is_session_cross_okay;
		size_t count;
	};
	typedef ox::utl::seplist_transform<ox::utl::seplist<char>,add_binded_param&> transform_t;
	typedef transform_t::transform_handle handle_t;

	ipc_list_make_helper(safe_ipc_all_list& id_list/*,cdata_t const& node_name*/)
		: _m_id_list(id_list)
		//, _m_node_name(node_name)
	{}

	static bool is_node_name_valid(cdata_t const& id)
	{
		bool is_valid =
			id.size>=2 &&
			id.begin && *id.begin && *(id.begin+1) &&
			*id.begin == 't' &&
			(*(id.begin+1)=='1' || *(id.begin+1)=='2');
		return is_valid;
	}

	/// the session factor
	static bool get_pid_sid(cdata_t& pid,cdata_t& sid,char const* b,char const* e)
	{
		assert (b && e && b<=e);
		char const* p = ox::str::astring_kit::strch<true>(b,e,'_');
		assert (p);
		if (p) pid.assign(b,p-b), sid.assign(p+1,e-(p+1));
		else pid.assign(b,e-b), sid.assign("0",1);
		return true;
	}
	bool is_add_enabled(size_t src_sid,cdata_t const& dest_sid,bool is_session_cross_okay)
	{
		bool is_zero = dest_sid.size==1 && *dest_sid.begin == '0';
		if (is_zero) return true;
		if (src_sid==0) return true;
		if (is_session_cross_okay) return true;
		size_t dst_sid = ox::str::astring_kit::to_ulong(dest_sid.begin,10);
		return src_sid==dst_sid;
	}
	bool add_pre(std::string& out,add_binded_param& bind,char const* b,char const* e)
	{
		cdata_t pid,sid;
		get_pid_sid(pid,sid,b,e);
		bool is_enabled = is_add_enabled(_m_id_list._m_sessionid,sid,bind.is_session_cross_okay);
		if (!is_enabled) return true;
		bind.count ++;
		out.append(bind.data.begin,bind.data.size).append(1,_m_char_sep).append(pid.begin,pid.size);
		return true;
	}
	bool add_post(std::string& out,add_binded_param& bind, char const* b,char const* e)
	{
		cdata_t node_name (b,e-b);
		std::string pidlist;
		_m_id_list.get_pid_list(pidlist,node_name);
		ox::utl::seplist<char> seplist(pidlist);
		add_binded_param param (node_name,bind.is_session_cross_okay);
		transform_t transform(seplist);
		transform(out,handle_t(this,&self::add_pre),param);
		bind.count += param.count;
		return true;
	}
	communication_const::error_selfdefine_enum get_id_list(std::string& list,size_t& count,cdata_t const& id,bool is_session_cross_okay=false,bool is_self_included=false)
	{
		communication_const::error_selfdefine_enum ret = communication_const::__error_null;
		if (!(id.size>2 && id.begin[0]=='t'))
		{
			count = 0;
			ret = communication_const::__error_ipcnode_format_error;
			assert (false && "invalid id name");
			return ret;
		}
		
		if (id.begin[1]=='1') /// node id
		{
			cdata_t pre = id;
			std::string pids;
			_m_id_list.get_pid_list(pids,id);
			ox::utl::seplist<char> sepl(pids);
			add_binded_param param (pre,is_session_cross_okay);
			transform_t transform(sepl);
			transform(list,handle_t(this,&self::add_pre),param);
			count = param.count;
			if (!is_self_included && count && remove_self_from_nidpidlist(list)) count--;
			if (count==0) ret=communication_const::__error_ipcnode_list_is_null;
		}
		else if (id.begin[1]=='2') /// group id
		{
			std::string nodeids;
			_m_id_list.get_nodeid_list(nodeids,id);
			ox::utl::seplist<char> idlist(nodeids);
			add_binded_param param (cdata_t(),is_session_cross_okay);
			transform_t transform(idlist);
			transform(list,handle_t(this,&self::add_post),param);
			count = param.count;
			if (!is_self_included && remove_self_from_nidpidlist(list)) count--;
			if (count==0) ret=communication_const::__error_ipcnode_list_is_null;
		}
		else
		{
			count = 0;
			ret = communication_const::__error_ipcnode_format_error;
			assert (false && "invalid id name");
			return ret;
		}
		return ret;
	}
	void remove_nidpid(cdata_t const& nidpid)
	{
		cdata_t pid = ipc_all_list::get_pid(nidpid);
		assert(pid.size<nidpid.size && pid.begin>nidpid.begin);
		cdata_t nid(nidpid.begin,(pid.begin-1)-nidpid.begin);
		ipc_id_list pidlist(nid);
		ox::mos::scope_mutex lock(_m_id_list._m_mutex);
		typedef ipc_all_list::cmp_pid_only cmp_pid_only;
		pidlist.erase<cmp_pid_only>(pid,cmp_pid_only());
	}
	bool remove_self_from_nidpidlist(std::string& list)
	{
		char self_nidpid [128] = {0};
		_m_id_list._m_ipc_all_list.get_self_nidpid(self_nidpid);
		size_t off = list.find(self_nidpid);
		if (off==list.npos) return false;
		size_t len = ox::str::astring_kit::length(self_nidpid);
		if (list[off+len]==',') len++;
		else if (off!=0 && list[off-1]==',') --off,++len;
		list.erase(off,len);
		return true;
	}

	safe_ipc_all_list& _m_id_list;
	static char const _m_char_sep = '_';
}; /// end of class list_make_helper


___namespace2_end()