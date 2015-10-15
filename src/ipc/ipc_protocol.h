

#include "../utl/data_t.h"
#include "../pkg/string_simple_kit.h"
#include "../utl/vecbuff.h"
#include "ipc_cmdid.h"


#pragma once


___namespace2_begin(ox,ipc)


	struct ipc_protocol
	{
		typedef ox::utl::cdata_t cdata_t;

		struct ipc_protocol_header
		{
			size_t size;
			size_t ver;
			size_t cmdid;
			static size_t head_size() {return sizeof(ipc_protocol_header);}
			char const* data() const {return (char const*)(this+1);}
			size_t data_size() const {assert(this && size>=head_size());return size-head_size();}
		};

		struct ipc_protocol_buff
		{
			typedef ox::utl::vecbuff_t vecbuff_t;
			typedef ipc_protocol_header header_t;
			typedef vecbuff_t::item_t item_t;

			enum item_sequence_enum
			{
				__item_index_source = 0,
				__item_index_back,
				__item_index_destinations,
				__item_index_data,
				__item_index_name = __item_index_data,
				__item_count,
			};

			header_t _m_head;

			size_t size() const
			{
				return _m_head.size;
			}

			cdata_t get_source_address() const
			{
				return get_item(__item_index_source);
			}
			cdata_t get_response_address() const
			{
				return get_item(__item_index_back);
			}
			size_t get_count_of_destination_address() const
			{
				cdata_t dests_data = get_item(__item_index_destinations);
				vecbuff_t* dests = (vecbuff_t*)dests_data.begin;
				return dests->_m_item_count;
			}
			cdata_t get_destination_addresses() const
			{
				return get_item(__item_index_destinations);
			}
			cdata_t get_destination_address(size_t i) const
			{
				cdata_t dests_data = get_destination_addresses();
				vecbuff_t* dests = (vecbuff_t*)dests_data.begin;
				cdata_t di = dests->data_item(i);
				return cdata_t(di.begin,di.size);
			}
			size_t get_commandid() const
			{
				return _m_head.cmdid;//cmdid_helper<size_t const>(_m_head.cmdid).get_cmdid();
			}
			//bool is_answer_needed() const
			//{
			//	return ::ox::ipc::ipcnode_define::is_back_cmdid(_m_head.cmdid);//cmdid_helper<size_t const>(_m_head.cmdid).is_anwser_needed();
			//}
			cdata_t get_data() const
			{
				return get_item(count()-1);
			}
			cdata_t get_item(size_t index) const
			{
				assert (count()>=__item_count && index<count());
				vecbuff_t* vbuff = vecbuff();
				cdata_t s = vbuff->data_item(index);
				return s;
			}
			size_t count() const
			{
				vecbuff_t* vbuff = vecbuff();
				return vbuff->_m_item_count;
			}
			vecbuff_t* vecbuff() const
			{
				return (vecbuff_t*)(this+1);
			}
		}; /// end of ipc_protocol_buff	

		/// source(0),back(1),destination(2),data(3)
		struct ipc_protocol_buff_helper
		{
			typedef ox::str::astring_kit strkit;
			typedef ox::utl::vecbuff_helper vbuff_helper;
			typedef ipc_protocol_buff_helper self;
			typedef ipc_protocol_header header_t;

			std::string& out;

			self(std::string& buff) : out(buff)
			{}
			ipc_protocol_buff& as_ipc_protocol()
			{
				return *(ipc_protocol_buff*)out.data();
			}
			ipc_protocol_buff const& as_ipc_protocol() const
			{
				return *(ipc_protocol_buff*)out.data();
			}
			self& add(cdata_t const& s,cdata_t const& f,cdata_t const& dests,size_t cmdid,cdata_t const& data)
			{
				add(s); /// source address, necessary
				add(f); /// response address, necessary
				add(dests); /// destination address, necessary
				add(data); /// data body, necessary
				reset_head_size(cmdid);
				return *this;
			}
			/// make a buffer of ipc protocol
			self& add(char const* str)
			{
				return add(cdata_t(str,strkit::length(str)));
			}
			self& addz(char const* str)
			{
				return add(cdata_t(str,strkit::length(str)+1));
			}
			//self& add(ipc_event_d const& event)
			//{
			//	cdata_t data((char const*)(&event),sizeof(event));
			//	return add(data);
			//}
			//self& add(vecbuff_t const& vecbuff)
			//{
			//	return add(cdata_t((char const*)(&vecbuff),vecbuff.size()));
			//}
			self& add(cdata_t const& item)
			{
				vbuff_helper(out,sizeof(header_t)).add(item.begin,item.size);
				return *this;
			}
			self& reset_head_size(size_t cmdid)
			{
				assert(out.size()>=sizeof(header_t));
				header_t* head = (header_t*)(out.data());
				head->cmdid = cmdid;
				head->size = out.size();
				return *this;
			}
			self& init()
			{
				out.clear();
				header_t head = {sizeof(header_t),1};
				out.append((char*)(&head),sizeof(head));
				vbuff_helper(out).init(); /// init vbuff
				return *this;
			}
		}; /// end of ipc_protocol_buff_helper


		struct ipc_protocol_each_buff
		{
			typedef ox::utl::vecbuff_t vecbuff_t;

			static ipc_protocol_each_buff* as_ipc_protocol_each(cdata_t const& data)
			{
				ipc_protocol_each_buff* p = (ipc_protocol_each_buff*)(data.begin);
				p->top(); /// check buffer is long enough
				return p;
			}
			enum item_sequence_enum
			{
				__item_index_head,
				__item_index_data,
			};
			cdata_t get_head() const
			{
				return top()->data_item(__item_index_head);
			}
			cdata_t get_data() const
			{
				return top()->data_item(__item_index_data);
			}
			vecbuff_t* top() const
			{
				return vecbuff_t::as_vecbuff(cdata_t((char const*)this,-1));
			}
		}; /// ipc_protocol_each_buff

		struct ipc_protocol_multi_buff
		{
			typedef ox::utl::vecbuff_t vecbuff_t;

			static ipc_protocol_multi_buff* as_ipc_protocol_multi(cdata_t const& data)
			{
				ipc_protocol_multi_buff* p = (ipc_protocol_multi_buff*)(data.begin);
				p->top(); /// check buffer is long enough
				return p;
			}
			size_t get_count() const
			{
				return top()->item_count();
			}
			cdata_t get_head(size_t i) const
			{
				return get_ipc_protocol_each(i)->get_head();
			}
			cdata_t get_data(size_t i) const
			{
				return get_ipc_protocol_each(i)->get_data();
			}
			ipc_protocol_each_buff* get_ipc_protocol_each(size_t i) const
			{
				return (ipc_protocol_each_buff*)(top()->data_item(i).begin);
			}
			vecbuff_t* top() const
			{
				return vecbuff_t::as_vecbuff(cdata_t((char const*)this,-1));
			}
		};
	}; /// end of ipc protocol


___namespace2_end(ox,ipc)