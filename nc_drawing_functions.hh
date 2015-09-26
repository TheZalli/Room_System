#ifndef NC_DRAWING_FUNCTIONS_HH
#define NC_DRAWING_FUNCTIONS_HH
//#include "coordinates.hh"
#include "room.hh"
#include "Components/components_include.hh"
#include "entity.hh"

#include <ncurses.h>
#include <string>
#include <sstream>
#include <assert.h>

using namespace Room_System;
using namespace Coordinates;

class Ascii_drawer {
public:
	class vis_array {
	public:
		vis_array(const unsigned rows, const unsigned cols, std::stringstream* debug_msgs_ptr = nullptr, const pos_t& offset = pos_t{}):
			rows{rows}, cols{cols}, offset{offset}, debug_msgs_ptr{debug_msgs_ptr}
		{
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
		void LOS_draw_room(const Room* const r, Room::room_tr const* room_tr_from, const Comps::Position& pos_from, pos_t pos_in_varr) throw(std::invalid_argument);
		//void update(Player* PC);

		void shift(pos_t offset) {//int horizontal, int vertical) {
			//row_offset += vertical;
			//col_offset += horizontal;
			this->offset += offset;
		}
		
		pos_t get_offset() const {
			return offset;
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

	private:
		//const size_t rows;
		//const size_t cols;
		const unsigned rows;
		const unsigned cols;
		
		pos_t offset;
		char* array;
		
		std::stringstream* debug_msgs_ptr;
	};

	Ascii_drawer(void* const eh_ptr, WINDOW* const win,
				 Entity* const anchor, char anchor_char, const pos_t& offset = pos_t()):

		eh_ptr{eh_ptr}, anchor{anchor},
		// prev_room_ptr{},
		
		anchor_pos_comp{dynamic_cast<Comps::Movable_position&>(anchor->get_component_with_name("position"))},
		
		prev_anchor_room{anchor_pos_comp.get_room_ptr()},
		
		anchor_pos_in_screen{win->_maxx/2, win->_maxy/2},
		//anchor_pos_in_screen{15, 15},
		anchor_obj_char{anchor_char},
		
		debug_msgs{},
	  
		src_win{win},
		//varr{(unsigned)win->_maxy, (unsigned)win->_maxx, &debug_msgs, offset}
		varr{128, 128, &debug_msgs, offset}
		
	{
		update(true);
	}

	inline void fill_vis_array(const char c) { varr.fill(c); }
	inline void clear_vis_array() { fill_vis_array(empty_char); }

	void draw_vis_array();
	
	/**
	 * @brief update updates the vis_array var
	 * @param update_all True if we want to update all, false when we just shift the varr.
	 */
	void update(bool update_all = false);
	
	//void shift_vis_array(int horizontal, int vertical);
	inline void shift_vis_array(pos_t offset) { varr.shift(offset); }
	inline void set_vis_array_offset(pos_t offset) { varr.set_offset(offset); }

	inline void shift_vis_array_and_update(pos_t offset) {
		shift_vis_array(offset); update();
	}
	
	Room* const& get_room_the_anchor_is_in() const;
	
	void add_debug_message(const std::string& msg) {
		debug_msgs << msg << std::endl;
	}
	
	void print_debug_messages(bool clear_msgs = true) {
		move(0,0);
		printw(debug_msgs.str().c_str());
		if (clear_msgs) debug_msgs.str(std::string());
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
	const Comps::Movable_position& anchor_pos_comp;
	
	Room* prev_anchor_room;
	
	// the position where we want the anchored position to be in in the screen
	pos_t anchor_pos_in_screen;
	char anchor_obj_char;
	
	std::stringstream debug_msgs;
	
	WINDOW* src_win;
	vis_array varr;
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
