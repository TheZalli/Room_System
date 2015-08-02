#ifndef ROOM_HH
#define ROOM_HH

#include "coordinates.hh"
//#include "entity.hh"
//#include <set>
#include <string>
#include <cstddef>
#include <vector>
//#include <assert.h>

using namespace Coordinates;
namespace Room_System {

enum class room_type {
	small_room,	// a room where you can see everywhere
	large_room,	// a hall where you can't see from one end to another
	corridor,	// a narrow room (l or w is 1-3)
	outside_area // an outside area without borders
};

class Room
{
public:
	//typedef std::set<const Entity*, ent_id_key> room_obj_set;

	//---

	// a struct for doors, windows, cojoined room borders and other room transitions
	struct room_tr {
		Room* room_in;
		Room* room_to;	// the room where the transition leads to
		pos_t pos_to;	// the position in the room where the transition leads to
		area_t area_from;	// transition area
		//Entity* obj_associated;
		room_tr* other_way_room_tr;
		// the object this is associated with. if this is null pointer then there is no object
		//world_object const* obj_associated;

		room_tr(Room* const room_to, const pos_t& pos_to, const area_t& area_from,
				//Entity* obj_associated = nullptr,
				room_tr* other_way_room_tr = nullptr);

		inline bool operator==(const room_tr& rhs) const {
			return area_from.pos1 == rhs.area_from.pos1;
		}
		inline bool operator!=(const room_tr& rhs) const {
			return area_from.pos1 != rhs.area_from.pos1;
		}

		void set_room_in(Room* room_in){
			this->room_in = room_in;
		} // remember to call this function after the constructor in the Room functions

		inline room_tr(Room* const room_in, Room* const room_to, const pos_t& pos_to, const area_t& area_from,
				/*Entity* obj_associated = nullptr,*/ room_tr* other_way_room_tr = nullptr):

			room_tr(room_to, pos_to, area_from, /*obj_associated,*/ other_way_room_tr)
		{
			set_room_in(room_in);
		}

		void generate_reverse_tr(); // creates a new room transition to the target room and sets other_way_room_tr to point to that
		room_tr* get_reverse_tr() const { return other_way_room_tr; }
		void set_other_way_room_tr(room_tr* other) { other_way_room_tr = other; }

		/*inline void set_other_way_room_tr_as_reverse() {
			other_way_room_tr = &generate_reverse_tr;
		}*/

		/*struct room_tr_key {
			bool operator ()(const room_tr& rtr1, const room_tr& rtr2) const {
				return pos_t::pos_key{}(rtr1.pos_to, rtr2.pos_to);
			}
		};*/
		std::string to_string() const { // DEBUG
			return room_in->get_name() + ": " + area_from.to_string() + " -> "
				 + room_to->get_name() + ": " + pos_to.to_string();
		}
	};
	static const room_tr none_room_tr;

	//typedef std::set<room_tr, room_tr::room_tr_key> room_tr_set;
	typedef std::vector<room_tr> room_tr_vector;
	// ---

	Room();
	Room(dim_t dim, std::string name, room_type type = room_type::small_room);

	// initializes the unique id counter, currently unnecessary
	static void Init(unsigned first_id = (unsigned)-1);

	std::string get_name() const	{ return name; }
	//shape_t get_shape() const		{ return dim; }
	dim_t get_dim() const			{ return dim; }

	bool is_outside_floor(pos_t pos) const;
	pos_t how_much_outside(pos_t pos) const;

	//void add_entity(const Entity* ent_ptr);
	//bool has_entity(const Entity* ent_ptr);
	//void remove_entity(const Entity* ent_ptr);

	void add_room_tr(room_tr rtr);
	void add_bi_room_tr(room_tr rtr);


	const room_tr& get_room_tr(const pos_t& at) const;
	const room_tr_vector& get_room_trs() const;

	//const room_obj_set& get_entities() const;

	const unsigned id{++prev_id};
private:
	static unsigned prev_id;

	// currently contains only the x,y dimensions
	// 1,1 is the upperleftmost part of the floor
	// walls are at 0 and dim+1 rows/colums
	/* 8x2 room
	  0123456789
    0 ##########
	1 #--------#
	2 #--------#
	3 ##########
	*/
	//shape_t shape;
	dim_t dim;

	// name of the room, eg. "kitchen"
	std::string name;

	// a set for room transitions, eg doorways, windows, cojoined room borders
	room_tr_vector transitions;

	// a set for objects in the room
	//std::set<world_object> objects;
	//boost::ptr_set<World_object> objects;
	//room_obj_set entities;

	room_type type;
};

//std::size_t hash_value(const Room&);

}

#endif // ROOM_HH
