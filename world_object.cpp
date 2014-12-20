#include "world_object.hh"
#include <assert.h>

using namespace Room_System;


// static initialization
unsigned World_object::prev_id = (unsigned)-1;
//World_object::all_object_set World_object::all_objects{};

World_object::all_obj_vector World_object::all_objects{};

World_object::World_object(): pos{}, dim{}, room_where{nullptr} {
	assert(0);
	//all_objects.insert(	boost::weak_ptr<World_object>(
	//					boost::shared_ptr<World_object>(this)));
	//all_objects.insert(this);
	all_objects.push_back(std::unique_ptr<World_object>(this));
}

World_object::World_object(const World_object& obj): pos{obj.pos}, dim{obj.dim} { //, room_where{obj.room_where} {
	//all_objects.insert(	boost::weak_ptr<World_object>(
	//					boost::shared_ptr<World_object>(this)));
	//all_objects.insert(this);
	all_objects.push_back(std::unique_ptr<World_object>(this));
}

World_object::World_object(pos_t pos, dim_t dim, Room* where): pos{pos}, dim{dim} { //, room_where{where} {
	//all_objects.insert(	boost::weak_ptr<World_object>(
	//					boost::shared_ptr<World_object>(this)));
	//all_objects.insert(this);
	//if (where) move_to_room(where, pos);
	all_objects.push_back(std::unique_ptr<World_object>(this));
}

World_object::~World_object() {
	//all_objects.erase(all_objects.find(this));
}

World_object* World_object::get_obj_ptr_by_id(unsigned id) {
	if (id > prev_id) return nullptr; // invalid id

	std::size_t search_pos{std::min((std::size_t) id, all_objects.size()-1)};

	do {
		World_object* obj_ptr = &*(all_objects[search_pos]);
		if (obj_ptr->id == id) return obj_ptr;
		else if (obj_ptr->id < id) return nullptr; // id is for a deleted object
	} while (--search_pos > 0);
	return nullptr;
}


// ---

door::door(): World_object(), is_vertical{true}, is_closed{true}, linked_door{nullptr} {}

door::door(pos_t pos, dim_t dim, bool is_vertical, bool is_closed, Room* where):
	World_object{pos, dim, where}, is_vertical{is_vertical}, is_closed{is_closed}, linked_door{nullptr}
{
	assert(dim.min() == 1);
	length = dim.max();
}

door::door(pos_t pos, dim_t dim, bool is_vertical, door* linked_door, bool is_closed, Room* where):
	World_object{pos, dim, where}, is_vertical{is_vertical}, is_closed{is_closed}, linked_door{linked_door}
{
	assert(dim.min() == 1);
	length = dim.max();
	if (linked_door) {
		linked_door->linked_door = this;
	}
}

std::string door::get_name() const {
	return (is_closed ? "a closed door":"an open door");
}

const std::vector<std::string> door::get_interaction_options() const
{
	return std::vector<std::string>({"close", "open"});
}

bool door::interact(int close_or_open)
{
	switch (close_or_open) {
	case 0:
		if (is_closed) return false;
		close();
	break;
	default:
		if (!is_closed) return false;
		open();
	break;
	}
	return true;
}

void door::open() {
	is_closed = false;
	if (linked_door) linked_door->is_closed = false;
}

void door::close() {
	is_closed = true;
	if (linked_door) linked_door->is_closed = true;
}

int door::get_length() const {
	return length;
}

door* door::get_linked_version(pos_t pos_to)
{
	door* copy = this->clone();
	copy->move(pos_to);
	copy->link(this);
	return copy;
}
