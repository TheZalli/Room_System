#ifndef ENTITY_H
#define ENTITY_H

#include "coordinates.hh"
#include "Components/components_include.hh"

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <string>

namespace Room_System {
class Room;

class Entity
{
public:
	Entity();
	Entity(std::initializer_list<Comps::Component*> components);
	Entity(const std::vector<Comps::Component*>& components);

	// deletes the comps
	~Entity();

	// note: this object takes ownership
	inline void add_component(Comps::Component*& cmp) {
		comps.push_back(cmp);
		cmp = nullptr; // just in case
	}

	bool has_component_with_name(const std::string& comp_name) const;
	std::string str_value_of_component(const std::string& comp_name) const;
	
	Comps::Component& get_component_with_name(const std::string& comp_name) const throw(std::out_of_range);
	const std::vector<Comps::Component*>& get_components() const;

	const unsigned uid{ ++prev_uid };
	static unsigned prev_uid;
private:

	//Room* room_in;
	std::vector<Comps::Component*> comps;
};


// for outside use
struct ent_id_key {
	bool operator()(const Entity& e1, const Entity& e2);
	bool operator()(const Entity* e1, const Entity* e2);
};

}
#endif // ENTITY_H
