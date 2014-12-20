#ifndef PC_HH
#define PC_HH
#include "character.hh"

using namespace Coordinates;
using namespace Room_System;

// the player character
class PC : public Character
{
public:
	//PC();
	PC(pos_t pos, dim_t dim, std::string name, Room* room = nullptr);

	PC* create() const {
		assert(0); // PC should be unique
		//return new PC();
	}
	PC* clone() const {
		assert(0); // PC should be unique
		//return new PC(*this);
	}

	bool is_opaque() const { return true; }

	const std::vector<std::string> get_interaction_options() const;
	bool interact(int) { return false; }



private:
};

#endif // PC_HH
