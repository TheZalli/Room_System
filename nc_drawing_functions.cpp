#include "nc_drawing_functions.hh"
#include <cmath>
#include <vector>
//#include <set>
//#include <stdlib.h>
//#include <boost/ptr_container/ptr_set.hpp>

void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor) {
	const int& x{pos.x};
	const int& y{pos.y};
	const int& w{(int)dim.w};
	const int& h{(int)dim.l};

	mvwaddch(win, y, x,			bor.tl);
	mvwaddch(win, y, x + w,		bor.tr);
	mvwaddch(win, y + h, x,		bor.bl);
	mvwaddch(win, y + h, x + w,	bor.br);

	mvwhline(win, y, x + 1,		bor.ts, w - 1);
	mvwhline(win, y + h, x + 1, bor.bs, w - 1);
	mvwvline(win, y + 1, x,		bor.ls, h - 1);
	mvwvline(win, y + 1, x + w,	bor.rs, h - 1);
}

void draw_rectangle(WINDOW* win, const area_t& area, const nc_border_t& bor)
{
	const int& x1{area.pos1.x};
	const int& y1{area.pos1.y};
	const int& x2{area.pos2.x};
	const int& y2{area.pos2.y};
	const int& w{(int)area.get_dim().w};
	const int& h{(int)area.get_dim().l};

	mvwaddch(win, y1, x1,		bor.tl);
	mvwaddch(win, y1, x2,		bor.tr);
	mvwaddch(win, y2, x1,		bor.bl);
	mvwaddch(win, y2, x2,		bor.br);

	mvwhline(win, y1, x2,		bor.ts, w - 1);
	mvwhline(win, y2, x1 + 1,	bor.bs, w - 1);
	mvwvline(win, y1 + 1, x1,	bor.ls, h - 1);
	mvwvline(win, y1 + 1, x2,	bor.rs, h - 1);

}

void draw_room(WINDOW* win, const Room* room, const pos_t& win_pos, const nc_border_t& bor)
{
	assert(room);

	// the point where the upper-left part of the room wall is
	const pos_t displ{win_pos + pos_t(0,1)};

	// write the name
	mvwprintw(win, win_pos.y, win_pos.x+1, room->get_name().c_str());
	draw_rectangle(win, displ, room->get_dim() + dim_t(2,2), bor);

	// objects
	// the floor starting location:
	const pos_t floor_start{displ + pos_t(1,1)};

	const Room::room_obj_set& objs = room->get_objects();
	if (objs.size() == 0) return;

	for (Room::room_obj_set::const_iterator cit = objs.cbegin(); cit != objs.cend(); cit++) {
		World_object* const obj_ptr = *cit;

		const pos_t draw_to1{obj_ptr->get_pos() + floor_start};
		const pos_t draw_to2{draw_to1 + obj_ptr->get_dim()};
		//pos_t draw_to{0,0};
		try {
			if (typeid(*obj_ptr) == typeid(door)) {
				const door& d = dynamic_cast<door&>(const_cast<World_object&>(*obj_ptr));
				chtype ch_to_draw1;
				chtype ch_to_draw2;

				bool is_vertical = d.get_is_vertical();
				if (is_vertical) {
					ch_to_draw1 = d.get_is_closed() ? 'I' : '-'; //ACS_S1;
				} else {
					ch_to_draw1 = d.get_is_closed() ? '=' : '/';
				}
				mvwaddch(win, draw_to1.y, draw_to1.x, ch_to_draw1);

				if (d.get_length() == 2) {
					if (is_vertical) {
						ch_to_draw2 = d.get_is_closed() ? 'I' : '-';//ACS_S9;
					} else {
						ch_to_draw2 = d.get_is_closed() ? '=' : '\\';
					}
					mvwaddch(win, draw_to2.y, draw_to2.x, ch_to_draw2);
				}
			} else if (typeid(*obj_ptr) == typeid(PC)) {
				const PC& pc = dynamic_cast<PC&>(const_cast<World_object&>(*obj_ptr));

				mvwaddch(win, draw_to1.y, draw_to1.x, '@');

			}
		} catch (std::bad_typeid) {
			assert(0);
		}
	}

	// highlights room transitions
	for (const Room::room_tr& rtr : room->get_room_trs()) {
		const pos_t draw_to1{rtr.area_from.pos1 + floor_start};
		const pos_t draw_to2{rtr.area_from.pos2 + floor_start};

		mvwchgat(win, draw_to1.y, draw_to1.x, 1, A_NORMAL, 2, NULL);
		mvwchgat(win, draw_to2.y, draw_to2.x, 1, A_NORMAL, 2, NULL);

		//mvwaddch(win, draw_to1.y, draw_to1.x, ACS_CKBOARD);
		//mvwaddch(win, draw_to2.y, draw_to2.x, ACS_CKBOARD);
	}
}

char debug_uint_to_hex_char(unsigned i) {
	if (i < 10)	return (char)((int)'0' + i);
	else if (i < 26-1-10)	return (char)((int)'A' + i - 10);
	else return '-';
}

void view_draw(WINDOW* win, const PC* player, const pos_t& players_pos_in_win)
{
	// a struct that represents the backtraced paths of fotons, for the ray-casting algorithm
	struct view_ray {
		pos_t travel_pos;
	};

	if (!player) return; // there is no player
	mvwaddch(win, players_pos_in_win.y, players_pos_in_win.x, '@');

	const Room* const players_room = player->get_room();
	if (!players_room) return; // the player resides in the void of nullness


	const Room::room_obj_set& objects = players_room->get_objects();

	// the opaque areas of the room
	std::vector<area_t> opaque_areas{}; // OPTIMIZATION: this could be cached and updated only when the room is changed

	for (World_object* const obj_ptr : players_room->get_objects()) {
		if (obj_ptr->is_opaque() && (typeid(*obj_ptr) != typeid(PC)))
			opaque_areas.push_back(obj_ptr->get_area());
	}

	// the room transitions
	//const std::set<Room::room_tr>& room_transitions{players_room->get_room_trs()};

	/*/ testing:

	const pos_t& p{player->get_pos()};
	pos_t draw_pos;
	for (dist_t x = 1; x <= 10; ++x) {
		for (dist_t y = 0; y <= 10; ++y) {
			draw_pos = players_pos_in_win + pos_t{x,y};

			unsigned value = std::sqrt(x*x + y*y);

			wmove(win, draw_pos.y, draw_pos.x);
			if (value%2) attron(A_REVERSE);
			else attroff(A_REVERSE);
			waddch(win, debug_uint_to_hex_char(value));
			//mvwchgat(win, draw_pos.y, draw_pos.x, 1, A_NORMAL, 2, NULL);
		}
	}*/

	// calculates the rooms we need to draw
	// OPTIMIZATION: set the number of maximum room transitions and then use a circular stack of that size as next_to_inspect
	Room* const inspected_room{const_cast<Room* const>(players_room)};
	std::vector<Room*> now_inspecting;
	std::vector<Room*> next_to_inspect;

	std::vector<Room*> rooms_to_draw;

	for (const Room::room_tr& rtr : inspected_room->get_room_trs()) {
		if (rtr.obj_associated && !rtr.obj_associated->is_opaque()) { // if the object breaks the line of sight
			rooms_to_draw.push_back((rtr.leads_to));
			next_to_inspect.push_back((rtr.leads_to)); //const_cast<Room* const>
		}
	}

}
