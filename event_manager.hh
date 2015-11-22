#ifndef EVENT_MANAGER_HH
#define EVENT_MANAGER_HH

#include "entity.hh"
#include "Components/components_include.hh"

#include <set>

using namespace Room_System::Comps;

namespace Room_System {

typedef void (*callback_func_ptr)(Component* _listened_comp);

struct event_listener {
	/**
	 * @brief _listened_comp is the component which we check the changes of
	 */
	Component* _listened_comp; // TODO
	
	/**
	 * @brief callback_function is the function that is called when _listened_comp changes
	 */
	callback_func_ptr _callback_function;
	
	/**
	 * @brief The ev_list_compare struct is a functor that compares two event_listener's listener components
	 * pointers addresses.
	 * for set compare purposes
	 */
	struct liscomp_compare {
		bool operator()(const event_listener& a, const event_listener& b) const {
			return a._listened_comp < b._listened_comp; // pointer comparison
		}
	};
};

class Event_manager
{
public:
	Event_manager();
	
	void add_listener(Component* listened, callback_func_ptr function) {
		listeners.insert({listened, function});
	}
	
	typedef std::set<event_listener, event_listener::liscomp_compare> listener_container;
	listener_container listeners;
};

}

#endif // EVENT_MANAGER_HH
