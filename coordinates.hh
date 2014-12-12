#ifndef COORDINATES_HH
#define COORDINATES_HH

#include <cstddef>
#include <limits>
#include <type_traits>
//#include <boost/functional/hash.hpp>

namespace Coordinates {

namespace dist {
	typedef int dist_t;

	//static const int max_dist = std::numeric_limits<int>::max();
	//static const int min_dist = std::numeric_limits<int>::min();
}
using namespace dist;

namespace length {
	typedef unsigned length_t;

	//static const long int max_length = std::numeric_limits<unsigned>::max();
	//static const long int min_length = std::numeric_limits<unsigned>::min();
}
using namespace length;

// ---

struct pos_t {
	dist_t x;
	dist_t y;

	pos_t(): x{}, y{} {}
	pos_t(dist_t x, dist_t y): x{x}, y{y} {}
	pos_t(const pos_t& p): x{p.x}, y{p.y} {}

	pos_t operator+(const pos_t& p) const;
	pos_t operator-(const pos_t& p) const;
	pos_t operator*(const pos_t& p) const;
	pos_t operator/(const pos_t& p) const;

	pos_t operator*(int rhs) const;
	pos_t operator/(int rhs) const;

	length_t max() const { return (x > y) ? x : y; }
	length_t min() const { return (x < y) ? x : y; }

	/*friend std::size_t hash_value(const pos_t& pos) {
		std::size_t seed{0};
		boost::hash_combine(seed, pos.x);
		boost::hash_combine(seed, pos.y);
		return seed;
		//return (size_t)(pos.x) * 1024 + (size_t)(pos.y);
	}*/
};

// ---

struct dim_t {
	length_t w; // width in x coordinate
	length_t l; // length in y coordinate

	dim_t() = default;
	dim_t(length_t w, length_t l): w{w}, l{l} {}
	dim_t(const pos_t& pos): w{(length_t)pos.x}, l{(length_t)pos.y} {}

	dim_t operator+(const dim_t& p) const;
	dim_t operator-(const dim_t& p) const;
	dim_t operator*(const dim_t& p) const;
	dim_t operator/(const dim_t& p) const;

	dim_t operator*(int rhs) const;
	dim_t operator/(int rhs) const;

	length_t max() const { return (w > l) ? w : l; }
	length_t min() const { return (w < l) ? w : l; }
};

pos_t operator+(const pos_t& pos, const dim_t& dim);
// ---

typedef dim_t shape_t; // shape, currently the same as dimensions

// ---

struct area_t {
	pos_t pos1;
	pos_t pos2;

	area_t(): pos1{}, pos2{} {}
	//area_t(const pos_t& p1, const pos_t& p2): pos1{p1}, pos1{p2} {}
	area_t(const pos_t& p): pos1{p}, pos2{p} {}
	area_t(const pos_t& pos, const dim_t& dim): pos1{pos}, pos2{pos + dim} {}
	area_t(dist_t x1, dist_t y1, dist_t x2, dist_t y2): pos1{x1,y1}, pos2{x2,y2} {}

	dim_t get_dim() const;
	inline pos_t get_pos1() const { return pos1; }

	pos_t get_mid_pos() const;

	void move_area_to(pos_t new_pos);

	bool is_in_area(const pos_t& p) const;
	bool is_overlap(const area_t area) const;

	/*friend std::size_t hash_value(const area_t& pos) {
		std::size_t seed{0};
		boost::hash_combine(seed, pos.pos1);
		boost::hash_combine(seed, pos.pos2);
		return seed;
	}*/
};


// ---

/*/ dimensions of ncurses screens and objects on them
struct scr_dim_t {
	unsigned rows; // y
	unsigned cols; // x

	scr_dim_t() = default;
	scr_dim_t(unsigned row, unsigned col): rows{row}, cols{col} {}
	scr_dim_t(const dim_t& dim): rows{dim.l}, cols{dim.w} {}

	dim_t to_dim() const { return dim_t(cols, rows); }
};//*/

// ---

/*struct overlap_key {
	bool operator()(const area_t& area1, const area_t& area2) const {
		return area1.is_overlap(area2);
	}
};*/

}

#endif // COORDINATES_HH
