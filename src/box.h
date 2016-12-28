#ifndef STEELJSON_BOX_H
#define STEELJSON_BOX_H

namespace steeljson {

	struct box {
		enum class type_t {
			null,
			boolean,
			fp_single,
			fp_double,
			int_8,
			int_16,
			int_32,
			int_64,
			uint_8,
			uint_16,
			uint_32,
			uint_64,
			string,
			array,
			object
		};

		box(type_t type) :
			type(type) { }

		type_t type;
	};

	template<typename T>
	struct typed_box : box {
		typed_box(type_t type, const T& load) :
			box(type),
			load(load) { }

		T load;
	};

}

#endif // STEELJSON_BOX_H
