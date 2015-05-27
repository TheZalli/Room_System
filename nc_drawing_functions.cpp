#include "nc_drawing_functions.hh"

//vis_array varr(10, 10);

/*void draw_rectangle(WINDOW* win, const pos_t& pos, const dim_t& dim, const nc_border_t& bor) {
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
}*/

char debug_uint_to_char(unsigned i) {
	if (i < 10)
		return (char)((int)'0' + i);
	else if (i < 26+10)
		return (char)((int)'A' + i - 10);
	else if (i < 26+10+26)
		return (char)((int)'a' + i - 10-26);
	else return '-';
}

void Ascii_drawer::update(bool update_all)
{
	if (update_all) {
		clear_vis_array();
		varr.draw_room(anchor->get_room_in(), anchor_pos_in_screen - anchor_pos); // note: anchor_pos is local pos in room
	} else {
		if (prev_anchor_pos != anchor_pos) { // we have moved
			shift_vis_array(anchor_pos - prev_anchor_pos);
		}
	}
	//varr.put(anchor_pos_in_screen.x, anchor_pos_in_screen.y, anchor_obj_char);
	
	prev_anchor_pos = anchor_pos;
}

void Ascii_drawer::draw_vis_array()
{
	for (int y = src_win->_begy; y <= src_win->_maxy; ++y) {
		for (int x = src_win->_begx; x <= src_win->_maxx; ++x) {
			move(y, x);
			waddch(src_win, varr.at_win(x, y, src_win));
		}
	}

	mvwaddch(src_win, anchor_pos_in_screen.y, anchor_pos_in_screen.x, anchor_obj_char);
}

void Ascii_drawer::vis_array::fill(const char c)
{
	for (unsigned y = 0; y < rows; ++y) {
		for (unsigned x = 0; x < cols; ++x) {
			put(x,y, c);
			//put(x, y, debug_uint_to_char(std::min(x,y)) ); // DEBUG
		}
	}
}

char Ascii_drawer::vis_array::at_win(const int x, const int y, WINDOW* src_win) const {
	assert((y <= src_win->_maxy) && (x <= src_win->_maxx) && (y >= src_win->_begy) && (x >= src_win->_begx));

	if (((unsigned)(y + offset.y) < rows) && ((unsigned)(x + offset.x) < cols))
		//return array[((size_t)y + offset.y) + ((size_t)x + offset.x)*cols];
		return at(x + offset.x, y + offset.y);
	else return empty_char;
}

void Ascii_drawer::vis_array::draw_room(const Room* const r, const pos_t& pos)
{
	for (unsigned y = 0; y <= r->get_dim().l + 1; ++y) {
		for (unsigned x = 0; x <= r->get_dim().w + 1; ++x) {
			if ((x == 0) || (x == r->get_dim().w + 1) ||
				(y == 0) || (y == r->get_dim().l + 1)) {

				put(x + pos.x, y + pos.y, wall_char);
			}
		}
	}

	for (const Room::room_tr& rtr : r->get_room_trs()) {
		pos_t pos1 = rtr.area_from.get_pos1();
		pos_t pos2; // = rtr.area_from.get_pos2();

		put(pos.x + pos1.x + 1, pos.y + pos1.y + 1, 'd');
		if ((pos2 = rtr.area_from.get_pos2()) != pos1) {
			put(pos.x + pos2.x + 1, pos.y + pos2.y + 1, 'd');
		}

	}

	/*if((pos.min() >= 0) &&
	   (pos.x + r->get_dim().w <= rows) &&
	   (pos.y + r->get_dim().l <= cols)) {

		const unsigned low_x = pos.x;
		const unsigned low_y = pos.y;
		const unsigned high_x = pos.x + r->get_dim().w + 1;
		const unsigned high_y = pos.y + r->get_dim().l + 1;

		for (unsigned y = low_y; y <= high_y; ++y) {
			for (unsigned x = low_x; x <= high_x; ++x) {
				if ((y == low_y) || (x == low_x) ||
					(y == high_y) || (x == high_x))
				{
					const Room::room_tr& rtr =
							r->get_room_tr({(signed)x - (signed)low_x, (signed)y - (signed)low_y});
					if (rtr == Room::none_room_tr) put(x,y, wall_char);
					else put(x,y, 'd');
				}
				else {
					put(x,y, floor_char);
				}
			}
		}
	}*/

}



