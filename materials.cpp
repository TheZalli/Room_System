#include "materials.hh"

using namespace Coordinates;
using namespace Room_System;
using namespace Materials;

unsigned material_t::prev_id = 0;

material_t::material_t(const std::string& name, const color_enum& color, bool is_opaque):
	name{name}, color{color}, max_view_distance{is_opaque ? 0 : (unsigned)-1}, light_distance_mod{0}
{}

material_t::material_t(const std::string& name, const color_enum& color, const length_t& max_view_distance, const dist_t& light_distance_mod):
	name{name}, color{color}, max_view_distance{max_view_distance}, light_distance_mod{light_distance_mod}
{}
