#include "pc.hh"

using namespace Coordinates;
using namespace Room_System;


Player::Player(pos_t pos, dim_t dim, std::string name, Room* room):
	Character(pos, dim, name, room) { assert(this->id == 0); /*unique*/ }

const std::vector<std::string> Player::get_interaction_options() const {
	return std::vector<std::string>();
}
