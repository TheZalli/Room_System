#ifndef ENTITY_MANAGER_HH
#define ENTITY_MANAGER_HH

#include "entity.hh"
#include "Entity_archetypes/entity_archetype.hh"
#include "room.hh"
#include <vector>

namespace Room_System {
class Room;

typedef std::vector<Entity*>  entities_t;
class Entity_manager
{
public:
	Entity_manager();
	~Entity_manager();

	// these functions create entities and returns a pointer to them
	Entity* add_entity(Room* room_in, std::vector<Comps::Component*> comps);
	Entity* add_entity_from_archetype(Room* room_in, std::string name, std::initializer_list<Comps::Component*> component_values);

	Entity* get_entity_by_id(unsigned id) const throw(std::out_of_range);
	entities_t& get_entities() const;
private:
	Entity_archetype_manager arch_manager;
	entities_t ents;
};
}

#endif // ENTITY_MANAGER_HH
