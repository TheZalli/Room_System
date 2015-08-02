#include "room.hh"
#include <utility>
#include <assert.h>

using namespace Room_System;

unsigned Room::prev_id = (unsigned)-1;

const Room::room_tr Room::none_room_tr = Room::room_tr(nullptr, {}, {}, nullptr);

Room::Room(): dim{1,1}, name{}, transitions{}, type{room_type::small_room}
{}

Room::Room(dim_t dim, std::string name, room_type type):
	/*id{Room::prev_id++},*/ dim{dim.w, dim.l}, name{name}, transitions{}, type{type} {}

void Room::Init(unsigned first_id)
{
	assert(0); // currently there is no need to use this function
	Room::prev_id = first_id;
}

bool Room::is_outside_floor(pos_t pos) const{
	const pos_t& pos_corner = dim.to_pos();
	return (pos.x < 0 || pos.y < 0 || pos.x >= pos_corner.x || pos.y >= pos_corner.y); // should work
	//return (unsigned)pos.x >= dim.w || (unsigned)pos.y >= dim.l; // haxy but simple (negative values are larger than positive when interpreted as unsigned)
}

pos_t Room::how_much_outside(pos_t pos) const {
	pos_t to_return{0,0};
	const pos_t& pos_corner = dim.to_pos();

	if (pos.x < 1) to_return.x = pos.x - 1;
	else if (pos.x > pos_corner.x) to_return.x = pos.x - pos_corner.x;
	
	if (pos.y < 1) to_return.y = pos.y - 1;
	else if (pos.y > pos_corner.y) to_return.y = pos.y - pos_corner.y;
	
	return to_return;
}

/*/ ---

void Room::add_entity(const Entity* ent_ptr)
{
	entities.insert(ent_ptr);
}

bool Room::has_entity(const Entity* ent_ptr)
{
	if (!ent_ptr) return false; // is a null pointer
	if (entities.size() == 0) return false;
	return (entities.find(const_cast<Entity*>(ent_ptr)) != entities.cend());
}

void Room::remove_entity(const Entity* ent_ptr)
{
	entities.erase(ent_ptr);
}

// ---*/

void Room::add_room_tr(room_tr rtr)
{
	rtr.set_room_in(this);
	transitions.push_back(rtr);
	if (rtr.get_reverse_tr()) {
		rtr.room_to->transitions.push_back(*rtr.get_reverse_tr());
	}
}

void Room::add_bi_room_tr(Room::room_tr rtr)
{
	rtr.set_room_in(this);
	rtr.generate_reverse_tr();
	
	transitions.push_back(rtr);
	rtr.room_to->transitions.push_back(*rtr.get_reverse_tr());
}

// ---

// OPTIMIZATION: rework rtr container to be fast with this operation
const Room::room_tr& Room::get_room_tr(const pos_t& at) const
{
	/*auto find_it = room_trs.right.find(area_t(at));
	if (find_it == room_trs.right.end()) return nullptr;
	else return find_it->second;*/
	for (room_tr_vector::const_iterator it = transitions.cbegin(); it != transitions.cend(); ++it) {
		if (it->area_from.is_in_area(at)) return *it;
	}
	return Room::none_room_tr;
}

const Room::room_tr_vector& Room::get_room_trs() const
{
	/*std::set<const Room::room_tr&> set;
	std::copy(room_trs.left.begin(), room_trs.left.end(), set.begin());
	return set;*/
	return transitions;
}

/*const Room::room_obj_set& Room::get_entities() const
{
	return entities;
}*/


/*void World_object::move_to_room(Room* room_ptr, pos_t pos)
{
	if (room_where) room_where->remove_object(this);

	if (room_ptr) room_ptr->add_object(this);
	else room_where = room_ptr;

	Event_handler::add_event_to_cur(e_tr_room(this,this->pos, pos, room_ptr));

	this->pos = pos;
}*/


/*std::pair<Room*, pos_t> World_object::check_room_transitions_on(pos_t pos) const
{
	const Room::room_tr& rtr = this->room_where->get_room_tr(pos);
	if (!rtr.room_to) return std::pair<Room*, pos_t>(nullptr, rtr.pos_to);

	const pos_t destination = rtr.pos_to + pos - rtr.area_from.pos1;
	return std::pair<Room*, pos_t>(rtr.room_to, destination);
}

bool World_object::is_allowed_pos(pos_t p) const
{
	if (room_where->get_room_tr(p).room_to != nullptr) return true; // p is in a room_tr
	else return !room_where->is_outside_floor(p);
}*/

/*std::size_t hash_value(const Room& r)
{
	//boost::hash<unsigned> hasher;
	//return hasher(r.id);
	return r.id;
}*/

Room::room_tr::room_tr(Room* const room_to, const pos_t& pos_to, const area_t& area_from,
					   room_tr* other_way_room_tr):
	/*room_in{nullptr},*/ room_to{room_to},
	pos_to{pos_to}, area_from{area_from},
	//obj_associated{obj_associated},
	other_way_room_tr{other_way_room_tr}
{}



void Room::room_tr::generate_reverse_tr()
{
	area_t rev_rtr_area{pos_to, area_from.get_dim()};
	other_way_room_tr =  new room_tr(room_to, room_in,
									 area_from.pos1, rev_rtr_area,
									 //obj_associated,
									 this);
	//other_way_room_tr->set_room_in(room_to);
}

/*Room::room_tr::room_tr(const Room* leads_to, const pos_t& pos_to, const area_t& area_from, world_object * const object_associated):
	leads_to{leads_to}, pos_to{pos_to}, area_from{area_from},
	obj_associated{object_associated} {}*/

/*Room::room_tr::room_tr(Room* const leads_to, const pos_t& pos_to, const area_t& area_from, world_object* object_associated):
	leads_to{leads_to}, pos_to{pos_to}, area_from{area_from}
{
	this->leads_to->add_object(object_associated);
}*/

