#ifndef WORLD_OBJECT_HH
#define WORLD_OBJECT_HH
#include "coordinates.hh"
#include <string>

using namespace Coordinates;

namespace Room_System {
/*enum material {

};*/

class world_object
{
public:
	world_object();
	//world_object(const world_object& obj);
	world_object(pos_t pos, dim_t dim);
	virtual ~world_object() {}

	virtual world_object* create() const = 0;	// virtual constructor for creation
	virtual world_object* clone() const = 0;	// virtual constructor for copying

	virtual std::string get_name() const = 0;
	virtual bool is_opaque() const { return true; }

	pos_t get_pos() const { return pos; }
	dim_t get_dim() const { return dim; }
	area_t get_area() const { return area_t{pos, dim}; }

	void move(pos_t to) { pos = to; }

	bool operator<(const world_object& rhs) const {
		return id < rhs.id;
	}

	const unsigned id;
private:
	static unsigned prev_id;

	//area_t area;
	pos_t pos;
	dim_t dim;
};

class door : public world_object
{
public:
	door();
	door(const door& to_clone) = default;
	door(pos_t pos, dim_t dim, bool is_vertical, bool is_closed = true);
	door(pos_t pos, dim_t dim, bool is_vertical, door* linked_door, bool is_closed = true);
	~door() {}

	door* create() const {
		return new door();
	}
	door* clone() const {
		return new door(*this);
	}

	std::string get_name() const;
	bool is_opaque() const { return is_closed; }

	bool get_is_closed() const { return is_closed; }
	void open();
	void close();

	bool get_is_vertical() const { return is_vertical; }
	int get_length() const;

	door* get_linked_version(pos_t pos_to);
	void link(door* door) { linked_door = door; }

private:
	bool is_vertical;
	bool is_closed;
	int length;
	door* linked_door;
};

}

#endif // WORLD_OBJECT_HH
