#ifndef NC_DRAWING_FUNCTIONS_HH
#define NC_DRAWING_FUNCTIONS_HH
//#include "coordinates.hh"
#include "room.hh"
#include <ncurses.h>
#include <string>
#include <assert.h>

using namespace Room_System;
using namespace Coordinates;

class Ascii_drawer {
public:
	class vis_array {
	public:
		vis_array(const unsigned rows, const unsigned cols, pos_t offset = pos_t{}):
			rows{rows}, cols{cols}, offset{offset} {//row_offset{0}, col_offset{0} {

			array = new char[this->rows * this->cols];
			clear();

		}
		/*vis_array(WINDOW* src_win, pos_t offset):
			rows{(unsigned)getmaxy(src_win)}, cols{(unsigned)getmaxx(src_win)}, offset{offset} {
			//getmaxyx(src_win, rows, cols);

			array = new char[rows * cols];
			clear();
		}*/

		~vis_array() {
			delete[] array;
		}

		void fill(const char c);
		inline void clear() { fill(empty_char); }

		inline void put(const unsigned x, const unsigned y, const char c) const {
			//assert (x < cols); assert(y < rows);
			if (x < cols && y < rows) array[x + y*cols] = c;
			//else TODO: create a new array
		}

		inline char at(const unsigned x, const unsigned y) const {
			assert (x <= cols); assert(y <= rows);
			return array[x + y*cols];
		}

		char at_win(const int x, const int y, WINDOW* src_win) const;

		inline bool in_area(const unsigned x, const unsigned y) const {
			return (y < rows) && (x < cols);
		}

		void draw_room(const Room* const r, const pos_t& pos);
		//void update(Player* PC);

		void shift(pos_t offset) {//int horizontal, int vertical) {
			//row_offset += vertical;
			//col_offset += horizontal;
			this->offset += offset;
		}
		void set_offset(pos_t offset) {
			this->offset = offset;
		}

		inline dim_t get_dim() const {
			return dim_t(cols, rows);
		}
		inline pos_t get_max_pos() const {
			return pos_t(cols-1, rows-1);
		}

		//const size_t rows;
		//const size_t cols;
		const unsigned rows;
		const unsigned cols;

		//long row_offset;
		//long col_offset;
		pos_t offset;
	private:
		char* array;
	};

	Ascii_drawer(void* const eh_ptr, WINDOW* const win,
				 Entity* const anchor, char anchor_char, pos_t offset = pos_t()):

		eh_ptr{eh_ptr}, anchor{anchor},
		// prev_room_ptr{},
		
		anchor_pos{dynamic_cast<Comps::Position&>(anchor->get_component_with_name("position")).get_value_ref()},
		prev_anchor_pos{anchor_pos},
		anchor_pos_in_screen{win->_maxx/2, win->_maxy/2},
		anchor_obj_char{anchor_char},
		
		src_win{win},
		varr{(unsigned)win->_maxy, (unsigned)win->_maxx, offset}
		//varr{20,20, offset}
	{
		update(true);
	}

	inline void fill_vis_array(const char c) { varr.fill(c); }
	inline void clear_vis_array() { fill_vis_array(empty_char); }

	void draw_vis_array();

	void update(bool update_all = false);
	//void shift_vis_array(int horizontal, int vertical);
	inline void shift_vis_array(pos_t offset) { varr.shift(offset); }
	inline void set_vis_array_offset(pos_t offset) { varr.set_offset(offset); }

	inline void shift_vis_array_and_update(pos_t offset) {
		shift_vis_array(offset); update();
	}

private:
	static const char empty_char = ' ';
	static const char fog_char = '~';
	static const char wall_char = '#';
	static const char floor_char = '.';

	//static const char PC_char = '@';

	//Event_handler* const eh_ptr;
	void* const eh_ptr; // DEPRECATED currently

	Entity* const anchor;
	//Room* prev_room_ptr;
	
	// the position which we are anchored to
	pos_t& anchor_pos;
	pos_t prev_anchor_pos;
	
	// the position where we want the anchored position to be in in the screen
	pos_t anchor_pos_in_screen;
	char anchor_obj_char;

	WINDOW* src_win;
	vis_array varr;

	//std::stringstream debug_msg;
};


struct nc_border_t {
	chtype	ls, rs, ts, bs; // sides
	chtype	tl, tr, bl, br; // corners
};

// put this only after the ncurses has done it's initializations
#define MY_NCDF_HELPER_BORDERS \
	const nc_border_t bor_empty\
	{' ',' ',' ',' ',\
	 ' ',' ',' ',' '};\
	const nc_border_t bor_ascii\
	{'|','|','-','-',\
	 '+','+','+','+'};\
	const nc_border_t bor_rectangle\
	{ACS_VLINE,ACS_VLINE, ACS_HLINE,ACS_HLINE,\
	 ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER};\
	const nc_border_t bor_rectangle_wplus\
	{ACS_VLINE,ACS_VLINE, ACS_HLINE,ACS_HLINE,\
	 ACS_PLUS, ACS_PLUS, ACS_PLUS,ACS_PLUS};

#define BOR_RECTANGLE \
	const nc_border_t bor_rectangle\
	{ACS_VLINE,ACS_VLINE, ACS_HLINE,ACS_HLINE,\
	 ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER};
#define BOR_ASCII \
	const nc_border_t bor_ascii\
	{'|','|','-','-',\
	 '+','+','+','+'};\
//---

char debug_uint_to_char(unsigned i);
//void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor);

#endif // NC_DRAWING_FUNCTIONS_HH
