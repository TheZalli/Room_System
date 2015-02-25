#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include "coordinates.hh"
#include "world_object.hh"
#include "room.hh"
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <typeinfo>
#include <stdexcept>
#include <assert.h>

using namespace Coordinates;

//class World_object;
//class Room;

namespace Room_System {

	enum event_id {
		e_wait_id,
		e_move_id,
		e_tr_room_id,
		e_invalid_event_id = -1
	};

	class world_event {
	public:
		const World_object* const actor;

		inline static event_id get_event_id_s() { return e_invalid_event_id; }
		inline event_id get_event_id() const { return id; }

		world_event(const World_object* const actor, event_id id): actor{actor}, id{id} {}
		virtual ~world_event();
	private:
		const event_id id;
	};

	class e_wait : public world_event { // does nothing
	public:
		inline static event_id get_event_id_s() { return e_wait_id; }
		e_wait(const World_object* const actor): world_event{actor, e_wait_id} {}
	};

	class e_move : public world_event { // moves from a to b
	public:
		inline static event_id get_event_id_s() { return e_move_id; }

		const pos_t from;
		const pos_t to;

		e_move(const World_object* const actor, pos_t from, pos_t to):
			world_event{actor, e_move_id}, from{from}, to{to} {}

	};

	class e_tr_room : public world_event {
	public:
		inline static event_id get_event_id_s() { return e_tr_room_id; }

		//Room* const room_from;
		//Room* const room_to;
		//pos_t pos;
		const Room::room_tr rtr;
		e_tr_room(const World_object* const actor, Room::room_tr rtr):
					world_event{actor, e_tr_room_id}, rtr{rtr} {}
		//e_tr_room(const World_object* const actor, Room* const room_from, Room* const room_to, pos_t pos):
		//	world_event{actor}, room_from{room_from}, room_to{room_to}, pos{pos} {}
	};

	class Event_handler
	{
	public:
		typedef std::vector<const world_event> event_dt;

		Event_handler(): events{} { /*cur_eh = this;*/ }

		/*inline static Event_handler* get_current_event_handler() {
			return cur_eh;
		}
		inline static void set_current_event_handler(Event_handler* eh_ptr) {
			cur_eh = eh_ptr;
		}*/

		// moves the object to a room and adds the event
		//void move_object_to_room(World_object* obj_ptr, Room::room_tr rtr);

		// moves the object to a position if possible and changes the room if needed
		//bool move_object(World_object* obj_ptr, pos_t to_add);

		inline void add_event(world_event e) { events.push_back(e); }

		// finishes turn
		void turn(); // TODO: use something more sophisticated than a simple turn based system

		inline world_event get_event(size_t i) const throw(std::out_of_range) {
			return events.at(i);
		}
		inline size_t get_event_amount() const { return events.size(); }
		inline const event_dt& get_events() const { return events; }
		inline const event_dt& get_realized_events() const { return realized_events; }
		//inline void clear_events() { events.clear(); }


		std::vector<world_event> get_events_by_obj(const World_object* const obj_ptr) const;
		//template<class E>
		std::vector<world_event> get_events_by_type(event_id id) const;
		//template<class E>
		std::vector<world_event> get_events_by_obj_n_type(const World_object* const obj_ptr, event_id id) const;

		inline world_event pop_event() {
			world_event e = events[0];
			events.pop_back();
			return e;
		}


		std::stringstream debug_msg; // DEBUG
	private:
		// moves object and adds a room transition event to the realized events if the obj walked into another room
		bool move_object(World_object* obj_ptr, pos_t to_add) const;

		// moves object to the room targeted by the rtr
		// there should be no need for this outside event_handler::move_object in normal walking
		void move_object_to_room(World_object* obj_ptr, Room::room_tr rtr) const;

		// processes the events and returns all the realized events
		void process_events();

		inline void realize_event(const world_event e) { realized_events.push_back(e); }

		event_dt events;
		event_dt realized_events;

		//static Event_handler* cur_eh;
	};
}
#endif // EVENT_HANDLER_H
