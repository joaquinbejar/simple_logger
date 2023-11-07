include(FetchContent)
FetchContent_Declare(simple_color
        GIT_REPOSITORY https://github.com/joaquinbejar/simple_color.git
        GIT_TAG dev
)
FetchContent_MakeAvailable(simple_color)
if (CMAKE_DEBUG)
    message(STATUS "SIMPLE_COLOR_SOURCE_DIR ${simple_color_SOURCE_DIR}")
endif ()
set(SIMPLE_COLOR_INCLUDE ${simple_color_SOURCE_DIR}/include CACHE INTERNAL "")