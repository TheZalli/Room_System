#include "ascii_drawer.hh"

Ascii_drawer::Vis_array::Vis_array(const unsigned rows, const unsigned cols, std::stringstream* debug_msgs_ptr,
								   const pos_t& offset):
	rows{rows}, cols{cols}, offset{offset}, debug_msgs_ptr{debug_msgs_ptr}
{
	array = new char[this->rows * this->cols];
	clear();
	
}

Ascii_drawer::Vis_array::~Vis_array()
{
	delete[] array;
}

void Ascii_drawer::Vis_array::put(const unsigned x, const unsigned y, const char c) const
{
	//assert (x < cols); assert(y < rows);
	if (x < cols && y < rows) array[x + y*cols] = c;
	//else TODO: create a new array
}

char Ascii_drawer::Vis_array::at(const unsigned x, const unsigned y) const
{
	assert (x <= cols); assert(y <= rows);
	return array[x + y*cols];
}

bool Ascii_drawer::Vis_array::in_area(const unsigned x, const unsigned y) const
{
	return (y < rows) && (x < cols);
}

void Ascii_drawer::Vis_array::shift(pos_t offset)
{
	this->offset += offset;
}

pos_t Ascii_drawer::Vis_array::get_offset() const
{
	return offset;
}

void Ascii_drawer::Vis_array::set_offset(pos_t offset)
{
	this->offset = offset;
}

dim_t Ascii_drawer::Vis_array::get_dim() const
{
	return dim_t(cols, rows);
}

pos_t Ascii_drawer::Vis_array::get_max_pos() const
{
	return pos_t(cols-1, rows-1);
}

void Ascii_drawer::Vis_array::fill(const char c)
{
	for (unsigned y = 0; y < rows; ++y) {
		for (unsigned x = 0; x < cols; ++x) {
			put(x,y, c);
			//put(x, y, debug_uint_to_char(std::min(x,y)) ); // DEBUG
		}
	}
}


char Ascii_drawer::Vis_array::at_win(const int x, const int y, WINDOW* src_win) const {
	assert((y <= src_win->_maxy) && (x <= src_win->_maxx) && (y >= src_win->_begy) && (x >= src_win->_begx));

	if (((unsigned)(y + offset.y) < rows) && ((unsigned)(x + offset.x) < cols))
		//return array[((size_t)y + offset.y) + ((size_t)x + offset.x)*cols];
		return at(x + offset.x, y + offset.y);
	else return empty_char;
}

void Ascii_drawer::Vis_array::draw_room(const Room* const r, const pos_t& pos)
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
	
}

// do not call if the pos_from is on the room transition because then they are in the both rooms at the same time
void Ascii_drawer::Vis_array::LOS_draw_room(const Room* const r, const Room::room_tr* room_tr_from,
											const Comps::Position& pos_from, pos_t pos_in_varr)
throw(std::invalid_argument)
{
	if (room_tr_from->room_to != r || room_tr_from->room_in != pos_from.get_room_ptr()) {
		throw std::invalid_argument("room transition " + room_tr_from->to_string() +
									" should be between the rooms " + pos_from.get_room_ptr()->get_name() +
									" and " + r->get_name());
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
		normal  = distance.x;
		
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
		normal  = distance.y;
		
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
		/*throw std::invalid_argument(std::string("vert: ") + (is_on_vertical_wall ?"1":"0") + " horiz: " +
									(is_on_horizontal_wall ?"1":"0") + " Position " + pos_from.get_value_str() +
									" can't be on the same position as the room transition " +
									room_tr_from->to_string() + " when ray casting.");
		*/
	}
	
	if ((normal == 1 || normal == -1) && (tangent > 1 || tangent < -1)) return; // too steep to see anything
	
	int slope = tangent + normal;
	*debug_msgs_ptr << "Room: " << r->get_name() << " slope: (" << normal << "," << tangent << ") "
					<< std::to_string(slope) << std::endl;
	
	// TODO
	for (dist_t n = room_tr_from->pos_to.select_x_or_y(is_on_vertical_wall) + normal_dir;
		 n <= normal_max && n >= 0;
		 n += normal_dir)
	{
		for (dist_t t = tangent_min; t <= tangent_max; ++t)
		{
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


