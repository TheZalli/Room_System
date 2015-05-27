#include "entity.hh"
#include "helper_functions.hh"

using namespace Room_System;

unsigned Entity::prev_uid = (unsigned)-1; // initialize uid

Entity::Entity(): room_in{nullptr}, comps{}
{
}

Entity::Entity(Room* room_in, std::initializer_list<Comps::Component*> components):
	room_in{room_in}, comps{components}
{
}

Entity::Entity(Room* room_in, const std::vector<Comps::Component*>& components):
	room_in{room_in}, comps{components}
{
}

Entity::~Entity()
{
	std::for_each(comps.begin(), comps.end(), delete_pointed_to<Comps::Component>);
	/*for (Comps::Component* cmp_ptr : comps) {
		delete cmp_ptr;
	}*/
}

bool Entity::has_component_with_name(const std::string& comp_name) const
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return true;
	}
	return false;
}

std::string Entity::str_value_of_component(const std::string& comp_name) const
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return cmp->get_value_str();
	}
	return std::string{};
}

Comps::Component& Entity::get_component_with_name(const std::string& comp_name) const throw(std::out_of_range)
{
	for (Comps::Component* cmp : comps) {
		if (cmp->get_name() == comp_name) return *cmp;
	}
	throw std::out_of_range("component " + comp_name + " not found");
}
