


#include <assert.h>
#include <string>
#include "data_t.h"


#pragma once

namespace ox
{
namespace utl
{
	template <size_t size_tn>
	struct good_buffer
	{
		good_buffer()
		{
			_m_buffer[0] = 0;
		}
		~good_buffer()
		{
			delete [] _m_heap_buffer.begin;
		}
		cdata_t allocate(size_t size)
		{
			if (size<=size_tn)
				return cdata_t(_m_buffer,size);
			if (_m_heap_buffer.size<size)
			{
				delete [] _m_heap_buffer.begin;
				_m_heap_buffer.clear();
			}
			if (_m_heap_buffer.begin)
				return cdata_t(_m_heap_buffer.begin,size);
			_m_heap_buffer.begin = new (std::nothrow) char [size];
			if (!_m_heap_buffer.begin) return cdata_t();
			_m_heap_buffer.size = size;
			return _m_heap_buffer;
		}
		cdata_t data() const
		{
			if (!_m_heap_buffer.is_empty()) return _m_heap_buffer;
			return cdata_t(_m_buffer,size_tn);
		}
		cdata_t _m_heap_buffer;
		char _m_buffer[size_tn];
	};

	/// veclist==>[item-count,data-offset][items]{data}
	/// items==>[item_t,,,,]
	/// item_t==>[offset,size]
	/// data==>[d1,d2,,,,]
	struct vecbuff_t
	{
		enum data_begin_mode_enum
		{
			__rel_address,
			__abs_address,
		};
		typedef vecbuff_t self;
		typedef ox::utl::cdata_t item_t;

		static self* as_vecbuff(cdata_t const& data)
		{
			self* vecbuff = (self*)(data.begin);
			assert (vecbuff->size()<=data.size);
			return vecbuff;
		}
		static self* unsafe_as_vecbuff(cdata_t const& data)
		{
			self* vecbuff = (self*)(data.begin);
			return vecbuff;
		}
		static self* unsafe_as_vecbuff(char const* data,size_t size=-1)
		{
			self* vecbuff = (self*)(data);
			return vecbuff;
		}
		self()
		{
			init_rel();
		}
		char const* _m_data_begin;
		unsigned short _m_begin_mode;
		unsigned short _m_item_count;
		void set_data_begin_mode(data_begin_mode_enum mode)
		{
			_m_begin_mode = mode;
		}
		void set_data_relative_mode()
		{
			_m_begin_mode = __rel_address;
		}
		void set_data_absolute_mode()
		{
			_m_begin_mode = __abs_address;
		}
		bool is_relative_mode() const
		{
			return _m_begin_mode==__rel_address;
		}
		bool is_absolute_mode() const
		{
			return _m_begin_mode==__abs_address;
		}
		cdata_t data() const
		{
			return cdata_t((char*)this,size());
		}
		size_t item_count() const
		{
			return _m_item_count;
		}
		size_t size() const
		{
			return size_of_meta()+size_of_head()+size_of_data();
		}
		static size_t size_of_meta()
		{
			return sizeof(self);
		}
		static size_t size_of_head(size_t count)
		{
			return count*sizeof(item_t);
		}
		size_t size_of_head() const
		{
			return size_of_head(_m_item_count);
		}
		size_t size_of_data() const
		{
			size_t size = 0;
			for (size_t i=0;i<_m_item_count;++i)
				size += item(i).size;
			return size;
		}
		static size_t minsize(size_t item_count)
		{
			return size_of_meta()+size_of_head(item_count);
		}
		item_t* head_begin() const
		{
			return (item_t*)(this+1);
		}
		item_t* head_end() const
		{
			return head_begin()+_m_item_count;
		}
		item_t item(size_t i) const
		{
			assert(i<_m_item_count);
			return *(head_begin()+i);
		}
		void unsafe_set_item(item_t const& item,size_t index)
		{
			*(head_begin()+index) = item;
		}
		char const* data_begin() const
		{
			assert (is_absolute_mode() || _m_data_begin==(char const*)(size_of_meta()+size_of_head()));
			return _m_data_begin;
		}
		char const* data_end() const
		{
			if (__rel_address!=_m_begin_mode) return 0;
			assert (is_absolute_mode() || _m_data_begin==(char const*)(size_of_meta()+size_of_head()));
			return _m_data_begin+size_of_data();
		}
		char const* data_begin_absolute() const
		{
			assert (is_absolute_mode() || _m_data_begin==(char const*)(size_of_meta()+size_of_head()));
			return is_relative_mode()?(char const*)(this)+(size_t)_m_data_begin : _m_data_begin;
		}
		char const* data_end_absolute() const
		{
			if (__rel_address!=_m_begin_mode) return 0;
			assert (is_absolute_mode() || _m_data_begin==(char const*)(size_of_meta()+size_of_head()));
			return data_begin_absolute()+size_of_data();
		}
		cdata_t data_item(size_t i) const
		{
			cdata_t ditem;
			item_t index = item(i);
			ditem.begin = data_begin_absolute()+(size_t)index.begin;
			ditem.size = index.size;
			return ditem;
		}
		void unsafe_add_index(item_t item)
		{
			unsafe_set_item(item,_m_item_count);
			_m_item_count++;
			if (__rel_address==_m_begin_mode) _m_data_begin += sizeof(item_t);
		}
		void unsafe_add_index(item_t* item,size_t count)
		{
			assert (item || !count);
			for (size_t i=0;i<count;++i)
				unsafe_add_index(item[i]);
		}
		static size_t offset_of_head()
		{
			return size_of_meta();
		}
		size_t offset_of_head_end() const
		{
			return offset_of_head()+size_of_head();
		}
		size_t offset_of_data()
		{
			return size_of_meta()+size_of_head();
		}
		void unsafe_add_index_by_data(cdata_t const* data,size_t size)
		{
			for (size_t i=0;i<size;++i)
				unsafe_add_index_by_data(data[i]);
		}
		void unsafe_add_index_by_data(cdata_t const& data)
		{
			unsafe_add_index_by_data(data.begin,data.size);
		}
		void unsafe_add_index_by_data(char const* begin,size_t size)
		{
			item_t item (begin,size);
			if (_m_begin_mode==__rel_address) item.begin = (char const*)size_of_data();
			unsafe_add_index(item);
		}
		void unsafe_add_index_and_data(cdata_t const& data)
		{
			size_t data_size = size_of_data();
			char const* data_begin_cur = data_begin_absolute();
			char* data_begin_new = (char*)data_begin_cur+sizeof(item_t);
			if (_m_begin_mode==__rel_address)
			{
				/// move data backward size of sizeof(item_t)
				memmove((void*)(data_begin_new),data_begin_cur,data_size);
			}
			unsafe_add_index_by_data(data);
			/// copy data
			if (_m_begin_mode==__rel_address)
			{
				memmove(data_begin_new+data_size,data.begin,data.size);
			}
		}
		void unsafe_add_index_and_data(cdata_t const* data,size_t size)
		{
			char const* data_begin_cur = (char*)data_begin_absolute();
			char* data_begin_new = (char*)data_begin_cur+sizeof(item_t)*size;
			size_t data_size = size_of_data();
			if (_m_begin_mode==__rel_address)
			{
				/// move data backward size of sizeof(item_t)
				memmove(data_begin_new,data_begin_cur,data_size);
			}
			char* data_next = data_begin_new+data_size;
			for (size_t i=0;i<size;++i)
			{
				cdata_t buf = data[i];
				/// set index
				unsafe_add_index_by_data(buf);
				if (_m_begin_mode!=__rel_address) continue;
				/// set data
				memmove(data_next,buf.begin,buf.size);
				/// move data next
				data_next += buf.size;
			}
			assert (_m_begin_mode!=__rel_address || _m_begin_mode==__rel_address && data_begin_absolute()==data_begin_new);
		}
		void convert_to_absolute()
		{
			if (_m_begin_mode==__abs_address) return;
			char const* data_begin = data_begin_absolute();
			item_t* p = head_begin();
			for (size_t i=0;i<_m_item_count;++i,++p)
				p->begin = data_begin+(size_t)(p->begin);
			_m_data_begin = 0;
		}
		void init_rel()
		{
			_m_begin_mode = __rel_address;
			_m_item_count = 0;
			_m_data_begin = (char const*)(size_of_meta());
		}
		void init_abs()
		{
			_m_begin_mode = __abs_address;
			_m_item_count = 0;
			_m_data_begin = 0;
		}
		static size_t size_needed_of_data(cdata_t const* data,size_t size)
		{
			size_t size_needed = 0;
			for (size_t i=0;i<size;++i)
				size_needed += data[i].size;
			return size_needed;
		}
		static size_t size_needed_of_index_and_data(cdata_t const* data,size_t size)
		{
			return size_needed_of_data(data,size)+size_of_head(size);
		}
		static size_t size_needed_of_index_and_data(cdata_t const& data)
		{
			return data.size+size_of_head(1);
		}
		static size_t size_needed_of_meta_and_index_and_data(cdata_t const* data,size_t size)
		{
			return size_needed_of_data(data,size)+size_of_head(size)+size_of_meta();
		}
		static size_t size_needed_of_meta_and_index_and_data(cdata_t const& data)
		{
			return data.size+size_of_head(1)+size_of_meta();
		}
	}; /// end of veclist_t


	template <typename buff_tn>
	struct vecbuff_tt
	{
		buff_tn& _m_buff;
		size_t _m_pre_reserved;
		typedef vecbuff_tt self;
		self(buff_tn& buff,size_t pre_served=0)
			: _m_buff(buff)
			, _m_pre_reserved(pre_served)
		{}
		static vecbuff_t& as_vecbuff(cdata_t const& data)
		{
			return *vecbuff_t::as_vecbuff(data);
		}
		static vecbuff_t& as_vecbuff(buff_tn& buff,size_t pre_served=0)
		{
			assert (buff.size() >= pre_served);
			cdata_t data (buff.data()+pre_served,buff.size()-pre_served);
			return as_vecbuff(data);
		}
		vecbuff_t& as_vecbuff()
		{
			return as_vecbuff(_m_buff,_m_pre_reserved);
		}
		vecbuff_t const& as_vecbuff() const
		{
			return *(vecbuff_t const*)(_m_buff.data()+_m_pre_reserved);
		}
		self& init()
		{
			_m_buff.resize(vecbuff_t::size_of_meta(),0);
			vecbuff_t* vlist = (vecbuff_t*)(_m_buff.data()+_m_pre_reserved);
			new (vlist) vecbuff_t();
			return *this;
		}
		self& add(cdata_t const& data)
		{
			size_t size_added = vecbuff_t::size_needed_of_index_and_data(data);
			_m_buff.resize(_m_pre_reserved+_m_buff.size()+size_added);
			vecbuff_t& vecbuff = as_vecbuff();
			vecbuff.unsafe_add_index_and_data(data);
			return *this;
		}
		self& add(cdata_t const* data,size_t size)
		{
			size_t size_added = vecbuff_t::size_needed_of_index_and_data(data,size);
			_m_buff.resize(_m_pre_reserved+_m_buff.size()+size_added);
			vecbuff_t& vecbuff = as_vecbuff();
			vecbuff.unsafe_add_index_and_data(data,size);
			return *this;
		}
		template <typename type_tn>
		self& add_binary(type_tn const& value)
		{
			cdata_t data(reinterpret_cast<char const*>(&value),sizeof(value));
			return add(data);
		}
		self& add_str0(char const* str)
		{
			return add(cdata_t(str,strlen(str)));
		}
		self& add_str1(char const* str)
		{
			return add(cdata_t(str,strlen(str)+1));
		}
		self& add_str0(char const* str1,char const* str2)
		{
			cdata_t datas[2] = {cdata_t(str1,strlen(str1)),cdata_t(str2,strlen(str2))};
			return add(datas,2);
		}
		self& add_str1(char const* str1,char const* str2)
		{
			cdata_t datas[2] = {cdata_t(str1,strlen(str1)+1),cdata_t(str2,strlen(str2)+1)};
			return add(datas,2);
		}
	};

	template <typename buff_tn>
	static void to_kvstring(buff_tn& buff,vecbuff_t const& vecbuff,char const* kvsep=":",char const* kvkvsep=";",bool last_need=true)
	{
		size_t count = vecbuff.item_count();
		assert (!(count & 1));
		if (count&1) return;
		if (count==0)
		{
			buff.clear();
			return;
		}
		if (count>0)
		{
			cdata_t k = vecbuff.data_item(0);
			cdata_t v = vecbuff.data_item(0+1);
			buff.append(k.begin,k.size);
			buff.append(kvsep);
			buff.append(v.begin,v.size);
		}
		for (size_t i=2;i<count;i+=2)
		{
			buff.append(kvkvsep);
			cdata_t k = vecbuff.data_item(i);
			cdata_t v = vecbuff.data_item(i+1);
			buff.append(k.begin,k.size);
			buff.append(kvsep);
			buff.append(v.begin,v.size);
		}
		if (last_need)
		{
			buff.append(kvkvsep);
		}
	}

	template <typename buff_tn>
	struct kvbuff_helper
	{
		typedef kvbuff_helper self;
		typedef vecbuff_tt<buff_tn> buff_helper;
		buff_helper _m_helper;
		self(buff_tn& buff,size_t pre_served=0)
			: _m_helper(buff,pre_served)
		{}
		static vecbuff_t& as_vecbuff(cdata_t const& data)
		{
			return *vecbuff_t::as_vecbuff(data);
		}
		static vecbuff_t& as_vecbuff(buff_tn& buff,size_t pre_served=0)
		{
			assert (buff.size() >= pre_served);
			cdata_t data (buff.data()+pre_served,buff.size()-pre_served);
			return as_vecbuff(data);
		}
		vecbuff_t& as_vecbuff()
		{
			return _m_helper.as_vecbuff();
		}
		vecbuff_t const& as_vecbuff() const
		{
			return _m_helper.as_vecbuff();
		}
		self& init()
		{
			_m_helper.init();
			return *this;
		}
		self& add(cdata_t const& k,cdata_t const& v)
		{
			cdata_t two[2] = {k,v};
			_m_helper.add(two,2);
			return *this;
		}
		self& add_str0(char const* k,char const* v)
		{
			_m_helper.add_str0(k,v);
			return *this;
		}
		self& add_str1(char const* k,char const* v)
		{
			_m_helper.add_str1(k,v);
			return *this;
		}
	};

	/// relative data begin mode
	struct vecbuff_helper
	{
		typedef vecbuff_helper self;
		typedef vecbuff_t vecbuff_t;
		typedef vecbuff_t::item_t item_t;

		std::string& out;
		size_t _m_pre_size;
		vecbuff_helper(std::string& buf)
			: out(buf),_m_pre_size(buf.size())
		{}
		vecbuff_helper(std::string& buf,size_t presize)
			: out(buf), _m_pre_size(presize)
		{
			assert (presize<=out.size());
		}
		static vecbuff_t& as_vecbuff(cdata_t const& data)
		{
			return *vecbuff_t::as_vecbuff(data);
		}
		static vecbuff_t& as_vecbuff(std::string& buff,size_t presize=0)
		{
			assert (buff.size() >= presize);
			cdata_t data (buff.data()+presize,buff.size()-presize);
			return as_vecbuff(data);
		}
		vecbuff_t& as_vecbuff()
		{
			return as_vecbuff(out,_m_pre_size);
		}
		vecbuff_t const& as_vecbuff() const
		{
			return *(vecbuff_t const*)(out.data()+_m_pre_size);
		}
		self& init()
		{
			out.append(vecbuff_t::size_of_meta(),0);
			vecbuff_t* vlist = (vecbuff_t*)(out.data()+_m_pre_size);
			new (vlist) vecbuff_t();
			return *this;
		}
		template <typename type_tn>
		self& add_binary(type_tn const& value)
		{
			cdata_t data(reinterpret_cast<char const*>(&value),sizeof(value));
			return add(data);
		}
		self& addstr(char const* str)
		{
			return add(str,strlen(str)+1);
		}
		self& add(char const* data,size_t size)
		{
			cdata_t di;
			di.begin = data;
			di.size = size;
			return add(di);
		}
		self& add(cdata_t const* data, size_t size)
		{
			for (size_t i=0;i<size;++i)
				add(data[i]);
			return *this;
		}
		self& add(size_t count,char c)
		{
			return add(cdata_t(0,count),c);
		}
		self& add(cdata_t data_item,char c=0)
		{
			assert(out.size()>=_m_pre_size+vecbuff_t::size_of_meta());
			size_t size_of_added = sizeof(item_t)+data_item.size;
			vecbuff_t* veclist = (vecbuff_t*)(out.data()+_m_pre_size);
			size_t size_of_meta = veclist->size_of_meta();
			size_t size_of_head = veclist->size_of_head();
			size_t size_of_data = veclist->size_of_data();
			size_t size_of_all = size_of_meta+size_of_head+size_of_data;
			size_t size_of_former = size_of_meta+size_of_head;
			/// preserver the item index buffer
			out.insert(size_of_former+_m_pre_size,sizeof(item_t),0);
			/// add the item data to the tail
			if (data_item.size)
				if (data_item.begin)
					out.append(data_item.begin,data_item.size);
				else
					out.append(data_item.size,c);
			/// recalculate the veclist pointer
			veclist = (vecbuff_t*)(out.data()+_m_pre_size);
			/// fill the item
			item_t item ((char const*)size_of_data,data_item.size);
			veclist->unsafe_add_index(item);
			return *this;
		}
	}; /// end of vecbuff helper
} /// end of namespace utl
} /// end of namespace ox
