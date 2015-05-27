#ifndef COORDINATE_COMPS_H
#define COORDINATE_COMPS_H

#include "component.hh"
#include "coordinates.hh"

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

class Position : public Simple_component<pos_t>//, public Base_position
{
public:
	Position(): Simple_component() {}
	Position(pos_t pos): Simple_component(pos) {}

	std::string get_name() const { return "position"; }
	std::string get_value_str() const { return val.to_string(); }
	
	Component* copy() const { return new Position{*this}; }

	inline void add_pos(pos_t pos) { this->val += pos; }
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