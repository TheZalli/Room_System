#ifndef ROOM_HH
#define ROOM_HH

#include "coordinates.hh"
#include "world_object.hh"
#include <string>
#include <cstddef>
#include <memory>
#include <set>
//#include <assert.h>

//#include <boost/ptr_container/ptr_set.hpp>
/*#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>*/

using namespace Coordinates;
namespace Room_System {

class Room
{
public:
	/*struct wp_obj_comp_key {
		bool operator ()(const std::weak_ptr<World_object>& o1, const std::weak_ptr<World_object>& o2) {
			return (*o1.lock() < *o2.lock());
		}
	};*/
	//typedef std::set<std::weak_ptr<World_object>, wp_obj_comp_key> room_obj_set;
	struct p_obj_comp_key {
			bool operator ()(const World_object* o1, const World_object* o2) {
				return (*o1 < *o2);
			}
		};
	typedef std::set<World_object*, p_obj_comp_key> room_obj_set;

	//---

	// a struct for doors, windows, cojoined room borders and other room transitions
	struct room_tr {
		Room* leads_to;	// the room where the transition leads to
		pos_t pos_to;	// the position in the room where the transition leads to
		area_t area_from;	// transition area
		// the object this is associated with. if this is null pointer then there is no object
		//world_object const* obj_associated;

		room_tr(Room* const leads_to, const pos_t& pos_to, const area_t& area_from);
		//room_tr(const Room* leads_to, const pos_t& pos_to, const area_t& area_from, world_object* const object_associated);
		// automatically adds the object
		//room_tr(Room* const leads_to, const pos_t& pos_to, const area_t& area_from, world_object* object_associated);

		bool operator<(const room_tr& rhs) const {
			return leads_to->id < rhs.leads_to->id;
		}
	}const none_room_tr{nullptr, {}, {}};

	Room();
	Room(dim_t dim, std::string name);

	// initializes the unique id counter, currently unnecessary
	static void Init(unsigned first_id = 0);

	std::string get_name() const	{ return name; }
	shape_t get_shape() const		{ return shape; }
	inline dim_t get_dim() const	{ return shape; }

	//scr_dim_t get_scrdim() const	{ return scr_dim_t(shape); }

	void add_object(World_object* obj);
	bool has_object(World_object* const obj);
	void remove_object(World_object* obj);

	friend void World_object::set_room(Room* room);//, pos_t to);

	void add_room_tr(room_tr rtr);
	void add_bi_room_tr(room_tr rtr);

	inline void add_room_tr_wobj(room_tr rtr, World_object* object_associated);
	void add_room_tr_wobj(Room* const leads_to, const pos_t pos_to, World_object* object_associated);

	void add_bi_room_tr_wobj(room_tr rtr, World_object* object_associated);
	void add_bi_room_tr_wobj(Room* const leads_to, const pos_t pos_to, World_object* object_associated);

	void add_door(Room* const second_room, const pos_t pos_to, door* door_in_first);

	const room_tr& get_room_tr(const pos_t& at) const;
	const std::set<room_tr>& get_room_trs() const;

	const room_obj_set& get_objects() const;

	const unsigned id{++prev_id};
private:
	static unsigned prev_id;

	/*typedef boost::bimaps::bimap<
			boost::bimaps::multiset_of<room_tr>,
			boost::bimaps::unordered_set_of<area_t, boost::hash<area_t>, overlap_key> >
			room_tr_map;*/

	// currently contains only the x,y dimensions
	// 0,0 is the upperleftmost part of the floor
	// walls are at -1 and dim+1 rows/colums
	/* 8x2 room
	 -1012345678
   -1 ##########
	0 #--------#
	1 #--------#
	2 ##########
	*/
	shape_t shape;

	// name of the room, eg. "kitchen"
	std::string name;

	/*// a bidirectional map for doorways
	room_tr_map room_trs;*/

	// a set for room transitions, eg doorways, windows, cojoined room borders
	std::set<room_tr> transitions;

	// a set for objects in the room
	//std::set<world_object> objects;
	//boost::ptr_set<World_object> objects;
	room_obj_set objects;

};

//std::size_t hash_value(const Room&);

}

#endif // ROOM_HH
