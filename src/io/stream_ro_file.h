
#include "./stream_ro.h"
#include "./file.h"

#pragma once

namespace ox
{
	namespace io
	{
		template <typename cell_tn>
		class stream_ro_random_tt<cell_tn,ox::io::file<cell_tn>>
		{
		public:
			typedef cell_tn cell_t;
			typedef ox::io::file<cell_t> object_t;
			typedef stream_ro_random_tt<cell_t,ox::io::file<cell_t>> self;

			typedef int position;
			typedef long offset_t;

			//static const size_t __alloc_init_size = 3;

			self () : _m_file (0)
			{}

			self (object_t & _inref_object)
				: _m_file (&_inref_object)
			{}

			void mount(object_t & _inref_object)
			{
				_m_file = &_inref_object;
			}
			void umount()
			{
				_m_file = 0;
			}
			position tell() const
			{
				assert(_m_file);
				return _m_file->tell();
			}
			bool skip (offset_t const & _inconref_offset) const
			{
				assert(_m_file);
				return 0==_m_file->seek (
					file_helper::__seek_currrent, _inconref_offset);
			}
			bool jump (position const & _inconref_pos) const
			{
				assert(_m_file);
				return 0 == _m_file->seek (
					file_helper::__seek_begin, _inconref_pos);
			}
			///
			///	after read, move the indicator to the next unit address
			///
			size_t get(cell_t * _inptr_cells, size_t const & _inconref_cells_count) const
			{
				assert(_m_file);
				return _m_file->read(_inptr_cells,sizeof(cell_t),_inconref_cells_count);
			}
			size_t get(cell_t & _inref_cell) const
			{
				assert(_m_file);
				_inref_cell = _m_file->getc();
				return is_eof()? 0 : 1;
			}

			bool is_eof() const
			{
				assert(_m_file);
				return _m_file->eof();
			}

		private:
			object_t * _m_file;
		}; /// end of stream ro file
	} /// end of io
} /// end of ox
