set(STEELJSON_UNITTEST_TARGET_NAME unittest)

add_executable(${STEELJSON_UNITTEST_TARGET_NAME}
	catch.hpp
	main.cpp
	reader/json_org.cpp
	#reader/string.cpp
)

set_property(TARGET ${STEELJSON_UNITTEST_TARGET_NAME} PROPERTY CXX_STANDARD 14)

target_link_libraries(${STEELJSON_UNITTEST_TARGET_NAME} ${STEELJSON_TARGET_NAME})

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${CONFIG} CONFIG)
	get_target_property(UNITTEST_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${STEELJSON_UNITTEST_TARGET_NAME} RUNTIME_OUTPUT_DIRECTORY_${CONFIG})
	file(COPY "data" DESTINATION ${UNITTEST_RUNTIME_OUTPUT_DIRECTORY_${CONFIG}})
endforeach(CONFIG CMAKE_CONFIGURATION_TYPES)
