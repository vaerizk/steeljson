#include <iostream>
#include <sstream>
#include <steeljson/value.h>
#include <steeljson/writer.h>

int main(int argc, char* argv[]) {
	try {
		const steeljson::value val{
			steeljson::object{
				{ "colours", steeljson::array{
						steeljson::object { { "name", "red" }, { "code", "#ff0000" } },
						steeljson::object { { "name", "yellow" }, { "code", "#ffff00" } },
						steeljson::object { { "name", "blue" }, { "code", "#0000ff" } }
					}
				}
			}
		};

		std::ostringstream val_stream;
		steeljson::write(val_stream, val);
		std::cout << val_stream.str() << std::endl;
	} catch (const std::runtime_error& e) {
		std::cout << e.what();
	}

	return 0;
}
