#include "writer.h"
#include <sstream>
#include <stack>
#include "symbol.h"

void write_number(std::ostream& stream, const steeljson::value& number) {
	if (number.is<float>()) {
		stream << number.as<float>();
		return;
	} else if (number.is<double>()) {
		stream << number.as<double>();
		return;
	} else if (number.is<std::int64_t>()) {
		stream << number.as<std::int64_t>();
		return;
	} else {
		throw std::exception();
	}
}

void write_array(std::ostream& stream, const steeljson::array& array, std::stack<steeljson::symbol>& symbols) {
	symbols.push(steeljson::symbol(']'));

	for (steeljson::array::const_reverse_iterator r = array.crbegin(); r != array.crend(); ++r) {
		symbols.push(steeljson::symbol(*r));
		symbols.push(steeljson::symbol(','));
	}
	if (array.size() > 0) {
		symbols.pop(); // remove unnecessary ',' before the first array element
	}
	symbols.push(steeljson::symbol('['));
}

void write_object(std::ostream& stream, const steeljson::object& object, std::stack<steeljson::symbol>& symbols) {
	symbols.push(steeljson::symbol('}'));

	for (steeljson::object::const_iterator r = object.cbegin(); r != object.cend(); ++r) {
		symbols.push(steeljson::symbol(*r));
		symbols.push(steeljson::symbol(','));
	}
	if (object.size() > 0) {
		symbols.pop(); // remove unnecessary ',' before the first object member
	}
	symbols.push(steeljson::symbol('{'));
}

void write_value(std::ostream& stream, const steeljson::value& value, std::stack<steeljson::symbol>& symbols) {
	switch (value.type()) {
		case (steeljson::value::type_t::null): {
			stream << "null";
			return;
		}
		case (steeljson::value::type_t::boolean): {
			stream << (value.as<bool>() ? "true" : "false");
			return;
		}
		case (steeljson::value::type_t::number): {
			write_number(stream, value);
			return;
		}
		case (steeljson::value::type_t::string): {
			stream << '"' << value.as<const std::string&>() << '"';
			return;
		}
		case (steeljson::value::type_t::array): {
			write_array(stream, value.as<const steeljson::array&>(), symbols);
			return;
		}
		case (steeljson::value::type_t::object): {
			write_object(stream, value.as<const steeljson::object&>(), symbols);
			return;
		}
		default: {
			throw std::exception();
		}
	}
}

void steeljson::write(std::ostream& stream, const steeljson::value& value) {
	std::stack<symbol> symbols; // json terminals and non-terminals

	symbols.push(symbol(value));

	while (!symbols.empty()) {
		const symbol current_symbol = symbols.top();
		symbols.pop();
		switch (current_symbol.type()) {
			case symbol::type_t::character: {
					stream << current_symbol.get_char();
					break;
				}
			case symbol::type_t::object_member: {
				const object_member member = current_symbol.get_object_member();
				stream << '"' << member.first << '"' << ':';
				symbols.push(symbol(member.second));
				break;
			}
			case symbol::type_t::json_value: {
				write_value(stream, current_symbol.get_json_value(), symbols);
				break;
			}
			default: {
				throw std::exception();
			}
		}
	}
}
