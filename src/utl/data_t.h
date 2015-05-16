

#pragma once


namespace ox
{
	namespace utl
	{
		template <typename character>
		struct data_tt
		{
			typedef character value_type;
			character* begin;
			size_t size;
			data_tt()
				:begin(0),size(0)
			{}
			data_tt(character* _data,size_t _size)
				: begin(_data),size(_size)
			{}
			data_tt(data_tt const& data)
				: begin(data.begin), size(data.size)
			{}
			void assign(data_tt const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(character* data,size_t size)
			{
				this->begin = data;
				this->size = size;
			}
			bool is_empty() const
			{
				return begin==0 || size==0;
			}
			void clear()
			{
				begin=0,size=0;
			}
			character* end() const
			{
				return begin+size;
			}
			bool operator == (data_tt const& other) const
			{
				return begin==other.begin && size==other.size;
			}
			bool operator != (data_tt const& other) const
			{
				return begin!=other.begin || size!=other.size;
			}
			character* rbegin() const
			{
				return size==0?begin:begin+size-1;
			}
			character* rend() const
			{
				return size==0?begin:begin-size;
			}
		}; /// end of name data_t
		template <typename character>
		struct cdata_tt
		{
			typedef character value_type;
			character const* begin;
			size_t size;
			cdata_tt()
				:begin(0),size(0)
			{}
			cdata_tt(character const* _data,size_t _size)
				: begin(_data),size(_size)
			{}
			cdata_tt(data_tt<character> const& data)
				: begin(data.begin), size(data.size)
			{}
			void assign(cdata_tt<character> const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(data_tt<character> const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(character const* data,size_t size)
			{
				this->begin = data;
				this->size = size;
			}
			bool is_empty() const
			{
				return begin==0 || size==0;
			}
			void clear()
			{
				begin=0,size=0;
			}
			character const* end() const
			{
				return begin+size;
			}
			bool operator == (cdata_tt const& other) const
			{
				return begin==other.begin && size==other.size;
			}
			bool operator != (cdata_tt const& other) const
			{
				return begin!=other.begin || size!=other.size;
			}
			character const* rbegin() const
			{
				return size==0?begin:begin+size-1;
			}
			character const* rend() const
			{
				return size==0?begin:begin-size;
			}
		}; /// end of name data_t
		typedef cdata_tt<char> cdata_t;
		typedef data_tt<char> data_t;
		typedef cdata_tt<wchar_t> wcdata_t;
		typedef data_tt<wchar_t> wdata_t;

		template <typename kt,typename vt>
		struct kv_tt
		{
			typedef cdata_tt<kt> key_t;
			typedef cdata_tt<vt> value_t;
			key_t key;
			value_t value;
		};
		typedef kv_tt<char,char> kvch_t;
	}
}