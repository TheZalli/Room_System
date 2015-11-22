#include "ascii_drawer.hh"
#include <iostream>

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

/**
 * @brief debug_uint_to_char returns the character representation of i in range 0-9A-Za-z and '-' if is is out of
 * range.
 * @param i an unsigned integer between between 0 and 61
 * @return the single character representation of i
 */
char debug_uint_to_char(unsigned i) {
	if (i < 10)
		return (char)((int)'0' + i);
	else if (i < 26 + 10)
		return (char)((int)'A' + i - 10);
	else if (i < 26 + 10 + 26)
		return (char)((int)'a' + i - 10-26);
	else return '-';
}


Ascii_drawer::Ascii_drawer(void* const eh_ptr, WINDOW* const win, Entity* const anchor, char anchor_char,
						   const pos_t& offset):
	
	eh_ptr{eh_ptr}, anchor{anchor},
	// prev_room_ptr{},
	
	anchor_pos_comp{ dynamic_cast<Comps::Movable_position&>(anchor->get_component_with_name("position")) },
	
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

void Ascii_drawer::fill_vis_array(const char c)
{
	varr.fill(c);
}

void Ascii_drawer::clear_vis_array()
{
	fill_vis_array(empty_char);
}

void Ascii_drawer::update(bool update_all)
{
	if (update_all) {
		clear_vis_array();
		varr.set_offset({0,0});
		// note: anchor_pos is local pos in room
		varr.draw_room(anchor_pos_comp.get_room_ptr(), anchor_pos_in_screen - anchor_pos_comp.get_pos());
		
		// the room transition we are currently standing on if any
		Room::room_tr const* cur_tr = anchor_pos_comp.get_room_tr_we_are_in();
		
		debug_msgs << "cur_tr is none: " << (cur_tr ? cur_tr->to_string() : "null") << std::endl;
		
		// when we are standing in a room transition we are in the both room at the same time
		if (cur_tr != nullptr) {
			varr.draw_room(cur_tr->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() +
						   cur_tr->area_from.get_pos1() - cur_tr->pos_to);
		}
		
		const Room::room_tr_vector& trs = anchor_pos_comp.get_room_ptr()->get_room_trs();
		for (Room::room_tr_vector::const_iterator it = trs.begin();
			 it != trs.cend(); ++it)
		{
			if (&*it == cur_tr) continue; // ignore the second current room
			
			varr.LOS_draw_room(it->room_to, &*it, anchor_pos_comp, anchor_pos_in_screen - varr.get_offset());
		}
	}
	else {
		//debug_msgs << "last move: " << anchor_pos_comp.get_last_move().to_string() << std::endl;
		
		shift_vis_array(anchor_pos_comp.get_last_move());
		
		Room::room_tr const* rtr_ptr = anchor_pos_comp.get_used_room_tr();
		
		if(rtr_ptr != nullptr) // room shift happened
		{
			const Room::room_tr_vector& trs = anchor_pos_comp.get_room_ptr()->get_room_trs();
			
			for (Room::room_tr_vector::const_iterator it = trs.begin();
				 it != trs.cend(); ++it)
			{
				if (*rtr_ptr != *it->get_reverse_tr()) // do not draw the room we came from again
					varr.draw_room(it->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() +
								   it->area_from.get_pos1() - it->pos_to + varr.get_offset());
			}
		}
	}
	
	debug_msgs << "varr offset: " << varr.get_offset().to_string() << " all updated: " << update_all << std::endl;
}

void Ascii_drawer::shift_vis_array(pos_t offset) 
{
	varr.shift(offset);
}

void Ascii_drawer::set_vis_array_offset(pos_t offset)
{
	varr.set_offset(offset);
}

void Ascii_drawer::shift_vis_array_and_update(pos_t offset)
{
	shift_vis_array(offset); update();
}

void Ascii_drawer::add_debug_message(const std::string& msg)
{
	debug_msgs << msg << std::endl;
}

void Ascii_drawer::print_debug_messages(bool clear_msgs)
{
	move(0,0);
	printw(debug_msgs.str().c_str());
	if (clear_msgs) debug_msgs.str(std::string());
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
