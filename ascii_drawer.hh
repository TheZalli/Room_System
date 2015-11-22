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
	class Vis_array {
	public:
		Vis_array(const unsigned rows, const unsigned cols, std::stringstream* debug_msgs_ptr = nullptr,
				  const pos_t& offset = pos_t{});

		~Vis_array();

		void fill(const char c);
		void put(const unsigned x, const unsigned y, const char c) const;
		char at(const unsigned x, const unsigned y) const;
		char at_win(const int x, const int y, WINDOW* src_win) const;
		bool in_area(const unsigned x, const unsigned y) const;

		void draw_room(const Room* const r, const pos_t& pos);
		void LOS_draw_room(const Room* const r, Room::room_tr const* room_tr_from, const Comps::Position& pos_from,
						   pos_t pos_in_varr) throw(std::invalid_argument);

		void shift(pos_t offset);
		
		pos_t get_offset() const;
		void set_offset(pos_t offset);

		dim_t get_dim() const;
		pos_t get_max_pos() const;

	private:
		const unsigned rows;
		const unsigned cols;
		
		pos_t offset;
		char* array;
		
		std::stringstream* debug_msgs_ptr;
	};
	
	//-------
	
	Ascii_drawer(void* const eh_ptr, WINDOW* const win,
				 Entity* const anchor, char anchor_char, const pos_t& offset = pos_t());
	
	void fill_vis_array(const char c);
	void clear_vis_array();
	
	void draw_vis_array();
	
	/**
	 * @brief update updates the vis_array var
	 * @param update_all True if we want to update all, false when we just shift the varr.
	 */
	void update(bool update_all = false);
	
	//void shift_vis_array(int horizontal, int vertical);
	void shift_vis_array(pos_t offset);
	void set_vis_array_offset(pos_t offset);
	void shift_vis_array_and_update(pos_t offset);
	
	Room* const& get_room_the_anchor_is_in() const;
	
	void add_debug_message(const std::string& msg);
	void print_debug_messages(bool clear_msgs = true);
	
private:
	static const char empty_char = ' ';
	static const char fog_char = '~';
	static const char wall_char = '#';
	static const char floor_char = '.';

	//static const char PC_char = '@';

	//Event_handler* const eh_ptr;
	void* const eh_ptr; // DEPRECATED currently

	Entity* const anchor;
	
	// the position which we are anchored to
	const Comps::Movable_position& anchor_pos_comp;
	
	// the position where we want the anchored position to be in in the screen
	pos_t anchor_pos_in_screen;
	char anchor_obj_char;
	
	std::stringstream debug_msgs;
	
	WINDOW* src_win;
	Vis_array varr;
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
	 '+','+','+','+'};
//---

char debug_uint_to_char(unsigned i);
//void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor);

#endif // NC_DRAWING_FUNCTIONS_HH
