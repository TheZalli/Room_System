#ifndef WORLD_OBJECT_HH
#define WORLD_OBJECT_HH
#include "coordinates.hh"
#include "materials.hh"
#include <string>
//#include <set>
#include <vector>
#include <memory>
//#include <boost/ptr_container/ptr_set.hpp>

using namespace Coordinates;
using namespace Room_System::Materials;

namespace Room_System {

/*enum material {

};*/

class Room;

class World_object
{
private:
	typedef std::vector<std::unique_ptr<World_object> > all_obj_vector;
	static all_obj_vector all_objects;
public:
	World_object();
	World_object(const World_object& obj);
	World_object(pos_t pos, dim_t dim, Room* where = nullptr);
	virtual ~World_object();

	virtual World_object* create() const = 0;	// virtual constructor for creation
	virtual World_object* clone() const = 0;	// virtual constructor for copying

	virtual std::string get_name() const = 0;
	virtual bool is_opaque() const { return true; }

	// returns the possible options for interaction in a std::vector
	virtual const std::vector<std::string> get_interaction_options() const = 0;
	// interacts with the target
	virtual bool interact(int) = 0;

	pos_t get_pos() const { return pos; }
	dim_t get_dim() const { return dim; }
	area_t get_area() const { return area_t{pos, dim}; }

	Room* get_room() const { return room_where; }
	void set_room(Room* room);


	void set_pos(pos_t to) { pos = to; }
	// moves the object given travel but checks before if it is possible
	void move(pos_t to_add);


	bool operator<(const World_object& rhs) const {
		return id < rhs.id;
	}

	static all_obj_vector& get_all_objects() { return all_objects; }

	// returns a nullpointer if an object with this id can't be found
	static World_object* get_obj_ptr_by_id(unsigned id);


	// these three are defined in room.cpp as they are Room's friends
	void move_to_room(Room* room_ptr, pos_t pos);
	std::pair<Room*, pos_t> check_room_transitions_on(pos_t pos) const;
	bool is_allowed_pos(pos_t p) const; // checks if the p is on an allowed position, like not inside the walls or other objects

	const unsigned id{++prev_id};
private:
	static unsigned prev_id;

	//area_t area;
	pos_t pos;
	dim_t dim;
	Room* room_where;

	//material_t mat;
};


// Boost's clone_allocator.hpp needs this:
inline World_object* new_clone(const World_object& obj) {
		return obj.clone();
}

// --- DOOR --- //

class door : public World_object
{
public:
	door();
	door(const door& to_clone) = default;
	door(pos_t pos, dim_t dim, bool is_vertical, bool is_closed = true, Room* where = nullptr);
	//door(pos_t pos, dim_t dim, bool is_vertical, door* linked_door, bool is_closed = true, Room* where = nullptr);

	door* create() const {
		return new door();
	}
	door* clone() const {
		return new door(*this);
	}

	std::string get_name() const;
	bool is_opaque() const { return is_closed; }

	const std::vector<std::string> get_interaction_options() const;
	bool interact(int close_or_open);

	bool get_is_closed() const { return is_closed; }
	void open();
	void close();

	bool get_is_vertical() const { return is_vertical; }
	int get_length() const;

	//door* get_linked_version(pos_t pos_to);
	//void link(door* door) { linked_door = door; }

private:
	bool is_vertical;
	bool is_closed;
	int length;
	//door* linked_door;
};

}

#endif // WORLD_OBJECT_HH
