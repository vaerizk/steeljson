
#include <cctype>
#include <iostream>
#include <steeljson/value.h>

// convert case of all string values to uppercase
struct uppercasing_visitor {
	template<typename T> steeljson::value operator()(T v) {
		// do nothing with the value that is not a string, array or object
		return v;
	}

	steeljson::value operator()(const std::string& str) {
		// convert all symbols to uppercase
		std::string result(str.size(), 0);

		for (std::size_t i = 0; i < result.size(); i++) {
			result[i] = std::toupper(str[i]);
		}

		return result;
	}

	// recursively handle array elements
	steeljson::value operator()(const steeljson::array& arr) {
		steeljson::array result(arr.size());

		for (std::size_t i = 0; i < arr.size(); ++i) {
			result[i] = steeljson::visit(*this, arr[i]);
		}

		return result;
	}

	// recursively handle object members
	steeljson::value operator()(const steeljson::object& obj) {
		steeljson::object result;

		for (const steeljson::object::value_type& member : obj) {
			result[member.first] = steeljson::visit(*this, member.second);
		}

		return result;
	}
};

int main(int argc, char* argv[]) {
	uppercasing_visitor vis;

	const steeljson::value v{
		steeljson::object{
			{ "null", steeljson::null },
			{ "integer", 12345 },
			{ "floating point", 12.3456789 },
			{ "string", "this is string" },
			{ "string with number", "string with number-123" },
			{ "array with string", steeljson::array{ 1, "array element", 3 } },
			{ "object", steeljson::object{
					{ "first object member", -10 },
					{ "second object member", -10.3579 },
					{ "third object member", "object member" },
				}
			}
		}
	};

	std::cout << steeljson::visit(vis, v);

	return 0;
}
