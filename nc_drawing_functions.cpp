#include "nc_drawing_functions.hh"
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
		
		Room::room_tr const* cur_tr = anchor_pos_comp.get_room_tr_we_are_in();
		if (cur_tr != nullptr) { // when we are standing in a room transition we are in the both room at the same time
			varr.draw_room(cur_tr->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() + cur_tr->area_from.get_pos1() - cur_tr->pos_to);
		}
		
		const Room::room_tr_vector& trs = anchor_pos_comp.get_room_ptr()->get_room_trs();
		for (Room::room_tr_vector::const_iterator it = trs.begin();
			 it != trs.cend(); ++it)
		{
			if (&*it == cur_tr) continue; // ignore the second current room
			
			//varr.draw_room(it->room_to, anchor_pos_in_screen - anchor_pos_comp.get_pos() + it->area_from.get_pos1() - it->pos_to);
			varr.LOS_draw_room(it->room_to, &*it, anchor_pos_comp, anchor_pos_in_screen - varr.get_offset());
		}
	}
	else {
		debug_msgs << "last move: " << anchor_pos_comp.get_last_move().to_string() << std::endl;
		shift_vis_array(anchor_pos_comp.get_last_move());
		
		Room::room_tr const* rtr_ptr = anchor_pos_comp.get_used_room_tr();
		
		if(rtr_ptr != nullptr) // room shift happened
		{
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
// BUG: segfaults when full updating after hitting a wall
// do not call if the pos_from is on the room transition because then they are in the both rooms at the same time
void Ascii_drawer::vis_array::LOS_draw_room(const Room* const r, const Room::room_tr* room_tr_from, const Comps::Position& pos_from, pos_t pos_in_varr) throw(std::invalid_argument)
{
	if (room_tr_from->room_to != r || room_tr_from->room_in != pos_from.get_room_ptr()) {
		throw std::invalid_argument("room transition " + room_tr_from->to_string() + " should be between the rooms " + pos_from.get_room_ptr()->get_name() + " and " + r->get_name());
	}
	
	pos_t distance = room_tr_from->area_from.pos1 - pos_from.get_pos();
	
	dim_t room_dim = r->get_dim();
	
	bool is_on_vertical_wall;
	bool is_on_horizontal_wall;
	
	room_tr_from->get_wall_orientation(is_on_vertical_wall, is_on_horizontal_wall);
	
	dist_t tangent;
	dist_t normal;
	
	dist_t tangent_min;
	dist_t tangent_max;
	
	dist_t normal_max;
	
	int normal_dir; // +1 or -1
	//pos_t normal_dir_vector; // points towards the normal
	
	if (is_on_vertical_wall && !is_on_horizontal_wall) {
		tangent = distance.y;
		normal =  distance.x;
		
		if (tangent == 0) { // we are at the same height as the doorway
			tangent_min = 0;
			tangent_max = room_dim.l + 1;
		}
		else if (tangent < 0){
			tangent_min = 0;
			tangent_max = room_tr_from->pos_to.y;
		}
		else if (tangent > 0) {
			tangent_min = room_tr_from->pos_to.y;
			tangent_max = room_dim.l + 1;
		}
		
		normal_max =  room_dim.w + 1;
		
		normal_dir = normal > 0 ? 1 : -1;
		//normal_dir_vector = pos_t(normal_dir, 0);
	}
	else { // TODO: handle other cases
		tangent = distance.x;
		normal =  distance.y;
		
		if (tangent == 0) { // we are at the same height as the doorway
			tangent_min = 0;
			tangent_max = room_dim.w + 1;
		}
		else if (tangent < 0){
			tangent_min = 0;
			tangent_max = room_tr_from->pos_to.x;
		}
		else if (tangent > 0) {
			tangent_min = room_tr_from->pos_to.x;
			tangent_max = room_dim.w + 1;
		}
		
		normal_max =  room_dim.l + 1;
		
		normal_dir = normal > 0 ? 1 : -1;
		//normal_dir_vector = pos_t(0, normal_dir);
	}
	
	if (normal == 0) {
		throw std::invalid_argument(std::string("vert: ") + (is_on_vertical_wall ?"1":"0") + " horiz: " + (is_on_horizontal_wall ?"1":"0") +
									" Position " + pos_from.get_value_str() + " can't be on the same position as the room transition " + room_tr_from->to_string() + " when ray casting.");
	}
	
	if (normal == 1 && tangent > 1) return; // too steep to see anything
	
	//int slope = tangent/
	
	// TODO
	for (dist_t n = room_tr_from->pos_to.select_x_or_y(is_on_vertical_wall) + normal_dir;
		 n <= normal_max && n >= 0;
		 n += normal_dir)
	{
		//std::cout << "debug!" << std::endl;
		for (dist_t t = tangent_min; t <= tangent_max; ++t)
		{
			//if ()
			
			pos_t draw_pos = pos_in_varr + distance - room_tr_from->pos_to;
			
			pos_t nt_pos;
			if (is_on_vertical_wall)
				 nt_pos = pos_t(n,t);
			else nt_pos = pos_t(t,n);
			
			draw_pos += nt_pos;
			
			if (!r->is_outside_floor(nt_pos)) put(draw_pos.x, draw_pos.y, floor_char);
			else put(draw_pos.x, draw_pos.y, wall_char);
			
		}
	}
}


