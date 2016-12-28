#ifndef STEELJSON_EXCEPTION_H
#define STEELJSON_EXCEPTION_H

#include <exception>

namespace steeljson {

	class parse_error : public std::exception {
		public:
			parse_error() = default;
	};

}

#endif // STEELJSON_EXCEPTION_H
