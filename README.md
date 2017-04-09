# steeljson

steeljson is a C++14 library for reading and writing JSON with DOM interface.

- [Features](#features)
	- [Type safety](#type-safety)
	- [Minimalistic interface](#minimalistic-interface)
	- [Immutable JSON value](#immutable-json-value)
	- [Mapping on C++ native types](#mapping-on-c-native-types)
	- [UTF-8 support](#utf-8-support)
- [Examples](#examples)
	- [Basic operations with JSON](#basic-operations-with-json)
	- [Type conversions](#type-conversions)
	- [Type checking](#type-checking)
	- [Reading / writing JSON](#reading--writing-json)
	- [Usage with a user-defined visitor class](#usage-with-a-user-defined-visitor-class)
- [Integration](#integration)
- [Building](#building)
- [Compatibility](#compatibility)

## Features

#### Type safety

The main goal of steeljson is to avoid implicit type conversions and runtime errors caused by a type mismatch. Hence, steeljson does not allow to perform any type-specific operations on the JSON value - before doing that the user must cast the JSON value to a specific type (e.g. `steeljson::object` or `float` - see Examples).

#### Minimalistic interface

steeljson offers a consistent minimalistic interface that at the same time provides sufficient means for JSON handling.

#### Immutable JSON value

Another steeljson goal is to offer a JSON value type that acts like a usual value type (e.g. built-in primitive types). The core library type representing the JSON value is the `steeljson::value` type. Once created, the `steeljson::value` cannot be mutated but can be entirely replaced by another `steeljson::value` via an assignment operation: for example, it is not possible to modify the JSON array that is stored in the `steeljson::value`. For the reason that the `steeljson::value` is immutable, it can be implemented using a container accessible through a shared pointer. steeljson benefits from this implementation detail by copying a pointer rather than the whole value in copy/move operations.

#### Mapping on C++ native types

The `steeljson::value` type implements the mapping between JSON types and native C++ types and types of C++ Standard Library in a convenient way:

|JSON types (RFC 7159)|C++ types|
|---|---|
|null|`steeljson::null_t`|
|boolean (true, false)|`bool`|
|number|`std::int{8, 16, 32, 64}_t`, `float`, `double`|
|string|`std::string`|
|array|`std::vector<steeljson::value>`|
|object|`std::unordered_map<std::string, steeljson::value>`|

steeljson carefully handles numeric types: it chooses the minimal required underlying type to store a numeric value and provides widening conversions for integer types and floating point types:

`std::int8_t` -> `std::int{16, 32, 64}_t`<br/>
`std::int16_t` -> `std::int{32, 64}_t`<br/>
`std::int32_t` -> `std::int64_t`<br/>
`float` -> `double`

#### UTF-8 support

steeljson provides full support of UTF-8 including escaped character representation.

## Examples

#### Basic operations with JSON

The `steeljson::value` can be constructed from each type that can be stored in it.

If we want to create the JSON value:

```json
{
	"player_id": 12345,
	"class": "mage",
	"level": 10,
	"name": "Gandalf",
	"friends": [11333, 11335, 11337],
	"attributes": {
		"strength": 10,
		"agility": 10,
		"spell_power": 135,
		"life": 1359.70
	},
	"quest": null
}
```

we can simply write:
```cpp
steeljson::value json{
	steeljson::object{
		{ "player_id", 12345 },
		{ "class", "mage" },
		{ "level", 10 },
		{ "name", "Gandalf" },
		{ "friends", steeljson::array{ 11333, 11335, 11337 } },
		{ "attributes", steeljson::object{
				{ "strength", 10 },
				{ "agility", 10 },
				{ "spell_power", 135 },
				{ "life", 1359.70 }
			}
		},
		{ "quest", steeljson::null }
	}
};
```

Then we might want to access the value or store another JSON in it:
```cpp
// use as<T>() method to cast the steeljson::value to the actual type
// note, that once created, the steeljson::value is immutable
steeljson::object current_player_data{ json.as<const steeljson::object&>() };

// but we can edit the steeljson::object before constructing the steeljson::value from this object
// since the steeljson::object is a mere alias for the std::unordered_map
steeljson::object player_data{
	{ "player_id", 11333 },
	{ "class", "monk" },
	{ "level", 15 },
	{ "name", "Daruma" },
	{ "friends", steeljson::array{ 12345 } },
	{ "attributes", steeljson::object{
			{ "strength", 15 },
			{ "agility", 197 },
			{ "spell_power", 15 },
			{ "life", 1735.57 }
		}
	},
	{ "quest", "Journey to the East" }
};

player_data["quest"] = "Shaolin Monastery";

// use operator=() to assign a new value to the existing steeljson::value
json = player_data;

// we can also use stream insertion operators
std::cout << json;
```

#### Type conversions

As mentioned above, steeljson supports widening numeric conversions. The idea is to hide implementation details and let the user work with JSON types. JSON has the only one numeric type - number. Assume we create JSON that stores the numeric value 15379. For this value the minimal required type from C++ type system is int16 and steeljson would use this type to store our value, but steeljson hides this detail and allows us to access this number using any other C++ numeric type if it does not lead to a data loss.

```cpp
steeljson::value number{ 75 }; // for this value steeljson uses int8 as the underlying type
std::int64_t number_i64 = number.as<std::int64_t>(); // ok: int8 can be converted to int64 without a data loss

number = 15379; // now the underlying type is int16
std::int8_t number_i8 = number.as<std::int8_t>(); // error: conversion is not allowed because of the loss of data
```

#### Type checking

```cpp
steeljson::value number{ 15379 };

// use is<T>() method for type checking before accessing the steeljson::value
// to avoid runtime errors
number.is<std::int8_t>(); // false
number.is<std::int32_t>(); // true
number.is<std::string>(); // false

// use type() method to get the JSON type of the JSON value
steeljson::value::type_t type = number.type(); // the JSON type is number
```

#### Reading / writing JSON

```cpp
// read JSON from a string
std::istringstream iss{ " [ ] " };
steeljson::value json_from_str{ steeljson::read_document(iss) };

// read JSON from a file
std::ifstream ifs{ "file.json" };
steeljson::value json_from_file{ steeljson::read_document(ifs) };

steeljson::value json{ "hello world!" };

// write JSON to a string
std::ostringstream oss;
steeljson::write(oss, json);

// write JSON to a file
std::ofstream ofs{ "file.json" };
steeljson::write(ofs, json);
ofs.close();
```

#### Usage with a user-defined *visitor* class

steeljson provides the visitor pattern support by defining the function *visit* that accepts a user-defined *visitor* and a `steeljson::value` and applies the *visitor* to the given value. The *visitor* class must define operator() overloads for each type that can be stored in the `steeljson::value`. Here is an example of a simple *visitor* implementation. Our *visitor* converts all string values of the JSON value to uppercase.

```cpp
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
```

Assuming we have some JSON value `json` we can apply our *visitor* to this value:

```cpp
uppercasing_visitor vis;

steeljson::value result{ steeljson::visit(vis, json) };
```

## Integration

steeljson can be used in other CMake projects by including `find_package(steeljson)` command in CMakeLists.txt as in the following example:

```
cmake_minimum_required(VERSION 3.0)

project(demo VERSION 0.1.0 LANGUAGES CXX)
find_package(steeljson)

add_executable(demo main.cpp)
target_link_libraries(demo steeljson)
```

`steeljson` is an exported target, so `target_link_libraries()` command configures compiler include directories as well.

Now you can include `steeljson/value.h` file in your source files to make the `steeljson::value` available in your code. To use `steeljson::read()` and `steeljson::write()` functions include `steeljson/reader.h` and `steeljson/writer.h` respectively.

## Building

Dependencies:
- [CMake](https://cmake.org) build system (version 3.8 or higher)
- Bison (version 3.0 or higher)
- Flex (version 2.5 or higher)
- A C++14 compiler

To build the library in the "build" directory execute the following commands:
```
mkdir build
cd build
cmake ..
```

CMake will generate projects for your build system. On Windows CMake will create a solution for building with Visual Studio or MSBuild. On Linux/BSD systems default project generator creates a Makefile for `make` command.

## Compatibility

steeljson has been compiled and tested with the following compilers on the specified platforms:

- Clang 3.8.0 on FreeBSD (64bit)
- Visual C++ 2017 on Windows (32/64bit)
