#include "room.hh"
#include <utility>

using namespace Room_System;

unsigned Room::prev_id = (unsigned)-1;

Room::Room(): /*id{Room::prev_id++},*/ shape{1,1}, name{}, transitions{}
{
}

Room::Room(dim_t dim, std::string name): /*id{Room::prev_id++},*/ shape{dim.w, dim.l}, name{name}, transitions{}
{
}

void Room::Init(unsigned first_id)
{
	Room::prev_id = first_id;
}

void Room::add_room_tr(room_tr rtr)
{
	transitions.insert(rtr);
}

void Room::add_bi_room_tr(Room::room_tr rtr)
{
	transitions.insert(rtr);

	// the area for the transition on the other room
	area_t rev_rtr_area{rtr.pos_to, rtr.area_from.get_dim()};
	room_tr rev_rtr = room_tr(this, rtr.area_from.pos1, rev_rtr_area);

	rtr.leads_to->transitions.insert(rev_rtr);
}

void Room::add_object(world_object* obj)
{
	objects.insert(obj);
}

// ---

void Room::add_room_tr_wobj(Room::room_tr rtr, world_object* object_associated)
{
	add_room_tr(rtr);
	add_object(object_associated);
}

void Room::add_room_tr_wobj(Room* const leads_to, const pos_t pos_to, world_object* object_associated)
{
	room_tr rtr{leads_to, pos_to, object_associated->get_area()};
	add_room_tr_wobj(rtr, object_associated);
}


void Room::add_bi_room_tr_wobj(Room::room_tr rtr, world_object* object_associated)
{
	add_bi_room_tr(rtr);
	add_object(object_associated);

	// the object in the second room
	world_object* new_obj = object_associated->clone();
	new_obj->move(rtr.pos_to);
	rtr.leads_to->add_object(new_obj);
}

void Room::add_bi_room_tr_wobj(Room* const leads_to, const pos_t pos_to, world_object* object_associated)
{
	room_tr rtr{leads_to, pos_to, object_associated->get_area()};
	add_bi_room_tr_wobj(rtr, object_associated);
}

void Room::add_door(Room* const second_room, const pos_t pos_to, door* door_in_first)
{
	room_tr rtr{second_room, pos_to, door_in_first->get_area()};
	add_bi_room_tr(rtr);

	door* second_door = door_in_first->get_linked_version(pos_to);
	door_in_first->link(second_door);

	objects.insert(door_in_first);
	second_room->objects.insert(second_door);
}
// ---

const Room::room_tr& Room::get_room_tr(const pos_t& at) const
{
	/*auto find_it = room_trs.right.find(area_t(at));
	if (find_it == room_trs.right.end()) return nullptr;
	else return find_it->second;*/
	for (std::set<room_tr>::const_iterator it = transitions.cbegin(); it != transitions.cend(); ++it) {
		if (it->area_from.is_in_area(at)) return *it;
	}
	return none_room_tr;
}

const std::set<Room::room_tr>& Room::get_room_trs() const
{
	/*std::set<const Room::room_tr&> set;
	std::copy(room_trs.left.begin(), room_trs.left.end(), set.begin());
	return set;*/
	return transitions;
}

const boost::ptr_set<world_object>& Room::get_objects() const
{
	return objects;
}

/*std::size_t hash_value(const Room& r)
{
	//boost::hash<unsigned> hasher;
	//return hasher(r.id);
	return r.id;
}*/

Room::room_tr::room_tr(Room* const leads_to, const pos_t& pos_to, const area_t& area_from):
	leads_to{leads_to}, pos_to{pos_to}, area_from{area_from} {}

/*Room::room_tr::room_tr(const Room* leads_to, const pos_t& pos_to, const area_t& area_from, world_object * const object_associated):
	leads_to{leads_to}, pos_to{pos_to}, area_from{area_from},
	obj_associated{object_associated} {}*/

/*Room::room_tr::room_tr(Room* const leads_to, const pos_t& pos_to, const area_t& area_from, world_object* object_associated):
	leads_to{leads_to}, pos_to{pos_to}, area_from{area_from}
{
	this->leads_to->add_object(object_associated);
}*/

