#ifndef STEELJSON_READER_H
#define STEELJSON_READER_H

#include <iostream>
#include <string>
#include "value.h"

namespace steeljson {

	// steeljson::value read(std::istream&);
	steeljson::value read_document(std::istream&);
	bool try_read(std::istream&, steeljson::value&); // TODO: use std::optional (c++17)

}

#endif // STEELJSON_READER_H
