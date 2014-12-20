#include "character.hh"

using namespace Coordinates;
using namespace Room_System;

Character::Character(): World_object() {}//Actor() {}

Character::Character(pos_t pos, dim_t dim, std::string name, Room* room): World_object(pos, dim, room), name{name}, inventory{} {}


void Character::give_item(World_object* obj)
{
	//inventory.insert(obj);
	inventory.push_back(obj);
}

bool Character::has_item(World_object* obj) const
{
	//if (inventory.find(*obj) == inventory.cend()) return false;
	for (boost::ptr_vector<World_object>::const_iterator it = inventory.cbegin();
		 it != inventory.cend(); ++it) {
		if (&(*it) == obj)
			return true;
	}
	return false;
}

bool Character::remove_item(World_object* obj)
{
	/*boost::ptr_set<world_object>::iterator it = inventory.find(*obj);
	if (it == inventory.end()) return false;
	inventory.remove(it);
	return true;*/

	for (boost::ptr_vector<World_object>::iterator it = inventory.begin();
		 it != inventory.end(); ++it) {
		if (&(*it) == obj) {
			inventory.erase(it);
			return true;
		}
	}
	return false;
}
