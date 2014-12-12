#ifndef CHARACTER_HH
#define CHARACTER_HH
//#include "actor.hh"
#include "world_object.hh"
#include <set>
#include <string>
#include <boost/ptr_container/ptr_set.hpp>

using namespace Coordinates;
namespace Room_System {

class Character : public world_object// : public Actor
{
public:
	Character();
	Character(pos_t pos, dim_t dim, std::string name);

	Character* create() const {
		return new Character();
	}
	Character* clone() const {
		return new Character(*this);
	}

	bool is_opaque() const { return true; }
	void update() {}
	const std::vector<std::string> get_interaction_options() const;
	bool interact(int) { return false; }

	std::string get_name() const { return name; }

	void give_item(world_object* obj);
	bool has_item(world_object* obj) const;
	bool remove_item(world_object* obj);

	const boost::ptr_set<world_object>& get_inventory() const { return inventory; }

private:
	std::string name;
	// inventory, consists of weak pointers to the static all_objects object pointer set
	boost::ptr_set<world_object> inventory;
};

}
#endif // CHARACTER_HH
