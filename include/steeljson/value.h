#ifndef STEELJSON_VALUE_H
#define STEELJSON_VALUE_H

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace steeljson {

	class value;
	struct box;

	typedef std::vector<steeljson::value> array;
	typedef std::unordered_map<std::string, steeljson::value> object;

	struct null_t { };
	constexpr null_t null{};

	class value {
		public:
			enum class type_t {
				null,
				boolean,
				number,
				string,
				array,
				object
			};

		public:
			value(); // bison requires default constructor
			value(steeljson::null_t);
			value(bool);
			value(float);
			value(double);
			value(std::int8_t);
			value(std::int16_t);
			value(std::int32_t);
			value(std::int64_t);
			value(const char*);
			value(const std::string&);
			value(const steeljson::array&);
			value(const steeljson::object&);

			value(const value& other);
			//value(value&&);

			~value() = default;

			value& operator=(const value& other);

			template <typename T> bool is() const noexcept;
			template <typename T> T as() const;
			type_t type() const;

		private:
			std::shared_ptr<box> box_ptr;
	};

	template<> bool value::is<steeljson::null_t>() const noexcept;
	template<> bool value::is<bool>() const noexcept;
	template<> bool value::is<float>() const noexcept;
	template<> bool value::is<double>() const noexcept;
	template<> bool value::is<std::int8_t>() const noexcept;
	template<> bool value::is<std::int16_t>() const noexcept;
	template<> bool value::is<std::int32_t>() const noexcept;
	template<> bool value::is<std::int64_t>() const noexcept;
	template<> bool value::is<std::string>() const noexcept;
	template<> bool value::is<steeljson::array>() const noexcept;
	template<> bool value::is<steeljson::object>() const noexcept;

	template<> steeljson::null_t value::as<steeljson::null_t>() const;
	template<> bool value::as<bool>() const;
	template<> float value::as<float>() const;
	template<> double value::as<double>() const;
	template<> std::int8_t value::as<std::int8_t>() const;
	template<> std::int16_t value::as<std::int16_t>() const;
	template<> std::int32_t value::as<std::int32_t>() const;
	template<> std::int64_t value::as<std::int64_t>() const;
	template<> const std::string& value::as<const std::string&>() const;
	template<> const steeljson::array& value::as<const steeljson::array&>() const;
	template<> const steeljson::object& value::as<const steeljson::object&>() const;

	std::ostream& operator<<(std::ostream& os, const steeljson::value& value);
	std::ostream& operator>>(const steeljson::value& value, std::ostream& os);

	template<typename Visitor> auto visit(Visitor&& visitor, const steeljson::value& value) {
		switch (value.type()) {
			case steeljson::value::type_t::null: {
				return visitor(steeljson::null);
			}
			case steeljson::value::type_t::boolean: {
				return visitor(value.as<bool>());
			}
			case steeljson::value::type_t::number: {
				if (value.is<float>()) {
					return visitor(value.as<float>());
				} else if (value.is<double>()) {
					return visitor(value.as<double>());
				} else if (value.is<std::int8_t>()) {
					return visitor(value.as<std::int8_t>());
				} else if (value.is<std::int16_t>()) {
					return visitor(value.as<std::int16_t>());
				} else if (value.is<std::int32_t>()) {
					return visitor(value.as<std::int32_t>());
				} else if (value.is<std::int64_t>()) {
					return visitor(value.as<std::int64_t>());
				}
			}
			case steeljson::value::type_t::string: {
				return visitor(value.as<const std::string&>());
			}
			case steeljson::value::type_t::array: {
				return visitor(value.as<const steeljson::array&>());
			}
			case steeljson::value::type_t::object: {
				return visitor(value.as<const steeljson::object&>());
			}
			default: {
				throw std::exception();
			}
		}
	}

}

#endif // STEELJSON_VALUE_H
