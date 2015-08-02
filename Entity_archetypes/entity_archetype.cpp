#include "entity_archetype.hh"

#include "helper_functions.hh"

using namespace Room_System;

// ---
// Entity_archetype

Entity_archetype::Entity_archetype(std::initializer_list<Comps::Component*> comps): comps{comps} {}

Entity_archetype::~Entity_archetype()
{
	// delete all
	std::for_each(comps.begin(), comps.end(), delete_pointed_to<Comps::Component>);
}

Entity* Entity_archetype::make_entity(std::initializer_list<Comps::Component*> comp_values) const throw(std::invalid_argument) {
	if (comp_values.size() > comps.size()) {
		throw std::invalid_argument("Function Entity_archetype::make_entity expected maximum "
									+ std::to_string(comps.size()) + " components, got " + std::to_string(comp_values.size()) );
	}
	
	std::vector<Comps::Component*> returned_comps{comp_values};
	
	// NOTE: this is probably one of the most important things of this archetype system
	// checks the validity of the comp_values
	{
		// NOTE: comps.size() >= comp_values.size() is true
		auto vec_it = comps.begin();
		auto il_it = comp_values.begin();
		
		for (; vec_it != comps.end(); ++il_it, ++vec_it) {
			if ((*vec_it)->get_name() != (*il_it)->get_name()) {
				throw std::invalid_argument(std::string("\"") + (*il_it)->get_name()
											+ "\" component expected, got \"" + (*vec_it)->get_name() + '"');
			}
		}
		for (; il_it != comp_values.end(); ++il_it) { // add the rest with their default values
			returned_comps.push_back((*il_it)->copy()); // copy the component
		}
	}
	
	return new Entity(comp_values);
}

// ---
// Entity_archetype_manager

Entity_archetype_manager::Entity_archetype_manager(): entity_archetypes{} { add_default_archetypes(); }

Entity_archetype_manager::~Entity_archetype_manager()
{
	// delete all
	std::for_each(entity_archetypes.begin(), entity_archetypes.end(),
				  delete_pointed_to_by_pair<std::string, Entity_archetype>);
}

// ---

void
Entity_archetype_manager::add_archetype(const std::string& name,
										std::initializer_list<Comps::Component*> comps)
{
	entity_archetypes.insert({std::string(name), new Entity_archetype{comps}});
}

const std::vector<Comps::Component*>&
Entity_archetype_manager::get_comps_of_archt(const std::string& name) const throw(std::out_of_range)
{
	return entity_archetypes.at(name)->comps;
}

void
Entity_archetype_manager::add_default_archetypes()
{
	add_archetype("player", {new Comps::Position(),
							 new Comps::Name()}
				  );
	
	add_archetype("door", {new Comps::Position(),
						   new Comps::Length(),
						   new Comps::H_or_V(true),
						   new Comps::Obstacle(true),
						   new Comps::Name("door")}
				  );
	
	add_archetype("box", {new Comps::Position(),
						  new Comps::Obstacle(true),
						  new Comps::Name("box")
				  });
}

