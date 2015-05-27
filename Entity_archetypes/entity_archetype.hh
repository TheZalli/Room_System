#ifndef ENTITY_ARCHETYPE_HH
#define ENTITY_ARCHETYPE_HH

#include "entity.hh"
#include <string>
#include <map>
#include <stdexcept>

namespace Room_System {

/**
 * @brief The Entity_archetype struct stores an archetype for a possible entity. Currently has only a vector of the required components
 */
struct Entity_archetype {
	//std::string arch_t_name; // name of the archetype, eg door, goblin, rock
	std::vector<Comps::Component*> comps; // this has the ownership of the components

	Entity_archetype(std::initializer_list<Comps::Component*> comps);
	~Entity_archetype();
	
	/**
	 * @brief make_entity makes an entity based on this archetype.
	 * @param room_in is the room the entity is in. nullptr if they are not in any room
	 * @param comp_values are the values of the components, the component names are checked if they are the same as in comps.
	 * Can be smaller than the archetypes component amount and the rest use the archetypes default value
	 * @return the pointer to the entity made based on this archetype
	 */
	Entity* make_entity(Room* room_in, std::initializer_list<Comps::Component*> comp_values) const throw(std::invalid_argument);
};

typedef std::map<std::string, Entity_archetype*> ent_arch_t;

/**
 * @brief The Entity_archetype_manager class manages entity archetypes
 */
class Entity_archetype_manager {
public:
	Entity_archetype_manager();
	~Entity_archetype_manager();

	inline void add_archetype(const std::string& name, std::initializer_list<Comps::Component*> comps);

	inline bool exists_archetype(const std::string& name) const {
		if (entity_archetypes.find(name) != entity_archetypes.cend()) return true;
		else return false;
	}

	Entity* make_entity_from_archetype(const std::string& name, Room* room_in, std::initializer_list<Comps::Component*> comp_values) const throw(std::out_of_range)
	{
		return entity_archetypes.at(name)->make_entity(room_in, comp_values);
	}
	std::vector<Comps::Component*> get_comps_of_archt(const std::string& name) const throw(std::out_of_range);
private:
	ent_arch_t entity_archetypes;

	void add_default_archetypes(); // doors windows etc
};

}

#endif // ENTITY_ARCHETYPE_HH
