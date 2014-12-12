#include "coordinates.hh"

using namespace Coordinates;

// ---

pos_t pos_t::operator+(const pos_t& p) const {
	return pos_t(x + p.x, y + p.y);
}

pos_t pos_t::operator-(const pos_t& p) const {
	return pos_t(x - p.x, y - p.y);
}

pos_t pos_t::operator*(const pos_t& p) const
{
	return pos_t(x*p.x, y*p.y);
}

pos_t pos_t::operator/(const pos_t& p) const
{
	return pos_t(x/p.x, y/p.y);
}

pos_t pos_t::operator*(int rhs) const
{
	return pos_t(x*rhs, y*rhs);
}

pos_t pos_t::operator/(int rhs) const
{
	return pos_t(x/rhs, y/rhs);
}

// ---

dim_t area_t::get_dim() const
{
	return (dim_t)(pos2-pos1);
}

pos_t area_t::get_mid_pos() const
{
	return (pos1 + pos2)/2;
}

void area_t::move_area_to(pos_t new_pos)
{
	pos_t dim{pos2-pos1};
	pos1 = new_pos;
	pos2 = pos1 + dim;
}

bool area_t::is_in_area(const pos_t& p) const
{
	return	(pos1.x <= p.x) && (p.x <= pos2.x) &&
			(pos1.y <= p.y) && (p.y <= pos2.y);
}

bool area_t::is_overlap(const area_t area) const
{
	return	(pos1.x <= area.pos2.x) && (pos2.x >= area.pos2.x) &&
			(pos1.y <= area.pos2.y) && (pos2.y >= area.pos2.y);
}

pos_t Coordinates::operator+(const pos_t& pos, const dim_t& dim)
{
	return pos_t(dim.w + pos.x, dim.l + pos.y);
}
