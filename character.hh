#ifndef CHARACTER_HH
#define CHARACTER_HH
//#include "actor.hh"
#include "world_object.hh"
#include <set>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace Coordinates;
namespace Room_System {

class Character : public World_object// : public Actor
{
public:
	Character();
	Character(pos_t pos, dim_t dim, std::string name, Room* room);

	/*Character* create() const {
		return new Character();
	}
	Character* clone() const {
		return new Character(*this);
	}

	bool is_opaque() const { return true; }
	void update() {}
	const std::vector<std::string> get_interaction_options() const {
		return std::vector<std::string>();
	}
	bool interact(int) { return false; }*/

	std::string get_name() const { return name; }

	void give_item(World_object* obj);
	bool has_item(World_object* obj) const;
	// returns true if an item was removed
	bool remove_item(World_object* obj);

	const boost::ptr_vector<World_object>& get_inventory() const { return inventory; }

private:
	std::string name;
	boost::ptr_vector<World_object> inventory;
};

}
#endif // CHARACTER_HH
