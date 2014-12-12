#include "world_object.hh"
#include <assert.h>

using namespace Room_System;

unsigned world_object::prev_id = (unsigned)-1;
std::set<world_object*> world_object::all_objects{};

world_object::world_object(): pos{}, dim{} {
	all_objects.insert(this);
}

world_object::world_object(const world_object& obj): pos{obj.pos}, dim{obj.dim} {
	all_objects.insert(this);
}

world_object::world_object(pos_t pos, dim_t dim): pos{pos}, dim{dim} {
	all_objects.insert(this);
}

world_object::~world_object() {
	all_objects.erase(all_objects.find(this));
}

// ---

door::door(): world_object(), is_vertical{true}, is_closed{true}, linked_door{nullptr} {}

door::door(pos_t pos, dim_t dim, bool is_vertical, bool is_closed):
	world_object{pos, dim}, is_vertical{is_vertical}, is_closed{is_closed}, linked_door{nullptr}
{
	assert(dim.min() == 1);
	length = dim.max();
}

door::door(pos_t pos, dim_t dim, bool is_vertical, door* linked_door, bool is_closed):
	world_object{pos, dim}, is_vertical{is_vertical}, is_closed{is_closed}, linked_door{linked_door}
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
