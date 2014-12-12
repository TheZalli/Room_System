#ifndef ACTOR_HH
#define ACTOR_HH
#include "world_object.hh"

namespace Room_System {

class Actor : public world_object
{
public:
	Actor();
	Actor(pos_t pos, dim_t dim = dim_t(1,1)): world_object{pos, dim} {}

	virtual void update() = 0;

	/*std::string get_name() const = 0;
	virtual bool is_opaque() const { return true; }*/
private:

};

}

#endif // ACTOR_HH
