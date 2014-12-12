#include "nc_drawing_functions.hh"
//#include <set>
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

	// the point where the upper-left part of the room is
	const pos_t displ{win_pos + pos_t(0,1)};

	// write the name
	mvwprintw(win, win_pos.y, win_pos.x, room->get_name().c_str());
	draw_rectangle(win, displ, room->get_dim(), bor);

	//const std::set<Room::room_tr>& transitions = room->get_room_trs();
	// room transitions
	for (const Room::room_tr& rtr : room->get_room_trs()) {
		pos_t draw_to{rtr.area_from.pos1 + displ};
		mvwaddch(win, draw_to.y, draw_to.x, ACS_CKBOARD);
	}

	// objects
	const boost::ptr_set<world_object>& objs = room->get_objects();
	for (boost::ptr_set<world_object>::const_iterator cit = objs.cbegin(); cit != objs.cend(); cit++) {
		pos_t draw_to1{cit->get_pos() + displ};
		pos_t draw_to2{draw_to1 + cit->get_dim() - pos_t{1,1}};
		//pos_t draw_to{0,0};
		try {
			const door& d = dynamic_cast<door&>(const_cast<world_object&>(*cit));
			chtype ch_to_draw1;
			chtype ch_to_draw2;

			bool is_vertical = d.get_is_vertical();
			if (is_vertical) {
				ch_to_draw1 = d.get_is_closed() ? 'I' : ACS_S1;
			} else {
				ch_to_draw1 = d.get_is_closed() ? '=' : '/';
			}
			mvwaddch(win, draw_to1.y, draw_to1.x, ch_to_draw1);

			if (d.get_length() == 2) {
				if (is_vertical) {
					ch_to_draw2 = d.get_is_closed() ? 'I' : ACS_S9;
				} else {
					ch_to_draw2 = d.get_is_closed() ? '=' : '\\';
				}
				mvwaddch(win, draw_to2.y, draw_to2.x, ch_to_draw2);
			}

		} catch (std::bad_cast) {
		}
	}
}

