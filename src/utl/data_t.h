

#pragma once


namespace ox
{
	namespace utl
	{

		struct data_t
		{
			char* begin;
			size_t size;
			data_t()
				:begin(0),size(0)
			{}
			data_t(char* _data,size_t _size)
				: begin(_data),size(_size)
			{}
			void assign(data_t const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(char* data,size_t size)
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
		}; /// end of name data_t
		struct cdata_t
		{
			char const* begin;
			size_t size;
			cdata_t()
				:begin(0),size(0)
			{}
			cdata_t(char const* _data,size_t _size)
				: begin(_data),size(_size)
			{}
			cdata_t(data_t const& data)
				: begin(data.begin), size(data.size)
			{}
			void assign(cdata_t const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(data_t const& data)
			{
				begin = data.begin;
				size = data.size;
			}
			void assign(char const* data,size_t size)
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
			char const* end() const
			{
				return begin+size;
			}
			bool operator == (cdata_t const& other) const
			{
				return begin==other.begin && size==other.size;
			}
			bool operator != (cdata_t const& other) const
			{
				return begin!=other.begin || size!=other.size;
			}
			char const* rbegin() const
			{
				return size==0?begin:begin+size-1;
			}
			char const* rend() const
			{
				return size==0?begin:begin-size;
			}
		}; /// end of name data_t
	}
}