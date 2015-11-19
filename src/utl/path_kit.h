/*
	author: 徐海龙 xuhailong
	date: 2006-6-20
	msn: hailongxu@msn.com
	Email: hailongxu@msn.com
	copyright：本代码属开源代码，版权归属作者所有
 */
//
//////////////////////////////////////////////////////////////////////////
// author : xuhailong
// desc : the frequent function & base common function in the socket programming
// history : 2006-11-22 17:55



#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../ox/nsab.h"
#include "../pkg/string_simple_kit.h"
#include "data_t.h"


#pragma once



___namespace2_begin(ox,utl)


template<typename character_tn>
struct localpath_kit
{
	typedef character_tn character;
	typedef ox::str::string_simple_kit<character> strkit;
	typedef typename strkit::std_string std_string;
	typedef typename strkit::stringz stringz;
	typedef typename strkit::stringi stringi;

	//localpath_kit(){path.refp_begin()=0;}
	//localpath_kit(const character * path,bool is_parse=true)
	//{
	//	this->path.refp_begin()=const_cast<character *>(path);
	//	if (is_parse) parse(path);
	//}

	static void add_pre(std::vector<std_string>& paths,character const* pre,character slash=strkit::backslash_character)
	{
		typedef std::vector<std_string>::iterator I;
		for (I i=paths.begin(); i!=paths.end(); ++i)
			add_pre(*i,pre,slash);
	}

	static bool is_abs(std_string const& path)
	{
		if (path.empty()) return false;
		return is_abs(path.c_str());
	}
	static bool is_abs(character const* path)
	{
		if (!path || !*path)
			return false;
		character const chars[3] = {strkit::slash_character,strkit::backslash_character,0};
		if (strkit::is_in_chset<true>(*path,chars))
			return true;
		return *(path+1)==strkit::colon_character;
	}

	static bool get_ext(std_string const& path, character const*& ext)
	{
		if (path.empty()) return false;
		character const* path_begin = path.c_str();
		character const* path_end = path_begin+path.length();
		return get_ext(path_begin,path_end,ext);
	}
	static bool get_ext(character const* path_begin, character const*& ext)
	{
		assert (path_begin);
		character const* path_end = path_begin+strkit::length(path_begin);
		return get_ext(path_begin,path_end,ext);
	}
	/// []
	static bool get_ext(character const* path_begin,character const* path_end, character const*& ext)
	{
		character const chars[] =
		{
			strkit::slash_character,
			strkit::backslash_character,
			strkit::dot_character,
			0
		};
		if (!path_begin || !*path_begin)
			return false;
		character const* end2 = strkit::find_last_in<true>(path_begin,path_end,chars);
		if (end2<path_begin)
			return false;
		if (*end2==strkit::dot_character)
		{
			ext = end2;
			return true;
		}
		return false;
	}

	template <ox::character::is_case_sensitive_c sensitive>
	static bool is_ext_equal_to(character const* path,character const* ext)
	{
		character const* ext2 = 0;
		if (!get_ext(path,ext2)) return ext==0||*ext==0;
		return 0==strkit::strcmp<sensitive>(ext,ext2+1);
	}
	static bool is_ext_equal_to_uncase(character const* path,character const* ext)
	{
		return is_ext_equal_to<ox::character::__case_insensitive>(path,ext);
	}

	template <ox::character::is_case_sensitive_c sensitive>
	static bool is_ext_equal_to(std_string const& path,character const* ext)
	{
		character const* ext2 = 0;
		if (!get_ext(path,ext2)) return ext==0||*ext==0;
		return strkit::strcmp<sensitive>(ext,ext2);
	}
	static bool is_ext_equal_to_uncase(std_string const& path,character const* ext)
	{
		return is_ext_equal_to<ox::character::__case_insensitive>(path,ext);
	}

	static bool replace_ext(wchar_t* path,wchar_t const* ext)
	{
		assert(path);
		size_t len = wcslen(path);
		if (len==0)
			return false;

		wchar_t* pend = path+len-1;
		wchar_t* p = pend;
		for (;p>=path;--p)
			if (*p==L'\\' || *p==L'/' || *p==L'.')
				break;

		if (*p==L'.')
			wcscpy(p+1,ext);
		else
			++pend,*pend=L'.',wcscpy(pend+1,ext);

		return true;
	}

	static void replace_last(std_string& path,character const* last_new)
	{
		remove_last(path);
		add_rpath(path,last_new,'/');
	}
	static bool replace_ext(std_string& path, character const* ext)
	{
		character const* pext = 0;
		bool b = get_ext(path,pext);
		if (!b)
		{
			path.push_back(strkit::dot_character);
			path.append(ext);
			return false;
		}
		path.erase(pext-&path[0]+1);
		path.append(ext);
		return true;
	}

	static bool erase_ext(std_string& path)
	{
		character const* pext = 0;
		bool b = get_ext(path,pext);
		if (!b) return false;
		path.erase(pext-&path[0]);
		return true;
	}

	static bool keep_ext(std_string& path,bool keepdot=true)
	{
		character const* pext = 0;
		bool b = get_ext(path,pext);
		if (!b)
		{
			path.clear();
			return false;
		}
		size_t len = pext-&path[0];
		if (!keepdot) ++len;
		path.erase(0,len);
		return true;
	}


	static bool get_dir_and_name_of_path(character* path, character*& filename)
	{
		assert(path);
		size_t len = strkit::length(path);
		if (len==0)
			return false;

		character* pend = path+len-1;
		character* p = pend;
		for (;p>=path;--p)
			if (*p==L'\\' || *p==L'/')
				break;
		character* file = p+1;
		::wmemmove(file+2,file,pend-file+2);
		filename = file+2;
		*file=0;
		return true;
	}

	static character const* get_last(std_string const& path)
	{
		if (path.empty())
			return 0;
		character const* begin = path.c_str();
		character const* end = &path[path.length()-1];
		return get_last(begin,end);
	}

	static character const* get_last(character const* begin)
	{
		if (!begin) return begin;
		if (!*begin) return begin;
		return get_last(begin,begin+strkit::length(begin)-1);
	}

	static character const* get_last(character const* begin, character const* end)
	{
		if (end<begin) return 0;
		character const chars[3] = {strkit::slash_character,strkit::backslash_character,0};
		if (strkit::is_in_chset<true>(*(end-1),chars))
			--end;
		if (end<=begin) return begin;
		character const* end2 = strkit::find_last_in<true>(begin,end,chars);
		end2<begin ? end2=begin : ++end2;
		return end2;
	}

	static bool is_end_with_drive(std_string const& str)
	{
		return is_end_with_drive(str.c_str(),str.c_str()+str.length());
	}
	static bool is_end_with_drive(character const* path)
	{
		return is_end_with_drive(path,path+strkit::length(path));
	}
	static bool is_end_with_drive(character const* begin, character const* end)
	{
		if (end-begin<2) return false;
		return *(end-1)==strkit::colon_character;
	}
	static bool is_drive_root(std_string const& str)
	{
		return is_drive_root(str.c_str(),str.c_str()+str.length());
	}
	static bool is_drive_root(character const* path)
	{
		return is_drive_root(path,path+strkit::length(path));
	}
	static bool is_drive_root(character const* begin, character const* end)
	{
		if (end-begin<2) return false;
		character const chars[3] = {strkit::slash_character,strkit::backslash_character,0};
		character last = *(end-1);
		if (last==strkit::colon_character)
			return true;
		character prelast = *(end-2);
		if (prelast!=strkit::colon_character)
			return false;
		return strkit::is_in_chset<true>(last,chars);
	}

	static std_string& add_pre(std_string& path,std_string const& pre,character const& slash)
	{
		return add_pre(path,pre.c_str(),slash);
	}
	static std_string& add_pre(std_string& path,character const* rpath,character const& slash)
	{
		if (!rpath) return path;
		if (!*rpath) return path;

		character const chars[3] = {strkit::slash_character,strkit::backslash_character,0};
		size_t size = strkit::length(rpath);
		bool b1 = strkit::is_in_chset<true>(*(rpath+size-1),chars);
		character c = path.empty()?0:path[0];
		bool b2 = strkit::is_in_chset<true>(c,chars);
		if (b1 && b2) path.insert(0,rpath,size-1);
		else if (!b1 && !b2) path.insert(path.begin(),slash), path.insert(0,rpath);
		else path.insert(0,rpath);
		return path;
	}

	static std_string& add_rpath(std_string& path,std_string const& rpath,character const& slash)
	{
		return add_rpath(path,rpath.c_str(),slash);
	}

	static std_string& add_rpath(std_string& path,character const* rpath,character const& slash)
	{
		if (!rpath) return path;
		if (!*rpath) return path;

		character const chars[3] = {strkit::slash_character,strkit::backslash_character,0};
		bool b2 = strkit::is_in_chset<true>(*rpath,chars);
		character c = path.empty()?0:path[path.size()-1];
		bool b1 = strkit::is_in_chset<true>(c,chars); (void)(b1);
		b2 ? remove_slash(path) : add_slash(path,slash);
		path += rpath;
		return path;
	}
	static std_string& make_path(std_string& path,
		character const* rpath1,character const*rpath2,
		character const& slash)
	{
		path.assign(rpath1);
		add_rpath(path,rpath2,slash);
		return path;
	}
	static std_string make_path(
		character const* rpath1,character const*rpath2,
		character const& slash)
	{
		std_string path = rpath1;
		make_path(path,rpath1,rpath2,slash);
		return path;
	}
	static std_string& make_path(std_string& path,
		character const* rpath1,character const*rpath2,character const*rpath3,
		character const& slash)
	{
		make_path(path,rpath1,rpath2,slash);
		add_rpath(path,rpath3,slash);
		return path;
	}
	static std_string make_path(
		character const* rpath1,character const*rpath2,character const*rpath3,
		character const& slash)
	{
		std_string path = rpath1;
		make_path(path,rpath1,rpath2,rpath3,slash);
		return path;
	}

	static character get_last_slash(std::string const& path)
	{
		if (path.empty()) return 0;
		return get_last_slash(&path[0],&path[0]+path.length());
	}

	static character get_last_slash(character const* begin)
	{
		if (!begin) return 0;
		return get_last_slash(begin,begin+strkit::length(begin));
	}

	/// [)
	static character get_last_slash(character const* begin, character const* end)
	{
		if (end<=begin) return false;
		character const chars[3] =
		{
			strkit::slash_character,
			strkit::backslash_character,
			0
		};
		character ret = *(end-1);
		bool b = strkit::is_in_chset<true>(ret,chars);
		return b ? ret: 0;
	}

	static bool remove_last(std_string& path)
	{
		bool b = remove_slash(path);
		character const* begin = path.data();
		character const* end = begin+path.size();
		character const chars[3] =
			{strkit::slash_character,strkit::backslash_character,0};

		do {
			if (end<=begin) break;
			character const* end2 = strkit::find_last_in<true>(begin,end-1,chars);
			if (end2>=begin)
			{
				path.erase(end2-begin+1);
				return true;
			}
		}	while(0);
		if (b) add_slash(path,L'\\');
		return false;
	}

	static bool keep_last(std_string& path)
	{
		if (path.empty()) return false;

		character const* begin = path.data();
		character const* end = begin+path.size()-1;
		character const chars[3]
			= {strkit::slash_character,strkit::backslash_character,0};
		if (strkit::is_in_chset<true>(*end,chars))
			--end;
		if (end<=begin)
			return true;
		character const* end2 = strkit::find_last_in<true>(begin,end-1,chars);
		if (end2<begin)
			return true;
		path.erase(0,end2-begin+1);
		return true;
	}

	static bool keep_top(std_string& path)
	{
		if (path.empty())
			return false;
		character const chars[3]
			= {strkit::slash_character,strkit::backslash_character,0};
		character const* begin = path.c_str();
		if (strkit::is_in_chset<true>(*begin,chars))
			++begin;
		character const* p = strkit::find_first_in<true>(begin,chars);
		if (!(p && *p)) return false;
		path.erase(p-path.c_str()+1);
		return true;
	}
	static bool remove_top(std_string& path,bool keepslash=true)
	{
		if (path.empty())
			return false;
		character const chars[3]
			= {strkit::slash_character,strkit::backslash_character,0};
		character const* begin = path.c_str();
		if (strkit::is_in_chset<true>(*begin,chars))
			++begin;
		character* p = strkit::find_first_in<true>(begin,chars);
		if (!(p && *p)) return false;
		path.erase(0,p-path.c_str()+(keepslash?0:1));
		return true;
	}
	static bool remove_top(std_string& path,size_t count,bool keepdot=true)
	{
		if (count==-1)
		{
			path.clear();
			return true;
		}
		for (size_t i=0; i<count; ++i)
		{
			if (!remove_top(path,keepdot))
				return false;
		}
		return true;
	}
	template <character slash_char>
	static character* reslash(character* path)
	{
		if (!path || !*path)
			return path;
		static character const other_slash_char =
			slash_char==strkit::slash_character?strkit::backslash_character:strkit::slash_character;
		for (character const* p=path;*p;++p)
			if (*p==other_slash_char) *p = slash_char;
		return path;
	}
	static bool is_top(character const* path)
	{
		if (!path || !*path)
			return true;

		static const character path_seps[] = 
		{
			strkit::slash_character,
			strkit::backslash_character,
			strkit::null_character
		};
		character const* p = strkit::find_first_in<true>(path,path_seps);
		return !(p && *p && *(p+1));
	}
	static bool is_top(std_string const& path)
	{
		return is_top(path.c_str());
	}

	static bool add_slash(std_string& path,character slash,bool badded)
	{
		if (badded) return add_slash(path,slash);
		return true;
	}
	static bool addremove_slash(std_string& path,character slash,bool badded)
	{
		if (badded) return add_slash(path,slash);
		return remove_slash(path);
	}
	static bool add_slash(std_string& path,character slash)
	{
		if (path.empty())
		{
			path.push_back(slash);
			return true;
		}
		size_t len = path.size();
		character const chars[3] =
		{
			strkit::slash_character,
			strkit::backslash_character,
			0
		};

		if (!strkit::is_in_chset<true>(path[len-1],chars))
		{
			path.push_back(slash);
			return true;
		}
		return false;
	}

	static std_string& add_begin_slash(std_string& path,character slash=L'\\')
	{
		if (path.empty())
		{
			path.push_back(slash);
			return path;
		}
		character const chars[3] =
		{
			strkit::slash_character,
			strkit::backslash_character,
			0
		};

		if (!strkit::is_in_chset<true>(path[0],chars))
		{
			path.insert(path.begin(),slash);
		}
		return path;
	}
	static bool remove_slash(character* begin)
	{
		if (!begin) return false;
		return remove_slash(begin,strkit::length(begin));
	}
	/// [)

	static character* trim_right(character* p, character c)
	{
		character*end;
		int len = 0;

		len = lstrlen(p);
		while ( *p && len)
		{
			end = p + len-1;
			if( c == *end)
				*end = 0;
			else
				break;
			len = lstrlen( p);
		}
		return p;
	}
	static bool remove_slash(character* begin, character* end)
	{
		if (end<=begin) return false;
		if (!*begin) return false;

		character& c = *(end-1);
		if (c==strkit::backslash_character || c==strkit::slash_character)
		{
			c = 0;
			return true;
		}
		return false;
	}
	static bool remove_slash(std_string& path)
	{
		if (path.empty()) return false;
		size_t len = path.size();
		if (path[len-1]==strkit::backslash_character || path[len-1]==strkit::slash_character)
		{
			path.erase(len-1);
			return true;
		}
		return false;
	}
	static bool remove_begin_slash(character* begin,character*& out)
	{
		if (!begin||!*begin) return false;

		character& c = *begin;
		if (c==strkit::backslash_character || c==strkit::slash_character)
		{
			c = 0;
			out = begin+1;
			return true;
		}
		out = begin;
		return false;
	}
	static bool remove_begin_slash(std_string& path)
	{
		if (path.empty()) return false;
		if (path[0]==strkit::backslash_character || path[0]==strkit::slash_character)
		{
			path.erase(path.begin());
			return true;
		}
		return false;
	}
	static bool is_started_with(std_string const& path, std_string const& subpath)
	{
		std_string whole = path;
		std_string sub = subpath;
		
		std::replace(whole.begin(),whole.end(),L'/',L'\\');
		std::replace(sub.begin(),sub.end(),L'/',L'\\');
		size_t size = strkit::get_same_length<ox::character::__case_insensitive>(
			whole.c_str(),sub.c_str());
		return size && size == sub.size();
	}
	static bool is_ended_with(std_string const& path, std_string const& subpath)
	{
		std_string whole = path;
		std_string sub = subpath;
		
		std::replace(whole.begin(),whole.end(),L'/',L'\\');
		std::replace(sub.begin(),sub.end(),L'/',L'\\');
		size_t size = strkit::get_rsame_length<ox::character::__case_insensitive>(
			whole.c_str(),sub.c_str());
		return size && size == sub.size();
	}

	static character const* find(character const* begin,character const* end,
		size_t index,size_t* _counted=0,
		character const** itemend=0,character const** next=0)
	{
		typedef typename strkit::get_line<strkit::__casive> get_line;

		if (!begin || begin>=end)
		{
			if (_counted) *_counted = 0;
			if (itemend) *itemend = end;
			if (next) *next = end;
			return 0;
		}

		character const str_slash[] = {'/',0};
		character const str_backslash[] = {'\\',0};
		character const* seps [] = {str_slash,str_backslash};
		size_t counted = 0;
		get_line getline(begin,end);
		getline.find.set_seps(seps);
		character const* lineend = 0;
		character const* line = 0;

		do {
			line = getline(&lineend);
			if (!line) break;
			if (lineend!=line) ++counted;
		} while(counted<=index);

		if (_counted) *_counted = counted;
		if (itemend) *itemend = lineend;
		if (next) *next = getline.current;
		return line;
	}
	static size_t count(character const* begin,character const* end)
	{
		size_t item_count = 0;
		find(begin,end,-1,&item_count);
		return item_count;
	}
	static size_t count(character const* begin)
	{
		return count(begin,begin+strkit::length(begin));
	}
	static size_t count(std_string const& path)
	{
		return count(path.c_str(),path.c_str()+path.length());
	}

//	/// 合并成一个path，现在没有判断各个路径是否合法
//	/// 只是智能的把他们拼接出来
//	/// 对扩展名进行了特殊处理，就是后面一定有一个点号
//	static std_string make(
//		const character * dir,
//		const character * file)
//	{
//		return make(0,dir,file,0);
//	}
//	static std_string make(
//		const character * drive,
//		const character * dir,
//		const character * file_name,
//		const character * file_ext=0)
//	{
//		std_string path;
//		return make(path,drive,dir,file_name,file_ext);
//	}
//	static std_string & make(
//		std_string & path,
//		const character * drive,
//		const character * dir,
//		const character * file_name,
//		const character * file_ext=0)
//	{
//		struct make_kit
//		{
//			static std_string & append_dir(std_string & path, const character * dir_be_apppended)
//			{
//				if (path.empty())
//					path.append(dir_be_apppended);
//				else if (dir_be_apppended && dir_be_apppended[0]!=strkit::null_character)
//				{
//					if (strkit::backslash_character==path[path.length()-1] ||
//						strkit::slash_character==path[path.length()-1])
//					{
//						if( dir_be_apppended[0]==strkit::backslash_character||
//							dir_be_apppended[0]==strkit::slash_character)
//							path.append(dir_be_apppended+1);
//						else
//							path.append(dir_be_apppended);
//					}
//					else
//					{
//						if( dir_be_apppended[0]==strkit::backslash_character||
//							dir_be_apppended[0]==strkit::slash_character)
//							path.append(dir_be_apppended);
//						else
//							path.append(1,strkit::slash_character),path.append(dir_be_apppended);
//					}
//				}
//				return path;
//			}
//		};
//		if (drive) path.append(drive);
//		make_kit::append_dir(path,dir);
//		make_kit::append_dir(path,file_name);
//		if (file_ext)
//		{
//			if(file_ext[0]!=strkit::dot_character)
//				path.append(1,strkit::dot_character);
//			path.append(file_ext);
//		}
//		return path;
//	}
//	void parse(const character * path)
//	{
//		stringz pod_path = {const_cast<character *>(path)};
//		pod_parse(pod_path);
//	}
//	void pod_parse(stringz local_path)
//	{
//		static const character pretext[] = 
//		{
//			strkit::space_character,
//			strkit::tab_character,
//			strkit::carriagereturn_character,
//			strkit::linefeed_character,
//			strkit::null_character
//		};
//		static const character path_seps[] = 
//		{
//			strkit::slash_character,
//			strkit::backslash_character,
//			strkit::null_character
//		};
//		// char * invalid_path_char = "\\/:*?\"<>|";
//		path.refp_begin()=strkit::find_first_in<false>(local_path.refp_begin(),pretext);
//		drive.refp_begin()=drive.end_=path.begin();
//		dir.refp_begin() = strkit::find_first_in<true>(path.refp_begin(),path_seps);
//
//		if (*dir.begin() && !dir.isempty()&&*(dir.begin()-1)==strkit::colon_character)
//			drive.end_=dir.begin();
//		else
//			dir.refp_begin() = path.begin();
//
//		for (character *curr=dir.begin(),* prev=curr; true;)
//		{
//			if (strkit::is_in_chset<true>(*curr,path_seps))
//				curr++;
//			curr = strkit::find_first_in<true>(curr,path_seps);
//			if (*curr!=0) prev=curr;
//			if (*curr==0)
//			{
//				if (strkit::is_in_chset<true>(*prev,path_seps))
//					prev++;
//				dir.end_=file_name.refp_begin()=prev;
//				break;
//			}
//		}
//		if (*file_name.begin()!=0)
//		{
//			for (character *curr=file_name.begin(),* prev=curr; true;)
//			{
//				character* pend;
//				curr = strkit::strch<true>(
//					curr+1,strkit::dot_character,(character const**)&pend);
//
//				if (curr)
//				{
//					prev=curr;
//				}
//				else if (prev==file_name.begin())
//				{
//					file_ext.refp_begin() = pend;
//					file_name.end_ = pend;
//					break;
//				}
//				else
//				{ 
//					file_ext.refp_begin() = prev;
//					file_name.end_ = prev;
//					break;
//				}
//			}
//		}
//	}
//public:
//	std_string const getv_path() {return std_string(refc_path().begin());}
//	std_string const getv_drive() { return std_string(refc_drive().begin(),refc_drive().end());}
//	std_string const getv_dir() { return std_string(refc_dir().begin(),refc_dir().end());}
//	std_string const getv_ddir() { return std_string(getc_ddir().begin(),getc_ddir().end());}
//	std_string const getv_file_name() { return std_string(refc_file_name().begin(),refc_file_name().end());}
//	std_string const getv_file_ext() { return std_string(refc_file_ext().begin());}
//	std_string const getv_file() { return std_string(getc_file().begin()); }
//	stringz const & refc_path()const {return path;}
//	stringi const getc_ddir()const { stringi ddir={refc_drive().begin(),refc_dir().end()}; return ddir;}
//	stringi const & refc_drive()const { return drive; }
//	stringi const & refc_dir()const { return dir; }
//	stringz const getc_file()const { stringz file={file_name.begin()}; return file; }
//	stringi const & refc_file_name()const { return file_name; }
//	stringz const & refc_file_ext()const { return file_ext; }
//private:
//	stringz path;
//	stringi drive;
//	stringi dir;
//	stringi file_name;
//	stringz file_ext;
};

template <typename character_tn>
struct localpath_ref
{
	typedef character_tn character;
	typedef ox::str::string_simple_kit<character> strkit;
	typedef typename strkit::std_string std_string;
	typedef localpath_kit<character> pathkit;

	typedef localpath_ref self;

	self(std_string& path,character sep=character('/')) : _m_path(path), _m_sep(sep) {}
	
	self& add(std_string const& rpath)
	{
		pathkit::add_rpath(_m_path,rpath,_m_sep);
		return *this;
	}
	self& add(std_string const& rpath1,std_string const& rpath2)
	{
		pathkit::add_rpath(_m_path,rpath1,_m_sep);
		pathkit::add_rpath(_m_path,rpath2,_m_sep);
		return *this;
	}
	self& add(std_string const& rpath1,std_string const& rpath2,std_string const& rpath3)
	{
		pathkit::add_rpath(_m_path,rpath1,_m_sep);
		pathkit::add_rpath(_m_path,rpath2,_m_sep);
		pathkit::add_rpath(_m_path,rpath3,_m_sep);
		return *this;
	}
	self& operator+=(std_string const& rpath)
	{
		pathkit::add_rpath(_m_path,rpath,_m_sep);
		return *this;
	}
	self& operator+=(character const* rpath)
	{
		pathkit::add_rpath(_m_path,rpath,_m_sep);
		return *this;
	}
	self& operator+=(character const& rpath)
	{
		character const path = {rpath,0};
		pathkit::add_rpath(_m_path,path,_m_sep);
		return *this;
	}
	//self& add(std_string& path1, ...)
	//{
	//	va_list arglist;
	//	va_start(arglist, slash);
	//	vwprintf(format,arglist);
	//}
	self& remove_front(size_t count)
	{
		if (count==0) return *this;
		if (count==-1) { _m_path.clear(); return *this; }
		size_t counted = 0;
		character const* next = 0;
		character* hit = pathkit::find(_m_path.c_str(),_m_path.c_str()+_m_path.length(),
			count-1,0,0,&next);
		if (!hit) 
		_m_path.erase(0,next-_m_path.c_str());
		return *this;
	}
	size_t count() const
	{
		size_t counted = 0;
		character* hit = pathkit::find(
			_m_path.c_str(),_m_path.c_str()+_m_path.length(),
			-1,&counted);
		return counted;
	}

	self& assign(std_string const& path)
	{
		_m_path = path;
		return *this;
	}
	self& assign(std_string const& rpath1,std_string const& rpath2)
	{
		_m_path = rpath1;
		return add(rpath2);
	}
	self& assign(std_string const& rpath1,std_string const& rpath2,std_string const& rpath3)
	{
		_m_path = rpath1;
		return add(rpath2,rpath3);
	}
	self& set_sep(character const sep)
	{
		_m_sep = sep;
		return *this;
	}
	self& clear(character const sep='/') { _m_sep=sep; _m_path.clear(); return *this; }

	std_string& value() { return _m_path; }
	std_string const& value() const { return _m_path; }

	character _m_sep;
	std_string& _m_path;
};

template<typename character_tn>
struct uri_kit
{
	typedef character_tn character;
	typedef ox::str::string_simple_kit<character> strkit;
	typedef typename strkit::std_string std_string;
	typedef typename strkit::stringz stringz;
	typedef typename strkit::stringi stringi;
	static character const* get_protocol(character const* path_begin,character const** ppend=0)
	{
		return get_protocol(path_begin,strkit::end(path_begin),ppend);
	}
	static character const* get_protocol(character const* path_begin,character const* path_end,character const** ppend=0)
	{
		if (!path_begin || !*path_begin)
			return 0;
		character const* str = strkit::strch<true>(path_begin,path_end,':',ppend);
		if (str && ppend) *ppend = str;
		return str?path_begin:0;
	}
	static character const* get_port(cdata_tt<character> const& buf)
	{
		character const* p = strkit::strch<true>(buf.begin,buf.size,':');
		if (p) ++p;
		return p;
	}
	/// return end offset
	static size_t get_host_and_port(character const* host_begin,size_t* host_end_offset,size_t* port_offset=0)
	{
		return get_host_and_port(host_begin,strkit::end(host_begin),host_end_offset,port_offset);
	}
	/// return end offset
	static size_t get_host_and_port(cdata_tt<character> const& buf,size_t* host_end_offset,size_t* port_offset=0)
	{
		size_t host_end_off=buf.size,port_off=host_end_off,end_off=host_end_off;
		do {
			if(buf.is_empty()) break;
			character const* p = strkit::strch<true>(buf.begin,buf.size,':');
			if (p)
			{
				host_end_off = p-buf.begin, port_off = host_end_off+1;
				character const* p2 = strkit::str2ch<false>(buf.begin+port_off,buf.end(),'0',10);
				if (p2==0) p2 = buf.end();
				end_off = p2-buf.begin;
			}
		} while(0);
		if (host_end_offset) *host_end_offset = host_end_off;
		if (port_offset) *port_offset = port_off;
		return end_off;
	}
	/// return host begin
	static character const* get_host_and_path(
		character const* uri_begin,
		character const** path_begin)
	{
		return get_host_and_path(uri_begin,strkit::end(uri_begin),path_begin);
	}
	/// return host begin
	static character const* get_host_and_path(
		character const* uri_begin,character const* uri_end,
		character const** path_begin)
	{

		character const seps[] = {'/',';','?','#',0};

		character const* host_begin = uri_begin;
		character const* path2_begin = uri_end;
		if (path_begin) *path_begin = uri_end;

		/// if first char is '/', it is path

		do {
			if (!uri_begin || !*uri_begin)
				break;

			if (*uri_begin=='/')
			{
				path2_begin = uri_begin;
				break;
			}
			character const* pp = strkit::find_first_in<true>(uri_begin,uri_end,seps);
			/// like : www.baidu.com/ or www.baidu.com#
			/// if out of array, no protocol
			if (pp+1 >= uri_end)
			{
				path2_begin = pp;
				break;
			}
			
			/// protocol or path
			if (*pp=='/')
			{
				if (*(pp+1)=='/') /// protocol
				{
					host_begin = pp+2;
					if (pp+2>=uri_end) break;
					path2_begin = strkit::find_first_in<true>(host_begin,uri_end,seps);
				}
				/// like : www.baidu.com/a?q or www.baidu.com#afdsf
				else 
				{
					path2_begin = pp;
				}
			}
		} while (0);

		if (path_begin) *path_begin = path2_begin;
		return host_begin;
	}
	static character const* get_host_and_port(character const* szpath,
		character const** port,character const** pphostend=0,character const** ppend=0)
	{
		return get_host_and_port(szpath,szpath+strkit::length(szpath),port,pphostend,ppend);
	}
	/// return host begin
	static character const* get_host_and_port(character const* path_begin,character const* path_end,
		character const** ppport,character const** pphostend=0,character const** ppend=0)
	{
		if (!path_begin || !*path_begin)
			return 0;

		if (ppport) *ppport = 0;
		if (pphostend) *pphostend = 0;
		if (ppend) *ppend = 0;

		character const sep[] = {':','/','\\',0};
		character const* host_begin = strkit::skip<true>(path_begin,path_end,sep);
		character const* host_end = 0;
		character const* port_begin = 0;
		character const* port_end = 0;

		/// find host
		character const* str = strkit::find_first_in<true>(host_begin,path_end,sep);
		if (str==path_end)
		{
			if (pphostend) *pphostend = str;
			if (ppend) *ppend = str;
			return host_begin;
		}

		switch (*str)
		{
		case ':':
			{
				if (*(str+1)=='/' && *(str+2)=='/')
					host_begin = str+3;
				else if (strkit::is_digital(*(str+1)))
					port_begin = str;
				else
					host_begin = 0;
				break;
			}
		default: host_end = str;
		}
		/// not find host
		if (!host_begin) return 0;

		/// find port
		if (!port_begin)
			port_begin = strkit::find_first_in<true>(host_begin,path_end,sep);
		if (port_begin>=path_end)
		{
			host_end = path_end;
			if (ppend) *ppend = host_end;
			return 0;
		}
		host_end = port_begin;
		/// has port
		if (*port_begin==':')
		{
			if (ppport) *ppport = port_begin+1;
			port_end = strkit::find_first_in<true>(port_begin+1,path_end,sep+1);
			if (port_end>=path_end) port_end = path_end;
		}

		if (pphostend) *pphostend = host_end;
		if (ppend) *ppend = port_end?port_end:host_end;
		return host_begin;
	}
};


___namespace2_end()



class ox_path_format_kit
{
public:
	enum path_id
	{
		protocol,
		hostid,
		port,
		localpath,
	};
	typedef std::map<path_id,std::string> dns_parts_t;

	static bool parse_ipv4(
		const char * ipaddr,
		unsigned char ipbin[4]=0)
	{
		// 判断参数
		const char * pIP = ipaddr;
		if (pIP==0) return false;

		// 略过空格和制表符
		for ( ;(*pIP==' ' || *pIP=='\t') && (*pIP!=0); pIP++);

		bool isPartSuccessfully = false;
		int part_count=0;
		int count = 0;
		for (const char * p = pIP, * pByteStart=pIP; true; p++, count++)
		{
			if (part_count<4 && count<4)
			{
				bool bCaptureSepterator = false;
				if(part_count<3)
				{
					bCaptureSepterator = *p=='.';
					if (*p==0) break;
				}
				else
				{
					bCaptureSepterator = (*p==0 || *p<'0'|| *p>'9');
				}

				if (!bCaptureSepterator)
					continue;

				isPartSuccessfully = false;
				//unsigned char bPartValue = 0;
				

				switch (count-1)
				{
				case 0:
					isPartSuccessfully =
						pByteStart[0]>='0' && pByteStart[0]<='9';
					break;
				case 1:
					isPartSuccessfully =
						pByteStart[0]>='0' && pByteStart[0]<='9' &&
						pByteStart[1]>='0' && pByteStart[1]<='9';
					break;
				case 2: 
					switch(pByteStart[0])
					{
					case '0':
						isPartSuccessfully =
							pByteStart[1]>='0' && pByteStart[1]<='9';
						break;
					case '1': 
						isPartSuccessfully =
							pByteStart[1]>='0' && pByteStart[1]<='9' &&
							pByteStart[2]>='0' && pByteStart[2]<='9';
						break;
					case '2':
						isPartSuccessfully =
							pByteStart[1]>='0' && pByteStart[1]<='5' && 
							pByteStart[2]>='0' && pByteStart[2]<='5';
						break;
					}
				}
				if (isPartSuccessfully)
				{
					if (ipbin)
					{
						ipbin[part_count] = pByteStart[0]-'0';
						for (int i = 1; i<count; i++)
							ipbin[part_count] = (ipbin[part_count] *= 10) + pByteStart[i]-'0';
					}
					count = -1;
					pByteStart = p+1;
					part_count ++;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		return part_count==4 && isPartSuccessfully;

		/* the second method to complete
		unsigned b1, b2, b3, b4;   
		unsigned char c;   
		int rc = sscanf(ipaddr, "%3u.%3u.%3u.%3u%c", &b1, &b2, &b3, &b4, &c);   
		if (rc < 4) return true;   
		if (rc==5 && ! isspace(c)) return   false;   
		if ((b1 | b2 | b3 | b4) > 255) return false;   
		return true;   
		*/
	}


	
	//////////////////////////////////////////////////////////////////////////
	// matchmode = 0x00000001 : 精确匹配，即必须符合 a.b.c.d 格式
	//                          中间可以穿插空格，但是a,b,c,d 数字中间不能有空格，
	//                          a,b,c,d为10进制数据
	// 返回值定义：0，成功；1，失败

	//////////////////////////////////////////////////////////////////////////
	//	判断字符串是否位一个有效的域名格式
	//	返回值定义：=0：成功；表示是域名
	//				=1：表示是单词，非零失败
	//				<0：不是协议的情况，但不能确定是不是单词
	//	特殊性说明：如果形如格式：****. 或　****://****.
	//				的格式会返回1，表示为本地或局域网主机名。
	//static int parse_dns(const char * pszDns,dns_parts_t * dns_parts=0)
	//{
	//	typedef ox::str::string_simple_kit<char> strkit;

	//	if(pszDns==NULL)
	//		return -1;

	//	char * p = NULL;

	//	if (dns_parts) dns_parts->clear();

	//	// trim the left whitespace or tab
	//	for(p=(char*)pszDns;(*p==' '||*p=='\t') && *p!='\0';p++);

	//	if(*p=='\0')
	//		return -1; // 无效的域名

	//	const char * pDns = p;

	//	char * pProtocal = strstr(p,"://");
	//	if (pProtocal)
	//	{
	//		if (dns_parts)
	//		{
	//			//dns_parts->find(protocol)->second.clear(); // error, why?
	//			//dns_parts->find(protocol)->second.append(pDns,pProtocal); // error, why?
	//			(*dns_parts)[protocol].clear();
	//			(*dns_parts)[protocol].append(pDns,pProtocal);
	//		}
	//		p=pProtocal+3;
	//	}

	//	char * pHostnameEnd = strchr(p,'/');
	//	(*dns_parts)[hostid].clear();
	//	if (pHostnameEnd)
	//	{
	//		const char * pPortBegin = strkit::strrch<true>(pHostnameEnd,p,':');
	//		if (pPortBegin)
	//			(*dns_parts)[port].append(pPortBegin+1,pHostnameEnd);
	//		(*dns_parts)[hostid].append(p,pPortBegin);
	//		p=pHostnameEnd+1;
	//	}
	//	else
	//	{
	//		(*dns_parts)[hostid].append(p);
	//		return 0;
	//	}

	//	(*dns_parts)[localpath].clear();
	//	(*dns_parts)[localpath].append(p);

	//	return 0;
	//}

	////
	//static int parse_dns2(const char * pszDns,dns_parts_t * dns_parts=0)
	//{
	//	if(pszDns==NULL)
	//		return -1;

	//	const int dnsprotocal_maxlen = 10;
	//	const int dnshostname_maxlen = 30;

	//	char * p = NULL;

	//	if (dns_parts) dns_parts->clear();

	//	// trim the left whitespace or tab
	//	for(p=(char*)pszDns;(*p==' '||*p=='\t') && *p!='\0';p++);

	//	if(*p=='\0')
	//		return -1; // 无效的域名

	//	const char * pDns = p;

	//	char * pProtocal = (char*)pDns;
	//	bool bHasProtocal = false;
	//	for(;*pProtocal!='\0'; pProtocal++)
	//	{
	//		if(*pProtocal==':')
	//			if(*(pProtocal+1)=='/' && *(pProtocal+1)!='\0')
	//				if(*(pProtocal+2)=='/' && *(pProtocal+2)!='\0')
	//				{
	//					bHasProtocal=true;
	//					break;
	//				}

	//	}
	//	if(!bHasProtocal) 
	//	{
	//		pProtocal = NULL; // no protocal;
	//	}

	//	// assure the protocal part is correct
	//	if(pProtocal)
	//	{
	//		if (dns_parts)
	//		{
	//			//dns_parts->find(protocol)->second.clear();
	//			//dns_parts->find(protocol)->second.append(pDns,pProtocal);
	//			(*dns_parts)[protocol].clear();
	//			(*dns_parts)[protocol].append(pDns,pProtocal);
	//		}

	//		if(pProtocal-pDns > dnsprotocal_maxlen)
	//			return -1; // 协议长度越界
	//		for(p=(char*)pDns;p<pProtocal;p++)
	//			if(!(
	//				(*p>='0' && *p<='9') ||
	//				(*p>='a' && *p<='z') ||
	//				(*p>='A' && *p<='Z') ||
	//				(*p=='-')            ||
	//				(*p=='_')))
	//				return -1; // 无效的protocal
	//	}

	//	char * pHostname = NULL;
	//	// 有协议和无协议两种情况
	//	if(bHasProtocal)
	//	{
	//		pHostname = p = pProtocal+3;
	//		// 处理协议和主机名之间的连结情况

	//		if( (*p == '\0') ||
	//			!(
	//			(*p>='0' && *p<='9') ||
	//			(*p>='a' && *p<='z') ||
	//			(*p>='A' && *p<='Z') ||
	//			(*p=='-')            ||
	//			(*p=='_')))
	//			return -2; // 出口：无效：主机名长度为0，或协议和主机名之间有无效字符
	//		//else
	//		// ok, 连结正常
	//	}
	//	else
	//	{
	//		// 无协议情况
	//		pHostname = (char*)pDns;
	//	}


	//	// find the 1st '.'
	//	// this level domain name is only to be English Word.
	//	bool bFind1stDot = false;
	//	for(p=pHostname;*p!='\0';p++)
	//	{
	//		if(!(
	//			(*p>='0' && *p<='9') ||
	//			(*p>='a' && *p<='z') ||
	//			(*p>='A' && *p<='Z') ||
	//			(*p=='-')            ||
	//			(*p=='_')))
	//		{
	//			if(*p=='.')
	//			{
	//				if( *(p+1)=='\0' )
	//				{
	//					*p='\0';
	//					//if(bHasProtocal)
	//					return 0;	// 出口：OK,  符合本地域名
	//					//  或 "xxxx://xxxx."
	//				}
	//				bFind1stDot=true; // 有"."且后有字符
	//				break; // 查找到，至少在此之前是有效的域名
	//			}
	//			else
	//			{
	//				return -3; // 出口：fail，有非法字符
	//			}
	//		}
	//	}

	//	//	没点的情况和有点的情况:形如：
	//	//	有协议：****://****.****（继续） 或 ****://****（继续）
	//	//	无协议：****.****（继续） 或 ****（单词）（返回）

	//	//	没有协议也没有点的情况，是单词了
	//	if(!bHasProtocal && !bFind1stDot)
	//	{
	//		//TRACE("\r\n______________");
	//		return 1;  // 是单词了
	//	}

	//	// 剩下的就是形如格式："xxxx://xxxx.xxx...." 或 "xxxx.xxx...."
	//	// 但有可能是中文域名
	//	// find all the next '.'
	//	if(*p=='\0')
	//		return -1; // "***.**." 格式失败
	//	if(*(p+1)=='.')
	//		return -1; // "****..****" 格式是失败的

	//	for(;*p!='\0';p++)
	//	{
	//		if(*p<0x80)
	//		{
	//			if(!(
	//				(*p>='0' && *p<='9') ||
	//				(*p>='a' && *p<='z') ||
	//				(*p>='A' && *p<='Z') ||
	//				(*p=='-') ||
	//				(*p=='.')))
	//			{
	//				return -3;
	//			}
	//			if( (*(p-1)==*p) && *p=='.' ) // "****..****" 格式是失败的
	//				return -3;
	//		}
	//	}

	//	return 0; // 可以从形式上确定是合法的域名。
	//}
};


class ox_localpath_format_kit
{

public:
	ox_localpath_format_kit ()
	{
	}

	ox_localpath_format_kit (const char * szpath)
	{
		m_strpath.append(szpath);
		split ();
	}

	ox_localpath_format_kit (const char * szpath, size_t length)
	{
		m_strpath.append(szpath,length);
		split ();
	}

	ox_localpath_format_kit (std::string & path)
	{
		m_strpath = path;
		split ();
	}

	ox_localpath_format_kit (const char * szpath, bool bParse)
	{
		m_strpath.append(szpath);
		if (bParse) split ();
	}

	ox_localpath_format_kit (const char * szpath, size_t length, bool bParse)
	{
		//m_strpath.
		m_strpath.append(szpath,length);
		if (bParse) split ();
	}

	ox_localpath_format_kit (std::string & path, bool bParse)
	{
		m_strpath = path;
		if (bParse) split ();
	}

	std::string & refv_path() { return m_strpath; }
	void split(std::string const & strpath)
	{
		char drive[10]={0}, dir[1024]={0}, fname[256]={0}, ext[128]={0};
		_splitpath_s (strpath.c_str(), 
			drive,sizeof(drive),
			dir,sizeof(dir),
			fname,sizeof(fname),
			ext,sizeof(ext));
		m_strdrive = drive;
		m_strdir = dir;
		m_strfilename = fname;
		m_strfileext = ext;	
	}
	void split() { split(m_strpath); }
	static std::string make(const char * drive, const char * dir, const char * filename, const char * fileext)
	{
		char path[_MAX_PATH] = {0};
		_makepath_s(path,drive,dir,filename,fileext); //==0 ok
		return path;
	}

	std::string& refv_drive()
	{
		return m_strdrive;
	}
	std::string& refv_dir()
	{
		return m_strdir;
	}
	std::string& refv_file_name()
	{
		return m_strfilename;
	}
	std::string& refv_file_ext()
	{
		return m_strfileext;
	}
	std::string getv_full_dir()
	{
		return m_strdrive+m_strdir;
	}
	std::string getv_full_file_name()
	{
		return m_strfilename+m_strfileext;
	}
private:
	std::string m_strpath;
	std::string m_strdrive;
	std::string m_strdir;
	std::string m_strfilename;
	std::string m_strfileext;
};

