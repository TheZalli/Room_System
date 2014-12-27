#ifndef NC_DRAWING_FUNCTIONS_HH
#define NC_DRAWING_FUNCTIONS_HH
#include "coordinates.hh"
#include "room.hh"
#include "pc.hh"
#include <ncurses.h>
#include <assert.h>

using namespace Room_System;
using namespace Coordinates;

struct nc_border_t {
	chtype	ls, rs, ts, bs; // sides
	chtype	tl, tr, bl, br; // corners
};

// put this after the ncurses has done it's initializations
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

void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor);
void draw_rectangle(WINDOW* win, const area_t& area, const nc_border_t& bor);

// a debug room drawing function
void draw_room(WINDOW* win, const Room* room, const pos_t& win_pos, const nc_border_t& bor);

void vd_draw_obj(WINDOW* win, const World_object* const obj_ptr, const pos_t& draw_pos);
void vd_draw_room(WINDOW* win, const Room* const room, const pos_t& pos_in_win, bool draw_only_outlines);
void view_draw(WINDOW* win, const PC* player, const pos_t& players_pos_in_win);

#endif // NC_DRAWING_FUNCTIONS_HH
