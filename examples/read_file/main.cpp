#include <iostream>
#include <fstream>
#include <steeljson/reader.h>
#include <steeljson/value.h>

int main(int argc, char* argv[]) {
	try {
		std::ifstream ifs{ "players.json" };
		std::cout << steeljson::read_document(ifs);
	} catch (const std::runtime_error& e) {
		std::cout << e.what();
	}

	return 0;
}
