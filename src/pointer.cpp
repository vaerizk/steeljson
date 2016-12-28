#include "pointer.h"
#include <sstream>

using namespace steeljson;

// splits an input string into tokens
// that are separated by '/' character;
// returns vector that contains tokens
std::vector<std::string> tokenize(const std::string& path) {
	std::vector<std::string> tokens;
	char delimiter{ '/' };

	if (path.empty()) {
		return tokens;
	}

	if (path.at(0) != delimiter) {
		throw std::invalid_argument("incorrect syntax: json pointer must have leading forward slash");
	}

	if (path.at(path.size() - 1) == delimiter) {
		throw std::invalid_argument("incorrect syntax: json pointer cannot end with a forward slash");
	}

	std::istringstream source{ path.substr(1) }; // remove first character which is a delimiter
	std::string token;

	while (std::getline(source, token, delimiter)) {
		if (token.empty()) {
			throw std::invalid_argument("incorrect syntax: empty path element is not allowed");
		}
		tokens.push_back(token);
	}

	return tokens;
}

// replaces "~0" and "~1" sequences with '~' and '/' symbols respectively in a given token
std::string& recover_escaped(std::string& token) {
	std::size_t char_position{ 0 };

	while (char_position < token.size()) {
		if ((token.at(char_position) == '~')) {
			if (char_position == token.size() - 1) {
				throw std::invalid_argument("incorrect syntax: invalid escape sequence");
			}

			if (token.at(char_position + 1) == '0') {
				token.erase(char_position + 1, 1);
				++char_position;
			} else if (token.at(char_position + 1) == '1') {
				token.erase(char_position, 2);
				token.insert(char_position, 1, '/');
				++char_position;
			} else {
				throw std::invalid_argument("incorrect syntax: invalid escape sequence");
			}
		} else {
			++char_position;
		}
	}

	return token;
}

// replaces '~' and '/' symbols with "~0" and "~1" escape sequences respectively
// in a given path element
std::string& escape_tildes_and_slashes(std::string& path_element) {
	std::size_t char_position{ 0 };

	while (char_position < path_element.size()) {
		if (path_element.at(char_position) == '~') {
			path_element.insert(char_position + 1, 1, '0');
			char_position = char_position + 2;
		} else if (path_element.at(char_position) == '/') {
			path_element.erase(char_position, 1);
			path_element.insert(char_position, "~1");
			char_position = char_position + 2;
		} else {
			++char_position;
		}
	}

	return path_element;
}

pointer steeljson::make_pointer(const std::string& path) {
	std::vector<std::string> tokens{ tokenize(path) };

	for (std::string& token : tokens) {
		recover_escaped(token);
	}

	return tokens;
}

std::string steeljson::to_string(const pointer& pointer) {
	std::string path;

	std::vector<std::string> path_elements{ pointer };
	for (std::string& element : path_elements) {
		escape_tildes_and_slashes(element);
		path.append(1, '/');
		path.append(element);
	}

	return path;
}
