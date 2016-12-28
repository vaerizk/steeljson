#include "symbol.h"
#include <exception>

using namespace steeljson;

symbol::symbol(char value) :
	character(value),
	symbol_type(type_t::character) { }

symbol::symbol(const steeljson::object_member& value) :
	object_member(value),
	symbol_type(type_t::object_member) { }

symbol::symbol(const steeljson::value& value) :
	json_value(value),
	symbol_type(type_t::json_value) { }

char symbol::get_char() const {
	if (!(this->symbol_type == type_t::character)) {
		throw std::exception();
	};

	return this->character;
}

const steeljson::object_member& symbol::get_object_member() const {
	if (!(this->symbol_type == type_t::object_member)) {
		throw std::exception();
	};

	return this->object_member;
}

const steeljson::value& symbol::get_json_value() const {
	if (!(this->symbol_type == type_t::json_value)) {
		throw std::exception();
	};

	return this->json_value;
}

symbol::type_t symbol::type() const {
	return this->symbol_type;
}
