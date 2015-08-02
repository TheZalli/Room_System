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
	//debug_msgs << "pos: " << anchor_pos.to_string() << " old pos: " << prev_anchor_pos.to_string() << std::endl;
	
	if (update_all) {
		prev_anchor_room = anchor_pos_comp.get_room_ptr();
		
		clear_vis_array();
		varr.set_offset({0,0});
		varr.draw_room(anchor_pos_comp.get_room_ptr(), anchor_pos_in_screen - anchor_pos_comp.get_pos()); // note: anchor_pos is local pos in room
		
		const Room::room_tr_vector& trs = anchor_pos_comp.get_room_ptr()->get_room_trs();
		for (Room::room_tr_vector::const_iterator it = trs.begin();
			 it != trs.cend(); ++it)
		{
			varr.draw_room(it->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() + it->area_from.get_pos1() - it->pos_to);
		}
		
	} else {
		shift_vis_array(anchor_pos_comp.get_last_move());
		
		if(prev_anchor_room != anchor_pos_comp.get_room_ptr()) // room shift happened
		{
			Room::room_tr const* rtr_ptr = anchor_pos_comp.get_used_room_tr();
			
			const Room::room_tr_vector& trs = anchor_pos_comp.get_room_ptr()->get_room_trs();
			
			for (Room::room_tr_vector::const_iterator it = trs.begin();
				 it != trs.cend(); ++it)
			{
				if (*rtr_ptr != *it->get_reverse_tr()) // do not draw the room we came from again
					varr.draw_room(it->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() + it->area_from.get_pos1() - it->pos_to + varr.get_offset());
			}
			
			prev_anchor_room = anchor_pos_comp.get_room_ptr();
		}
	}
	
	debug_msgs << "varr offset: " << varr.get_offset().to_string() << " all updated: " << update_all << std::endl;
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
			else {
				put(x + pos.x, y + pos.y, floor_char);
			}
		}
	}

	for (const Room::room_tr& rtr : r->get_room_trs()) {
		pos_t pos1 = rtr.area_from.get_pos1();
		//*debug_msgs_ptr << "rtr pos: " << pos1.to_string() << std::endl;
		put(pos.x + pos1.x, pos.y + pos1.y, floor_char);

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

/*Room* Ascii_drawer::get_room_the_anchor_is_in() const
{
	Comps::Position& p = dynamic_cast<Comps::Position&>(
							 anchor->get_component_with_name(std::string("position")) );
	return p.get_room_ptr();
}*/



