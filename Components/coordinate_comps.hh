#ifndef COORDINATE_COMPS_H
#define COORDINATE_COMPS_H

#include "component.hh"
#include "coordinates.hh"
#include "room.hh"

using namespace Coordinates;

namespace Room_System {
namespace Comps {

// an interface for any component that has a location like position and area
/*class Base_position : virtual public Component
{
public:
	virtual void set_pos(pos_t pos) = 0;
	virtual pos_t get_pos() const = 0;
};

// an interface for any component that has an area
class Base_area : virtual public Component
{
	// checks if the pos in the room_in is in the area
	virtual bool is_in_area(pos_t pos) const = 0;
	virtual surf_area_t get_surf_area() const = 0;
};*/

class Position : public Component//, public Base_position
{
public:
	Position(): Component() {}
	Position(Room* room_in, pos_t pos): Component{}, room_ptr{room_in}, pos{pos} {}

	std::string get_name() const { return "position"; }
	std::string get_value_str() const { return room_ptr->get_name() + ": " + pos.to_string(); }
	
	Component* copy() const { return new Position{*this}; }
	
	Room* get_room_ptr() const { return room_ptr; }
	void set_room_ptr(Room* ptr) { room_ptr = ptr; }
	
	Room* const& get_room_ptr_ref() const { return room_ptr; }
	Room*& get_room_ptr_ref() { return room_ptr; }
	
	
	pos_t get_pos() const { return pos; }
	pos_t& get_pos_ref()  { return pos; }
	const pos_t& get_pos_ref() const { return pos; }
	
	void set_pos(pos_t p) { pos = p; }
	//void add_pos(pos_t p) { pos += p; }

	
	//bool watch_by_default() const { return true; }
	
protected:
	Room* room_ptr;
	pos_t pos;
};

// a position that moves actively between rooms
class Movable_position : public Position
{
public:
	Movable_position(): Position(), room_tr_used{nullptr}, last_move{0,0} {}
	Movable_position(Room* room_in, pos_t pos): Position{room_in, pos}, room_tr_used{nullptr}, last_move{0,0} {}
	
	//std::string get_name() const { return "movable position"; }
	
	Component* copy() const { return new Movable_position(*this); }
	
	void move_pos(pos_t to_add)
	{
		room_tr_used = nullptr;
		
		pos_t out = room_ptr -> how_much_outside(this->pos + to_add);
		
		if (out == pos_t(0,0)) // normal movement
		{
			this->pos += to_add;
			last_move = to_add;
		}
		else { // blocked by a wall
			const Room::room_tr& rtr = room_ptr->get_room_tr(pos);
			
			if (rtr != Room::none_room_tr) // do a room transition
			{
				Room* room_to = rtr.room_to;
				pos_t pos_to = rtr.pos_to;
				
				// if the moved position would lead to an invalid position don't move
				if (room_to->how_much_outside(pos_to + to_add) == pos_t(0,0))
				{
					room_ptr = room_to;
					pos = pos_to + to_add;
					room_tr_used = &rtr;
					
					last_move = to_add;
				}
			}
			// OPTIMIZATION: store this transition for future use
			else if (room_ptr->get_room_tr(pos + to_add) != Room::none_room_tr) // room transition overrides wall
			{
				pos += to_add;
				last_move = to_add;
			}
			else {
				this->pos += to_add - out; // we could just do nothing here but if we move more than 1 in any direction then this is relevant
				last_move = to_add - out;
			}
		}
		
	}
	
	Room::room_tr const* get_used_room_tr() const { return room_tr_used; }
	pos_t get_last_move() const { return last_move; }
	
private:
	// the room_tr used in the previous move_pos operation. nullptr if it didn't use any. probably useless right now
	Room::room_tr const* room_tr_used;
	pos_t last_move; // the vector we moved the last time. to_add of the previous operation
};

class Area : public Simple_component<area_t>//, public Base_position, public Base_area
{
public:
	Area(): Simple_component() {}
	Area(area_t area): Simple_component(area) {}
	Area(pos_t pos = pos_t{}) { val = area_t{pos}; } // special constructor

	std::string get_name() const { return "area"; }
	std::string get_value_str() const { return val.to_string(); }
	
	Component* copy() const { return new Area{*this}; }

	// from Base_position ---
	inline void set_pos(pos_t pos) { this->val = area_t(pos); }
	inline pos_t get_pos() const { return val.get_pos1(); }
	// ---
	// from Base_area ---
	inline bool is_in_area(pos_t pos) const { return val.is_in_area(pos); } // we assume the room is same
	inline surf_area_t get_surf_area() const { return val.get_dim().get_surf_area(); }
	// ---
private:
};

/*enum class Dir {
	NORTH, EAST, SOUTH, WEST//, NUMBER_OF_DIRS
};
std::string dir_to_string(const Dir dir, bool concise = false) {
	switch (dir) {
	case Dir::NORTH: return concise?"N":"north";
	case Dir::EAST:  return concise?"E":"east";
	case Dir::SOUTH: return concise?"S":"south";
	case Dir::WEST:  return concise?"W":"west";
	default: assert(0);
	}
}

class Direction : public Simple_component<Dir>
{
public:
	Direction(Dir dir): Simple_component(dir) {}

	std::string get_name() const { return "direction"; }
	std::string get_value_str() const { return dir_to_string(val); }
private:
};*/

class H_or_V : public Simple_component<bool>
{
public:
	H_or_V(bool horizontal): Simple_component(horizontal) {}

	std::string get_name() const { return "H_or_V"; }
	std::string get_value_str() const { return val?"horizontal":"vertical"; }
	
	Component* copy() const { return new H_or_V{*this}; }

	inline void toggle_status() { val = !val; }

	inline bool is_horizontal() const { return val; }
	inline bool is_vertical() const { return !val; }
private:
};

class Length : public Simple_component<length_t>
{
public:
	Length(): Simple_component() {}
	Length(length_t l): Simple_component(l) {}

	std::string get_name() const { return "length"; }
	std::string get_value_str() const { return std::to_string(val); }
	
	Component* copy() const { return new Length{*this}; }
private:
};

}
}

#endif // COORDINATE_COMPS_H
