include(FetchContent)
FetchContent_Declare(common
        GIT_REPOSITORY https://github.com/joaquinbejar/common_cpp.git
        GIT_TAG v0.1.4
)
FetchContent_MakeAvailable(common)
if (CMAKE_DEBUG)
    message(STATUS "simple_logger/cmake COMMON_SOURCE_DIR ${common_SOURCE_DIR}")
endif ()
set(COMMON_INCLUDE ${common_SOURCE_DIR}/include CACHE INTERNAL "")