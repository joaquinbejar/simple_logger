cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(simple_logger)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(SIMPLE_LOGGER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
if (CMAKE_DEBUG)
    message(STATUS "simple_logger SIMPLE_LOGGER_INCLUDE ${SIMPLE_LOGGER_INCLUDE}")
endif ()

include(cmake/common.cmake)
include(cmake/simple_config.cmake)


add_library(simple_logger STATIC src/logger.cpp)
target_include_directories(simple_logger PRIVATE
        ${SIMPLE_LOGGER_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
)
target_link_libraries(simple_logger PUBLIC
        simple_color
        simple_config
        common
)

install(TARGETS simple_logger DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

option(NO_SIMPLE_LOGGER_TESTS "simple_logger Disable Current tests" OFF)

if (NOT NO_SIMPLE_LOGGER_TESTS)
    include(FetchContent)

    FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
    )
    FETCHCONTENT_GETPROPERTIES(Catch2)
    FETCHCONTENT_MAKEAVAILABLE(Catch2)

    add_subdirectory(test)
endif ()