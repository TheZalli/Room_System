#ifndef COORDINATES_HH
#define COORDINATES_HH

#include <cstddef>
#include <limits>
#include <type_traits>
#include <string>
//#include <boost/functional/hash.hpp>

namespace Coordinates {

typedef int dist_t;
typedef unsigned length_t;

// ---

class pos_t {
public:
	pos_t();
	pos_t(dist_t x, dist_t y);
	pos_t(const pos_t& p);
	
	dist_t get_x() const;
	void set_x(const dist_t& value);
	
	dist_t get_y() const;
	void set_y(const dist_t& value);
	
	pos_t operator+(const pos_t& p) const;
	pos_t operator-(const pos_t& p) const;
	pos_t operator-() const;

	void operator+=(const pos_t& p);
	void operator-=(const pos_t& p);

	pos_t operator*(const pos_t& p) const;
	pos_t operator/(const pos_t& p) const;

	pos_t operator*(int rhs) const;
	pos_t operator/(int rhs) const;

	bool operator==(const pos_t& rhs) const;
	bool operator!=(const pos_t& rhs) const;

	dist_t max() const { return (x > y) ? x : y; }
	dist_t min() const { return (x < y) ? x : y; }
	
	dist_t select_x_or_y(bool select_x) const;

	struct pos_key {
		bool operator()(const pos_t& p1, const pos_t& p2) const {
			return (p1.x < p2.x && p1.y < p2.y) ||
				   (p1.x > p2.x && p1.y > p2.y) ; // true if it is on the right and down or left and up from &this
		}
	};

	std::string to_string() const;

	/*friend std::size_t hash_value(const pos_t& pos) {
		std::size_t seed{0};
		boost::hash_combine(seed, pos.x);
		boost::hash_combine(seed, pos.y);
		return seed;
		//return (size_t)(pos.x) * 1024 + (size_t)(pos.y);
	}*/
public:
	dist_t x;
	dist_t y;
};

// ---

// a datatype for surface areas (if length_t is m this is m²)
typedef unsigned long surf_area_t;

class dim_t {
public:
	dim_t() = default;
	dim_t(length_t w, length_t l);
	dim_t(const pos_t& pos);
	
	length_t get_w() const;
	void set_w(const length_t& value);
	
	length_t get_l() const;
	void set_l(const length_t& value);
	
	pos_t to_pos() const;

	surf_area_t get_surf_area() const;

	dim_t operator+(const dim_t& p) const;
	dim_t operator-(const dim_t& p) const;
	dim_t operator-() const;

	dim_t operator*(const dim_t& p) const;
	dim_t operator/(const dim_t& p) const;

	dim_t operator*(int rhs) const;
	dim_t operator/(int rhs) const;

	bool operator==(const dim_t& rhs) const;

	length_t max() const;
	length_t min() const;

	std::string to_string() const;
	
public:
	length_t w; // width in x coordinate
	length_t l; // length in y coordinate
};

pos_t operator+(const pos_t& pos, const dim_t& dim);
// ---


// ---

typedef dim_t shape_t; // shape, currently the same as dimensions

// ---

class area_t {
public:
	area_t(): pos1{}, pos2{} {}
	//area_t(const pos_t& p1, const pos_t& p2): pos1{p1}, pos1{p2} {}
	area_t(const pos_t& p): pos1{p}, pos2{p} {}
	area_t(const pos_t& pos, const dim_t& dim): pos1{pos}, pos2{pos + dim} {}
	area_t(dist_t x1, dist_t y1, dist_t x2, dist_t y2): pos1{x1,y1}, pos2{x2,y2} {}

	dim_t get_dim() const;
	
	pos_t get_pos1() const;
	pos_t get_pos2() const;

	pos_t get_mid_pos() const;

	void move_area_to(pos_t new_pos);

	bool is_in_area(const pos_t& p) const;
	bool is_overlap(const area_t area) const;

	// measures the areas of the two areas
	bool operator<(const area_t& rhs) const;

	std::string to_string() const {
		return '(' + pos1.to_string() + "), (" + pos2.to_string() + ')';
	}

	/*friend std::size_t hash_value(const area_t& pos) {
		std::size_t seed{0};
		boost::hash_combine(seed, pos.pos1);
		boost::hash_combine(seed, pos.pos2);
		return seed;
	}*/
	
public:
	pos_t pos1;
	pos_t pos2;
};

}

#endif // COORDINATES_HH
