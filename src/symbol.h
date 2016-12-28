#ifndef STEELJSON_SYMBOL_H
#define STEELJSON_SYMBOL_H

#include <utility>
#include "value.h"

namespace steeljson {

	typedef std::pair<std::string, steeljson::value> object_member;

	class symbol {
		public:
			enum class type_t {
				character,
				object_member,
				json_value
			};

		public:
			symbol(char value);
			symbol(const object_member& value);
			symbol(const steeljson::value& value);

			symbol(const symbol& other) = default;
			symbol(symbol&& other) = default;

			~symbol() = default;

			symbol& operator=(const symbol& other) = default;
			symbol& operator=(symbol&& other) = default;

			char get_char() const;
			const object_member& get_object_member() const;
			const steeljson::value& get_json_value() const;
			type_t type() const;

		private:
			type_t symbol_type;
			char character;
			object_member object_member;
			steeljson::value json_value;
	};

}

#endif // STEELJSON_SYMBOL_H
