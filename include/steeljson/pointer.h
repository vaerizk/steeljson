#ifndef STEELJSON_POINTER_H
#define STEELJSON_POINTER_H

#include <string>
#include <vector>

namespace steeljson {

	typedef std::vector<std::string> pointer;

	pointer make_pointer(const std::string&);
	std::string to_string(const pointer&);

}

#endif // STEELJSON_POINTER_H
