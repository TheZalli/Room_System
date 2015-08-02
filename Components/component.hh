#ifndef COMPONENT_HH
#define COMPONENT_HH

#include <string>
#include <assert.h>
#include <set>
#include <map>

namespace Room_System {
namespace Comps { // namespace for all components (because I don't like adding cmp_ or something in fromt of every component name)

class Component
{
public:
	Component() {}
	virtual ~Component() {}
	
	/**
	 * @brief get_name
	 * @return a static name that should be same for all of the components of same type
	 */
	virtual std::string get_name() const = 0;
	
	/**
	 * @brief get_value_str
	 * @return a string representation of the value of the component
	 */
	virtual std::string get_value_str() const = 0;
	
	/**
	 * @brief copy function copies the component with new operator
	 * @return pointer to the copied component
	 */
	virtual Component* copy() const = 0;
	
	/* *
	 * @brief subscribe_by_default
	 * @return true if an entity with this component should be subscribed to the watched entity list of the entity manager
	 */
	//virtual bool watch_by_default() const { return false; }//*/
private:
	//const std::string& name_ref;
};



/**
 * @brief The Simple_component class is a component that has a single value of type T
 */
template<typename T>
class Simple_component : public Component
{
protected:
	Simple_component(): Component(), val{} {}
	Simple_component(T value): Component(), val{value} {}
	
public:
	inline const T& get_value() const	{ return val; }
	inline       T& get_value_ref()		{ return val; }

	inline void set_value(const T& value) { val = value; }
	
protected:
	T val;
};


class Name : public Simple_component<std::string>
{
public:
	Name(): Simple_component() {}
	Name(const std::string& str): Simple_component(str) {}

	std::string get_name() const { return "name"; }
	std::string get_value_str() const { return val; }
	
	Component* copy() const { return new Name(*this); }
	
};

// a collidable component
class Obstacle : public Simple_component<bool>
{
public:
	Obstacle(bool collidable = true): Simple_component(collidable) {}

	std::string get_name() const { return "obstacle"; }
	std::string get_value_str() const { return val?"collidable":"not collidable"; }
	
	Component* copy() const { return new Obstacle(*this); }

	void toggle_collide() { val = !val; }
};

/*typedef void (*action_fun_ptr)(Component*); // a simple action function pointer
// only has one action when interacted with (eg open/close door)
class Interactable_simple : public Component
{
public:
	Interactable_simple(action_fun_ptr = nullptr): func_ptr{action_fun_ptr} {}
	~Interactable_simple() {}

	std::string get_name() const { return "interactable_simple"; }
	std::string get_value_str() const { return std::string((size_t)func_ptr); }

	inline void set_fun(action_fun_ptr function_pointer) { this->func_ptr= function_pointer; }
	inline action_fun_ptr get_fun() const { return func_ptr; }
private:
	action_fun_ptr func_ptr;
};*/

}
}
#endif // COMPONENT_HH
