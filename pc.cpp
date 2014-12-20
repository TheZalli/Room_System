#include "pc.hh"

using namespace Coordinates;
using namespace Room_System;


PC::PC(pos_t pos, dim_t dim, std::string name, Room* room):
	Character(pos, dim, name, room) { assert(this->id == 0); }

const std::vector<std::string> PC::get_interaction_options() const {
	return std::vector<std::string>();
}
