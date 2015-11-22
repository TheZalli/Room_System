#include "coordinates.hh"

using namespace Coordinates;

// ---

pos_t::pos_t():
	x{}, y{}
{}

pos_t::pos_t(dist_t x, dist_t y):
	x{x}, y{y}
{}

pos_t::pos_t(const pos_t& p):
	x{p.x}, y{p.y}
{}


void pos_t::set_x(const dist_t& value)
{
	x = value;
}

dist_t pos_t::get_y() const
{
	return y;
}

void pos_t::set_y(const dist_t& value)
{
	y = value;
}


pos_t pos_t::operator+(const pos_t& p) const {
	return pos_t(x + p.x, y + p.y);
}

pos_t pos_t::operator-(const pos_t& p) const {
	return pos_t(x - p.x, y - p.y);
}

pos_t pos_t::operator-() const{
	return pos_t(-x, -y);
}

void pos_t::operator+=(const pos_t& p) {
	this->x += p.x;
	this->y += p.y;
}

void pos_t::operator-=(const pos_t& p) {
	this->x -= p.x;
	this->y -= p.y;
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

bool pos_t::operator==(const pos_t& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

bool pos_t::operator!=(const pos_t& rhs) const
{
	return (x != rhs.x) || (y != rhs.y);
}

dist_t pos_t::select_x_or_y(bool select_x) const {
	return select_x ? x : y;
}

std::string pos_t::to_string() const
{
	return std::to_string(x) + ", " + std::to_string(y);
}

dist_t pos_t::get_x() const
{
	return x;
}

// ---

dim_t::dim_t(length_t w, length_t l):
	w{w},
	l{l}
{}

dim_t::dim_t(const pos_t& pos):
	w{ (length_t) pos.x },
	l{ (length_t) pos.y }
{}


length_t dim_t::get_w() const
{
	return w;
}

void dim_t::set_w(const length_t& value)
{
    w = value;
}
length_t dim_t::get_l() const
{
    return l;
}

void dim_t::set_l(const length_t& value)
{
    l = value;
}


pos_t dim_t::to_pos() const
{
	return pos_t((dist_t) w, (dist_t) l);
}

surf_area_t dim_t::get_surf_area() const
{
	return w * l;
}

dim_t dim_t::operator+(const dim_t& p) const
{
	return dim_t(w + p.w, l + p.l);
}

dim_t dim_t::operator-(const dim_t& p) const
{
	return dim_t(w - p.w, l - p.l);
}

dim_t dim_t::operator-() const
{
	return dim_t(-w, -l);
}

dim_t dim_t::operator*(const dim_t& p) const
{
	return dim_t(w*p.w, l*p.l);
}

dim_t dim_t::operator/(const dim_t& p) const
{
	return dim_t(w/p.w, l/p.l);
}

dim_t dim_t::operator*(int rhs) const
{
	return dim_t(w*rhs, l*rhs);
}

dim_t dim_t::operator/(int rhs) const
{
	return dim_t(w/rhs, l/rhs);
}

bool dim_t::operator==(const dim_t& rhs) const
{
	return (w == rhs.w) && (l == rhs.l);
}

length_t dim_t::max() const
{
	return (w > l) ? w : l;
}

length_t dim_t::min() const
{
	return (w < l) ? w : l;
}

std::string dim_t::to_string() const
{
	return std::to_string(w) + "x" + std::to_string(l);
}


// ---

dim_t area_t::get_dim() const
{
	return (dim_t)(pos2 - pos1 + pos_t(1,1));
}

pos_t area_t::get_pos1() const
{
	return pos1;
}

pos_t area_t::get_pos2() const
{
	return pos2;
}

pos_t area_t::get_mid_pos() const
{
	return (pos1 + pos2)/2;
}

void area_t::move_area_to(pos_t new_pos)
{
	pos_t dim{ pos2 - pos1 };
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

bool area_t::operator<(const area_t& rhs) const {
	return this->get_dim().get_surf_area() < rhs.get_dim().get_surf_area();
}

pos_t Coordinates::operator+(const pos_t& pos, const dim_t& dim)
{
	return pos_t(dim.w + pos.x - 1, dim.l + pos.y -1);
}
