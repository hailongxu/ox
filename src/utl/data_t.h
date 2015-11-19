

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
			template <size_t n>
			explicit data_tt(character(&array)[n])
			{
				begin = array;
				size = n;
			}
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
			void set_intv(character* begin, character* end)
			{
				this->begin = begin;
				this->size = end-begin;
			}
			bool is_empty() const
			{
				return begin==0 || size==0;
			}
			void clear()
			{
				begin=0,size=0;
			}
			void set_end(character const& c=0)
			{
				*end() = c;
			}
			bool is_end_eq(character const& c=0)
			{
				return *end() == c;
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
			template <size_t n>
			explicit cdata_tt(character const(&array)[n])
			{
				begin = array;
				size = n;
			}
			template <size_t n>
			explicit cdata_tt(character(&array)[n])
			{
				begin = array;
				size = n;
			}
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
			void set_intv(character const* begin, character const* end)
			{
				this->begin = begin;
				this->size = end-begin;
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

		template <typename character>
		struct buff_append_tt
		{
			buff_append_tt(character* ptr):p(ptr){}
			void push_back(character const& c) {*p++ = c;}
			character* p;
		};
		typedef buff_append_tt<char> buff_append;
		typedef buff_append_tt<wdata_t> wbuff_append;

		template <typename character>
		struct buff_data_tt
		{
			typedef data_tt<character> data_t;
			data_t _m_buffer;
			size_t _m_size;
			template <size_t n>
			buff_data_tt(character(&array)[n]): _m_buffer(array) {}
			buff_data_tt(character* ptr,size_t size):_m_buffer(ptr,size),_m_size(0){}
			buff_data_tt(data_t const& data):_m_buffer(data),_m_size(0) {}
			void push_back(character const& c)
			{assert(_m_size<_m_buffer.size);_m_buffer.begin[_m_size++] = c;}
			size_t size() const {return _m_size;}
			size_t capacity() const {return _m_buffer.size;}
			character* data() {return _m_buffer.begin;}
			character const* data() const {return _m_buffer.begin;}
			void resize(size_t size)
			{
				_m_buffer.size>=size ? _m_size=size : 0;
			}
			data_t as_data() const {return data_t(_m_buffer.begin,_m_size);}
		};
		typedef buff_data_tt<char> buff_data;
		typedef buff_data_tt<wdata_t> wbuff_data;
	}
}