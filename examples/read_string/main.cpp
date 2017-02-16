#include <iostream>
#include <sstream>
#include <steeljson/reader.h>
#include <steeljson/value.h>

void print_value(const steeljson::value v) {
	if (v.is<bool>()) {
		std::cout << (v.as<bool>() ? "true" : "false");
	} else if (v.is<std::int64_t>()) {
		std::cout << v.as<std::int64_t>();
	} else if (v.is<float>()) {
		std::cout << std::fixed << v.as<float>();
	} else if (v.is<std::string>()) {
		std::cout << v.as<const std::string&>();
	} else if (v.is<steeljson::array>()) {
		auto arr = v.as<const steeljson::array&>();
		std::cout << '[';
		for (auto v : arr) {
			print_value(v);
			std::cout << ',';
		}
		std::cout << ']';
	} else if (v.is<steeljson::object>()) {
		auto obj = v.as<const steeljson::object&>();
		std::cout << "{" << std::endl;
		for (auto kv : obj) {
			std::cout << kv.first << " : ";
			print_value(kv.second);
			std::cout << ',' << std::endl;
		}
		std::cout << "}" << std::endl;
	}
}

int main(int argc, char* argv[]) {
	try {
		std::istringstream iss1{ " [ ] " };
		print_value(steeljson::read_document(iss1));
		std::cout << std::endl;
		std::istringstream iss2{ " { \"colour\": { \"name\": \"red\", \"code\": \"#ff0000\" } } " };
		print_value(steeljson::read_document(iss2));
	} catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
