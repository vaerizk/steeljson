#include "../catch.hpp"

#include <fstream>
#include <string>
#include <steeljson/exception.h>
#include <steeljson/reader.h>

TEST_CASE("reader test") {

	SECTION("failures") {
		for (const std::string& filename : {
				//"data/json.org/fail1.json",
				"data/json.org/fail2.json",
				"data/json.org/fail3.json",
				"data/json.org/fail4.json",
				"data/json.org/fail5.json",
				"data/json.org/fail6.json",
				"data/json.org/fail7.json",
				"data/json.org/fail8.json",
				"data/json.org/fail9.json",
				"data/json.org/fail10.json",
				"data/json.org/fail11.json",
				"data/json.org/fail12.json",
				"data/json.org/fail13.json",
				"data/json.org/fail14.json",
				"data/json.org/fail15.json",
				"data/json.org/fail16.json",
				"data/json.org/fail17.json",
				//"data/json.org/fail18.json",
				"data/json.org/fail19.json",
				"data/json.org/fail20.json",
				"data/json.org/fail21.json",
				"data/json.org/fail22.json",
				"data/json.org/fail23.json",
				"data/json.org/fail24.json",
				"data/json.org/fail25.json",
				"data/json.org/fail26.json",
				"data/json.org/fail27.json",
				"data/json.org/fail28.json",
				"data/json.org/fail29.json",
				"data/json.org/fail30.json",
				"data/json.org/fail31.json",
				"data/json.org/fail32.json",
				"data/json.org/fail33.json"
			}
		) {
			CAPTURE(filename);
			std::ifstream ifs{ filename };
			CHECK_THROWS_AS(steeljson::read_document(ifs), steeljson::parse_error);
		}
	}

	SECTION("passes") {
		for (const std::string& filename : {
				"data/json.org/pass1.json",
				"data/json.org/pass2.json",
				"data/json.org/pass3.json",
				"data/json.org/example_01.json",
				"data/json.org/example_02.json",
				"data/json.org/example_03.json",
				"data/json.org/example_04.json",
				"data/json.org/example_05.json"
			}
		) {
			CAPTURE(filename);
			std::ifstream ifs{ filename };
			CHECK_NOTHROW(steeljson::read_document(ifs));
		}
	}
}
