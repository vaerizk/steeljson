find_package(FLEX 2.5 REQUIRED)
set(FLEX_INPUT lexer.l)
set(FLEX_OUTPUT ${PROJECT_BINARY_DIR}/src/lexer.cpp)
if(FLEX_FOUND)
	if(WIN32)
		flex_target(lexer ${FLEX_INPUT} ${FLEX_OUTPUT} COMPILE_FLAGS "--wincompat")
	else(WIN32)
		flex_target(lexer ${FLEX_INPUT} ${FLEX_OUTPUT})
	endif(WIN32)
endif(FLEX_FOUND)

find_package(BISON 3.0 REQUIRED)
set(BISON_INPUT parser.y)
set(BISON_DEFINES ${PROJECT_BINARY_DIR}/src/parser.h)
set(BISON_OUTPUT ${PROJECT_BINARY_DIR}/src/parser.cpp)
if(BISON_FOUND)
	bison_target(parser ${BISON_INPUT} ${BISON_OUTPUT} DEFINES_FILE ${BISON_DEFINES})
endif(BISON_FOUND)

# header and source files
file(GLOB STEELJSON_PRIVATE_HEADERS *.h)
file(GLOB STEELJSON_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/include/${STEELJSON_TARGET_NAME}/*.h)
file(GLOB STEELJSON_SOURCES *.cpp)

add_library(${STEELJSON_TARGET_NAME} STATIC
	${BISON_DEFINES}
	${BISON_OUTPUT}
	${FLEX_OUTPUT}
	${STEELJSON_PRIVATE_HEADERS}
	${STEELJSON_PUBLIC_HEADERS}
	${STEELJSON_SOURCES}
)

target_compile_features(${STEELJSON_TARGET_NAME} PUBLIC cxx_std_14)

target_include_directories(${STEELJSON_TARGET_NAME}
	PRIVATE
		${FLEX_INCLUDE_DIRS}
		${PROJECT_BINARY_DIR}/src
		${PROJECT_SOURCE_DIR}/include/${STEELJSON_TARGET_NAME}
		${PROJECT_SOURCE_DIR}/src
	INTERFACE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

install(TARGETS ${STEELJSON_TARGET_NAME}
	EXPORT ${STEELJSON_EXPORT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

# group source files for IDE source explorers (e.g. Visual Studio)
source_group("Generated Files" FILES ${FLEX_OUTPUT} ${BISON_DEFINES} ${BISON_OUTPUT})
source_group("Private Header Files" FILES ${STEELJSON_PRIVATE_HEADERS})
source_group("Public Header Files" FILES ${STEELJSON_PUBLIC_HEADERS})
source_group("Source Files" FILES ${STEELJSON_SOURCES})

# TODO: remove this when CMake supports C++17 and MSVC compiler switch
if(MSVC)
	set(CMAKE_CXX_FLAGS "/std:c++latest")
endif()
