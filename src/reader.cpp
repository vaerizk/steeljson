#include "reader.h"
#include <sstream>
#include "exception.h"
#include "lexer.h"
#include "parser.h"

//steeljson::value steeljson::read(std::istream& stream) {
//	steeljson::lexer lexer { &stream };
//	steeljson::value result = steeljson::null;
//	steeljson::parser parser { &lexer, &result };
//
//	if (parser.parse()) {
//		throw std::exception("parse error");
//	}
//
//	return result;
//}

steeljson::value steeljson::read_document(std::istream& stream) {
	steeljson::lexer lexer{ &stream };
	steeljson::value result{ steeljson::null };
	steeljson::parser parser{ &lexer, &result };

	try {
		if (parser.parse()) {
			throw steeljson::parse_error();
		}
	} catch (const std::exception&) {
		throw steeljson::parse_error();
	}

	return result;
}

bool steeljson::try_read(std::istream& stream, steeljson::value& value) {
	value = steeljson::null;
	steeljson::lexer lexer{ &stream };
	steeljson::parser parser{ &lexer, &value };

	try {
		return (parser.parse() == 0);
	} catch (...) {
		return false;
	}
}
