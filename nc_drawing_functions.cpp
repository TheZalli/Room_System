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

void vd_draw_obj(WINDOW* win, const World_object* const obj_ptr, const pos_t& draw_pos)
{
	const pos_t draw_pos2 = draw_pos + obj_ptr->get_dim();
	//pos_t draw_pos{0,0};
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
			mvwaddch(win, draw_pos.y, draw_pos.x, ch_to_draw1);

			if (d.get_length() == 2) {
				if (is_vertical) {
					ch_to_draw2 = d.get_is_closed() ? 'I' : '-';//ACS_S9;
				} else {
					ch_to_draw2 = d.get_is_closed() ? '=' : '\\';
				}
				mvwaddch(win, draw_pos2.y, draw_pos2.x, ch_to_draw2);
			}
		} /*else if (typeid(*obj_ptr) == typeid(PC)) {
			const PC& pc = dynamic_cast<PC&>(const_cast<World_object&>(*obj_ptr));

			mvwaddch(win, draw_pos.y, draw_pos.x, '@');

		}*/
	} catch (std::bad_typeid) {
		assert(0);
	}
}


void Ascii_drawer::update(bool update_all)
{
	if (update_all) {
		clear_vis_array();
		varr.draw_room(PC_ptr->get_room(), PC_pos - PC_ptr->get_pos() - pos_t{1,1}); // note: get_pos gives local pos in room
	} else {
		//varr.put(prev_PC_pos.x, prev_PC_pos.y, floor_char);

		//if (prev_room_ptr != PC_ptr->get_room()) {
		std::vector<world_event> etrr_vector = eh_ptr->get_events_by_obj_n_type(PC_ptr, e_tr_room_id);
		//mvwaddch(src_win, 0,0, debug_uint_to_char(etrr_vector.size())); // DEBUG
		eh_ptr->debug_msg << std::string("tr_room events: ") << etrr_vector.size() << std::endl;
		if (etrr_vector.size() != 0) {
			eh_ptr->debug_msg << std::string("tr: ") << static_cast<e_tr_room&>(etrr_vector[0]).rtr.to_string();
			varr.draw_room(PC_ptr->get_room(), PC_pos - PC_ptr->get_pos() - pos_t{1,1});

			prev_room_ptr = PC_ptr->get_room();
		}
	}
	//varr.put(PC_pos.x, PC_pos.y, PC_char);

	prev_PC_pos = PC_pos;
}

void Ascii_drawer::draw_vis_array()
{
	/*for (unsigned y = 0; y < varr.rows; ++y) {
		for (unsigned x = 0; x < varr.cols; ++x) {
			//mvwaddch(src_win, j + varr.col_offset, i + varr.col_offset, varr.at(i,j));
			move(y + varr.offset.y, x + varr.offset.x);
			waddch(src_win, varr.at(x,y));

		}
	}*/
	for (int y = src_win->_begy; y <= src_win->_maxy; ++y) {
		for (int x = src_win->_begx; x <= src_win->_maxx; ++x) {
			//mvwaddch(src_win, j + varr.col_offset, i + varr.col_offset, varr.at(i,j));
			move(y, x);
			waddch(src_win, varr.at_win(x, y, src_win));
		}
	}
	mvwaddstr(src_win, 0, 0, eh_ptr->debug_msg.str().c_str()); // DEBUG
	eh_ptr->debug_msg.str(std::string());

	mvwaddch(src_win, PC_pos.y, PC_pos.x, PC_char);
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



