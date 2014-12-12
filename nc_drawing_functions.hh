#ifndef NC_DRAWING_FUNCTIONS_HH
#define NC_DRAWING_FUNCTIONS_HH
#include "coordinates.hh"
#include "room.hh"
#include <ncurses.h>
#include <assert.h>

using namespace Room_System;
using namespace Coordinates;

struct nc_border_t {
	chtype	ls, rs, ts, bs; // sides
	chtype	tl, tr, bl, br; // corners
};

// put tjis after the ncurses has done it's initializations
#define MY_NCDF_HELPER_BORDERS \
	const nc_border_t bor_empty\
	{' ',' ',' ',' ',\
	 ' ',' ',' ',' '};\
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
//---

void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor);
void draw_rectangle(WINDOW* win, const area_t& area, const nc_border_t& bor);

void draw_room(WINDOW* win, const Room* room, const pos_t& win_pos, const nc_border_t& bor);

#endif // NC_DRAWING_FUNCTIONS_HH
