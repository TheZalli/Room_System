#ifndef HELPER_FUNCTIONS_HH
#define HELPER_FUNCTIONS_HH

#include <algorithm> // for pair

template<typename T>
void delete_pointed_to(T* const ptr) {
	delete ptr;
}

template<typename A, typename pointed_t>
void delete_pointed_to_by_pair(const std::pair<A, pointed_t*>& pair) {
	delete pair.second;
}


#endif // HELPER_FUNCTIONS_HH
