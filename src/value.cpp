#include "value.h"
#include <cmath>
#include <exception>
#include <limits>
#include "box.h"
#include "writer.h"

using namespace steeljson;

value::value() {
	this->box_ptr = std::make_shared<typed_box<steeljson::null_t>>(box::type_t::null, steeljson::null);
}

value::value(steeljson::null_t null) {
	this->box_ptr = std::make_shared<typed_box<steeljson::null_t>>(box::type_t::null, steeljson::null);
}

value::value(bool v) {
	this->box_ptr = std::make_shared<typed_box<bool>>(box::type_t::boolean, v);
}

value::value(float v) {
	if (std::isnan(v)
		|| std::isinf(v)) {
		throw std::domain_error("NaN or infinity is not acceptable");
	}

	this->box_ptr = std::make_shared<typed_box<float>>(box::type_t::fp_single, v);
}

value::value(double v) {
	if (std::isnan(v)
		|| std::isinf(v)) {
		throw std::domain_error("NaN or infinity is not acceptable");
	}

	if (((v >= 0) && (v <= std::numeric_limits<float>::max()))
		|| ((v < 0) && (v >= std::numeric_limits<float>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<float>>(box::type_t::fp_single, static_cast<float>(v));
	} else {
		this->box_ptr = std::make_shared<typed_box<double>>(box::type_t::fp_double, v);
	}
}

value::value(std::int8_t v) {
	this->box_ptr = std::make_shared<typed_box<std::int8_t>>(box::type_t::int_8, v);
}

value::value(std::int16_t v) {
	if (((v >= 0) && (v <= std::numeric_limits<std::int8_t>::max()))
		|| ((v < 0) && (v >= std::numeric_limits<std::int8_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int8_t>>(box::type_t::int_8, static_cast<std::int8_t>(v));
	} else {
		this->box_ptr = std::make_shared<typed_box<std::int16_t>>(box::type_t::int_16, v);
	}
}

value::value(std::int32_t v) {
	if (((v >= 0) && (v <= std::numeric_limits<std::int8_t>::max()))
		|| ((v < 0) && (v >= std::numeric_limits<std::int8_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int8_t>>(box::type_t::int_8, static_cast<std::int8_t>(v));
	} else if (((v >= 0) && (v <= std::numeric_limits<std::int16_t>::max()))
				|| ((v < 0) && (v >= std::numeric_limits<std::int16_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int16_t>>(box::type_t::int_16, static_cast<std::int16_t>(v));
	} else {
		this->box_ptr = std::make_shared<typed_box<std::int32_t>>(box::type_t::int_32, v);
	}
}

value::value(std::int64_t v) {
	if (((v >= 0) && (v <= std::numeric_limits<std::int8_t>::max()))
		|| ((v < 0) && (v >= std::numeric_limits<std::int8_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int8_t>>(box::type_t::int_8, static_cast<std::int8_t>(v));
	} else if (((v >= 0) && (v <= std::numeric_limits<std::int16_t>::max()))
				|| ((v < 0) && (v >= std::numeric_limits<std::int16_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int16_t>>(box::type_t::int_16, static_cast<std::int16_t>(v));
	} else if (((v >= 0) && (v <= std::numeric_limits<std::int32_t>::max()))
				|| ((v < 0) && (v >= std::numeric_limits<std::int32_t>::lowest()))) {
		this->box_ptr = std::make_shared<typed_box<std::int32_t>>(box::type_t::int_32, static_cast<std::int32_t>(v));
	} else {
		this->box_ptr = std::make_shared<typed_box<std::int64_t>>(box::type_t::int_64, v);
	}
}

value::value(const char* v) {
	this->box_ptr = std::make_shared<typed_box<std::string>>(box::type_t::string, std::string(v));
}

value::value(const std::string& v) {
	this->box_ptr = std::make_shared<typed_box<std::string>>(box::type_t::string, v);
}

value::value(const steeljson::array& v) {
	this->box_ptr = std::make_shared<typed_box<steeljson::array>>(box::type_t::array, v);
}

value::value(const steeljson::object& v) {
	this->box_ptr = std::make_shared<typed_box<steeljson::object>>(box::type_t::object, v);
}

value::value(const value& other) {
	this->box_ptr = other.box_ptr;
}

value& value::operator=(const value& other) {
	this->box_ptr = other.box_ptr;
	return *this;
}

template<> bool value::is<steeljson::null_t>() const noexcept {
	return (this->box_ptr->type == box::type_t::null);
}

template<> bool value::is<bool>() const noexcept {
	return (this->box_ptr->type == box::type_t::boolean);
}

template<> bool value::is<float>() const noexcept {
	return (this->box_ptr->type == box::type_t::fp_single);
}

template<> bool value::is<double>() const noexcept {
	return (this->box_ptr->type == box::type_t::fp_single)
		|| (this->box_ptr->type == box::type_t::fp_double);
}

template<> bool value::is<int8_t>() const noexcept {
	return (this->box_ptr->type == box::type_t::int_8);
}

template<> bool value::is<int16_t>() const noexcept {
	return (this->box_ptr->type == box::type_t::int_8)
		|| (this->box_ptr->type == box::type_t::int_16);
}

template<> bool value::is<int32_t>() const noexcept {
	return (this->box_ptr->type == box::type_t::int_8)
		|| (this->box_ptr->type == box::type_t::int_16)
		|| (this->box_ptr->type == box::type_t::int_32);
}

template<> bool value::is<int64_t>() const noexcept {
	return (this->box_ptr->type == box::type_t::int_8)
		|| (this->box_ptr->type == box::type_t::int_16)
		|| (this->box_ptr->type == box::type_t::int_32)
		|| (this->box_ptr->type == box::type_t::int_64);
}

template<> bool value::is<std::string>() const noexcept {
	return (this->box_ptr->type == box::type_t::string);
}

template<> bool value::is<steeljson::array>() const noexcept {
	return (this->box_ptr->type == box::type_t::array);
}

template<> bool value::is<steeljson::object>() const noexcept {
	return (this->box_ptr->type == box::type_t::object);
}

template<> steeljson::null_t value::as<steeljson::null_t>() const {
	if (this->box_ptr->type == box::type_t::null) {
		return steeljson::null;
	} else {
		throw std::bad_cast();
	}
}

template<> bool value::as<bool>() const {
	if (this->box_ptr->type == box::type_t::boolean) {
		return (reinterpret_cast<typed_box<bool>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

template<> float value::as<float>() const {
	if (this->box_ptr->type == box::type_t::fp_single) {
		return (reinterpret_cast<typed_box<float>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

template<> double value::as<double>() const {
	switch (this->box_ptr->type) {
		case box::type_t::fp_single: {
			return (reinterpret_cast<typed_box<float>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::fp_double: {
			return (reinterpret_cast<typed_box<double>*> (this->box_ptr.get()))->load;
		}
		default: {
			throw std::bad_cast();
		}
	}
}

template<> std::int8_t value::as<std::int8_t>() const {
	if (this->box_ptr->type == box::type_t::int_8) {
		return (reinterpret_cast<typed_box<std::int8_t>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

template<> std::int16_t value::as<std::int16_t>() const {
	switch (this->box_ptr->type) {
		case box::type_t::int_8: {
			return (reinterpret_cast<typed_box<std::int8_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_16: {
			return (reinterpret_cast<typed_box<std::int16_t>*> (this->box_ptr.get()))->load;
		}
		default: {
			throw std::bad_cast();
		}
	}
}

template<> std::int32_t value::as<std::int32_t>() const {
	switch (this->box_ptr->type) {
		case box::type_t::int_8: {
			return (reinterpret_cast<typed_box<std::int8_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_16: {
			return (reinterpret_cast<typed_box<std::int16_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_32: {
			return (reinterpret_cast<typed_box<std::int32_t>*> (this->box_ptr.get()))->load;
		}
		default: {
			throw std::bad_cast();
		}
	}
}

template<> std::int64_t value::as<std::int64_t>() const {
	switch (this->box_ptr->type) {
		case box::type_t::int_8: {
			return (reinterpret_cast<typed_box<std::int8_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_16: {
			return (reinterpret_cast<typed_box<std::int16_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_32: {
			return (reinterpret_cast<typed_box<std::int32_t>*> (this->box_ptr.get()))->load;
		}
		case box::type_t::int_64: {
			return (reinterpret_cast<typed_box<std::int64_t>*> (this->box_ptr.get()))->load;
		}
		default: {
			throw std::bad_cast();
		}
	}
}

template<> const std::string& value::as<const std::string&>() const {
	if (this->box_ptr->type == box::type_t::string) {
		return (reinterpret_cast<typed_box<std::string>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

template<> const steeljson::array& value::as<const steeljson::array&>() const {
	if (this->box_ptr->type == box::type_t::array) {
		return (reinterpret_cast<typed_box<steeljson::array>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

template<> const steeljson::object& value::as<const steeljson::object&>() const {
	if (this->box_ptr->type == box::type_t::object) {
		return (reinterpret_cast<typed_box<steeljson::object>*> (this->box_ptr.get()))->load;
	} else {
		throw std::bad_cast();
	}
}

value::type_t value::type() const {
	switch (this->box_ptr->type) {
		case box::type_t::null: {
			return value::type_t::null;
		}
		case box::type_t::boolean: {
			return value::type_t::boolean;
		}
		case box::type_t::fp_single:
		case box::type_t::fp_double:
		case box::type_t::int_8:
		case box::type_t::int_16:
		case box::type_t::int_32:
		case box::type_t::int_64: {
			return value::type_t::number;
		}
		case box::type_t::string: {
			return value::type_t::string;
		}
		case box::type_t::array: {
			return value::type_t::array;
		}
		case box::type_t::object: {
			return value::type_t::object;
		}
		default: {
			throw std::exception();
		}
	}
}

std::ostream& steeljson::operator<<(std::ostream& os, const steeljson::value& value) {
	write(os, value);
	return os;
}

std::ostream& steeljson::operator>>(const steeljson::value& value, std::ostream& os) {
	return os << value;
}
