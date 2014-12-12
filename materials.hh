#ifndef MATERIALS_HH
#define MATERIALS_HH
#include "coordinates.hh"
#include <string>

using namespace Coordinates;

namespace Room_System {
namespace Materials {

enum color_enum {
	BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

struct material_t {
public:
	material_t(const std::string& name, const color_enum& color, bool is_opaque);
	material_t(const std::string &name, const color_enum &color, const length_t& max_view_distance, const dist_t& light_distance_mod);

	std::string get_name() const { return name; }
	color_enum get_color() const { return color; }
	std::string get_color_name() const;

	length_t get_max_view_distance() const { return max_view_distance; }
	dist_t get_light_distance_mod() const { return light_distance_mod; }
	bool is_opaque() const { return max_view_distance == 0; }

	bool operator<(const material_t& rhs) const {
		return id < rhs.id;
	}

	const unsigned id{prev_id++};
private:
	static unsigned prev_id;

	std::string name;
	color_enum color;

	unsigned hardness; // 0 is like cotton, 10 is like diamond

	// TRANSPARENCY TRAITS //
	// this is the maximum distance you can see after this material no matter how good your eye-sight is
	// 0 is fully opaque, (unsigned)-1 is clear like vacuum
	// represents translucensy (scattering/fuzzyness)
	const length_t max_view_distance;
	// how much does looking through this material alters your normal maximum viewing distance
	// 0 means the material is clear, negative values increase opaqueness, positive values enhance your vision like lenses.
	// represents transparency (when <=0)
	const dist_t light_distance_mod;
	// if light_distance_mod is ~0 but max_view_distance is ~0 then the effect is the same as fuzzy glass or bad fog.
	// i light_distance_mod is >0 then the effect is same as telescope or eyeglasses
};

}
}

#endif // MATERIALS_HH
