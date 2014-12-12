#include "character.hh"

using namespace Coordinates;
using namespace Room_System;

Character::Character(): world_object() {}//Actor() {}

Character::Character(pos_t pos, dim_t dim, std::string name): world_object(pos, dim), name{name}, inventory{} {}

const std::vector<std::string> Character::get_interaction_options() const {
	return std::vector<std::string>();
}

void Character::give_item(world_object* obj)
{
	inventory.insert(obj);
}

bool Character::has_item(world_object* obj) const
{
	if (inventory.find(*obj) == inventory.cend()) return false;
	return true;
}

bool Character::remove_item(world_object* obj)
{
	/*boost::ptr_set<world_object>::iterator it = inventory.find(*obj);
	if (it == inventory.end()) return false;
	inventory.remove(it);
	return true;*/
	return obj->is_opaque();
}
