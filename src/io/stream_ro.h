

#pragma once

namespace ox
{
	namespace io
	{
		/// random access a stream
		template <typename cell_tn, typename object_tn>
		struct stream_ro_random_tt
		{
			typedef object_tn object_t;
			typedef cell_tn cell_t;
			struct position;
			struct offset_t;

			bool mount (object_t & _in_object);
			bool umount ();
			position tell() const;
			bool skip (offset_t);
			bool jump(position);
			
			/// after getting, move the pointer to the next
			size_t get(cell_t * _outval_cells, size_t const & _inconref_cells_count); // 取units
			size_t get(cell_t & _outval_cell); // 取一个cell
			bool is_eof() const;
		}; /// end of stream ro
	} /// end of io
} /// end of ox
