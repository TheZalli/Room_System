#include "event_handler.hh"


/*void Room_System::Event_handler::move_object_to_room(Room_System::World_object* obj_ptr, Room::room_tr rtr) {
													 //, Room_System::Room* room_ptr, pos_t pos) {
	assert(obj_ptr->get_room() == rtr.room_in);
	assert(rtr.area_from.is_in_area(obj_ptr->get_pos())); // checks if the transition is valid

	add_event(e_tr_room(obj_ptr, rtr));//, obj_ptr->get_room(), room_ptr, pos));

	if (obj_ptr->get_room()) obj_ptr->get_room()->remove_object(obj_ptr);

	if (rtr.room_to) rtr.room_to->add_object(obj_ptr);
	else obj_ptr->set_room(rtr.room_to);

	obj_ptr->set_pos(rtr.pos_to +
				(obj_ptr->get_pos() - rtr.area_from.pos1)); // if rtr is larger than one square
}

bool Room_System::Event_handler::move_object(Room_System::World_object* obj_ptr, pos_t to_add) {
	pos_t destination = obj_ptr->get_pos() + to_add;
	if (obj_ptr->get_room()) {
		if (obj_ptr->is_allowed_pos(destination)) {
			add_event(e_move(obj_ptr, obj_ptr->get_pos(), destination));
			obj_ptr->set_pos(destination);
			return true;
		} else {
			//const std::pair<Room*, pos_t>& ptr_n_pos = obj_ptr->check_room_transitions_on(obj_ptr->get_pos());
			const Room::room_tr rtr = obj_ptr->get_room()->get_room_tr(obj_ptr->get_pos());
			//ptr_n_pos.first) {
			if (rtr != Room::none_room_tr) {
				move_object_to_room(obj_ptr, rtr);

				add_event(e_move(obj_ptr, obj_ptr->get_pos(), obj_ptr->get_pos() + to_add));
				obj_ptr->set_pos(obj_ptr->get_pos() + to_add);
				return true;
			}
		}
	} else {
		add_event(e_move(obj_ptr, obj_ptr->get_pos(), obj_ptr->get_pos() + to_add));
		obj_ptr->set_pos(obj_ptr->get_pos() + to_add);
		return true;
	}
	return false;
}*/

void Room_System::Event_handler::process_events()
{
	for (world_event& e : events) {
		switch (e.get_event_id_s()) {
		case e_move_id:
			e_move me = dynamic_cast<e_move&>(e);
			assert(me.actor->get_pos() == me.from);
			move_object(const_cast<World_object*>(me.actor), me.to); // tries to move
		break;
		}
	}
}

void Room_System::Event_handler::turn()
{
	realized_events.empty();
	process_events();
}

std::vector<Room_System::world_event> Room_System::Event_handler::get_events_by_obj(const World_object* const obj_ptr) const {
	std::vector<world_event> to_return{};
	for (const world_event e : events) {
		if (e.actor == obj_ptr) to_return.push_back(e);
	}
	return to_return;
}

std::vector<Room_System::world_event> Room_System::Event_handler::get_events_by_type(Room_System::event_id id) const
{
	std::vector<world_event> to_return{};
	for (auto it = events.cbegin(); it != events.cend(); it++) {
		//if (typeid(*it) == typeid(E)) {
		//if (E::get_event_id() == (*it).get_event_id()) {
		if (id == it->get_event_id()) {
			world_event& er = const_cast<world_event&>(*it);
			//to_return.push_back(dynamic_cast<E&>(er));
			to_return.push_back(er);
		}
	}
	return to_return;
}

std::vector<Room_System::world_event> Room_System::Event_handler::get_events_by_obj_n_type(const Room_System::World_object* const obj_ptr, Room_System::event_id id) const
{
	std::vector<world_event> to_return{};
	for (event_dt::const_iterator it = events.cbegin(); it != events.cend(); it++) {
		//if ((typeid(*it) == typeid(E)) && (it->actor == obj_ptr)) {
		//if ((E::get_event_id_s() == (*it).get_event_id()) && (it->actor == obj_ptr)) {
		if ((id == it->get_event_id()) && (it->actor == obj_ptr)) {
			world_event& er = const_cast<world_event&>(*it);
			//to_return.push_back(dynamic_cast<E&>(er));
			to_return.push_back(er);
		}
	}
	return to_return;
}

bool Room_System::Event_handler::move_object(Room_System::World_object* obj_ptr, pos_t to_add) const
{
	pos_t destination = obj_ptr->get_pos() + to_add;
	if (obj_ptr->get_room()) {
		if (obj_ptr->is_allowed_pos(destination)) { // normal unobstructed moving
			realize_event(e_move(obj_ptr, obj_ptr->get_pos(), destination));
			obj_ptr->set_pos(destination);

			return true;
		} else {
			//const std::pair<Room*, pos_t>& ptr_n_pos = obj_ptr->check_room_transitions_on(obj_ptr->get_pos());
			const Room::room_tr rtr = obj_ptr->get_room()->get_room_tr(obj_ptr->get_pos());
			//ptr_n_pos.first) {
			if (rtr != Room::none_room_tr) { // room transition
				move_object_to_room(obj_ptr, rtr); // changes player's position

				realize_event(e_move(obj_ptr, obj_ptr->get_pos(), obj_ptr->get_pos() + to_add));
				obj_ptr->set_pos(obj_ptr->get_pos() + to_add);
				return true;
			}
		}
	} else { // we are in the void of nullness, no idea why would we want to be in there
		realize_event(e_move(obj_ptr, obj_ptr->get_pos(), destination));
		obj_ptr->set_pos(destination);
		return true;
	}
	return false;
}

void Room_System::Event_handler::move_object_to_room(Room_System::World_object* obj_ptr, Room_System::Room::room_tr rtr) const
{
	assert(obj_ptr->get_room() == rtr.room_in); // checks if the transition is valid
	assert(rtr.area_from.is_in_area(obj_ptr->get_pos())); // ditto

	realize_event(e_tr_room(obj_ptr, rtr));

	if (obj_ptr->get_room()) obj_ptr->get_room()->remove_object(obj_ptr);

	if (rtr.room_to) rtr.room_to->add_object(obj_ptr);
	else obj_ptr->set_room(rtr.room_to); // should be nullptr here

	obj_ptr->set_pos(rtr.pos_to +
			(obj_ptr->get_pos() - rtr.area_from.pos1)); // if rtr is larger than one square
}

/*template<class E>
std::vector<E> Room_System::Event_handler::get_events_by_event_id() const
{
	std::vector<E> to_return{};
	for (const world_event e : events) {
		if (typeid(e) == typeid(E)) to_return.push_back(dynamic_cast<E>(e));
	}
	return to_return;
}*/

/*template<class E>
std::vector<E> Room_System::Event_handler::get_events_by_obj_n_type(const World_object* const obj_ptr) const
{
	std::vector<E> to_return{};
	for (const world_event e : events) {
		if ((typeid(e) == typeid(E)) && (e.actor == obj_ptr))
			to_return.push_back(dynamic_cast<E>(e));
	}
	return to_return;
}*/


Room_System::world_event::~world_event() {

}
