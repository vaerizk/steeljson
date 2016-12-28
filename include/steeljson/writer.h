#ifndef STEELJSON_WRITER_H
#define STEELJSON_WRITER_H

#include <ostream>
#include <string>
#include "value.h"

namespace steeljson {

	void write(std::ostream&, const steeljson::value&);

}

#endif // STEELJSON_WRITER_H
