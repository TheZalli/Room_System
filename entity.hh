#ifndef ENTITY_H
#define ENTITY_H

#include "coordinates.hh"
#include "Components/components_include.hh"
#include <vector>
#include <initializer_list>
#include <stdexcept>

namespace Room_System {
class Room;

class Entity
{
public:
	Entity();
	Entity(Room* room_in, std::initializer_list<Comps::Component*> components);
	Entity(Room* room_in, const std::vector<Comps::Component*>& components);

	// deletes the components
	~Entity();

	// note: this object takes ownership
	inline void add_component(Comps::Component*& cmp) {
		comps.push_back(cmp);
		cmp = nullptr; // just in case // OPTIMIZATION: Remove this
	}

	inline Room* get_room_in() const { return room_in; }

	bool has_component_with_name(const std::string& comp_name) const;
	std::string str_value_of_component(const std::string& comp_name) const;
	Comps::Component& get_component_with_name(const std::string& comp_name) const throw(std::out_of_range);

	inline const std::vector<Comps::Component*>& get_components() const { return comps; }

	const unsigned uid{++prev_uid};
	static unsigned prev_uid;
private:

	Room* room_in;
	std::vector<Comps::Component*> comps;
};


// for outsider use (room.hh)
struct ent_id_key {
	bool operator()(const Entity& e1, const Entity& e2) {
		return e1.uid < e2.uid;
	}
	bool operator()(const Entity* e1, const Entity* e2) {
		return e1->uid < e2->uid;
	}
};

}
#endif // ENTITY_H
