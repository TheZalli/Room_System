#include "room.hh"
#include <utility>
#include <assert.h>

using namespace Room_System;

unsigned Room::prev_id = (unsigned)-1;

const Room::room_tr Room::none_room_tr = Room::room_tr{nullptr, {}, {}, nullptr, nullptr};

Room::Room(): /*id{Room::prev_id++},*/ dim{1,1}, name{}, transitions{}, type{small_room}
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

	if (pos.x < 0) to_return.x = pos.x;
	else to_return.x = pos.x - pos_corner.x;
	if (pos.y < 0) to_return.y = pos.y;
	else to_return.y = pos.y - pos_corner.x;

	return to_return;
}

// ---

void Room::add_object(World_object* obj)
{
	objects.insert(World_object::get_obj_ptr_by_id(obj->id));
	obj->set_room(this);
}

bool Room::has_object(World_object* const obj)
{
	if (!obj) return false; // is a null pointer
	if (objects.size() == 0) return false;
	return (objects.find(obj) != objects.end());
}

void Room::remove_object(World_object* obj)
{
	objects.erase(obj);
}

// ---

void Room::add_room_tr(room_tr rtr)
{
	transitions.push_back(rtr);
	if (rtr.get_other_way_room_tr()) {
		//rtr.room_to->transitions.insert(*rtr.get_other_way_room_tr());
		rtr.room_to->transitions.push_back(*rtr.get_other_way_room_tr());
	}
}

/*void Room::add_bi_room_tr(Room::room_tr rtr)
{
	transitions.insert(rtr);

	// the area for the transition on the other room
	//area_t rev_rtr_area{rtr.pos_to, rtr.area_from.get_dim()};
	//room_tr rev_rtr = room_tr(this, rtr.area_from.pos1, rev_rtr_area);
	room_tr rev_rtr = *rtr.other_way_room_tr;

	rtr.leads_to->transitions.insert(rev_rtr);
}*/

// ---

/*void Room::add_room_tr_wobj(Room::room_tr rtr, World_object* object_associated)
{
	add_room_tr(rtr);
	add_object(object_associated);
}*/

void Room::add_room_tr_wobj(Room* const leads_to, const pos_t pos_to, World_object* object_associated, bool two_way)
{

	room_tr rtr{this, leads_to, pos_to, object_associated->get_area(), object_associated};
	if (two_way) rtr.generate_reverse_tr();

	//add_room_tr_wobj(rtr, object_associated);
	add_room_tr(rtr);
	add_object(object_associated);
}


/*void Room::add_bi_room_tr_wobj(Room::room_tr rtr, World_object* object_associated)
{
	add_bi_room_tr(rtr);
	add_object(object_associated);

	// the object in the second room
	World_object* new_obj = object_associated->clone();
	new_obj->move(rtr.pos_to);
	rtr.leads_to->add_object(new_obj);
}*/

/*void Room::add_bi_room_tr_wobj(Room* const leads_to, const pos_t pos_to, World_object* object_associated)
{
	room_tr rtr{leads_to, pos_to, object_associated->get_area()};
	add_bi_room_tr_wobj(rtr, object_associated);

}*/

void Room::add_door(Room* const second_room, const pos_t pos_to, door* door_in_first)
{
	door_in_first->set_room(this);
	room_tr rtr{this, second_room, pos_to, door_in_first->get_area(), door_in_first};
	//rtr.set_room_in(this);
	rtr.generate_reverse_tr();
	//add_bi_room_tr(rtr);
	add_room_tr(rtr);


	//door* second_door = door_in_first->get_linked_version(pos_to);
	//door_in_first->link(second_door);

	objects.insert(door_in_first);
	//second_room->objects.insert(second_door);
}

void Room::make_door(bool is_vertical, Room* const room1, Room* room2, const pos_t& pos1, const pos_t& pos2, const dim_t& dim, bool is_closed)
{
	door* dr_ptr = new door(pos1, dim, is_vertical, is_closed, room1);
	room_tr rtr(room1, room2, pos2, area_t(pos1, dim), dr_ptr);
	rtr.generate_reverse_tr();
	room1->add_room_tr(rtr);

	room1->objects.insert(dr_ptr);
}
// ---

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

const Room::room_obj_set& Room::get_objects() const
{
	return objects;
}


/*void World_object::move_to_room(Room* room_ptr, pos_t pos)
{
	if (room_where) room_where->remove_object(this);

	if (room_ptr) room_ptr->add_object(this);
	else room_where = room_ptr;

	Event_handler::add_event_to_cur(e_tr_room(this,this->pos, pos, room_ptr));

	this->pos = pos;
}*/


std::pair<Room*, pos_t> World_object::check_room_transitions_on(pos_t pos) const
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
}

/*std::size_t hash_value(const Room& r)
{
	//boost::hash<unsigned> hasher;
	//return hasher(r.id);
	return r.id;
}*/

Room::room_tr::room_tr(Room* const room_to, const pos_t& pos_to, const area_t& area_from,
					   World_object* obj_associated,
					   room_tr* other_way_room_tr):
	/*room_in{nullptr},*/ room_to{room_to},
	pos_to{pos_to}, area_from{area_from},
	obj_associated{obj_associated},
	other_way_room_tr{other_way_room_tr} {}



void Room::room_tr::generate_reverse_tr()
{
	area_t rev_rtr_area{pos_to, area_from.get_dim()};
	other_way_room_tr =  new room_tr(room_to, room_in,
									 area_from.pos1, rev_rtr_area,
									 obj_associated, this);
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

