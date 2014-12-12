#ifndef ROOM_HH
#define ROOM_HH

#include "coordinates.hh"
#include "world_object.hh"
#include <string>
#include <cstddef>
#include <set>
#include <boost/ptr_container/ptr_set.hpp>
/*#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>*/

using namespace Coordinates;
namespace Room_System {

class Room
{
public:
	// a struct for doors, cojoined room borders and other room transitions
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

	void add_room_tr(room_tr rtr);
	void add_bi_room_tr(room_tr rtr);
	void add_object(world_object* obj);

	inline void add_room_tr_wobj(room_tr rtr, world_object* object_associated);
	void add_room_tr_wobj(Room* const leads_to, const pos_t pos_to, world_object* object_associated);

	void add_bi_room_tr_wobj(room_tr rtr, world_object* object_associated);
	void add_bi_room_tr_wobj(Room* const leads_to, const pos_t pos_to, world_object* object_associated);

	void add_door(Room* const second_room, const pos_t pos_to, door* door_in_first);

	const room_tr& get_room_tr(const pos_t& at) const;
	const std::set<room_tr>& get_room_trs() const;

	const boost::ptr_set<world_object>& get_objects() const;

	const unsigned id;
private:
	static unsigned prev_id;

	/*typedef boost::bimaps::bimap<
			boost::bimaps::multiset_of<room_tr>,
			boost::bimaps::unordered_set_of<area_t, boost::hash<area_t>, overlap_key> >
			room_tr_map;*/

	// currently contains only the x,y dimensions
	// 1,1 is the upperleftmost part of the floor
	// walls are at 0 and dim rows/colums
	/* 10x4 room
	  0123456789
	0 ##########
	1 #--------#
	2 #--------#
	3 ##########
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
	boost::ptr_set<world_object> objects;

};

//std::size_t hash_value(const Room&);

}

#endif // ROOM_HH
